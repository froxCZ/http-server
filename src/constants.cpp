#include "constants.h"

//CLogger* G_ServerLogger;
//CConfig* G_Config;
/**
 * @param n to convert
 * @return n int string
 */
string intToString(const int n) {
    stringstream sstream;
    sstream << n;
    string s = sstream.str();
    return s;
}
/**
 * 
 * @return date in string
 */
string getDate() {
    time_t rawtime;
    time(&rawtime);
    string timeStr = asctime(localtime(&rawtime));
    timeStr[timeStr.length() - 1] = 0;
    return timeStr;
}
