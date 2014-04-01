/* 
 * File:   CConfig.h
 * Author: frox
 *
 * Created on April 14, 2012, 1:16 PM
 */

#ifndef CCONFIG_H
#define	CCONFIG_H

#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "CDomain.h"
#include "CExceptions.h"
using namespace std;

const string G_configFile = "";
#define IOERROR         1
#define CFGERROR        2
#define MAX_DOMAINS     30

class CDomain;

/**
 * \brief Class for all config data. Parses config file also.
 */
class CConfig {
public:
    CConfig();

    ~CConfig() {
//        int iDomain;
//        for (iDomain = 0; iDomain < mDomainsCnt; iDomain++) {
//            cout << "Domain delete " << iDomain << endl;
//            delete mDomainsList[iDomain];
//        }
    }
    void LoadConfig(const string filePath);
    CDomain* GetDomainByName(const string domainName);
    string GetDomainPathByName(const string domainName);
    int PORT;
    int FILE_CACHE_EXPIRATION_TIME;///< seconds file can stay in cache
    string SERVER_ROOT_PATH;
    string SERVER_LOG_PATH;
    string NETWORK_LOG_PATH;
    int THREADS_CNT; ///< how many threads to use
    int SERVER_FILE_LOG_LEVEL;
    int SERVER_CONSOLE_LOG_LEVEL;
    int DOMAIN_FILE_LOG_LEVEL;
    int DOMAIN_CONSOLE_LOG_LEVEL;
    static string SERVER_NAME;
    static string TIME_ZONE;
    int mDomainsCnt;
    CDomain* mDomainsList[30]; ///< CDomains array pointer
private:
    void parseLine(string line);
    void leaveOutComment(string& line);    
    void processLogLevel(stringstream& stream,int& log);
    int mAliasesCnt;
    int mLine;    ///< config file read lines counter (used for error messages in config file)

};

extern CConfig* G_Config;

#endif	/* CCONFIG_H */

