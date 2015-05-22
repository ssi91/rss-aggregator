#ifndef SERVSOCK_H
#define SERVSOCK_H
#include <pthread.h> 
#include <sys/types.h> 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include "fcgi_config.h" 
#include "fcgiapp.h" 
#include "../dir.h"

#define THREAD_COUNT 1
#define SOCKET_PATH "127.0.0.1:9000" 
using namespace std;
//хранит дескриптор открытого сокета 
//int socketId;

bool getGetParam(char *to, const char *paramName, FCGX_Request *request);

void *doit(void *a);
#endif
