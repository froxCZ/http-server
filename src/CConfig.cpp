
#include "CConfig.h"
using namespace std;
string CConfig::SERVER_NAME = "MY HTTP SERVER";
string CConfig::TIME_ZONE = "CEST";
CConfig* G_Config;

/**
 * Creates instance. Default port number is 8060 and threads count 1
 */
CConfig::CConfig() {
    mDomainsCnt = 0;
    mAliasesCnt = 0;
    mLine = 1;
    PORT = 8060;
    THREADS_CNT = 1;
    SERVER_CONSOLE_LOG_LEVEL = SERVER_FILE_LOG_LEVEL = FATAL_LOG | ERROR_LOG | WARNING_LOG;
    SERVER_CONSOLE_LOG_LEVEL=INFO_LOG;
    DOMAIN_CONSOLE_LOG_LEVEL = DOMAIN_FILE_LOG_LEVEL = FATAL_LOG | ERROR_LOG | WARNING_LOG;
    DOMAIN_CONSOLE_LOG_LEVEL = INFO_LOG | NETWORK_LOG0 | NETWORK_LOG1;
}

/**
 * Opens config file and reads line by line. Call parseLine to process line.
 * @param filePath
 */
void CConfig::LoadConfig(const string filePath) {
    try {
        cout << "\nConfig file: "<<filePath<<endl;
        ifstream cfgFile(filePath.c_str(), ios::in);
        string line;
        while (!cfgFile.eof()) {
            if (!cfgFile.good() || cfgFile.fail())throw CErrorException(IOERROR, "IOERROR");
            getline(cfgFile, line);
            parseLine(line);
            mLine++;
        }
    } catch (CErrorException e) {
        if (e.GetErrorCode() == IOERROR) {
            G_ServerLogger->append("IOError while loading config file", FATAL_LOG);
        } else {
            string logMsg = intToString(e.GetErrorCode()) + ": " + e.GetErrorMessage();
            G_ServerLogger->append("Config Error #" + logMsg, FATAL_LOG);
        }
        exit(e.GetErrorCode());
    }
}

/**
 * Processes line. 
 * everything behind # is ignored
 * First word is keyword, then value
 * VirtualServer is multiline block config closed by DataFolder keyword on last line
 * @param lineended
 * @throw CErrorException on config file error
 */
