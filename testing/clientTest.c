#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>

int main(int argc, char const *argv[]) {
    int sockD = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port
            = htons(8080); // use some unused port number
    servAddr.sin_addr.s_addr = INADDR_ANY;

    int connectStatus
            = connect(sockD, (struct sockaddr *) &servAddr,
                      sizeof(servAddr));
    printf("CLIENT1\n");
    if (connectStatus == -1) {
        printf("Error...\n");
    } else {
        printf("CLIENT2\n");
        char strData[255] = "Oui";

        send(sockD, strData, sizeof(strData), 0);

        printf("CLIENT3\n");

        recv(sockD, strData, sizeof(strData), 0);

        printf("Message: %s\n", strData);
    }

    return 0;
}