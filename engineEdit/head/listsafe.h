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
   
     void pollList(std::function<void(T)>callback)
    {
        std::unique_lock lk(mut);
        for (auto it =this->begin(); it != this->end();)
        {
            if (!(*it)->isValid)
            {
                delete *it;
                it =this->erase(it);
                continue;
            }
            callback(*it);
            it++;
        }
    }
    void remove(T&ptr){
          std::unique_lock lk(mut);
          
    }
};


#endif // LISTSAFE_H
