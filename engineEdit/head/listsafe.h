#if !defined(LISTSAFE_H)
#define LISTSAFE_H
#include <functional>
#include <mutex>
#include <set>
template <class T> class ListSafe : public std::multiset<T> {
  private:
  public:
    std::mutex mut;
    void addActor(const T &a) {
        std::unique_lock lk(mut);
        std::multiset<T>::insert(a);
    }
    void pollList(std::function<void(T)> callback) {
        std::unique_lock lk(mut);
        for (auto it = this->begin(); it != this->end();) {
            if ((!(*it)->isValid) && (*it)->getAsyncTaskNumber() == 0) {
                delete (*it);
                it = this->erase(it);
                continue;
            }
            callback(*it);
            it++;
        }
    }
    void remove(T &ptr) {
        std::unique_lock lk(mut);
        std::multiset<T>::erase(ptr);
    }
};

#endif // LISTSAFE_H
