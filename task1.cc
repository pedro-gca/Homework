#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"


//.                  TOPOLOGY
//.
//.                    n4
//.                  / | \ 
//.  n0  n1  n2----n3--n5--n7    n8  n9  
//.  |   |   |       \ | /  |    |   |
//.  =========         n6   ==========
//.
//.
//.




using namespace ns3;


NS_LOG_COMPONENT_DEFINE ("task1");

int
main (int argc, char *argv[])
{   
    uint32_t configuration = 0;

    CommandLine cmd (__FILE__);
    cmd.AddValue ("configuration", "Which configuration should be used", configuration);

    cmd.Parse (argc,argv);

    
    
    Time::SetResolution (Time::NS);

    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    NS_LOG_INFO ("Install internet stack on all nodes.");
    InternetStackHelper internet;

    // nodes 0, 1, 2 - CSMA LEFT
    NS_LOG_INFO ("CSMA Left");
    NodeContainer CSMA1_nodes;
    CSMA1_nodes.Create(3);

    CsmaHelper CSMA1;
    CSMA1.SetChannelAttribute ("DataRate", StringValue ("25Mbps"));
    CSMA1.SetChannelAttribute ("Delay", StringValue ("10us"));
    
    NetDeviceContainer CSMA1_ND = CSMA1.Install(CSMA1_nodes);

    internet.Install(CSMA1_nodes);

    Ipv4AddressHelper address;
    address.SetBase ("192.128.1.0", "255.255.255.0");
    Ipv4InterfaceContainer CSMA1_interfaces;
    CSMA1_interfaces = address.Assign (CSMA1_ND);



    // star node5
    NS_LOG_INFO ("Build star topology.");
    uint32_t nSpokes = 4;

    PointToPointHelper starN5;
    starN5.SetDeviceAttribute ("DataRate", StringValue ("80Mbps"));
    starN5.SetChannelAttribute ("Delay", StringValue ("5s"));

    PointToPointStarHelper star (nSpokes, starN5);

    star.InstallStack (internet);

    NS_LOG_INFO ("Assign IP Addresses.");
    star.AssignIpv4Addresses (Ipv4AddressHelper ("10.10.1.0", "255.255.255.0"));
    

    // nodes 2 and 3
    NS_LOG_INFO ("Point to Point n2 - n3");
    NodeContainer n2n3_nodes;
    n2n3_nodes.Add(CSMA1_nodes.Get(2));
    n2n3_nodes.Add(star.GetSpokeNode(0));

    PointToPointHelper n2n3;
    n2n3.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
    n2n3.SetChannelAttribute ("Delay", StringValue ("15us"));

    NetDeviceContainer n2n3_ND = n2n3.Install(n2n3_nodes);

    address.SetBase ("10.1.1.0", "255.255.255.252");
    Ipv4InterfaceContainer n2n3_interfaces;
    n2n3_interfaces = address.Assign (n2n3_ND);

    // Nella star, n3 dovrebbe essere il nodo 0 e n7 dovrebbe essere il nodo 2
    // Link n3 n4
    NS_LOG_INFO ("Point to Point n3 - n4");
    NodeContainer n3n4_nodes;
    n3n4_nodes.Add(star.GetSpokeNode(0));
    n3n4_nodes.Add(star.GetSpokeNode(1));

    PointToPointHelper n3n4;
    n3n4.SetDeviceAttribute ("DataRate", StringValue ("80Mbps"));
    n3n4.SetChannelAttribute ("Delay", StringValue ("5us"));

    NetDeviceContainer n3n4_ND = n3n4.Install(n3n4_nodes);

    address.SetBase ("10.0.1.0", "255.255.255.252");
    Ipv4InterfaceContainer n3n4_interfaces;
    n3n4_interfaces = address.Assign (n3n4_ND);


    // Link n4 n7
    NS_LOG_INFO ("Point to Point n4 - n7");
    NodeContainer n4n7_nodes;
    n4n7_nodes.Add(star.GetSpokeNode(1));
    n4n7_nodes.Add(star.GetSpokeNode(2));

    
    PointToPointHelper n4n7;
    n4n7.SetDeviceAttribute ("DataRate", StringValue ("80Mbps"));
    n4n7.SetChannelAttribute ("Delay", StringValue ("5us"));

    NetDeviceContainer n4n7_ND = n4n7.Install(n4n7_nodes);

    address.SetBase ("10.0.2.0", "255.255.255.252");
    Ipv4InterfaceContainer n4n7_interfaces;
    n4n7_interfaces = address.Assign (n4n7_ND);


    // Link n7 n6
    NS_LOG_INFO ("Point to Point n7 - n6");
    NodeContainer n7n6_nodes;
    n7n6_nodes.Add(star.GetSpokeNode(2));
    n7n6_nodes.Add(star.GetSpokeNode(3));

    
    PointToPointHelper n7n6;
    n7n6.SetDeviceAttribute ("DataRate", StringValue ("80Mbps"));
    n7n6.SetChannelAttribute ("Delay", StringValue ("5us"));

    NetDeviceContainer n7n6_ND = n7n6.Install(n7n6_nodes);

    address.SetBase ("10.0.3.0", "255.255.255.252");
    Ipv4InterfaceContainer n7n6_interfaces;
    n7n6_interfaces = address.Assign (n7n6_ND);


    // Link n6 n3
    NS_LOG_INFO ("Point to Point n6 - n3");
    NodeContainer n6n3_nodes;
    n6n3_nodes.Add(star.GetSpokeNode(3));
    n6n3_nodes.Add(star.GetSpokeNode(0));

    
    PointToPointHelper n6n3;
    n6n3.SetDeviceAttribute ("DataRate", StringValue ("80Mbps"));
    n6n3.SetChannelAttribute ("Delay", StringValue ("5us"));

    NetDeviceContainer n6n3_ND = n6n3.Install(n6n3_nodes);

    address.SetBase ("10.0.4.0", "255.255.255.252");
    Ipv4InterfaceContainer n6n3_interfaces;
    n6n3_interfaces = address.Assign (n6n3_ND);

    // Nella star, n3 dovrebbe essere il nodo 0 e n7 dovrebbe essere il nodo 2
    // nodes 7, 8, 9 - CSMA Right
    NS_LOG_INFO ("CSMA Right");
    NodeContainer CSMA2_nodes;
    CSMA2_nodes.Create(2);
    internet.Install(CSMA2_nodes);
    CSMA2_nodes.Add(star.GetSpokeNode(2));

    CsmaHelper CSMA2;
    CSMA2.SetChannelAttribute ("DataRate", StringValue ("30Mbps"));
    CSMA2.SetChannelAttribute ("Delay", StringValue ("20us"));
    
    NetDeviceContainer CSMA2_ND = CSMA2.Install(CSMA2_nodes);

    address.SetBase ("192.128.2.0", "255.255.255.0");
    Ipv4InterfaceContainer CSMA2_interfaces;
    CSMA2_interfaces = address.Assign (CSMA2_ND);
    if (configuration == 0) {
        NS_LOG_INFO ("Install internet stack on all nodes.");
        
                // certo
        // uint16_t port = 2300;
        // Address sinkAddress(InetSocketAddress(Ipv4Address::GetAny(), port));
        // PacketSinkHelper sinkHelper("ns3::TcpSocketFactory", sinkAddress);
        // ApplicationContainer sinkApp = sinkHelper.Install(star.GetHub());
        // sinkApp.Start(Seconds(1.0));
        // sinkApp.Start(Seconds(10.0));  

        // teste
        uint16_t port = 2300;
        Address sinkAddress(InetSocketAddress("0.0.0.0", port));
        PacketSinkHelper sinkHelper("ns3::TcpSocketFactory", sinkAddress);
        ApplicationContainer sinkApp = sinkHelper.Install(star.GetHub());
        sinkApp.Start(Seconds(1.0));
        sinkApp.Start(Seconds(10.0));   

        OnOffHelper onOffHelper ("ns3::TcpSocketFactory", Address ());
        onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
        onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
        onOffHelper.SetAttribute("PacketSize",UintegerValue(2500));
        //onOffHelper sul n9
        ApplicationContainer clientApps;
         AddressValue remoteAddress (InetSocketAddress (star.GetHubIpv4Address (2), port));//Ho cambiato nuovamente il parametro,non definitivo
        
        // AddressValue remoteAddress(InetSocketAddress(GetHubIpv4Address(CSMA2_nodes.Get(2)) , port));
        
        onOffHelper.SetAttribute("Remote", remoteAddress);
        clientApps.Add(onOffHelper.Install(CSMA2_nodes.Get(2)));

        clientApps.Start(Seconds(3.0));
        clientApps.Stop(Seconds(15.0));

        // Per aggiungere onOffHelper sui nodi della star
        // ApplicationContainer spokeApps;      

        // for (uint32_t i = 0; i < star.SpokeCount (); ++i)
        //     {
        //     AddressValue remoteAddress (InetSocketAddress (star.GetHubIpv4Address (i), port));
        //     onOffHelper.SetAttribute ("Remote", remoteAddress);
        //     spokeApps.Add (onOffHelper.Install (star.GetSpokeNode (i)));
        //     }
        
        // spokeApps.Start (Seconds (1.0));
        // spokeApps.Stop (Seconds (10.0));

        // Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

        starN5.EnablePcapAll ("deleteme_test");
        //starN5.EnablePcapAll ("star_test");
        //CSMA2.EnablePcap("Csma2_test",CSMA1_ND.Get(2),true);
    }

    if (configuration == 1) {
        NS_LOG_INFO ("Install internet stack on all nodes.");
           uint16_t port1 = 2300;
           uint16_t port2 = 7457;

        // Sink n5
        Address sinkAddress1(InetSocketAddress(Ipv4Address::GetAny(), port1));
        PacketSinkHelper sinkHelper1("ns3::TcpSocketFactory", sinkAddress1);
        ApplicationContainer sinkApp = sinkHelper1.Install(star.GetHub());
        sinkApp.Start(Seconds(0.0));
        sinkApp.Stop(Seconds(17.0));


        // OnOffHelper n9
        OnOffHelper onOffHelper1 ("ns3::TcpSocketFactory", Address ());
        onOffHelper1.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
        onOffHelper1.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
        onOffHelper1.SetAttribute("PacketSize",UintegerValue(2500));


        // AppContainer n9
        ApplicationContainer clientApps1;
        AddressValue remoteAddress1 (InetSocketAddress (star.GetHubIpv4Address (2), port1));
        onOffHelper1.SetAttribute("Remote", remoteAddress1);
        clientApps1.Add(onOffHelper1.Install(CSMA2_nodes.Get(2)));

        // Sink n0
        Address sinkAddress2(InetSocketAddress(Ipv4Address::GetAny(), port2));
        PacketSinkHelper sinkHelper2("ns3::TcpSocketFactory", sinkAddress2);
        ApplicationContainer sinkApp2 = sinkHelper2.Install(CSMA1_nodes.Get(0));
        sinkApp2.Start(Seconds(1.0));
        sinkApp2.Stop(Seconds(16.0));

        // OnOffHelper n8
        OnOffHelper onOffHelper2 ("ns3::TcpSocketFactory", Address ());
        onOffHelper2.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
        onOffHelper2.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
        onOffHelper2.SetAttribute("PacketSize",UintegerValue(5000));

        clientApps1.Start(Seconds(3.0));
        clientApps1.Stop(Seconds(15.0));
        
        // AppContainer n8
        ApplicationContainer clientApps2;
        AddressValue remoteAddress2 (InetSocketAddress (CSMA1_interfaces.GetAddress(0), port2));
        onOffHelper2.SetAttribute("Remote", remoteAddress2);
        clientApps2.Add(onOffHelper2.Install(CSMA2_nodes.Get(1)));

        clientApps2.Start(Seconds(2.0));
        clientApps2.Stop(Seconds(9.0));
        
		Ipv4GlobalRoutingHelper::PopulateRoutingTables();


       starN5.EnablePcapAll ("task1-configuration1-n5.pcap");
       CSMA1.EnablePcap("task1-configuration1-n0.pcap", CSMA1_ND.Get(0), true);
    }

    if (configuration == 2) {
        // Configurazione EchoServer/Client sui nodi n2 <- n8
        NS_LOG_INFO ("Configuring EchoServer/Client");
        UdpEchoServerHelper echoServer(63);

        ApplicationContainer serverApps = echoServer.Install(CSMA1_nodes.Get(2));
        serverApps.Start(Seconds(1.0));
        serverApps.Stop(Seconds(10.0));

        UdpEchoClientHelper echoClient(CSMA1_interfaces.GetAddress(2), 63);
        echoClient.SetAttribute("MaxPackets", UintegerValue(5));
        echoClient.SetAttribute("Interval", TimeValue(Seconds(2.0)));
        echoClient.SetAttribute("PacketSize", UintegerValue(2560));
    
        ApplicationContainer clientApps = echoClient.Install(CSMA2_nodes.Get(1));
        clientApps.Start(Seconds(3.0));
        clientApps.Stop(Seconds(11.0));

        starN5.EnablePcapAll("star test");
        CSMA2.EnablePcap("csma2 test", CSMA2_ND.Get(1), true);

        // Configurazione TCP sink e TCP onOffClient n5 <- n9
        NS_LOG_INFO ("Configuring TCP sink on n5");
        uint16_t port1 = 2300;
        Address sinkAddress1(InetSocketAddress(Ipv4Address::GetAny(), port1))
        PacketSinkHelper sinkHelper1("ns3::TcpSocketFactory", sinkAddress1);
        ApplicationContainer sinkApp = sinkHelper1.Install(star.GetHub());
        sinkApp.Start(Seconds 3.0);
        sinkApp.Stop(Seconds 9.0);

        // OnOffHelper n9
        OnOffHelper onOffHelper1 ("ns3::TcpSocketFactory", Address ());
        onOffHelper1.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
        onOffHelper1.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
        onOffHelper1.SetAttribute("PacketSize",UintegerValue(3000));


        // AppContainer n9
        ApplicationContainer clientApps1;
        AddressValue remoteAddress1 (InetSocketAddress (star.GetHubIpv4Address (2), port1));
        onOffHelper1.SetAttribute("Remote", remoteAddress1);
        clientApps1.Add(onOffHelper1.Install(CSMA2_nodes.Get(2)));


        // Configurazione UDP sink e UDP OnOffClient n0 <- n8
        NS_LOG_INFO ("Configuring TCP sink on n5");
        uint16_t port2 = 7454;
        Address sinkAddress2(InetSocketAddress(Ipv4Address::GetAny(), port2))
        PacketSinkHelper sinkHelper2("ns3::UdpSocketFactory", sinkAddress2);
        ApplicationContainer sinkApp = sinkHelper2.Install(star.GetHub());
        sinkApp.Start(Seconds 5.0);
        sinkApp.Stop(Seconds 15.0);

        // OnOffHelper n8
        OnOffHelper onOffHelper2 ("ns3::TcpSocketFactory", Address ());
        onOffHelper2.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
        onOffHelper2.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
        onOffHelper2.SetAttribute("PacketSize",UintegerValue(3000));


        // AppContainer n8
        ApplicationContainer clientApps2;
        AddressValue remoteAddress2 (InetSocketAddress (CSMA1_interfaces.GetAddress(0), port2));
        onOffHelper2.SetAttribute("Remote", remoteAddress2);
        clientApps2.Add(onOffHelper2.Install(CSMA2_nodes.Get(1)));





		Ipv4GlobalRoutingHelper::PopulateRoutingTables();


       starN5.EnablePcapAll ("task1-configuration1-n5.pcap");
       CSMA1.EnablePcap("task1-configuration1-n0.pcap", CSMA1_ND.Get(0), true);
    }

    
    // AnimationInterface anim("animation.xml");
    // anim.SetConstantPosition(CSMA1_nodes.Get(0), 1.0, 1.0);
    // anim.SetConstantPosition(CSMA1_nodes.Get(1), 1.0, 1.0);

	Simulator::Run();
    Simulator::Destroy();

    return 0;
}
