#include <assert.h>
#include <timer.h>
#include <windows.h>
thread_pool thread_pool::ThreadPool;
//时间流
float  timeScale=1;
unsigned int getTime() { return GetTickCount() * timeScale; }
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
        for (; it != tasks.end(); it++) {
            if ((*it).times <= 0||!(*it).isTaskValid) {
                
                it = tasks.erase(it);
                it--;
                continue;
                
            }
            int temp = getTime();
            if (temp - (*it).time0 >= (*it).Delay && (*it).times > 0) {
               //此处必须用线程池，否则会死锁
                thread_pool::ThreadPool.addTask((*it).funcPtr);
                
                (*it).time0 = temp;
                (*it).times--;
            }
            
            
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
    printf("gettiemr\n");
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

