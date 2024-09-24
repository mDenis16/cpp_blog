#include <memory>
#include <functional>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "DispatchTask.hpp"

CDispatchTask::CDispatchTask(std::shared_ptr<CHttpRequest>& request, std::function<CHttpResponse()> callback)
: mCallback(callback), mHttpRequest(request) {

}
CHttpResponse CDispatchTask::Execute() {
   return mCallback();
}
CDispatchTask::CDispatchTask(std::function<CHttpResponse()> callback) :mCallback(callback) {

}