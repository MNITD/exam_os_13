//
// Created by bogdan on 06.06.17.
//

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

void error(const std::string &m) {
    std::string message("[server] ERROR " + m);
    perror(message.c_str());
    exit(1);
}

void log(const std::string &m) {
    printf("[server] %s \n", m.c_str());
}

std::string get_current_time() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::string time;
    time += std::to_string(ltm->tm_hour);
    time += ":";
    time += std::to_string(ltm->tm_min);
    time += ":";
    time += std::to_string(ltm->tm_sec);
    return time;
}

std::string get_current_date() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::string date;
    date += std::to_string(ltm->tm_mday);
    date += ".";
    date += std::to_string(1 + ltm->tm_mon);
    date += ".";
    date += std::to_string(1900 + ltm->tm_year);
    return date;
}

int main() {
    int sockfd, newsockfd, portno, pid;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    std::string response;
    std::vector<std::string> log_list;

    /* Create socket file descriptor */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        error("opening socket");
    else
        log("Has opened socket");


    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = 5002; // default port

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Bind the host address with file descriptor.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("on binding");
    else
        log("Has bind host address");

    /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
      * place all incoming connection into a backlog queue
      * until accept() call accepts the connection.
      * set the maximum size for the backlog queue to 5
   */
    listen(sockfd, 10);
    clilen = sizeof(cli_addr);

    while (true) {
        /* Accept actual connection from the client
            * returns a new socket file descriptor for the accepted connection.
            * the original socket file descriptor can continue to be used
         */
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0) {
            error("on accept");
        }

        char str[INET_ADDRSTRLEN];
        std::string address = inet_ntop(AF_INET, &(cli_addr.sin_addr), str, INET_ADDRSTRLEN);
        log_list.push_back(address + "::"+ std::to_string(portno) + " " + get_current_time()+ " " +  get_current_date());

        printf("[server] Got connection from %s\n", log_list.back().c_str());

        response = "Hello";

        if (write(newsockfd, response.c_str(), response.size()) < 0) {
            error("writing to socket");
        }

        pid = fork();

        if(pid < 0){
            error("on fork");
        } else if(pid != 0){ //parent process
            close(newsockfd);
        } else { //client process
            close(sockfd);

            /* Start communicating */
            bzero(buffer, 256);

//            if (read(newsockfd, buffer, 255) < 0) {
//                error("reading from socket");
//            }
//
//            printf("received message: %s\n",buffer);

            /* Write a response to the client
                * equal to send() without any flags as last arg
             */
            response = "create separate process";

            if (write(newsockfd, response.c_str(), response.size()) < 0) {
                error("writing to socket");
            }
            sleep(10000);
            return 0;
        }
    }

}