
#include "CListenSocket.h"

/**
 * Creates listen socket instance with basic settings
 * @param address Server listen address (usually 0(all))
 * @param port Server port
 * @param acceptQue How many connection can wait in line for accept()
 */
CListenSocket::CListenSocket(const string address, const string port, int acceptQue) {
    m_Port = port;
    m_Address = address;
    m_SocketID = -1;
    m_AcceptQue = acceptQue;
}

/**
 * Sets up listening server socket. Throws ErrExc on error.
 * @return true on success
 * @throws CErrorException 
 */
bool CListenSocket::Launch() {
    if (!setSocketAddr())throw CErrorException(LISTEN_SOCKET_LAUNCH_ERR, "Preparing address failed.");
    if (!initSocket())throw CErrorException(LISTEN_SOCKET_LAUNCH_ERR, "Cannot init socket at address: " + m_Address);
    if (!setSocketOpt())throw CErrorException(LISTEN_SOCKET_LAUNCH_ERR, "Cannot set socket option");
    if (!bindSocket())throw CErrorException(LISTEN_SOCKET_LAUNCH_ERR, "Cannot bind to port " + m_Port);
    if (!listenSocket())throw CErrorException(LISTEN_SOCKET_LAUNCH_ERR, "Cannot set socket to listen state.");
    G_ServerLogger->append("Server running. Listening at " + m_Address + " port: " + m_Port, INFO_LOG);
    return true;

}
/**
 * Closes listening socket
 * @return true
 */
bool CListenSocket::Close() {
    close(m_SocketID);
    return true;
}

/**
 * Accepts incoming connections on listen socket.
 * @param inAddr
 * @return 
 */
int CListenSocket::AcceptConn(sockaddr_in& inAddr) {
    socklen_t inAddr_size = sizeof inAddr;
    int ret = accept(m_SocketID, (sockaddr*) & inAddr, &inAddr_size);
    string clientAddress = inet_ntoa(inAddr.sin_addr);
    G_ServerLogger->append(clientAddress + " connected.", NETWORK_LOG1);
    return ret;
}

/**
 * Sets listen address
 * @return true on success
 */
bool CListenSocket::setSocketAddr() {
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me           
    getaddrinfo(m_Address.c_str(), m_Port.c_str(), &hints, &m_AddrInfo); //prepares final addrinfo
    return true;
}

/**
 * Sets socket info
 * @return true on success
 */
bool CListenSocket::initSocket() {
    if ((m_SocketID = socket(m_AddrInfo->ai_family, m_AddrInfo->ai_socktype, m_AddrInfo->ai_protocol)) == -1) {
        return false;
    }
    return true;

}

/**
 * Set socket options (keep alive etc)
 * @return true on success
 */
bool CListenSocket::setSocketOpt() {
    int *p_int = new int(1);
    if ((setsockopt(m_SocketID, SOL_SOCKET, SO_REUSEADDR, p_int, sizeof (int)) == -1) || //SO_REUSEADDR - allows others to bind() to the port, if there is no other active listening socket.
            (setsockopt(m_SocketID, SOL_SOCKET, SO_KEEPALIVE, p_int, sizeof (int)) == -1)) {//SO_KEEPALIVE - send keep alive packets. if fails, connection closes.
        throw CErrorException(LISTEN_SOCKET_LAUNCH_ERR, "Error setting socket options. Errno #" + intToString(errno));
        delete p_int;
        return false;
    }
    delete p_int;
    return true;
}

/**
 * Attach to the port
 * @return true on success
 */
bool CListenSocket::bindSocket() {
    if (bind(m_SocketID, m_AddrInfo->ai_addr, m_AddrInfo->ai_addrlen) == -1) {//attach the socket to port
        return false;
    }
    return true;
}

/**
 * Start listening. m_AcceptQue - number of connections in the que waiting for accept();
 * @return true on success
 */
bool CListenSocket::listenSocket() {
    if (listen(m_SocketID, m_AcceptQue) == -1) {// start listening on port. 10(backlog) - number of connections in the que waiting for accept();
        return false;
    }
    return true;
}