void CConfig::parseLine(string line) {
    static CDomain* lastDomain = NULL;
    leaveOutComment(line);
    stringstream lineStream;
    lineStream << line;
    string tmpConfWord;
    lineStream >> tmpConfWord;
    if (tmpConfWord == "ListenPort") {
        lineStream >> tmpConfWord;
        PORT = atoi(tmpConfWord.c_str());
        if (PORT < 0 || PORT > 65535)throw CErrorException(CFGERROR, "Invalid port number\nLine " + intToString(mLine) + ": " + line);
    } else if (tmpConfWord == "ServerFileLogLevel") {
        SERVER_FILE_LOG_LEVEL=0;
        processLogLevel(lineStream, SERVER_FILE_LOG_LEVEL);
    } else if (tmpConfWord == "ServerConsoleLogLevel") {
        SERVER_CONSOLE_LOG_LEVEL=0;
        processLogLevel(lineStream, SERVER_CONSOLE_LOG_LEVEL);
    } else if (tmpConfWord == "DomainFileLogLevel") {
        DOMAIN_FILE_LOG_LEVEL=0;
        processLogLevel(lineStream, DOMAIN_FILE_LOG_LEVEL);
    } else if (tmpConfWord == "DomainConsoleLogLevel") {
        DOMAIN_CONSOLE_LOG_LEVEL=0;
        processLogLevel(lineStream, DOMAIN_CONSOLE_LOG_LEVEL);
    } else if (tmpConfWord == "Threads") {
        lineStream >> tmpConfWord;
        THREADS_CNT = atoi(tmpConfWord.c_str());
        if (THREADS_CNT < 1 || THREADS_CNT > 100)throw CErrorException(CFGERROR, "Invalid threads count.\nLine " + intToString(mLine) + ": " + line);
    } else if (tmpConfWord == "FileCacheTime") {
        lineStream >> tmpConfWord;
        FILE_CACHE_EXPIRATION_TIME = atoi(tmpConfWord.c_str());
        if (FILE_CACHE_EXPIRATION_TIME < 0)throw CErrorException(CFGERROR, "Invalid file cache expiration time\nLine " + intToString(mLine) + ": " + line);
    } else if (tmpConfWord == "ServerLog") {
        lineStream >> SERVER_LOG_PATH;
        if (SERVER_LOG_PATH[SERVER_LOG_PATH.length() - 1] == '/')throw CErrorException(CFGERROR, "Invalid server log path - path is not file.\nLine " + intToString(mLine) + ": " + line);
        tmpConfWord.clear();
        lineStream >> tmpConfWord;
        if (tmpConfWord.length())throw CErrorException(CFGERROR, "Invalid server log path\nLine: " + line);
    } else if (tmpConfWord == "NetworkLog") {
        lineStream >> NETWORK_LOG_PATH;
        if (NETWORK_LOG_PATH[NETWORK_LOG_PATH.length() - 1] == '/')throw CErrorException(CFGERROR, "Invalid network log path - path is not file.\nLine " + intToString(mLine) + ": " + line);
        tmpConfWord.clear();
        lineStream >> tmpConfWord;
        if (tmpConfWord.length())throw CErrorException(CFGERROR, "Invalid network log path\nLine " + intToString(mLine) + ": " + line);
    } else if (tmpConfWord == "VirtualServer") {
        tmpConfWord.clear();
        lineStream >> tmpConfWord;
        if (tmpConfWord.length())throw CErrorException(CFGERROR, "Invalid VirtualServer declaration\nLine " + intToString(mLine) + ": " + line);
        if (mDomainsCnt >= MAX_DOMAINS)throw CErrorException(CFGERROR, "Max domains or aliases allowed: 30\nLine " + intToString(mLine) + ": " + line);
        lastDomain = mDomainsList[mDomainsCnt++] = new CDomain();
    } else if (tmpConfWord == "Domain") {
        if (lastDomain == NULL)throw CErrorException(CFGERROR, "Declare VirtualServer keyword first.\nLine " + intToString(mLine) + ": " + line);
        tmpConfWord.clear();
        lineStream >> tmpConfWord;
        lastDomain->SetDomainName(tmpConfWord);
        tmpConfWord.clear();
        lineStream >> tmpConfWord;
        if (tmpConfWord.length())throw CErrorException(CFGERROR, "Invalid Domain declaration. Only one Domain name allowed per VirtualServer. Use 'Alias'\nLine " + intToString(mLine) + ": " + line);
    } else if (tmpConfWord == "Alias") {
        if (lastDomain == NULL)throw CErrorException(CFGERROR, "Declare VirtualServer keyword first.\nLine " + intToString(mLine) + ": " + line);
        tmpConfWord.clear();
        lineStream >> tmpConfWord;
        while (tmpConfWord.length()) {
            if (mDomainsCnt >= MAX_DOMAINS)throw CErrorException(CFGERROR, "Max domains or aliases allowed: 30\nLine " + intToString(mLine) + ": " + line);
            mDomainsList[mDomainsCnt++] = new CAlias(tmpConfWord, lastDomain);
            tmpConfWord.clear();
            lineStream >> tmpConfWord;
        }
    } else if (tmpConfWord == "DataFolder") {
        if (lastDomain == NULL)throw CErrorException(CFGERROR, "Declare VirtualServer keyword first.\nLine " + intToString(mLine) + ": " + line);
        tmpConfWord.clear();
        lineStream >> tmpConfWord;
        if (tmpConfWord[tmpConfWord.length() - 1] != '/')throw CErrorException(CFGERROR, "Invalid path - path is not directory.\nLine " + intToString(mLine) + ": " + line);
        lastDomain->SetDomainPath(tmpConfWord);
    }

}

/**
 * Loops domain list and returns domain pointer which equals domainName
 * @param domainName
 * @return domain pointer
 */
CDomain* CConfig::GetDomainByName(const string domainName) {
    int iDomain;
    CDomain* domain = NULL;
    for (iDomain = 0; iDomain < mDomainsCnt; iDomain++) {
        if (mDomainsList[iDomain]->GetDomainName() == domainName) {
            domain = mDomainsList[iDomain];
        }
    }
    return domain;
}

/**
 * Gets domain data path by domainName
 * @param domainName
 * @return domainPath
 */
string CConfig::GetDomainPathByName(const string domainName) {
    string domainPath = "";
    CDomain* domain = GetDomainByName(domainName);
    if (domain != NULL) domainPath = domain->GetDomainPath();
    return domainPath;
}

/**
 * Leaves comments from line
 * @param line from which comments gets removed
 */
void CConfig::leaveOutComment(string& line) {
    int len = line.length();
    if (len == 0)return;
    int commentStart = line.find('#');
    if (commentStart == -1)return;
    line = line.substr(0, commentStart);
}

/**
 * Process log level config
 * @param stream Stream log level values
 * @param log For what log
 */
void CConfig::processLogLevel(stringstream& stream, int& log) {
    string value;
    stream >> value;
    while (value.length()) {
        if (value == "FATAL") {
            log |= FATAL_LOG;
        } else if (value == "ERROR") {
            log |= ERROR_LOG;
        } else if (value == "WARNING") {
            log |= WARNING_LOG;
        } else if (value == "INFO") {
            log |= INFO_LOG;
        } else if (value == "DEBUG0") {
            log |= DEBUG_LOG;
        } else if (value == "DEBUG1") {
            log |= DEBUG_LOG2;
        } else if (value == "NETWORK0") {
            log |= NETWORK_LOG0;
        } else if (value == "NETWORK1") {
            log |= NETWORK_LOG1;
        } else if (value == "NETWORK2") {
            log |= NETWORK_LOG2;
        }
        value.clear();
        stream >> value;
    }
}
