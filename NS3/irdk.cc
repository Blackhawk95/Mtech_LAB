#include <iostream>
#include <string>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/applications-module.h"

#include <sstream>
#include <fstream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FastRetransmission");

uint32_t avg_cwd = 0;


// Default Network Topology
//
//       10.1.2.0
// n1   n2   n3   n4
// |    |    |    |
//  ================
//   LAN 10.1.2.0

class MyApp : public Application 
{
public:

  MyApp ();
  virtual ~MyApp();

  void Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate);

private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void ScheduleTx (void);
  void SendPacket (void);

  Ptr<Socket>     m_socket;
  Address         m_peer;
  uint32_t        m_packetSize;
  uint32_t        m_nPackets;
  DataRate        m_dataRate;
  EventId         m_sendEvent;
  bool            m_running;
  uint32_t        m_packetsSent;
};

MyApp::MyApp ()
  : m_socket (0), 
    m_peer (), 
    m_packetSize (0), 
    m_nPackets (0), 
    m_dataRate (0), 
    m_sendEvent (), 
    m_running (false), 
    m_packetsSent (0)
{
}

MyApp::~MyApp()
{
  m_socket = 0;
}

void
MyApp::Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate)
{
  m_socket = socket;
  m_peer = address;
  m_packetSize = packetSize;
  m_nPackets = nPackets;
  m_dataRate = dataRate;
}

void
MyApp::StartApplication (void)
{
  m_running = true;
  m_packetsSent = 0;
  m_socket->Bind ();
  m_socket->Connect (m_peer);
  SendPacket ();
}

void 
MyApp::StopApplication (void)
{
  m_running = false;

  if (m_sendEvent.IsRunning ())
    {
      Simulator::Cancel (m_sendEvent);
    }

  if (m_socket)
    {
      m_socket->Close ();
    }
}

void 
MyApp::SendPacket (void)
{
  Ptr<Packet> packet = Create<Packet> (m_packetSize);
  m_socket->Send (packet);

  if (++m_packetsSent < m_nPackets)
    {
      ScheduleTx ();
    }
}

void 
MyApp::ScheduleTx (void)
{
  if (m_running)
    {
      Time tNext (Seconds (m_packetSize * 8 / static_cast<double> (m_dataRate.GetBitRate ())));
      m_sendEvent = Simulator::Schedule (tNext, &MyApp::SendPacket, this);
    }
}

static void
CwndChange (uint32_t oldCwnd, uint32_t newCwnd)
{ std::fstream f;
  f.open("output3.csv", std::fstream::in| std::fstream::app);
  f << Simulator::Now ().GetSeconds () << " " << newCwnd << "\n";
  //NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "\t" << newCwnd);
}

static void
RxDrop (Ptr<const Packet> p)
{
  NS_LOG_UNCOND ("RxDrop at " << Simulator::Now ().GetSeconds ());
}


int
main (int argc, char *argv[])
{
  uint32_t nCsma = 2;

  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of CSMA nodes/devices", nCsma);

  cmd.Parse (argc,argv);

  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpNewReno::GetTypeId ()));
  Config::SetDefault ("ns3::TcpSocketBase::Sack", true);
  
  nCsma = nCsma == 0 ? 1 : nCsma;

  NodeContainer csmaNodes;
  csmaNodes.Create (nCsma);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("10Kbps"));
  csma.SetChannelAttribute ("Delay", StringValue ("10ms"));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  InternetStackHelper stack;
  stack.Install (csmaNodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  /*UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma-1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma-1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (csmaNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
  */
	ApplicationContainer cbrApps;
  uint16_t cbrPort = 12345;
  for (size_t i = 0; i < nCsma/2; ++i)
  {
    std::string datarate_cbr;
    std::ostringstream oss;
    oss << 20000 + ((i+1)*1000);
    datarate_cbr += oss.str();
    //std::cout<<datarate_cbr<<"\n";

    //double time_cbr = 1.000 + ((.001)*(i));

    //getting ip address of a sink node
    Ptr<Ipv4> ipv4 = NodeList::GetNode((nCsma/2) + i)->GetObject<Ipv4>();
    Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
    Ipv4Address addri = iaddr.GetLocal ();
    
	//uint16_t sinkPort = 8080;
	Address sinkAddress (InetSocketAddress (csmaInterfaces.GetAddress ((nCsma/2)+i), cbrPort));
	PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), cbrPort));
	ApplicationContainer sinkApps = packetSinkHelper.Install (csmaNodes.Get ((nCsma/2)+i));
	sinkApps.Start (Seconds (0.));
	sinkApps.Stop (Seconds (1000.));
    
    Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (csmaNodes.Get (i), TcpSocketFactory::GetTypeId ());
    
    if(i==0)
		ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", MakeCallback (&CwndChange));
    
    Ptr<MyApp> app = CreateObject<MyApp> ();
    app->Setup (ns3TcpSocket, InetSocketAddress(addri,cbrPort) , 1500, 700000, DataRate ( datarate_cbr+"bps"));
    csmaNodes.Get (i)->AddApplication (app);
    app->SetStartTime (Seconds (1.));
    app->SetStopTime (Seconds (1000.));

    csmaDevices.Get ((nCsma/2)+i)->TraceConnectWithoutContext ("PhyRxDrop", MakeCallback (&RxDrop));
  }

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  csma.EnablePcap ("second", csmaDevices.Get (1), true);

	//Install FlowMonitor on all nodes
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

	Simulator::Stop (Seconds (1000.0));
  Simulator::Run ();

	//Print per flow statistics
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
      // first 2 FlowIds are for ECHO apps, we don't want to display them
      //
      // Duration for throughput measurement is 9.0 seconds, since
      //   StartTime of the OnOffApplication is at about "second 1"
      // and
      //   Simulator::Stops at "second 10".
      //first few are echo packets we sent
      //if (i->first > 0)//(uint16_t)(nwifisource + nwifisink - 1))
      //  {
          Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
          std::cout << "Flow " << i->first - 0 << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";//(uint16_t)(nwifisource + nwifisink - 1)
          std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
          std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
          std::cout << "  TxOffered:  " << i->second.txBytes * 8.0 / 999.0 / 1000 / 1000  << " Mbps\n"; // 20 - 7 = 13
          std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
          std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
          std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 999.0 / 1000 / 1000  << " Mbps\n";
          float prx = (i->second.rxBytes);// / i->second.txBytes);
          float ptx = (i->second.txBytes);
          std::cout << "  Loss Packets " << (int)(ptx - prx) <<"\n";
          //std::cout << " Loss Packets " << i->second.lostPackets << "\n"; only for 10 seconds
          std::cout << "  PDR " << prx/ptx << "\n";
          std::cout << "  PLR " << (ptx - prx)/ptx << "\n";
          std::cout << "  Sum of all Jitter " << (i->second.jitterSum).GetSeconds() << "\n" ;
      //  }
    }

  Simulator::Destroy ();
  return 0;
}
