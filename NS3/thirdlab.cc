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
#include "ns3/node-list.h"
#include "ns3/netanim-module.h"
#include <sstream>
#include <fstream>

using namespace ns3;

void write_csv(std::string filename,uint16_t node,float pdr,float plr,double jitter){
	std::fstream f;
	f.open(filename, std::fstream::in| std::fstream::app);
	f << node << "," << pdr/node << "," << plr/node << "," << jitter/node << "\n";
	f.close();
}


///analysis on DSDV routing protocol on (i)mobile adhoc network
void experiment (bool enableCtsRts,uint16_t nwifisource,bool tracing,bool saturated,std::string filename)
{
  //uint16_t nwifisource = 10;
  uint16_t nwifisink = nwifisource;
  // 0. Enable or disable CTS/RTS
  UintegerValue ctsThr = (enableCtsRts ? UintegerValue (100) : UintegerValue (2200));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);

  // 1. Create 3 nodes
  NodeContainer nodes;
  nodes.Create (nwifisource + nwifisink);

  // 2. Place nodes somehow, this is required by every wireless simulation
  for (size_t i = 0; i < nwifisource + nwifisink ; ++i)
    {
      nodes.Get (i)->AggregateObject (CreateObject<ConstantPositionMobilityModel> ());
    }

  // 3. Create propagation loss matrix
  Ptr<MatrixPropagationLossModel> lossModel = CreateObject<MatrixPropagationLossModel> ();
  lossModel->SetDefaultLoss (100); // set default loss to 200 dB (no link)
  //lossModel->SetLoss (nodes.Get (0)->GetObject<MobilityModel> (), nodes.Get (1)->GetObject<MobilityModel> (), 50); // set symmetric loss 0 <-> 1 to 50 dB
  //lossModel->SetLoss (nodes.Get (2)->GetObject<MobilityModel> (), nodes.Get (1)->GetObject<MobilityModel> (), 50); // set symmetric loss 2 <-> 1 to 50 dB
  for (size_t i = 0; i < (nwifisource) ; ++i) //symmetric source and sink
  {
    lossModel->SetLoss (nodes.Get (i)->GetObject<MobilityModel> (), nodes.Get (nwifisource + i)->GetObject<MobilityModel> (), 50); // set symmetric loss 0 <-> 1 to 50 dB
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
  wifiMac.SetType ("ns3::AdhocWifiMac"); // use ad-hoc MAC
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);

  // uncomment the following to have athstats output
  // AthstatsHelper athstats;
  // athstats.EnableAthstats(enableCtsRts ? "rtscts-athstats-node" : "basic-athstats-node" , nodes);

  // uncomment the following to have pcap output
  // wifiPhy.EnablePcap (enableCtsRts ? "rtscts-pcap-node" : "basic-pcap-node" , nodes);

  //mobility
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                               "MinX", DoubleValue (0.0),
                               "MinY", DoubleValue (0.0),
                               "DeltaX", DoubleValue (20.0),
                               "DeltaY", DoubleValue (20.0),
                               "GridWidth", UintegerValue (5),
                               "LayoutType", StringValue ("RowFirst"));

  //constant position
  //mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  //move
  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel","Bounds", RectangleValue (Rectangle (-100, 100, -100, 100)));
  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel","Speed", StringValue ("ns3::UniformRandomVariable[Min=5.0|Max=5.0]"));
  
  mobility.Install (nodes);

  // 6. Install TCP/IP stack & assign IP addresses
  InternetStackHelper internet;

  //dsdv helper 
  DsdvHelper dsdv;
  dsdv.Set ("PeriodicUpdateInterval", TimeValue (Seconds (10.0)));
  dsdv.Set ("SettlingTime", TimeValue (Seconds (2.5)));
  internet.SetRoutingHelper (dsdv);
  internet.Install (nodes);
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.0.0.0");
  ipv4.Assign (devices);
  
  
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>("dynamic-global-routing.routes", std::ios::out);
  dsdv.PrintRoutingTableAllAt(Seconds (19),routingStream);
  
  // 7. Install applications: two CBR streams each saturating the channel
  ApplicationContainer cbrApps;

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

