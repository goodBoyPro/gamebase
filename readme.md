#reademe

##1

64位\n

##2

在loop内创建actor会出现死锁，更换数据结构时解决

##3

GDebug::~GDebug() {  
   debugs__.erase(this);//可能存在线程安全问题，遇到时解决
}
##4

有现成安全需求的类调用delay时可对 回调函数 和 析构函数 加锁
