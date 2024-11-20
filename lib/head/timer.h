#if !defined(TIMER_H)
#define TIMER_H
#include <atomic>
#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include <windows.h>
#ifdef EXPORT
#define TIMERAPI __declspec(dllexport)
#else
#define TIMERAPI __declspec(dllimport)
#endif

// 时间
extern float TIMERAPI timeScale;
unsigned int TIMERAPI getTime();
namespace xlib {

using funcType = std::function<void()>;

// 用户创建对象已禁止;系统已自动创建对象，用户无需创建
class TIMERAPI Timer20240522 {

  public:
    struct DelayTask {
        void operator()() {
            funcPtr();
            --(*taskNumber);
        }
        DelayTask() {}
        template <class T>
        DelayTask(int a, int b, int c, T func_)
            : time0(a), Delay(b), times(c), funcPtr(func_) {}
        template <class T>
        DelayTask(int a, int b, int c, std::atomic<int> *taskNum_,
                  std::atomic<bool> *isCanceled_, T func_)
            : time0(a), Delay(b), times(c), taskNumber(taskNum_),
              isAllTaskCanceled(isCanceled_), funcPtr(func_) {
            ++(*taskNumber);
        }
        ~DelayTask() {
            
            }
        int time0 = 0;
        int Delay = 0;
        int times = 1;
        // 线程安全
        std::atomic<int> *taskNumber = nullptr;
        std::atomic<bool> *isAllTaskCanceled = nullptr;

        funcType funcPtr = nullptr;
    };

    bool brun = 1;

  private:
    // 单例模式
    Timer20240522(/* args */);
    friend Timer20240522 &getTimer();
    // 用于保护容器操作
    std::mutex mut_;

    std::list<Timer20240522::DelayTask>::iterator it;

    std::thread *t1;
    void loop();

  public:
    std::condition_variable cond_;
    std::list<Timer20240522::DelayTask> tasks;

    ~Timer20240522();
    bool TIMERAPI bCont();

    template <class T>
    DelayTask *addTask(int time0, int timeDelay, int times, T funcPtr_) {

        std::unique_lock lk(mut_);
        auto x = &(tasks.emplace_back(time0, timeDelay, times, funcPtr_));
        cond_.notify_all();
        return x;
    }
    template <class T>
    DelayTask *addTaskSafe(int time0, int timeDelay, int times,
                           std::atomic<int> *taskNum_,
                           std::atomic<bool> *isCanceled_, T funcPtr_) {

        std::unique_lock lk(mut_);
        auto x = &(tasks.emplace_back(time0, timeDelay, times, taskNum_,
                                      isCanceled_, funcPtr_));
        cond_.notify_all();
        return x;
    }
};
TIMERAPI Timer20240522 &getTimer();

} // namespace xlib
// 每隔指定时间执行操作，执行n次；
template <class T>
xlib::Timer20240522::DelayTask *delay(int timeDelay, int times, T funcPtr_) {
    return xlib::getTimer().addTask(getTime(), timeDelay, times, funcPtr_);
}
// 立即执行一次，每隔指定时间执行操作，执行n次；
template <class T>
xlib::Timer20240522::DelayTask *delayRnw(int timeDelay, int times, T funcPtr_) {
    return xlib::getTimer().addTask(0, timeDelay, times, funcPtr_);
}
// 一段时间后 执行一次
template <class T>
xlib::Timer20240522::DelayTask *delay(int timeDelay, T funcPtr_) {
    return xlib::getTimer().addTask(getTime(), timeDelay, 1, funcPtr_);
}

struct TIMERAPI canRun {
    int time0 = 0;
    canRun() { time0 = getTime(); }
    // 指定时间后 返回true ,配合eventTick使用
    bool delay(int delayTime);
};

class DelayTask {
  public:
    enum unitName { microS = 0, milliS = 1 };
    // template <typename T>
    // DelayTask(int dalay_, T callback_func, unitName unit_ = milliS) :
    // callback(callback_func), delaytime(dalay_), uni(unit_) {}
    // 增加可读性，牺牲少许性能
    DelayTask(int dalay_, std::function<void()> callback_func, //
              unitName unit_ = milliS)
        : delaytime(dalay_), callback(std::move(callback_func)), uni(unit_) {}
    void operator()() {
        if (uni)
            delay();
        else
            delayH();
    }

    void start() { is_running = true; }
    void stop() { is_running = false; }
    void setDelaytime(int dalay_) { delaytime = dalay_; }
    void setFirstDelay() {
        t = GetTickCount();
        lt = getTickH();
    }

  private:
    unsigned int delaytime;
    std::function<void()> callback;
    bool is_running = true;
    unsigned int t = 0;
    unsigned long lt = 0;
    unitName uni;
    void delay() {
        if (is_running && getTime() - t >= delaytime) {
            t = getTime();
            callback();
        }
    }

    void delayH() {
        if (is_running && getTickH() - lt >= delaytime * msCount) {
            lt = getTickH();
            callback();
        }
    }
    // 每微妙次数
    TIMERAPI static unsigned long msCount;
    // 千万分之一秒
    long getTickH() {
        static LARGE_INTEGER freq;
        static LARGE_INTEGER count;
        static bool once = [&]() -> bool {
            if (!QueryPerformanceFrequency(&freq))
                printf("timerError");
            msCount = freq.LowPart / 1000000;
            return true;
        }();

        if (!QueryPerformanceCounter(&count))
            printf("timerError");
        return count.LowPart;
    }
};

// 休眠线程：毫秒
extern "C" void TIMERAPI threadSleep(int time);
#endif // TIMER_H

////////////////////////////////////////////////////////////////////////////
// 线程池
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
    static thread_pool TIMERAPI ThreadPool;
    bool isStop = 0;

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
        tasks.emplace(std::move(f));

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
        while (!isStop) {
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

    std::vector<std::thread> threads;

    std::queue<taskType> tasks;
    std::mutex mutex_;
    std::condition_variable cond_;

}; // class

#endif
