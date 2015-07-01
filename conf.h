#ifndef _CONF_H_
#define _CONF_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <sys/select.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 1783

void server();
void clientA();
void clientB();

#endif // _CONF_H_
