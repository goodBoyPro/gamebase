#if !defined(LISTSAFE_H)
#define LISTSAFE_H
#include <list>
#include <mutex>
#include <functional>
template<class T>
class ListSafe : public std::list<T>
{
private:
    std::mutex mut;  
public:
    void addActor(const T &a)
    {
        std::unique_lock lk(mut);
        std::list<T>::push_back(a);
    }
    void pollList(std::function<bool(T)>condition,std::function<void(T)>callback)
    {
        std::unique_lock lk(mut);
        for (auto it =this->begin(); it != this->end();)
        {
            if (condition(*it))
            {
                it =this->erase(it);
                continue;
            }
            callback(*it);
            it++;
        }
    }
    //特殊需求，不定义移除方法，以免死锁
};


#endif // LISTSAFE_H
