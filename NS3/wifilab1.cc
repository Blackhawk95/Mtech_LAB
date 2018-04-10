/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 IITP RAS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Pavel Boyko <boyko@iitp.ru>
 */

/*
 * Classical hidden terminal problem and its RTS/CTS solution.
 *
 * Topology: [node 0] <-- -50 dB --> [node 1] <-- -50 dB --> [node 2]
 *
 * This example illustrates the use of
 *  - Wifi in ad-hoc mode
 *  - Matrix propagation loss model
 *  - Use of OnOffApplication to generate CBR stream
 *  - IP flow monitor
 */

/*Notes

removed echo packets which was meant for arpish work, but didnt changed anything much


*/




#include "ns3/node.h"
#include "ns3/core-module.h"
#include "ns3/propagation-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/wifi-module.h"
#include "ns3/dsdv-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/node-list.h"
#include "ns3/mac-low.h"
#include "ns3/wifi-mac.h"
#include "ns3/dca-txop.h"
#include <sstream>

using namespace ns3;

uint32_t countAck = 0;
UintegerValue ctsThr = UintegerValue (100);



static void
CountAck ()
{ countAck++;
  if(countAck >=2)
  {
  	countAck = 0;
	ctsThr = ctsThr + 100;  
  }
}


/// Run single 10 seconds experiment with enabled or disabled RTS/CTS mechanism
void experiment (uint16_t nwifisource,bool tracing)
{
	
   //LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  //uint16_t nwifisource = 10;
  uint16_t nwifisink = nwifisource;
  // 0. Setting CTS/RTS threshold value initially
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);

  // 1. Create 3 nodes
  NodeContainer nodes;
  nodes.Create (nwifisource + nwifisink);
  //**AP
  NodeContainer wifiApNode;// = wifiStaNodes.Get(17);
  wifiApNode.Create (1);

  //mobility
  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                               "MinX", DoubleValue (0.0),
                               "MinY", DoubleValue (0.0),
                               "DeltaX", DoubleValue (20.0),
                               "DeltaY", DoubleValue (20.0),
                               "GridWidth", UintegerValue (6),
                               "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                           "Bounds", RectangleValue (Rectangle (-100, 100, -100, 100)));
  mobility.Install (nodes);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);

