/* 
 * File:   CDomain.h
 * Author: frox
 *
 * Created on May 4, 2012, 7:43 PM
 */

#ifndef CDOMAIN_H
#define	CDOMAIN_H
#include <string>

#include "constants.h"
#include "CLogger.h"
class CConfig;
/**
 * \brief Domain info
 */
class CDomain {
public:
    CDomain(const string& domainName, const string& domainPath);
    CDomain(const string& domainName);
    CDomain();
    virtual ~CDomain();
    string GetDomainName()const;
    void SetDomainName(const string& domainName);
    virtual void SetDomainPath(const string& domainPath);
    virtual string GetDomainPath()const;    
    void OpenLogger();
    virtual void LogMessage(const string logMessage,const int logLevel);
    CLogger * mDomainLogger;///< Pointer to logger
    static string mServerRootPath;
protected:
    string mDomainName;     
private:
    string mDomainPath;
};
/**
 * \brief Alias info
 */
class CAlias : public CDomain {
public:
    CAlias(const string& aliasName,CDomain* mainDomain);
    virtual string GetDomainPath()const;
    virtual void SetDomainPath(const string& domainPath);
    virtual void LogMessage(const string logMessage,const int logLevel);
private:
    CDomain* mMainDomain;///< Pointer to domain that this is alias to
};


#endif	/* CDOMAIN_H */

