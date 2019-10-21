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

    PointToPointHelper p2p1, p2p2, p2p3;
    p2p1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p1.SetChannelAttribute("Delay", StringValue("2ms"));

    p2p2.SetDeviceAttribute("DataRate", StringValue("7Mbps"));
    p2p2.SetChannelAttribute("Delay", StringValue("1ms"));

    p2p3.SetDeviceAttribute("DataRate", StringValue("7Mbps"));
    p2p3.SetChannelAttribute("Delay", StringValue("1ms"));

    NetDeviceContainer nd1, nd2, nd3;
    nd1 = p2p1.Install(nodes.Get(0), nodes.Get(1));
    nd2 = p2p2.Install(nodes.Get(1), nodes.Get(2));
    nd3 = p2p3.Install(nodes.Get(1), nodes.Get(3));

    InternetStackHelper inet;
    inet.Install(nodes);

    Ipv4AddressHelper addr1, addr2, addr3;
    addr1.SetBase("10.1.1.0", "255.255.255.0");
    addr2.SetBase("10.1.2.0", "255.255.255.0");
    addr3.SetBase("10.1.3.0", "255.255.255.0");


    Ipv4InterfaceContainer ifaces1, ifaces2, ifaces3;
    ifaces1 = addr1.Assign(nd1);
    ifaces2 = addr2.Assign(nd2);
    ifaces3 = addr3.Assign(nd3);

    UdpEchoServerHelper eServer1(9), eServer2(10);

    ApplicationContainer serverApp1 = eServer1.Install(nodes.Get(1));
    ApplicationContainer serverApp2 = eServer2.Install(nodes.Get(1));

    serverApp1.Start(Seconds(1.0));
    serverApp1.Stop(Seconds(10.0));

    serverApp2.Start(Seconds(1.0));
    serverApp2.Stop(Seconds(10.0));



    UdpEchoClientHelper eClient1(ifaces1.GetAddress(1), 9), eClient2(ifaces2.GetAddress(0), 10), eClient3(ifaces3.GetAddress(0), 10);

    eClient1.SetAttribute("MaxPackets", UintegerValue(1));
    eClient1.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    eClient1.SetAttribute("PacketSize", UintegerValue(1024));

    eClient2.SetAttribute("MaxPackets", UintegerValue(1));
    eClient2.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    eClient2.SetAttribute("PacketSize", UintegerValue(1024));

    eClient3.SetAttribute("MaxPackets", UintegerValue(1));
    eClient3.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    eClient3.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApp1 = eClient1.Install(nodes.Get(0));
    ApplicationContainer clientApp2 = eClient2.Install(nodes.Get(2));
    ApplicationContainer clientApp3 = eClient3.Install(nodes.Get(3));


    clientApp1.Start(Seconds(2.0));
    clientApp1.Stop(Seconds(10.0));

    clientApp2.Start(Seconds(2.0));
    clientApp2.Stop(Seconds(10.0));

    clientApp3.Start(Seconds(2.0));
    clientApp3.Stop(Seconds(10.0));

    Simulator::Run();
    Simulator::Destroy();

    return 0;

}
