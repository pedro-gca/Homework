#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"

// Default Network Topology
//
//       
//                                                             
//

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("FirstScriptExample");

int
main(int argc, char* argv[])
{
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

    uint32_t nCsma = 3;
//log mancante
    nCsma = nCsma == 0 ? 1 : nCsma; 
    NodeContainer csmaNodes;
    csmaNodes.Create(nCsma); 

    NodeContainer p2pnodes;
    p2pnodes.Add(csmaNodes.Get(2));
    p2pnodes.Create(1);
    
    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", StringValue("25Mbps"));
    csma.SetChannelAttribute("Delay", StringValue("10us"));

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("15us"));
    
    NetDeviceContainer csmaDevices;
    csmaDevices = csma.Install(csmaNodes);

    NetDeviceContainer p2pdevices;
    p2pdevices = pointToPoint.Install(p2pnodes);

    InternetStackHelper stack;
    stack.Install(csmaNodes);
    stack.Install(p2pnodes);
    
    Ipv4AddressHelper address;
    address.SetBase("192.128.1.0", "255.255.255.0");
    Ipv4InterfaceContainer csmaInterfaces;
    csmaInterfaces = address.Assign(csmaDevices);
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign(p2pdevices);
 
     

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
