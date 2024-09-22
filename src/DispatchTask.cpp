
#include "CDispatchTask.hpp"
#include "HttpRequest.hpp"

CDispatchTask::CDispatchTask(std::function<void()> callback, CHttpRequest* request) : mCallback(callback), mHttpRequest(request) {

}