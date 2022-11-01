#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"



//                  TOPOLOGY
//
//                    n4
//                  / | \ 
//  n0  n1  n2----n3--n5--n7    n8  n9  
//  |   |   |       \ | /  |    |   |
//  =========         n6   ==========
//
//
//




using namespace ns3;


NS_LOG_COMPONENT_DEFINE ("task1");

int
main (int argc, char *argv[])
{
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

    Time::SetResolution (Time::NS);

    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    NS_LOG_INFO ("Install internet stack on all nodes.");
    InternetStackHelper internet;

    // nodes 0, 1, 2
    NodeContainer CSMA1_nodes;
    CSMA1_nodes.Create(3);

    CsmaHelper CSMA1;
    CSMA1.SetChannelAttribute ("DataRate", StringValue ("25Mbps"));
    CSMA1.SetChannelAttribute ("Delay", StringValue ("10us"));
    
    NetDeviceContainer CSMA1_ND = CSMA1.Install(CSMA1_nodes);

    internet.Install(CSMA1_nodes);

    // nodes 2 and 3
    NodeContainer n2n3_nodes;
    n2n3_nodes.Add(CSMA1_nodes.Get(2));
    n2n3_nodes.Create (1);

    PointToPointHelper n2n3;
    n2n3.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
    n2n3.SetChannelAttribute ("Delay", StringValue ("15us"));

    NetDeviceContainer n2n3_ND = n2n3.Install(n2n3_nodes);

    internet.Install(n2n3_nodes.Get(1));
    
	Simulator::Run();
    Simulator::Destroy();
    return 0;
}



