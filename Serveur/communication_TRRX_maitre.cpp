#include <iostream>
#include <socket>
#include <netinet>
#include <string>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(8080);

    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(sockfd, 5);

    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    int newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);

    char buffer[256];
    memset(buffer, 0, 256);

    int n = recv(newsockfd, buffer, 255, 0);
    std::cout << "Received: " << buffer << std::endl;

    memset(buffer, 0, 256);
    strcpy(buffer, "Hello from the master");

    n = send(newsockfd, buffer, strlen(buffer), 0);

    close(newsockfd);
    close(sockfd);

    return 0;
}
