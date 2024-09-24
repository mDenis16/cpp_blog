
#include "DispatchQueue.hpp"
CDispatchQueue::CDispatchQueue() {

}
CDispatchQueue::~CDispatchQueue() {

}
CDispatchTask CDispatchQueue::Pop()
{
    std::unique_lock<std::mutex> queue_lock{_queue_mutex};
    _queue_cv.wait(queue_lock, [&]{ return !_queue.empty(); });
    CDispatchTask ret = _queue.front();
    _queue.pop();
    return ret;
}