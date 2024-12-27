#include <assert.h>
#include <chrono>
#include <timeManager.h>
#include <windows.h>

// thread_pool thread_pool::ThreadPool;
// 时间流
float timeScale = 1.f;
int ts1 = 2;
unsigned int getTime() {
    auto now = std::chrono::steady_clock::now();
    auto duration_since_epoch = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                            duration_since_epoch)
                            .count();
    return milliseconds;
    //  return GetTickCount64();
}
namespace xlib {

void TimeManager::loop() {
    std::unique_lock lk(mut_, std::defer_lock);
    thread_pool &tp = thread_pool::getThreadPool();
    while (true) {
        // 动态调整计算频率
        if (isPaused)
            continue;
        static int timecost = 0;
        int a = getTime();
        
        lk.lock();
        cond_.wait(lk, [this]() { return (!tasks.empty())||(!brun); });
        if(!brun){     
            lk.unlock();
            
            break;}
        it = tasks.begin();
        static int temp = getTime();
        for (; it != tasks.end();) {
            if ((*it).____times == 0 || ((*it).____isAllTaskCanceled)->load()) {
                --(*((*it).____taskNumber));
                it = tasks.erase(it);
                continue;
            }

            if (temp - (*it).____time0 >= (*it).____Delay &&
                (*it).____times != 0) {
                // 此处必须用线程池，否则会死锁
                ++*((*it).____taskNumber);
                tp.addTask(&(*it));

                (*it).____time0 = temp;
                
            }
            it++;
            temp = getTime();
        } // for
        lk.unlock();
        //  频率越高性能越差的原因找到了：clock()在切换到别的线程时不计时，被其他线程高频抢占cpu,时间看起来变慢了
        timecost = getTime() - a;
        if (timecost < 1) {
            // threadSleep(1);
        }
    }
}

bool TimeManager::bCont() { return false; }

TimeManager::TimeManager() {
    t1 = new std::thread(&TimeManager::loop, this);
}
TimeManager::~TimeManager() { 
    t1->join();
    delete t1; 
    printf("TimerManagerReleased\n");   
}

// TimeManager &getTimer() {
//     static TimeManager timerIns;
//     return timerIns;
// }

} // namespace xlib

bool canRun::delay(int delayTime) {

    int temp = getTime();
    if (temp - time0 >= delayTime) {
        time0 = temp;
        return 1;
    }
    return 0;
}

unsigned long DelayTask::msCount;

// 毫秒
void threadSleep(int time) {
    timeBeginPeriod(1);
    Sleep(time);
    timeEndPeriod(1);
}
