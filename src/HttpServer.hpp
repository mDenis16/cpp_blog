#pragma once
#include <unordered_map>
#include <array>
#include <vector>
#include <thread>
#include <deque>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <sys/event.h>
#include <sys/un.h>
#include <netdb.h>
#include <fcntl.h>
#include <deque>
#include <queue>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "libs/picohttpparser.h"

#include "DispatchQueue.hpp"

class CHttpServer {
private:
    int mPort;
    int mMaxHeaders;
    std::vector<phr_header> mHeaders;
    int mCurrentHeaders;
    std::thread mSqlThread;
    int mKQ;
    std::array<struct kevent, 128> mEvList;
    struct kevent mEvSet;
public:


    CHttpServer(int _port, int _maxHeaders);
    void Listen();

    int mSockFd;

    static constexpr int kBacklogSize = 1000;
    static constexpr int kMaxConnections = 10000;
    static constexpr int kMaxEvents = 10000;

    void HandleRequest(CHttpRequest* request);

    int ProcessBuffer(int receivedLength);

    std::array<char8_t, 1024 * 8> buffer{0};

    std::map<std::string, std::function<bool(CHttpRequest* request)>> routes;

    void OnGet(std::string path,std::function<bool(CHttpRequest* request)> callback);



    void RunDispatcher();

    void InvokeAsync(std::function<void()> callback, CHttpRequest* request);


    void ReturnResponse(CHttpRequest* request);

    CDispatchQueue Dispatcher;

};




