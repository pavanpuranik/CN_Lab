#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"
#include"ns3/mobility-module.h"
#include "ns3/animation-interface.h"
 
 
using namespace ns3;
 
NS_LOG_COMPONENT_DEFINE ("NS3netAnimScriptExample");
 
int main (int argc, char *argv[])
{
  bool verbose=true;
  uint32_t nCsma=3,np2p=2;
 
  CommandLine cmd;
  cmd.AddValue("nCsma","Number of \"extra\" CSMA nodes/devices",nCsma);
  cmd.AddValue("verbose","Tell echo applications to log if true",verbose);
  cmd.Parse(argc,argv);
 
  if(verbose){
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  }
  nCsma=nCsma==0?1:nCsma;
 
  NodeContainer p2pNodes;
  p2pNodes.Create(np2p);
 
  NodeContainer csmaNodes;
  csmaNodes.Add(p2pNodes.Get(1));
  csmaNodes.Create(nCsma);
 
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
  p2p.SetChannelAttribute("Delay",StringValue("5ms"));
 
  NetDeviceContainer p2pDevices;
  p2pDevices=p2p.Install(p2pNodes);
 
  CsmaHelper csma;
  csma.SetChannelAttribute("DataRate",StringValue("100Mbps"));
  csma.SetChannelAttribute("Delay",TimeValue(NanoSeconds(6560)));
 
  NetDeviceContainer csmaDevices;
  csmaDevices=csma.Install(csmaNodes);
 
  InternetStackHelper stack;
  stack.Install(p2pNodes.Get(0));
  stack.Install(csmaNodes);
 
  Ipv4AddressHelper address;
  address.SetBase("10.1.1.0","255.255.255.0");
  Ipv4InterfaceContainer p2pIntf;
  p2pIntf=address.Assign(p2pDevices);
 
  address.SetBase("10.1.2.0","255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces=address.Assign(csmaDevices);
 
  UdpEchoServerHelper echoServer(9);
  ApplicationContainer serverApps=echoServer.Install(csmaNodes.Get(nCsma));
  serverApps.Start(Seconds(1.0));
  serverApps.Stop(Seconds(10.0));
 
  UdpEchoClientHelper echoClient(csmaInterfaces.GetAddress(nCsma),9);
  echoClient.SetAttribute("MaxPackets",UintegerValue(1));
  echoClient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
  echoClient.SetAttribute("PacketSize",UintegerValue(1024));
 
  ApplicationContainer clientApps=echoClient.Install(p2pNodes.Get(0));
  clientApps.Start(Seconds(2.0));
  clientApps.Stop(Seconds(10.0));
 
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();
 
  p2p.EnablePcapAll("second");
  csma.EnablePcap("second",csmaDevices.Get(1),true);
 
  AnimationInterface anim("anim_ec.xml");
 
  anim.SetConstantPosition(p2pNodes.Get(0),0.5,0.5);
  anim.SetConstantPosition(csmaNodes.Get(0),11.0,11.0);
  anim.SetConstantPosition(csmaNodes.Get(1),31.5,31.25);
  anim.SetConstantPosition(csmaNodes.Get(2),52.0,51.5);
  anim.SetConstantPosition(csmaNodes.Get(3),72.5,71.75);
 
  anim.EnablePacketMetadata(true);
 
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
 
}