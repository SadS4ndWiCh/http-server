#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
    /*
        Create the socket.

        To create a socket, the `socket()` receives the DOMAIN and TYPE.
        - `AF_INET` domain means that the socket is using `IPv4`.
        - `SOCK_STREAM` type means that the socket is using `TCP` connection.
    */
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        fprintf(stderr, "ERROR: socket() failed to creates socket\n");
        return 1;
    }

    /*
        Set socket options.

        Is possible to chance some behaviours from the socket by calling the 
        `setsockopt()`.

        Setting the `SO_REUSEADDR`, the socket can be bind to same address with 
        differents ports.
    */
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))) {
        fprintf(stderr, "ERROR: setsockopt() failed to set `SO_REUSEADDR` option\n");
        return 1;
    }

    /*
        Define the server address struct.

        Is creating an IPv4 127.0.0.1:3000 address.
    */
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(3000),
        .sin_addr = { htonl(INADDR_LOOPBACK) },
    };

    /* Bind the socket to the address. */
    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
        fprintf(stderr, "ERROR: bind() failed to bind the socket to the address\n");
        return 1;
    }

    /*
        Start to receive connections.

        The `listen()` function allow to define how many connection requests can 
        stack before further requests are refused.
    */
    int request_stack = 5;
    if (listen(server_fd, request_stack) != 0) {
        fprintf(stderr, "ERROR: listen() failed to start receiving connections.\n");
        return 1;
    }

    struct sockaddr_in *client_addr;
    int client_addr_len;

    /* 
        Accept a new connection.

        When accept a request, the client address is assign to `client_addr` 
        struct variable.
    */
    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
    if (client_fd == -1) {
        fprintf(stderr, "ERROR: accept() failed to accept a new connection.\n");
        return 1;
    }

    char request[1024] = {0};
    int request_len = sizeof(request) / sizeof(request[0]);

    /* Receive the request data from the client. */
    if (recv(client_fd, request, request_len, 0) == -1) {
        fprintf(stderr, "ERROR: recv() failed to receive request data.\n");
        return 1;
    }

    char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html;\r\n\r\nHello World!";
    int response_len = sizeof(response) / sizeof(response[0]);

    /* Send the response to the client. */
    if (send(client_fd, response, response_len, 0) == -1) {
        fprintf(stderr, "ERROR: send() failed to send response to client.\n");
        return 1;
    }

    close(server_fd);
    return 1;
}