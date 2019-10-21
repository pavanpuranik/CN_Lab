#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("NodeCreationExample");

int main(int argc, char *argv[]) {

    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    NodeContainer nodes;
    nodes.Create(4);

    PointToPointHelper p2p1, p2p2;
    p2p1.SetDeviceAttribute("DataRate", StringValue("8Mbps"));
    p2p1.SetChannelAttribute("Delay", StringValue("3ms"));

    p2p2.SetDeviceAttribute("DataRate", StringValue("7Mbps"));
    p2p2.SetChannelAttribute("Delay", StringValue("1ms"));


    NetDeviceContainer nd1, nd2;
    nd1 = p2p1.Install(nodes.Get(0), nodes.Get(1));
    nd2 = p2p2.Install(nodes.Get(2), nodes.Get(3));

    InternetStackHelper inet;
    inet.Install(nodes);

    Ipv4AddressHelper addr1, addr2;
    addr1.SetBase("10.1.1.0", "255.255.255.0");
    addr2.SetBase("10.1.2.0", "255.255.255.0");

    Ipv4InterfaceContainer ifaces1, ifaces2;
    ifaces1 = addr1.Assign(nd1);
    ifaces2 = addr2.Assign(nd2);

    UdpEchoServerHelper eServer1(9);
    ApplicationContainer serverApp1 = eServer1.Install(nodes.Get(1));
    serverApp1.Start(Seconds(1.0));
    serverApp1.Stop(Seconds(10.0));

    UdpEchoClientHelper eClient1(ifaces1.GetAddress(1), 9);
    eClient1.SetAttribute("MaxPackets", UintegerValue(1));
    eClient1.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    eClient1.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApp1 = eClient1.Install(nodes.Get(1));
    clientApp1.Start(Seconds(2.0));
    clientApp1.Stop(Seconds(10.0));



    UdpEchoServerHelper eServer2(9);
    ApplicationContainer serverApp2 = eServer2.Install(nodes.Get(2));
    serverApp2.Start(Seconds(1.0));
    serverApp2.Stop(Seconds(10.0));

    UdpEchoClientHelper eClient2(ifaces2.GetAddress(0), 9);
    eClient2.SetAttribute("MaxPackets", UintegerValue(1));
    eClient2.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    eClient2.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApp2 = eClient2.Install(nodes.Get(2));
    clientApp2.Start(Seconds(2.0));
    clientApp2.Stop(Seconds(10.0));


    Simulator::Run();
    Simulator::Destroy();


    return 0;
}
