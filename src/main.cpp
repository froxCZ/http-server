#include "main.h"

using namespace std;

/**
* @mainpage 
* @par Introduction: 
* Documentation for a semestral project HTTP Server \n
 * * All configurations are in server root file server.conf\n
 * * Supports multiple domains and aliases on one port (configurable)\n
 * * Supports following headers: GET, HEAD,POST,Host, User-Agent,If-None-Match(ETag)\n
 * * File cache (balanced tree for quick files search) on server side and ETag on clients side (configure file cache time)\n
 * * Log file for each domain\n
 * * Multiple threads (configurable)\n
 * 
 * 
* @author Vojtěch Udržal
*/


/**
 * @brief Server exit function.
 * Stop _ALL_ the threads! Free _ALL_ the memory! Close listen socket.
 * @param sig
 */
void ServerExit(int sig) {
    G_ServerLogger->append("Server shutting down.", INFO_LOG);
    for (int i = 0; i < G_Config->THREADS_CNT; i++) {
        G_Threads[i]->Stop();
        delete G_Threads[i];
    }
    //must be here.. had problems with forward declaration destructor when it was in CConfig (CDomain was declared forward)
    int iDomain;
    for (iDomain = 0; iDomain < G_Config->mDomainsCnt; iDomain++) {
        delete G_Config->mDomainsList[iDomain];
    }
    delete[]G_Threads;
    delete G_ServerLogger;
    delete G_Config;
    delete CFile::mCacheQue;
    delete CFile::mCacheFilesTreap;
    listenSocket->Close();
    delete listenSocket;
    exit(0);
    //(void) signal(SIGINT, SIG_DFL);
}

/**
 * Inits signal catcher\n
 * Loads config file and process it\n
 * Opens server logger\n
 * Prepares listen socket\n
 * Runs threads (depending on Threads value in config file)\n
 * Starts listening\n
 * On new connection, inserts visitor to threads waiting que and keeps listening. Meanwhile, thread serves the visitor.\n
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char** argv) {
    (void) signal(SIGINT, ServerExit);
    G_Config = new CConfig();
    string serverPath = string(argv[0]);
    G_Config->SERVER_ROOT_PATH = serverPath.substr(0,serverPath.find_last_of('/')+1)+"examples/";
    //G_Config->SERVER_ROOT_PATH = string("/home/frox/Dropbox/Dropbox/private/PA2/HTTP_SERVER/udrzavoj/src/");
    G_ServerLogger = new CLogger(G_Config->SERVER_ROOT_PATH + "logs/server.log", FATAL_LOG | ERROR_LOG | WARNING_LOG | INFO_LOG | DEBUG_LOG, FATAL_LOG | ERROR_LOG | WARNING_LOG | INFO_LOG | DEBUG_LOG | DEBUG_LOG2);
    G_Config->LoadConfig(G_Config->SERVER_ROOT_PATH + "server.conf");
    G_ServerLogger->ChangeLogLevel(G_Config->SERVER_FILE_LOG_LEVEL,G_Config->SERVER_CONSOLE_LOG_LEVEL);
    G_ServerLogger->append("Config file loaded.", INFO_LOG);
    CDomain::mServerRootPath=G_Config->SERVER_ROOT_PATH;

    listenSocket = new CListenSocket("0", intToString(G_Config->PORT)); //"0" - any address, port must be string    
    cout << "Cache expiration time: " << G_Config->FILE_CACHE_EXPIRATION_TIME << endl;
    cout << "Threads cnt: " << G_Config->THREADS_CNT << endl;
    //CThread** threads;
    sem_t visitorQueSem;
    sem_init(&visitorQueSem, 0, 0);
    G_Threads = new CThread*[G_Config->THREADS_CNT];
    int i;
    CThread::SetMQueSem_t(&visitorQueSem);
    for (i = 0; i < G_Config->THREADS_CNT; i++) {
        G_Threads[i] = new CThread(i);
        G_Threads[i]->Run();
    }
    try {
        listenSocket->Launch();
    } catch (CErrorException e) {
        G_ServerLogger->append(e.GetErrorMessage(), ERROR_LOG);
        G_ServerLogger->append("Starting server failed.", ERROR_LOG);
        exit(e.GetErrorCode());
    }
    int inSocket;
    struct sockaddr_in inAddr;
    while (true) {
        inSocket = listenSocket->AcceptConn(inAddr);
        CThread::InsertToQue(new CVisitor(inSocket, inAddr));
    }
    ServerExit(0);
    return 0;
}

