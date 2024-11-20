#include <assert.h>
#include <timer.h>
#include <windows.h>
#include<chrono>
thread_pool thread_pool::ThreadPool;
//时间流
float timeScale=1.f;
int ts1 = 2;
unsigned int getTime() { 
    auto now = std::chrono::steady_clock::now();
    auto duration_since_epoch = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch).count();
    return milliseconds*timeScale;
    //return GetTickCount64()*timeScale; 
    }
namespace xlib {







void Timer20240522::loop() {
    std::unique_lock lk(mut_,std::defer_lock);
    
    //lk.unlock();
    while (brun) {
        // 动态调整计算频率
        static int timecost = 0;
        int a = getTime();
        lk.lock();
        cond_.wait(lk, [&]() { return !tasks.empty(); });        
        it = tasks.begin();
        for (; it != tasks.end();) {
            if ((*it).times <= 0||((*it).isAllTaskCanceled)->load()) {
                --(*((*it).taskNumber));
                it = tasks.erase(it);
                continue;               
            }
            int temp = getTime();
            if (temp - (*it).time0 >= (*it).Delay && (*it).times > 0) {
               //此处必须用线程池，否则会死锁
                ++*( (*it).taskNumber);
                thread_pool::ThreadPool.addTask(*it);
                
                (*it).time0 = temp;
                (*it).times--;
            }
            it++;

        } // for
        lk.unlock();
        //频率越高性能越差的原因找到了：clock()在切换到别的线程时不计时，被其他线程高频抢占cpu,时间看起来变慢了
        timecost = getTime() - a;
        if (timecost < 1) {
            threadSleep(1);
        }
    }
}

bool Timer20240522::bCont() { return false; }

Timer20240522::Timer20240522() {
    t1 = new std::thread(loop, this);
    //assert(timer20240522Ins == nullptr);
    //timer20240522Ins = this;
}
Timer20240522::~Timer20240522() {
    brun = 0;
    thread_pool::ThreadPool.isStop = true;
    t1->join();
    delete t1;
}

Timer20240522& getTimer() {
    static Timer20240522 timerIns;
    return timerIns;
}

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

//毫秒
void threadSleep(int time) {
    timeBeginPeriod(1);
    Sleep(time);
    timeEndPeriod(1);
}

