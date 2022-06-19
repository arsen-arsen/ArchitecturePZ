#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace std;

int main() {
    int sock;
    struct sockaddr_in addr;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    
    char mess[10];
    std::string response, x;

    while (true) {
        std::cout << "Enter command" << std::endl;
        std::cout << "Available commands:\n Moving, Rotate, Cleaning" << std::endl;
        std::cin >> response;

        if (response == "Moving")
        {
            std::cout << "Set distance:\n";
            std::cin >> x;
        }

        if (response == "Rotate")
        {
            std::cout << "Set angle:\n";
            std::cin >> x;
        }

        response = response + " " + x;
        strcpy(mess, response.c_str());
        sendto(sock, mess, sizeof(mess), 0, (struct sockaddr*)&addr, sizeof(addr));
    }
    close(sock);
    return 0;
}
