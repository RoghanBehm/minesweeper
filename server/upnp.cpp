#include <miniupnpc/miniupnpc.h>
#include <miniupnpc/upnpcommands.h>
#include <iostream>
#include <string>

bool tryUpnpMapping(unsigned short port, const std::string &description) {
    int error = 0;
    // Discover routers on the network
    UPNPDev* devlist = upnpDiscover(
        2000,      // timeout in milliseconds
        nullptr,   // multicast interface, default nullptr
        nullptr,   // minissdpd socket, default nullptr
        0,         // source port
        0,         // sameport
        2,         // TTL
        &error
    );
    if (!devlist) {
        std::cerr << "UPnP discovery failed, error code: " << error << std::endl;
        return false;
    }

    UPNPUrls urls;
    IGDdatas igdData;
    char lanAddress[64]; // local LAN IP of the gateway

    int r = UPNP_GetValidIGD(devlist, &urls, &igdData, lanAddress, sizeof(lanAddress));
    if (r != 1) {
        std::cerr << "No valid Internet Gateway Device found.\n";
        FreeUPNPDevlist(devlist);
        return false;
    }

    // We have a valid IGD, attempt to add the port mapping
    char portStr[16];
    snprintf(portStr, sizeof(portStr), "%hu", port);

    // This call attempts to map external port `port` -> internal port `port` on our LAN IP
    // Protocol: TCP
    // Duration: 0 => "as long as possible"
    // Note: 'lanAddress' is the local IP we discovered from the IGD
    int result = UPNP_AddPortMapping(
        urls.controlURL,
        igdData.first.servicetype,
        portStr,       // external port
        portStr,       // internal port
        lanAddress,    // internal client (our local IP)
        description.c_str(), 
        "TCP", 
        nullptr, 
        "0"
    );

    if (result != UPNPCOMMAND_SUCCESS) {
        std::cerr << "UPnP port mapping failed, code: " << result << std::endl;
    } else {
        std::cout << "UPnP port mapping added: " 
                  << lanAddress << ":" << port << " => external port " 
                  << port << std::endl;
    }

    // Clean up
    FreeUPNPDevlist(devlist);
    FreeUPNPUrls(&urls);
    return (result == UPNPCOMMAND_SUCCESS);
}

