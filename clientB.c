#include "conf.h"

int main(void)
{
    clientB();
    return EXIT_SUCCESS;
}

void server()
{
    int fd ;
    struct sockaddr_in localAddress ;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(fd >= 0 );

    localAddress.sin_port = htons(SERVER_PORT);
    localAddress.sin_addr.s_addr = INADDR_ANY;
    localAddress.sin_family = AF_INET;

    int ret ;

    ret = bind(fd, (struct sockaddr *)&localAddress, sizeof(localAddress) );
    assert(ret == 0);

    while ( 1 )
    {
        struct sockaddr_in remoteAddr;
        socklen_t remoteAddrLen = sizeof(struct sockaddr_in);
        char bufferRecive[BUFSIZ]="";
        char bufferSend[BUFSIZ]="";

        ssize_t sz= recvfrom(fd, bufferRecive, BUFSIZ, 0,
                 (struct sockaddr *)&remoteAddr,&remoteAddrLen);
        assert(sz > 0);
        const char *ip = inet_ntoa(remoteAddr.sin_addr);
        sprintf(bufferSend, "%s %d\n", ip, ntohs(remoteAddr.sin_port));
        sz = recvfrom(fd, bufferRecive, BUFSIZ, 0,
                 (struct sockaddr *)&remoteAddr,&remoteAddrLen);
        assert(sz >0);
        sendto(fd, bufferSend, BUFSIZ, 0,
               (const struct sockaddr *)&remoteAddr,remoteAddrLen);
    }
}

void clientA()
{
    int fd ;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in remoteAddr;
    socklen_t remoteAddrLen = sizeof(struct sockaddr_in);
    char bufferRecive[BUFSIZ]="";
    char bufferSend[BUFSIZ]="Hello , this is a test package.";

    remoteAddr.sin_family = AF_INET;
    int ret ;

    ret = inet_aton(SERVER_ADDR, &remoteAddr.sin_addr);
    assert(ret !=0);

    remoteAddr.sin_port = htons(SERVER_PORT);

    ssize_t sz = sendto(fd, bufferSend, BUFSIZ, 0,
             (struct sockaddr *)&remoteAddr,remoteAddrLen);
    assert(sz >0);
    sleep(5);

    while ( 1 )
    {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        FD_SET(fd, &fds);

        ret = select(fd+1, &fds, 0, 0, 0);

        assert(ret >=0 );

        if ( FD_ISSET(fd, &fds) ) {
            sz = recvfrom(fd, bufferRecive, BUFSIZ, 0,
                  (struct sockaddr *)&remoteAddr,&remoteAddrLen);
            assert(sz >0);
            sz = write(STDIN_FILENO, bufferRecive, sz);
            assert(sz >0);
        }
        if ( FD_ISSET(STDIN_FILENO, &fds) ) {
            sz = read(STDIN_FILENO, &bufferSend, BUFSIZ);
            assert(sz >0);
            sz = sendto(fd, bufferSend, sz, 0,
                   (const struct sockaddr *)&remoteAddr, remoteAddrLen);
            assert(sz >0);
        }
    }
}

void clientB()
{
    int fd ;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in remoteAddr;
    socklen_t remoteAddrLen = sizeof(struct sockaddr_in);
    char bufferRecive[BUFSIZ]="";
    char bufferSend[BUFSIZ]="Hello , this is a test package.";

    remoteAddr.sin_family = AF_INET;
    int ret ;

    ret = inet_aton(SERVER_ADDR, &remoteAddr.sin_addr);
    assert(ret !=0);

    remoteAddr.sin_port = htons(SERVER_PORT);

    ssize_t sz = sendto(fd, bufferSend, BUFSIZ, 0,
             (struct sockaddr *)&remoteAddr,remoteAddrLen);
    assert(sz >0);

    sz = recvfrom(fd, bufferRecive, BUFSIZ, 0,
                  (struct sockaddr *)&remoteAddr,&remoteAddrLen);

    char ip[BUFSIZ];
    int  port ;

    sscanf(bufferRecive, "%s %d", ip, &port);
    ret = inet_aton( ip, &remoteAddr.sin_addr);
    assert(ret !=0);

    remoteAddr.sin_port = htons(port);

//    strcpy(bufferSend, "Hello , this package is from B.");

//    sz = sendto(fd, bufferSend, BUFSIZ, 0,
//             (struct sockaddr *)&remoteAddr,remoteAddrLen);
    while ( 1 )
    {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        FD_SET(fd, &fds);

        ret = select(fd+1, &fds, 0, 0, 0);

        assert(ret >=0 );

        if ( FD_ISSET(fd, &fds) ) {
            sz = recvfrom(fd, bufferRecive, BUFSIZ, 0,
                  (struct sockaddr *)&remoteAddr,&remoteAddrLen);
            assert(sz >0);
            sz = write(STDIN_FILENO, bufferRecive, sz);
            assert(sz >0);
        }
        if ( FD_ISSET(STDIN_FILENO, &fds) ) {
            sz = read(STDIN_FILENO, &bufferSend, BUFSIZ);
            assert(sz >0);
            sz = sendto(fd, bufferSend, sz , 0,
                   (const struct sockaddr *)&remoteAddr, remoteAddrLen);
            assert(sz >0);
        }
    }
    assert(sz >0);
}
