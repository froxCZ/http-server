GXX=g++
CFLAGS=-Wall -pedantic -Wno-long-long -O0 -ggdb -c
LIBS=-lpthread
TARGET=../udrzavoj

all: compile doc
compile: $(TARGET)
$(TARGET): CVisitor.o CThread.o CDomain.o main.o CCacheQue.o CHttpMessage.o constants.o CHttpRequest.o CBuffer.o CListenSocket.o CConfig.o CFile.o CLogger.o CExceptions.o CTreap.o CHttpResponse.o
	$(GXX) CVisitor.o CThread.o CDomain.o main.o CCacheQue.o CHttpMessage.o constants.o CHttpRequest.o CBuffer.o CListenSocket.o CConfig.o CFile.o CLogger.o CExceptions.o CTreap.o CHttpResponse.o -o $(TARGET) $(LIBS)
CVisitor.o: CVisitor.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CVisitor.o.d CVisitor.cpp	

CThread.o: CThread.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CThread.o.d CThread.cpp	

CDomain.o: CDomain.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CDomain.o.d CDomain.cpp	

main.o: main.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF main.o.d main.cpp	

CCacheQue.o: CCacheQue.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CCacheQue.o.d CCacheQue.cpp	

CHttpMessage.o: CHttpMessage.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CHttpMessage.o.d CHttpMessage.cpp	

constants.o: constants.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF constants.o.d constants.cpp	

CHttpRequest.o: CHttpRequest.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CHttpRequest.o.d CHttpRequest.cpp	

CBuffer.o: CBuffer.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CBuffer.o.d CBuffer.cpp	

CListenSocket.o: CListenSocket.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CListenSocket.o.d CListenSocket.cpp	

CConfig.o: CConfig.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CConfig.o.d CConfig.cpp	

CFile.o: CFile.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CFile.o.d CFile.cpp	

CLogger.o: CLogger.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CLogger.o.d CLogger.cpp

CExceptions.o: CExceptions.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CExceptions.o.d CExceptions.cpp	

CTreap.o: CTreap.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CTreap.o.d CTreap.cpp	

CHttpResponse.o: CHttpResponse.cpp
	$(GXX) $(CFLAGS) -MMD -MP -MF CHttpResponse.o.d CHttpResponse.cpp		

clean:
	rm -fr CVisitor.o CVisitor.o.d CThread.o CThread.o.d CDomain.o CDomain.o.d main.o main.o.d CCacheQue.o CCacheQue.o.d CHttpMessage.o CHttpMessage.o.d constants.o constants.o.d CHttpRequest.o CHttpRequest.o.d CBuffer.o CBuffer.o.d CListenSocket.o CListenSocket.o.d CConfig.o CConfig.o.d CFile.o CFile.o.d CLogger.o CLogger.o.d CExceptions.o CExceptions.o.d CTreap.o CTreap.o.d CHttpResponse.o CHttpResponse.o.d  $(TARGET) ../doc/
doc:	
	doxygen doxyConf

