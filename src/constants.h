/* 
 * File:   constants.h
 * Author: frox
 *
 * Created on March 20, 2012, 3:47 PM
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H
#include <string>
#include <sstream>

using namespace std;

//PROGRAM ERROR CODES
#define LISTEN_SOCKET_LAUNCH_ERR        2
#define HTTP_REQUEST_ERR                3

//LOG LEVELS
#define NO_LOG                  0
#define FATAL_LOG               1
#define ERROR_LOG               2
#define WARNING_LOG             4
#define INFO_LOG                8
#define DEBUG_LOG               16
#define NETWORK_LOG0            32
#define NETWORK_LOG1            64
#define NETWORK_LOG2            128
#define DEBUG_LOG2              256

//HTTP CODES
#define STATUS_OK               200
#define NOT_MODIFIED            304
#define BAD_REQUEST             400
#define NO_PERMISSION           403
#define NOT_FOUND               404
#define INTERNAL_ERROR          500
#define NOT_IMPLEMENTED         501
#define UNAVAILABLE             503
#define HTTP_NOT_SUPPORTED      505

//Header Types
#define hDomain          0
#define hHEAD            1
#define hGET             2
#define hPOST            3
#define hHOST            4
#define hUSER_AGENT      5
#define hCHARSET         6
#define hACCEPT          7
#define hCONTENT_LEN     8
#define hENCODING        9
#define hETAG           10

//File types visitor accepts
#define ACCEPT_PLAIN 1
#define ACCEPT_HTML  2
#define ACCEPT_TEXT  3

#define ACCEPT_JPEG  8
#define ACCEPT_PNG   16
#define ACCEPT_GIF   32
#define ACCEPT_IMAGE 56

#define FILE_UNKNOWN 0
#define FILE_HTML    1
#define FILE_JPEG    2
#define FILE_GIF     3
#define FILE_PNG     4
#define FILE_CSS     5

typedef char byte;
//#include "CLogger.h" //must be declared after log levels constants
//class CLogger;
//extern CLogger* G_ServerLogger;
//#include "CConfig.h"
//class CConfig;
//extern CConfig* G_Config;

extern string intToString(int n);
extern string getDate();
extern byte* readBinFile(int& fileLen);



#endif	/* CONSTANTS_H */


