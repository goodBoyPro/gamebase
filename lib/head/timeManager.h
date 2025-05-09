#if !defined(TIMER_H)
#define TIMER_H
#include <atomic>
#include <condition_variable>
#include <functional>
#include <list>
// #include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>
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

// 用户创建对象已禁止;系统已自动创建对象，用户无需创建
class TIMERAPI TimeManager {

  public:
    struct DelayTask {
        void operator()() {
            ____funcPtr();
            --(*____taskNumber);
            if (____times != -5)
                ____times--;
        }
        DelayTask() {}
        template <class T>
        DelayTask(int a, int b, int c, std::atomic<int> *taskNum_,
                  std::atomic<bool> *isCanceled_, T func_)
            : ____time0(a), ____Delay(b), ____times(c),
              ____taskNumber(taskNum_), ____isAllTaskCanceled(isCanceled_),
              ____funcPtr(func_) {
            ++(*____taskNumber);
        }

        void cancelTask() { ____times = 0; }
        void setDelayTime(int dl_) { ____Delay = dl_; }
        int ____time0 = 0;
        int ____Delay = 0;
        int ____times = 1;
        // 线程安全
        // 被计时队列持有时+1，被线程池队列持有时也+1,执行中也+1
        std::atomic<int> *____taskNumber = nullptr;
        std::atomic<bool> *____isAllTaskCanceled = nullptr;
        std::function<void()> ____funcPtr = nullptr;
    };
    typedef DelayTask delaytaskPtr;

  private:
    // 单例模式
    TimeManager(/* args */);
    friend TimeManager &getTimer();
    // 用于保护容器操作
    std::mutex mut_;
    std::list<TimeManager::DelayTask>::iterator it;
    std::thread *t1;
    void loop();
    bool isPaused = true;
    bool brun = true;
    std::condition_variable cond_;
    std::list<TimeManager::DelayTask> tasks;

  public:
    void stop() {
        brun = false;
        cond_.notify_one();
    }
    ~TimeManager();

    void setPause(bool pause_) { isPaused = pause_; }
    void clearAllTasks();
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
        lk.unlock();
        cond_.notify_all();
        return x;
    }
};
TimeManager TIMERAPI &getTimer();

} // namespace xlib

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

class TIMERAPI thread_pool {

  public:
    static thread_pool &getThreadPool();
    bool isStop = 0;

  public:
    void stop() {
        isStop = true;
        cond_.notify_all();
    }
    void clear() {
        std::unique_lock lk(mutex_);
        while (!tasks.empty())
            tasks.pop();
    }
    template <class T> void addTask(T f) {

        std::unique_lock lk(mutex_);
        // tasks.emplace(f);
        tasks.push(f);
        cond_.notify_one();
    }
    ~thread_pool() {
        stop();
        for (auto &x : threads)
            x.join();
        printf("thread pool released\n");
    };

  private:
    thread_pool() {
        // int threadNum = std::thread::hardware_concurrency() * 2 + 2;
        int threadNum = 3;
        for (int i = 0; i < threadNum; i++) {
            threads.emplace_back(&thread_pool::threadFunc, this);
        }
    }
    void threadFunc() {
        std::unique_lock lk(mutex_, std::defer_lock);
        while (!isStop) {
            lk.lock();
            cond_.wait(lk, [this]() { return isStop || !tasks.empty(); });
            // if (isStop && tasks.empty()) {
            if (isStop) {
                lk.unlock();
                while (!tasks.empty())
                    tasks.pop();
                break;
            }
            xlib::TimeManager::DelayTask *task = tasks.front();
            tasks.pop();
            lk.unlock();
            (*task)();
        }
        printf("thread pool loop stoped:%d\n", isStop);
    }

    std::vector<std::thread> threads;
    std::queue<xlib::TimeManager::DelayTask *> tasks;
    std::mutex mutex_;
    std::condition_variable cond_;

}; // class
#endif
