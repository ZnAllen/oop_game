#include <enet/enet.h>
#include <iostream>
#include <unordered_map>

std::unordered_map<int, ENetAddress> hosts; // 存儲房主資訊

int main() {
    if (enet_initialize() != 0) {
        std::cerr << "ENet 初始化失敗！\n";
        return EXIT_FAILURE;
    }

    ENetAddress address;
    ENetHost* server;
    ENetEvent event;

    // 設定伺服器監聽的 IP 和 Port
    address.host = ENET_HOST_ANY;
    address.port = 1234;

    // 建立伺服器
    server = enet_host_create(&address, 32, 2, 0, 0);
    if (!server) {
        std::cerr << "server build failed!\n";
        return EXIT_FAILURE;
    }

    std::cout << "server is activate, waiting for client...\n";

    while (true) {
        ENetEvent event;
        while (enet_host_service(server, &event, 1000) > 0) {
            if (event.type == ENET_EVENT_TYPE_CONNECT) {
                std::cout << "player connected\n";
            } else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                std::string msg(reinterpret_cast<char*>(event.packet->data));

                if (msg == "REGISTER_HOST") {
                    hosts[event.peer->incomingPeerID] = event.peer->address;
                    std::cout << "Homeowner has registered\n";
                } else if (msg == "FIND_HOST") {
                    if (!hosts.empty()) {
                        auto it = hosts.begin();
                        std::string response = "HOST " + std::to_string(it->second.host) + " " + std::to_string(it->second.port);
                        ENetPacket* packet = enet_packet_create(response.c_str(), response.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                        enet_peer_send(event.peer, 0, packet);
                    } else {
                        std::string response = "NO_HOST";
                        ENetPacket* packet = enet_packet_create(response.c_str(), response.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                        enet_peer_send(event.peer, 0, packet);
                    }
                }

                enet_packet_destroy(event.packet);
            } else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
                hosts.erase(event.peer->incomingPeerID);
                std::cout << "player disconnected\n";
            }
        }
    }

    enet_host_destroy(server);
    enet_deinitialize();
    return EXIT_SUCCESS;
}
