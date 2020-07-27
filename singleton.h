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
缺点：
用户使用也要用智能指针，使用锁也有开销。
某些平台（编译器和指令集架构有关），双检锁会失效
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
        //双检锁，先判断为空，才加锁
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


/*
局部静态变量  懒汉式
基于magic static的方法
当变量在初始化的时候，并发同时进入声明语句，并发线程会阻塞等待初始化结束
返回引用，不返回指针，防止用户使用delete

*/

class Singleton3 {

public:
    ~Singleton3(){
        std::cout << "destructor called"<<std::endl;
    }

    Singleton3(const Singleton3&)=delete;
    Singleton3& operator=(const Singleton3&)=delete;
    static Singleton3& getInstance(){
        static Singleton3 instance;
        return instance;
    }

private:
    Singleton3(){
        std::cout << "constructor called"<<std::endl;
    }
};


//为了重复使用代码
//用模板来实现单例
/*
CRTP奇异递归模板模式实现
模板基类的构造函数为protected ， 子类才能继承
*/

template<typename T>
class Singleton4{
public:
    static T&getInstance(){
        static T instance;
        return instance;
    }

    virtual ~Singleton4(){
        std::cout << "desructor called"<<std::endl;
    }
    Singleton4(const Singleton4&)=delete;
    Singleton4& operator=(const Singleton4&)=delete;
protected:
    Singleton4(){
        std::cout << "structor called"<< std::endl;
    }
};

//例子
class ExamSingle:public Singleton4<ExamSingle>{
    friend class Singleton4<ExamSingle>;  //注意，声明为友元类， 因为要在这个类中调用私有的构造方法
    
public:
    ExamSingle(const ExamSingle&)=delete;
    ExamSingle& operator=(const ExamSingle&)=delete;
private:
    ExamSingle()=default;

};

//可以不用在子类声明友元的方法，就是使用一个代理类token,子类构造函数需要传递
//token类才能构造，但是把token类保护起来，然后子类的构造函数就可以是公有的。
template<typename T>
class Singleton5{
public:
    static Singleton5& getInstance()noexcept(std::is_nothrow_constructible<T>::value){
        static T instance{token()};
        return instance;
    }
    virtual ~Singleton5()=default;
    Singleton5(const Singleton5&)=delete;
    Singleton5& operator=(const Singleton5&)=delete;

protected:
    struct token
    {
        /* data */
    };
    Singleton5() noexcept=default;
};

//例子
class ExamSingle2:public Singleton5<ExamSingle2>{
public:
    ExamSingle2(token){
        std::cout << "constructor called"<<std::endl;
    }
    ~ExamSingle2(){
        std::cout << "destructor called"<<std::endl;
    }
    ExamSingle2(const ExamSingle2&)=delete;
    ExamSingle2& operator=(const ExamSingle2&)=delete;
};
#endif   //SINGLETON_H