if(saturated)
{ uint16_t cbrPort = 12345; //port
  for (size_t i = 0; i < nwifisource; ++i)
  { 
    std::string datarate_cbr;
    std::ostringstream oss;
    oss << 800000 + ((i+1)*1000);
    datarate_cbr += oss.str();
    //std::cout<<datarate_cbr<<"\n";
    
    double time_cbr_start = 5.000 + ((.005)*(i));
    
    //getting ip address of a sink node
    Ptr<Ipv4> ipv4 = NodeList::GetNode(nwifisource + i)->GetObject<Ipv4>();
    Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
    Ipv4Address addri = iaddr.GetLocal (); 

    OnOffHelper onOffHelper ("ns3::UdpSocketFactory", InetSocketAddress (addri, cbrPort));
    onOffHelper.SetAttribute ("PacketSize", UintegerValue (1400));
    onOffHelper.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
    onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));


    onOffHelper.SetAttribute ("DataRate", StringValue (datarate_cbr + "bps"));
    onOffHelper.SetAttribute ("StartTime", TimeValue (Seconds (time_cbr_start))); //using different start times to workaround Bug 388 and Bug 912
    cbrApps.Add (onOffHelper.Install (nodes.Get (i)));
    
  }

}
else{
	
	uint16_t  echoPort = 9; //port
	for(size_t i = 0; i < nwifisource; ++i)
	{ 
		
		double time_echo_start = 5.000 + ((.005)*(i)); //start time
  //to get address
  Ptr<Ipv4> ipv4 = NodeList::GetNode(nwifisource + i)->GetObject<Ipv4>();
    Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
    Ipv4Address addri = iaddr.GetLocal (); 
  
  UdpEchoClientHelper echoClientHelper (addri, echoPort);
  echoClientHelper.SetAttribute ("MaxPackets", UintegerValue (100));
  echoClientHelper.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
  echoClientHelper.SetAttribute ("PacketSize", UintegerValue (10));
  ApplicationContainer pingApps;
  
  echoClientHelper.SetAttribute ("StartTime", TimeValue (Seconds (time_echo_start)));
  pingApps.Add (echoClientHelper.Install (nodes.Get (i)));
  

	}
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
  // 8. Install FlowMonitor on all nodes
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

  // 9. Run simulation for 20 seconds
  Simulator::Stop (Seconds (20));
  
  //for netanim
  AnimationInterface::SetConstantPosition (nodes.Get(0), 0.0,0.0);
  AnimationInterface anim("animation.xml");

  
  //tracing
  if (tracing == true)
    { 
      wifiPhy.EnablePcap ("output", devices.Get (0),true);
    }

  Simulator::Run ();

  // 9.5 To get average for each test
  float sum_pdr = 0;
  float sum_plr = 0;
  float sum_jit = 0;

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
          //std::cout << "  TxOffered:  " << i->second.txBytes * 8.0 / 15.0 / 1000 / 1000  << " Mbps\n"; // 20 - 5 = 15
          std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
          std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
          //std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 15.0 / 1000 / 1000  << " Mbps\n";
          float prx = (i->second.rxBytes);// / i->second.txBytes);
          float ptx = (i->second.txBytes);
          float pdr = prx/ptx;
          float plr = (ptx - prx)/ptx;
          double jit_in_sec = (i->second.jitterSum).GetSeconds();
          std::cout << "  Loss Bytes " << (int)(ptx - prx) <<"\n";
          //std::cout << " Loss Packets " << i->second.lostPackets << "\n"; only for 10 seconds
          std::cout << "  PDR " << pdr << "\n";
          std::cout << "  PLR " << plr << "\n";      
          std::cout << "  Sum of all Jitter " << jit_in_sec  << "\n" ;   
      //  }
      sum_pdr+=pdr;
      sum_plr+=plr;
      sum_jit+=jit_in_sec;
    }

	//10.5 write csv
	write_csv(filename,nwifisource,sum_pdr,sum_plr,sum_jit);



  // 11. Cleanup
  Simulator::Destroy ();
}

int main (int argc, char **argv)
{
  uint16_t nwifisource = 1;
  //uint16_t nwifisink = nwifisource;
  uint16_t verbose = false;
  bool tracing = false;
  bool saturated = true;
  std::string filename = "default.csv";

  CommandLine cmd;
  cmd.AddValue ("nwifisource", "No of source nodes / sink nodes", nwifisource);
  cmd.AddValue ("saturated", "Whether the network is saturated or not", saturated);
  cmd.AddValue ("verbose", "Tell applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);
  cmd.AddValue ("filename", "Filename for csv generation", filename); 
 
  cmd.Parse (argc, argv);
  
    if (verbose)
    {
      LogComponentEnable ("OnOffApplication", LOG_LEVEL_INFO);
    }

  nwifisource = (nwifisource ? nwifisource : 1);  // if someone give 0 as nodenumber then give default value of 1

  std::cout << "Running:\n" << std::flush;
  experiment (false,nwifisource,tracing,saturated,filename);
  std::cout << "------------------------------------------------\n";
  //std::cout << "Hidden station experiment with RTS/CTS enabled:\n";
  //experiment (true);

  return 0;
}
