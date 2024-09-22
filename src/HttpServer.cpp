#include "HttpServer.hpp"





#include "HttpRequest.hpp"
#include "libs/picohttpparser.h"
#include "HttpResponse.hpp"
#include "CDispatchTask.hpp"
CHttpServer::CHttpServer(int _port, int _maxHeaders) : mPort(_port), mMaxHeaders(_maxHeaders) {
   mHeaders.resize(100);
}

int CHttpServer::ProcessBuffer(int receivedLength) {
    const char *method, *path;
    size_t buflen = 0, method_len, path_len, num_headers;

    int pret, minor_version;

    auto ret =  phr_parse_request((const char*)(&buffer[0]), receivedLength, &method, &method_len, &path, &path_len,
                         &minor_version, mHeaders.data(), &num_headers, 0);
    if (ret > 0) {
        std::cout << "got request " << std::endl;
    }


}
void CHttpServer::Listen() {
    int opt = 1;

    // Creating socket file descriptor
    if ((mSockFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cout << "Failed to create socket file descriptor" << std::endl;
        return;
    }

    // Setting socket options
    if (setsockopt(mSockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("Failed to set socket options");
    }

    // Defining server address structure
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;  // Bind to all available interfaces
    server_address.sin_port = htons(mPort);  // Specify the port

    // Binding the socket to the specified IP and port
    if (bind(mSockFd, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        throw std::runtime_error("Failed to bind to socket");
    }

    // Start listening for incoming connections
    if (listen(mSockFd, kBacklogSize) < 0) {
        std::cout << "Failed to listen on port: " << mPort << std::endl;
        return;
    }

    mKQ = kqueue();

    std::cout << "Server listening on port: " << mPort << std::endl;

    sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    EV_SET(&mEvSet, mSockFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    assert(-1 != kevent(mKQ, &mEvSet, 1, NULL, 0, NULL));


    std::array<uint8_t, 1024> buffer;

    memset(buffer.data(), 0, 1024);

    std::thread t([this]() {
           RunDispatcher();
       });

    while (true) {
        int nev = kevent(mKQ, NULL, 0, mEvList.data(), mEvList.size(), NULL);

        for (int i = 0; i < nev; i++) {
            auto& event = mEvList.at(i);


            if (event.flags & EV_EOF) {
                std::cout << "Closed connection for " << event.ident << std::endl;
                close(event.ident);
            } else if (event.ident == mSockFd) {
                struct sockaddr_storage addr;
                socklen_t socklen = sizeof(addr);
                int connfd = accept(event.ident, (struct sockaddr *)&addr, &socklen);
                if (connfd != -1) {
                    EV_SET(&mEvSet, connfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                    if (kevent(mKQ, &mEvSet, 1, NULL, 0, NULL) < 0)
                    {
                        std::cout << "kevent " << std::endl;
                    }
                }else {
                    std::cout << "failed accepting connection " << std::endl;
                }

            }
            else if (event.filter == EVFILT_READ) {

                size_t bytes_read = recv(event.ident, buffer.data(), buffer.size(), 0);

                if (bytes_read > 0) {

                    auto result = CHttpRequest::parseFromBuffer(buffer.data(), bytes_read, event.ident);
                    if (result)
                        HandleRequest(result);


                    /*  EV_SET(&mEvSet, event.ident, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
                      kevent(mKQ, &mEvSet, 1, NULL, 0, NULL);*/

                    EV_SET(&mEvSet, event.ident, EVFILT_USER, EV_ADD, 0, 0, NULL);
                    if (kevent(mKQ, &mEvSet, 1, NULL, 0, NULL) < 0)
                    {
                        std::cout << "kevent " << std::endl;
                    }
                }

            }
            else if (event.filter == EVFILT_WRITE) {

                if (event.udata) {
                    auto request = static_cast<CHttpRequest*>(event.udata);
                    auto& response = request->getResponse();


                    const auto& buffer = response.Serialize();


                    auto ret = send(event.ident, (const void*)(buffer.data()), buffer.size(), 0);

                    std::cout << "Sent response for request id " << request->getId() << " original size " << buffer.size() << std::endl;


                    shutdown(event.ident, SHUT_WR);
                    //  close(event.ident);

                }
            }
        }
    }
}

void CHttpServer::HandleRequest(CHttpRequest *request) {
   // std::cout << "HandleRequest path: " << request->getPath() << std::endl;

    auto route_itx = routes.find(request->getPath());

    if (route_itx != routes.end())
        if ((*route_itx).second(request))
            ReturnResponse(request);

}

void CHttpServer::OnGet(std::string path, std::function<bool(CHttpRequest* request)> callback) {
    routes[path] = std::move(callback);
}
void CHttpServer::RunDispatcher() {
    while (true) {
        CDispatchTask task = Dispatcher.Pop();
        task.Execute();
        const auto request = task.GetHttpRequest();

        ReturnResponse(task.GetHttpRequest());
    }
}
void CHttpServer::InvokeAsync(std::function<void()> callback, CHttpRequest* request) {
    CDispatchTask task(callback, request );
    Dispatcher.Push(task);
}
void CHttpServer::ReturnResponse(CHttpRequest* request) {
    EV_SET(&mEvSet, request->GetConnection(), EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, request);
    kevent(mKQ, &mEvSet, 1, NULL, 0, NULL);
}