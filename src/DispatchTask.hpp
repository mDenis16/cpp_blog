#pragma once
class CHttpRequest;
class CHttpResponse;

class CDispatchTask {
private:
    std::function<CHttpResponse()> mCallback;
    std::shared_ptr<CHttpRequest> mHttpRequest;
public:
    CDispatchTask(std::shared_ptr<CHttpRequest>& request, std::function<CHttpResponse()> callback);

    CDispatchTask(std::function<CHttpResponse()> callback);

    inline std::shared_ptr<CHttpRequest>& GetHttpRequest() { return mHttpRequest; }

    CHttpResponse Execute();
};

