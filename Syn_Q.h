#pragma once
#include <deque>
#include <thread>
#include <condition_variable>
#include <mutex>

template <class T>
class SyncQueue {
    protected:
        std::deque<T> queue;
        mutable std::mutex mtx;
        std::condition_variable cv;

    public:
        void put(const T &m) {
            std::unique_lock<std::mutex> lck{mtx};     // protect operation
            queue.push_back(m);
            cv.notify_one();                 // notify
        }
        T get() {
            std::unique_lock<std::mutex> lck{mtx};        // acquire mutex
            while( queue.size() == 0)
                cv.wait(lck);                   // release lock and wait 
            auto m = queue.front();
            queue.pop_front();  // get message
            return m;
        }        
};


