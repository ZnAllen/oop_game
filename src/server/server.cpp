#include <enet/enet.h>
#include <iostream>

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
        while (enet_host_service(server, &event, 1000) > 0) {
            
        }
    }

    enet_host_destroy(server);
    enet_deinitialize();
    return EXIT_SUCCESS;
}
