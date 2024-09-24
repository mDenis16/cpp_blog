#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>
#include "DispatchTask.hpp"

class CDispatchQueue {
public:
    CDispatchQueue();
    ~CDispatchQueue();

    void Push(CDispatchTask& task)
    {
        std::lock_guard<std::mutex> queue_lock{_queue_mutex};
        _queue.push(std::move(task));
        _queue_cv.notify_one();
    }

    CDispatchTask Pop();

    std::queue<CDispatchTask> _queue;
    std::condition_variable _queue_cv;
    std::mutex _queue_mutex;
};

