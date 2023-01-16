#include <iostream>
#include <socket>
#include <netinet>
#include <string>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8080);

    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    char buffer[256];
    memset(buffer, 0, 256);

    strcpy(buffer, "Hello from the slave");

    int n = send(sockfd, buffer, strlen(buffer), 0);

    memset(buffer, 0, 256);
    n = recv(sockfd, buffer, 255, 0);
    std::cout << "Received: " << buffer << std::endl;

    close(sockfd);

    return 0;
}
