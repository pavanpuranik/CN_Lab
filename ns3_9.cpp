#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("NodeCreationExample");

int main(int argc, char *argv[]) {

    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    NodeContainer nodes1, nodes2, nodes3;
    nodes1.Create(3);
    nodes3.Create(3);
    nodes2.Add(nodes1.Get(2));
    nodes2.Create(2);
    nodes2.Add(nodes3.Get(0));

    PointToPointHelper p2p1, p2p2, p2p3, p2p4;
    p2p1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p1.SetChannelAttribute("Delay", StringValue("2ms"));

    p2p2.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p2.SetChannelAttribute("Delay", StringValue("2ms"));

    p2p3.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p3.SetChannelAttribute("Delay", StringValue("2ms"));

    p2p4.SetDeviceAttribute("DataRate", StringValue("50Mbps"));
    p2p4.SetChannelAttribute("Delay", StringValue("1ms"));

    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
    csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));


    NetDeviceContainer nd1, nd2, nd3, nd4, nd5;
    nd1 = p2p1.Install(nodes1.Get(0), nodes1.Get(1)); // separately pass multiple nodes
    nd2 = p2p2.Install(nodes1.Get(1), nodes1.Get(2));
    nd3 = csma.Install(nodes2);
    nd4 = p2p3.Install(nodes3.Get(0), nodes3.Get(1));
    nd5 = p2p4.Install(nodes3.Get(1), nodes3.Get(2));

    InternetStackHelper inet;
    inet.Install(nodes2); // separately pass single node
    inet.Install(nodes1.Get(0));
    inet.Install(nodes1.Get(1));
    inet.Install(nodes3.Get(1));
    inet.Install(nodes3.Get(2));

    Ipv4AddressHelper addr;
    Ipv4InterfaceContainer ifaces1, ifaces2, ifaces3, ifaces4, ifaces5;

    addr.SetBase("10.1.1.0", "255.255.255.0");
    ifaces1 = addr.Assign(nd1);
    ifaces2 = addr.Assign(nd2);

    addr.SetBase("20.1.2.0", "255.255.255.0");
    ifaces3 = addr.Assign(nd3);

    addr.SetBase("30.1.3.0", "255.255.255.0");
    ifaces4 = addr.Assign(nd4);
    ifaces5 = addr.Assign(nd5);

    UdpEchoServerHelper echoServer(9);
    ApplicationContainer serverApp = echoServer.Install(nodes2.Get(2));

    serverApp.Start(Seconds(1.0));
    serverApp.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(ifaces3.GetAddress(2), 9);

    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApp = echoClient.Install(nodes3.Get(2));

    clientApp.Start(Seconds(2.0));
    clientApp.Stop(Seconds(10.0));


    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // p2p1.EnablePcapAll("second");
    // csma.EnablePcap("second", nd3.Get(2), true);
    // p2p2.EnablePcapAll("second");


    Simulator::Run();
    Simulator::Destroy();


    return 0;
}
