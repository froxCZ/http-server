#include "CDomain.h"
#include "CConfig.h"
string CDomain::mServerRootPath;
/**
 * Creates CDomain record for G_Config->mDomainsList
 * @param domainName 
 * @param domainPath 
 */
CDomain::CDomain(const string& domainName, const string& domainPath) {
    mDomainName = domainName;
    mDomainPath = domainPath;
    mDomainLogger = NULL;
}
/**
 * Creates CDomain record for G_Config->mDomainsList
 * This constructor is used by alias, thus no need to specify domain Path, that is handled in CAlias constructor
 * @param domainName
 */
CDomain::CDomain(const string& domainName) {
    mDomainName = domainName;
    mDomainLogger = NULL;
}
/**
 * Not Used
 */
CDomain::CDomain() {
    mDomainLogger = NULL;    
}
/**
 * Deletes Domain Logger. In CAlias mDomainLogger is always NULL. CAlias uses his parent CDomain logger.
 */
CDomain::~CDomain() {    
    delete mDomainLogger;
}

void CDomain::SetDomainName(const string& domainName) {
    mDomainName = domainName;
}

void CDomain::SetDomainPath(const string& domainPath) {
    mDomainPath = domainPath;
}

string CDomain::GetDomainPath()const {
    return mDomainPath;
}

/**
 * Creates CLogger instance for domain in logs/domainName.log
 */
void CDomain::OpenLogger() {
    if (mDomainLogger == NULL) {
        mDomainLogger = new CLogger(mServerRootPath + "logs/" + GetDomainName() + ".log", G_Config->DOMAIN_FILE_LOG_LEVEL, G_Config->DOMAIN_CONSOLE_LOG_LEVEL);        
    }
}

/**
 * Logs message to logs/domainName.log
 * @param logMessage
 * @param logLevel
 */
void CDomain::LogMessage(const string logMessage, const int logLevel) {
    mDomainLogger->append(logMessage, logLevel);
}

string CDomain::GetDomainName()const {
    return mDomainName;
}

/**
 * Derived from CDomain. Has a pointer to main domain to get data path
 * @param aliasName
 * @param mainDomain pointer to main domain
 */
CAlias::CAlias(const string& aliasName, CDomain* mainDomain) :  CDomain(aliasName),mMainDomain(mainDomain) {
    ;
}

/**
 * Uses main domain pointer to get data path
 * @return data path
 */
string CAlias::GetDomainPath()const {
    return mMainDomain->CDomain::GetDomainPath();
}

/**
 * Sets main domain path
 * @param domainPath
 */
void CAlias::SetDomainPath(const string& domainPath) {
    mMainDomain->CDomain::SetDomainPath(domainPath);
}

/**
 * Save message to main domain log
 * @param logMessage
 * @param logLevel
 */
void CAlias::LogMessage(const string logMessage, const int logLevel) {
    mMainDomain->CDomain::LogMessage(logMessage, logLevel);
}
