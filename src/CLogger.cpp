#include "CLogger.h"
CLogger* G_ServerLogger;
/**
 * 
 * @param filePath FilePath
 * @param logLevel What will be logged to file
 * @param consoleLevel What will be logged to console
 */
CLogger::CLogger(const string filePath,const int logLevel,const int consoleLevel) {
    mLogFile=new ofstream(filePath.c_str(),ios::out | ios::app);
    mFileLogLevel = logLevel;
    mConsoleLevel = consoleLevel;
    mLogFile->write((char *) "\n", 1);
    mLogFile->flush();
}
/**
 * Changes log levels according to parameters
 * @param fileLevel
 * @param consoleLevel
 */
void CLogger::ChangeLogLevel(int fileLevel,int consoleLevel){
    mFileLogLevel=fileLevel;
    mConsoleLevel=consoleLevel;
}
/**
 * Logs or prints the logMsg to file or console, depending on logLevel and mFileLogLevel and mConsoleLogLevel
 * @param logMsg Message to log
 * @param logLevel Message level 
 */
void CLogger::append(const string logMsg,const int logLevel) {
    string longLogMsg;
    string timeStr = getDate();    
    longLogMsg = "[" + timeStr + "] [" + getLogLevelName(logLevel) + "] " + logMsg + "\n"; //set some log layout
    if (logLevel & mFileLogLevel) {
        mLogFile->write((char *) longLogMsg.c_str(), longLogMsg.length());
        mLogFile->flush();
    }
    if (logLevel & mConsoleLevel) {
        cout << longLogMsg;
    }
}


/**
 * Returns logLevel # name
 * @param logLevel 
 * @return logLevel name in string
 */
string CLogger::getLogLevelName(int logLevel)const {
    switch (logLevel) {
        case 0: return "";
        case 1: return "FATAL";
        case 2: return "ERROR";
        case 4: return "WARNING";
        case 8: return "INFO";
        case 16: return "DEBUG";
        case 32: return "NETWORK";
        case 64: return "NETWORK";
        case 128: return "NETWORK";
        case 256: return "DEBUG2";
    }
    return "UNKNOWN";
}
