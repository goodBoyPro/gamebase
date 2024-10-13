#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <stdio.h>
#include <thread>
#include <vector>
class thread_pool {

  public:
    // static thread_pool &getThreadPool() {
    //     static thread_pool pool;
    //     return pool;
    // }
    static thread_pool ThreadPool;
    void func();

  public:
    using taskType = std::function<void()>;

    thread_pool() {
        int threadNum = std::thread::hardware_concurrency() * 2 + 2;
        for (int i = 0; i < threadNum; i++) {
            threads.emplace_back(threadFunc, this);
        }
    }

    template <class T> void addTask(T f) {
        std::unique_lock lk(mutex_);
        tasks.emplace(f);
        cond_.notify_one();
    }
    ~thread_pool() {
        // std::unique_lock lk(mutex_);
        isStop = 1;
        cond_.notify_all();
        for (auto &x : threads)
            x.join();
        
    };

  private:
    void threadFunc() {
        while (1) {
            std::unique_lock lk(mutex_);
            cond_.wait(lk, [this]() { return isStop || !tasks.empty(); });
            if (isStop && tasks.empty()) {
                lk.unlock();
                break;
            }
            taskType task = std::move(tasks.front());
            tasks.pop();
            lk.unlock();
            task();
        }
    }
    bool isStop = 0;
    std::vector<std::thread> threads;

    std::queue<taskType> tasks;
    std::mutex mutex_;
    std::condition_variable cond_;

}; // class



#endif
