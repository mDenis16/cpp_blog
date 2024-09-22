#pragma once
#include <functional>

class CHttpRequest;

class CDispatchTask {
private:
    std::function<void()> mCallback;
    CHttpRequest* mHttpRequest;
public:
    CDispatchTask(std::function<void()> callback, CHttpRequest* request);

    inline CHttpRequest* GetHttpRequest() { return mHttpRequest; }

    void Execute() {
        if (mCallback)
            mCallback();
    }
};