/*
  // 2. Place nodes somehow, this is required by every wireless simulation
  for (size_t i = 0; i < nwifisource + nwifisink ; ++i)
    {
      nodes.Get (i)->AggregateObject (CreateObject<ConstantPositionMobilityModel> ());
    }
      wifiApNode.Get (0)->AggregateObject (CreateObject<ConstantPositionMobilityModel> ());

*/

  // 3. Create propagation loss matrix
  Ptr<MatrixPropagationLossModel> lossModel = CreateObject<MatrixPropagationLossModel> ();
  lossModel->SetDefaultLoss (100); // set default loss to 200 dB (no link)
  //lossModel->SetLoss (nodes.Get (0)->GetObject<MobilityModel> (), nodes.Get (1)->GetObject<MobilityModel> (), 50); // set symmetric loss 0 <-> 1 to 50 dB
  //lossModel->SetLoss (nodes.Get (2)->GetObject<MobilityModel> (), nodes.Get (1)->GetObject<MobilityModel> (), 50); // set symmetric loss 2 <-> 1 to 50 dB
  for (size_t i = 0; i < (nwifisource+nwifisink) ; ++i) //symmetric source and sink
  {
    lossModel->SetLoss (nodes.Get (i)->GetObject<MobilityModel> (), wifiApNode.Get(0)->GetObject<MobilityModel> (), 50); // set symmetric loss 0 <-> 1 to 50 dB
  }



  // 4. Create & setup wifi channel
  Ptr<YansWifiChannel> wifiChannel = CreateObject <YansWifiChannel> ();
  wifiChannel->SetPropagationLossModel (lossModel);
  wifiChannel->SetPropagationDelayModel (CreateObject <ConstantSpeedPropagationDelayModel> ());

  // 5. Install wireless devices
  WifiHelper wifi;

  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue ("DsssRate2Mbps"),
                                "ControlMode",StringValue ("DsssRate1Mbps"));
  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  wifiPhy.SetChannel (wifiChannel);
  WifiMacHelper wifiMac;
  Ssid ssid = Ssid ("ns-3-ssid");

  wifiMac.SetType ("ns3::StaWifiMac",
             "Ssid", SsidValue (ssid),
             "ActiveProbing", BooleanValue (false));
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);

  wifiMac.SetType ("ns3::ApWifiMac",
             "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevice;
  apDevice = wifi.Install (wifiPhy, wifiMac, wifiApNode);
  
  //attempt at listening ack
  /*
  Ptr<NetDevice> dev = apDevice.Get (1);
  Ptr<WifiNetDevice> wd = DynamicCast<WifiNetDevice> (dev);
  Ptr<WifiPhy> p = wd->GetPhy ();
  
  //ns3::RegularWifiMac::m_rxMiddle
  
  Ptr<MacRxMiddle> m_rxMiddle;
  m_rxMiddle = new MacRxMiddle ();
  m_rxMiddle->SetForwardCallback (MakeCallback (&RegularWifiMac::Receive, this));
  Ptr< MacLow > m_low = CreateObject<MacLow> ();
  (DynamicCast<MacLow> (m_low))->SetPhy (p);
  m_low->SetRxCallback (MakeCallback (&MacRxMiddle::Receive, m_rxMiddle));
  */
  //Modify EDCA configuration (TXOP limit) for AC_VO
  
/*
  Ptr<NetDevice> dev = wifiApNode.Get (0)->GetDevice (0);
  Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice> (dev);
  Ptr<WifiMac> wifi_mac = wifi_dev->GetMac ();
  PointerValue ptr1;
  Ptr<DcaTxop> edca;
  wifi_mac->GetAttribute ("DcaTxop", ptr1);
  edca = ptr1.Get<DcaTxop> ();
  //apDevice.EnableLogComponents ();
  //std::cout<<edca->MissedAck(true);
 */ 
  //TracedCallback m_txTrace;
	
	
//using a tracesource from DcaTxop which invokes CountAck whenever a packet is missed
Config::ConnectWithoutContext ("/NodeList/0/DeviceList/0/DcaTxop/MissedAck", MakeCallback (&CountAck));
  
  //k.SetupPhyMacLowListener(p);

  // uncomment the following to have athstats output
  // AthstatsHelper athstats;
  // athstats.EnableAthstats(enableCtsRts ? "rtscts-athstats-node" : "basic-athstats-node" , nodes);

  // uncomment the following to have pcap output
  // wifiPhy.EnablePcap (enableCtsRts ? "rtscts-pcap-node" : "basic-pcap-node" , nodes);


  // 6. Install TCP/IP stack & assign IP addresses
  InternetStackHelper internet;

  //dsdv helper
  DsdvHelper dsdv;
  dsdv.Set ("PeriodicUpdateInterval", TimeValue (Seconds (2.0)));
  dsdv.Set ("SettlingTime", TimeValue (Seconds (3.0)));
  internet.SetRoutingHelper (dsdv);
  internet.Install (nodes);
  internet.Install (wifiApNode);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.0.0.0");
  ipv4.Assign (devices);
  Ipv4InterfaceContainer wifiInterfaces;
  wifiInterfaces = ipv4.Assign (apDevice);

  // 7. Install applications: two CBR streams each saturating the channel
  ApplicationContainer cbrApps;
  uint16_t cbrPort = 12345;
  /*
  OnOffHelper onOffHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address ("10.0.0.2"), cbrPort));
  onOffHelper.SetAttribute ("PacketSize", UintegerValue (1400));
  onOffHelper.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  */

  // flow 1:  node 0 -> node 1
  //onOffHelper.SetAttribute ("DataRate", StringValue ("3000000bps"));
  //onOffHelper.SetAttribute ("StartTime", TimeValue (Seconds (1.000000)));
  //cbrApps.Add (onOffHelper.Install (nodes.Get (0)));

  // flow 2:  node 2 -> node 1
  /** \internal
   * The slightly different start times and data rates are a workaround
   * for \bugid{388} and \bugid{912}
   */
  //onOffHelper.SetAttribute ("DataRate", StringValue ("3001100bps"));
  //onOffHelper.SetAttribute ("StartTime", TimeValue (Seconds (1.001)));
  //cbrApps.Add (onOffHelper.Install (nodes.Get (2)));

  for (size_t i = 0; i < nwifisource; ++i)
  {
    std::string datarate_cbr;
    std::ostringstream oss;
    oss << "300" << ((i+1)*1100);
    datarate_cbr += oss.str();
    //std::cout<<datarate_cbr<<"\n";

    double time_cbr = 7.000 + ((.001)*(i));

    //getting ip address of a sink node
    Ptr<Ipv4> ipv4 = NodeList::GetNode(nwifisource + i)->GetObject<Ipv4>();
    Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
    Ipv4Address addri = iaddr.GetLocal ();

    OnOffHelper onOffHelper ("ns3::UdpSocketFactory", InetSocketAddress (addri, cbrPort));
    onOffHelper.SetAttribute ("PacketSize", UintegerValue (1400));
    onOffHelper.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
    onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));


    onOffHelper.SetAttribute ("DataRate", StringValue (datarate_cbr + "bps"));
    onOffHelper.SetAttribute ("StartTime", TimeValue (Seconds (time_cbr)));
    cbrApps.Add (onOffHelper.Install (nodes.Get (i)));

  }


  /** \internal
   * We also use separate UDP applications that will send a single
   * packet before the CBR flows start.
   * This is a workaround for the lack of perfect ARP, see \bugid{187}
   */
  /*
  uint16_t  echoPort = 9;
  UdpEchoClientHelper echoClientHelper (Ipv4Address ("10.0.0.2"), echoPort);
  echoClientHelper.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClientHelper.SetAttribute ("Interval", TimeValue (Seconds (0.01)));
  echoClientHelper.SetAttribute ("PacketSize", UintegerValue (10));
  ApplicationContainer pingApps;

  // again using different start times to workaround Bug 388 and Bug 912

  //echoClientHelper.SetAttribute ("StartTime", TimeValue (Seconds (0.001)));
  //pingApps.Add (echoClientHelper.Install (nodes.Get (0)));
  //echoClientHelper.SetAttribute ("StartTime", TimeValue (Seconds (0.006)));
  //pingApps.Add (echoClientHelper.Install (nodes.Get (2)));

  for (size_t i = 0; i < nwifisource + nwifisink ; ++i)
  {
    if(i!=1){
    double time_echo = 0.001 * (i + 5);
    echoClientHelper.SetAttribute ("StartTime", TimeValue (Seconds (time_echo)));
    pingApps.Add (echoClientHelper.Install (nodes.Get (i)));
    }
  }
*/
	
