# reademe

## 1

64位\n

## 2

在loop内创建actor会出现死锁，更换数据结构时解决

## 3

GDebug::~GDebug() {  
   debugs__.erase(this);//可能存在线程安全问题，遇到时解决
}
## 4

有现成安全需求的类调用delay时可对 回调函数 和 析构函数 加锁

## 5

避免定义带参数的构造函数，其初始化应在eventBegin中进行

## 6

GetTickCount64()*timeScale; timeScale时浮点数时，会随着时间推移越来越慢，原因未知

## 7

如果变量名以4个下划线开头，说明引擎建议用户不要直接访问该变量

## 8
**在tick中应该引用deltatime**