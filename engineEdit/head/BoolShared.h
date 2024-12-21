#if !defined(BOOLSHARED_H)
#define BOOLSHARED_H
template <class T> class BoolShared {
  public:
    BoolShared() {}
    ~BoolShared() {
        if(flag)
            curTrue = nullptr;
    }
    bool flag = false;
    static bool *curTrue;
    void operator=(const bool&b) {
        flag = b;
        if (b&&curTrue) {
            *curTrue = false;          
        }
        if(b) curTrue = &flag;
       
        }
};
template<class T>
inline bool *BoolShared<T>::curTrue = nullptr;


#endif // BOOLSHARED_H
