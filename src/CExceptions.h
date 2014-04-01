/* 
 * File:   CExceptions.h
 * Author: frox
 *
 * Created on May 4, 2012, 8:51 PM
 */

#ifndef CEXCEPTIONS_H
#define	CEXCEPTIONS_H
#include <string>
#include "constants.h"

using namespace std;
/**
 * \brief Exception used across the whole project
 */
class CErrorException {
public:
    /**
     * Simple exception class
     * @param errorCode
     * @param errorMessage
     */
    CErrorException(const int errorCode, const string errorMessage = "") {
        mErrorCode = errorCode;
        mErrorMessage = errorMessage;
    }
    string GetErrorMessage()const {
        return mErrorMessage;
    }

    int GetErrorCode()const {
        return mErrorCode;
    };
private:
    string mErrorMessage;
    int mErrorCode;
};


#endif	/* CEXCEPTIONS_H */

