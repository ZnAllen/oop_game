#include <enet/enet.h>
#include <iostream>
#include "GameMap.h"

int main() {
    if (enet_initialize() != 0) {
        std::cerr << "ENet failed\n";
        return EXIT_FAILURE;
    }

    ENetHost* client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!client) {
        std::cerr << "client create failed\n";
        return EXIT_FAILURE;
    }

    ENetAddress address;
    ENetPeer* peer;
    enet_address_set_host(&address, "127.0.0.1");
    address.port = 1234;

    peer = enet_host_connect(client, &address, 2, 0);
    if (!peer) {
        std::cerr << "Can't connect\n";
        return EXIT_FAILURE;
    }

    std::cout << "connect to server...\n";

    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "connent to server successfully!\n";

        const char* message = "REGISTER_HOST";
        ENetPacket* packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
        enet_host_flush(client);
    } else {
        std::cerr << "connect failed\n";
    }

    enet_host_destroy(client);
    enet_deinitialize();
    return 0;
}
