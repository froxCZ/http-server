/* 
 * File:   CLogger.h
 * Author: frox
 *
 * Created on May 4, 2012, 7:24 PM
 */

#ifndef CLOGGER_H
#define	CLOGGER_H
#include <string>
#include <fstream>
#include <iostream>
#include <time.h>

#include "constants.h"

using namespace std;
/**
 * \brief Logs messages to specified files
 */
class CLogger {
public:
    CLogger(const string logFilePath, const int logLevel = FATAL_LOG | ERROR_LOG, const int consoleLevel = FATAL_LOG | ERROR_LOG | WARNING_LOG | INFO_LOG | NETWORK_LOG0);

    ~CLogger() {
        mLogFile->close();
        delete mLogFile;
    }
    void ChangeLogLevel(int fileLevel,int consoleLevel);
    void append(const string msg, const int logLevel);
private:
    int mFileLogLevel;///< value which logs to save to file
    int mConsoleLevel;///< value which logs to print to console
    ofstream * mLogFile;
    string getLogLevelName(const int logLevel)const;
};

extern CLogger* G_ServerLogger;

#endif	/* CLOGGER_H */

