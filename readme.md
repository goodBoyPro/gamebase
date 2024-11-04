1
64位
2
在loop内创建actor会出现死锁，更换数据结构时解决
3
GDebug::~GDebug() {  
   debugs__.erase(this);//可能存在线程安全问题，遇到时解决
}