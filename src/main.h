/* 
 * File:   main.h
 * Author: frox
 *
 * Created on May 13, 2012, 8:40 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <cstdlib>

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <unistd.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "CLogger.h"
#include "CBuffer.h"
#include "CFile.h"
#include "CConfig.h"
#include "CExceptions.h"
#include "CListenSocket.h"
#include "CVisitor.h"
#include "CThread.h"

#include "constants.h"
void ServerExit(int sig);
int main(int argc, char** argv);

CListenSocket* listenSocket;
CThread** G_Threads;///< CThreads array


#endif	/* MAIN_H */