// Changing RTS threshold on the fly
Ptr<NetDevice> dev = wifiApNode.Get (0)->GetDevice (0);
Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice> (dev);
PointerValue tmp;
wifi_dev->GetAttribute ("WifiRemoteStationManager", tmp);
Ptr<Object> wrsm = tmp.GetObject (); //wifiremotestationmanager pointer object
wrsm->SetAttribute("RtsCtsThreshold", UintegerValue (ctsThr));	

  // 8. Install FlowMonitor on all nodes
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

  // 9. Run simulation for 10 seconds
  Simulator::Stop (Seconds (20));
  if (tracing == true)
    {
      wifiPhy.EnablePcap ("output", apDevice.Get (0));
    }

  AnimationInterface::SetConstantPosition (wifiApNode.Get(0), 25.0,25.0);
  AnimationInterface anim("animation.xml");


  Simulator::Run ();

  // 10. Print per flow statistics
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
          std::cout << "  TxOffered:  " << i->second.txBytes * 8.0 / 13.0 / 1000 / 1000  << " Mbps\n"; // 20 - 7 = 13
          std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
          std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
          std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 13.0 / 1000 / 1000  << " Mbps\n";
          float prx = (i->second.rxBytes);// / i->second.txBytes);
          float ptx = (i->second.txBytes);
          //std::cout << "  Loss Packets " << (int)(ptx - prx) <<"\n";
          //std::cout << " Loss Packets " << i->second.lostPackets << "\n"; only for 10 seconds
          std::cout << "  PDR " << prx/ptx << "\n";
          std::cout << "  Collision rate " << (ptx - prx)/13.0 << "\n"; // lost packets/simulation time
          std::cout << "  End to End delay " << ((i->second.jitterSum).GetSeconds())/nwifisource << "\n" ;
      //  }
    }

  // 11. Cleanup
  Simulator::Destroy ();
}

int main (int argc, char **argv)
{
  uint16_t nwifisource = 5;
  //uint16_t nwifisink = nwifisource;
  uint16_t verbose = false;
  bool tracing = false;

  CommandLine cmd;
  cmd.AddValue ("nwifisource", "No of source nodes / sink nodes", nwifisource);
  //cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc, argv);

    if (verbose)
    {
      LogComponentEnable ("OnOffApplication", LOG_LEVEL_INFO);
    }


  std::cout << "Running:\n" << std::flush;
  experiment (nwifisource,tracing);
  std::cout << "------------------------------------------------\n";

  return 0;
}
