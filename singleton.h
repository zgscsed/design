/*
author:zhoudong
time:2020.7.23
desc:单例模式
*/

#ifndef SINGLETON_H
#define SINGLETON_H


#include <iostream>
#include <memory>
#include <mutex>
/*
方式一：有缺陷的懒汉式
调用时才实例化对象
缺点：线程不安全,多线程中可能引发竟态条件，多个线程同时判断pInstance_
        为空，于是都实例化。解决办法加锁
    不是释放内存：只new 出对象，没有delete对象
*/

class Singleton1 {
private:
    Singleton1(){
        std::cout << " constructor called"<<std::endl;
    }

    Singleton1(Singleton1&)=delete;
    Singleton1& operator=(const Singleton1&)=delete;
    static Singleton1* pInstance_;              //静态指针

public:
    ~Singleton1(){
        std::cout << "destructor called"<<std::endl;
    }

    static Singleton1* getIntance(){
        if(pInstance_==nullptr){
            pInstance_ = new Singleton1();
        }
        return pInstance_;
    }
};

Singleton1* Singleton1::pInstance_ = nullptr;

/*
线程安全、内存安全的懒汉式单例
使用了智能指针和锁来解决上方法1的缺点
*/
class Singleton2{
public:
    typedef std::shared_ptr<Singleton2> Ptr; 
    ~Singleton2(){
        std::cout << "destructor called"<<std::endl;
    }
    Singleton2(Singleton2&)=delete;
    Singleton2& operator=(const Singleton2&)=delete;
    static Ptr getInstance(){
        if(pInstance_==nullptr){
            std::lock_guard<std::mutex> lock(mutex_);
            //这里在判断一次，是因为多线程中，可能几个线程都同时判断了。
            if(pInstance_==nullptr)
            {
                pInstance_=std::shared_ptr<Singleton2>(new Singleton2);
            }
        }
        return pInstance_;
    }

private:
    Singleton2(){
        std::cout << "constructor called"<<std::endl;
    }

    static Ptr pInstance_;
    static std::mutex mutex_;
};

Singleton2::Ptr Singleton2::pInstance_ = nullptr;
std::mutex Singleton2::mutex_;


#endif   //SINGLETON_H

