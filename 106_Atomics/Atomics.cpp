//
//  Atomics.cpp
//  TryCPP
//
//  Created by Sid on 31/10/14.
//  Copyright (c) 2014 whackylabs. All rights reserved.
//
#include <cassert>
#include <iostream>
#include <thread>
#include <string>
#include <map>
#include <iomanip>

#pragma mark -
#pragma mark atomic types
#pragma mark -

//void TryCopy()
//{
//    std::atomic<bool> b1(false);
//    std::atomic<bool> b2 = b1;
//}

void DoLoad()
{
    std::atomic<int> i(10);
    std::atomic<int> j(i.load());
    std::cout << j << std::endl;
}

void DoStore(const int n)
{
    std::atomic<int> i(0);
    i.store(n);
    std::cout << i << std::endl;
}

void DoExchange(const int n)
{
    std::atomic<int> i(50);
    int j = i.exchange(n);
    std::cout << i << " " << j << std::endl; // j = i; i = n;
}

void DoExchangeWeak(const int desired)
{
    int expected = 0;
    std::atomic<int> i(5);

    bool success = i.compare_exchange_weak(expected, desired);
    
    std::cout << std::this_thread::get_id() << " " << std::boolalpha << success << " "
    << desired << " " << i << " " << expected << std::endl; // true 100 100 50 or true 50 50 50
}

void DoExchangeStrong(const int desired)
{
    int expected = 0;
    std::atomic<int> i(expected);

    bool success = i.compare_exchange_strong(expected, desired);
    
    std::cout << std::this_thread::get_id() << " " << std::boolalpha << success << " "
    << desired << " " << i << " " << expected << std::endl; // true 100 100 50 or true 50 50 50
}

void DoFetchAdd(const int n)
{
    std::atomic<int> i(100);
    int j = i.fetch_add(n);
    std::cout << i << " " << j << std::endl;
    //for n = 50; output: 150, 100 => j = i; i += n;
}

struct MyType {
    int a;
    int b;
};

std::ostream &operator<<(std::ostream &os, const MyType &t)
{
    os << "{" << t.a << ", " << t.b << "}";
    return os;
}

void DoCustomExchange()
{
    std::atomic<MyType> a;
    a.store({10, 20});
    MyType b = {11, 21};
    MyType c = a.exchange(b);

    std::cout << "a: " << a << std::endl; // a: {11, 21}
    std::cout << "b: " << b << std::endl; // b: {11, 21}
    std::cout << "c: " << c << std::endl; // c: {10, 20}
}


class CustomMutex {
public:
    CustomMutex() :
    flag(ATOMIC_FLAG_INIT)
    {}
    
    void lock()
    {
        while(flag.test_and_set(std::memory_order_seq_cst)) {
        }
    }
    
    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
    
private:
    std::atomic_flag flag;
};

class UnLOLText {
public:
    UnLOLText(const std::string &name) :
    username_(name),
    modify_(0)
    {
        srand((unsigned int)time(0));
    }
    
    void ToUpper()
    {
        std::lock_guard<CustomMutex> lock(mutex_);

        while (modify_ < username_.size()) {
            username_[modify_] = toupper(username_[modify_]);
            modify_++;
            thread_sleep();
        }
    }

    void ToLower()
    {
        std::lock_guard<CustomMutex> lock(mutex_);
        
        while (modify_ < username_.size()) {
            username_[modify_] = tolower(username_[modify_]);
            modify_++;
            thread_sleep();
        }
    }

    void Reset()
    {
        modify_ = 0;
    }
    
    friend std::ostream &operator<<(std::ostream &os, const UnLOLText &txt);
    
private:

    void thread_sleep()
    {
        std::this_thread::sleep_for(std::chrono::microseconds(rand()%5+1));
    }
    
    size_t modify_;
    std::string username_;
    CustomMutex mutex_;
};


std::ostream &operator<<(std::ostream &os, const UnLOLText &txt)
{
    os << txt.username_;
    return os;
}


void Scene2()
{
    UnLOLText txt("hEy How aRe yoU dOinG!");
    
    std::thread tab1(&UnLOLText::ToUpper, &txt);
    std::thread tab2(&UnLOLText::ToLower, &txt);

    tab1.join();
    tab2.join();
    
    std::cout << "Concurrent random: " << txt << std::endl;

    txt.Reset();
}

void Scene1()
{
    UnLOLText txt("hEy How aRe yoU dOinG!");

    txt.ToUpper();
    std::cout << "Serial upper: " << txt << std::endl;

    txt.Reset();
    
    txt.ToLower();
    std::cout << "Serial lower: " << txt << std::endl;

    txt.Reset();
}

#pragma mark -
#pragma mark memory ordering
#pragma mark -

#pragma mark default

namespace defult {
    std::atomic<bool> hasHat;
    std::atomic<bool> hasCoffee;
    std::atomic<int> hipLevel;
    
    void WearHat()
    {
        std::this_thread::sleep_for(std::chrono::seconds(rand()%3+1));
        hasHat.store(true, std::memory_order_seq_cst);
        std::cout << "H" << std::endl;
    }
    
    void HoldCoffee()
    {
        std::this_thread::sleep_for(std::chrono::seconds(rand()%3+1));
        hasCoffee.store(true, std::memory_order_seq_cst);
    }
    
    void CheckHatAndCoffee()
    {
        while (!hasHat.load(std::memory_order_seq_cst)) {
            /* wait till employee gets a hat */
        }
        
        if (hasCoffee.load(std::memory_order_seq_cst)) {
            hipLevel++;
            std::cout << "1" << std::endl;
        }
    }
    
    void CheckCoffeeAndHat()
    {
        while (!hasCoffee.load(std::memory_order_seq_cst)) {
            /* wait till employee gets a coffee */
        }
        
        if (hasHat.load(std::memory_order_seq_cst)) {
            hipLevel++;
            std::cout << "2" << std::endl;
        }
    }
    
    void EmployeeEnter()
    {
        hasHat = false;
        hasCoffee = false;
        hipLevel = 0;
        
        std::thread a(WearHat);
        std::thread b(HoldCoffee);
        std::thread c(CheckHatAndCoffee);
        std::thread d(CheckCoffeeAndHat);
        
        a.join();
        b.join();
        c.join();
        d.join();
        
        if (hipLevel == 0) {
            std::cout << "Entry denied" << std::endl;
        } else {
            std::cout << "Entry granted with hip level: " << hipLevel << std::endl;
        }
    }
}

#pragma mark unordered

namespace unordered {
    std::atomic<bool> hasHat;
    std::atomic<bool> hasCoffee;
    std::atomic<int> hipLevel;

    void GetThings()
    {
        hasHat.store(true, std::memory_order_relaxed);
        hasCoffee.store(true, std::memory_order_relaxed);
    }
    
    void CheckCoffeeAndHat()
    {
        while (!hasCoffee.load(std::memory_order_relaxed)) {
            /* wait till employee gets a coffee */
        }
        
        if (hasHat.load(std::memory_order_relaxed)) {
            hipLevel++;
        }
    }
    
    void EmployeeEnter()
    {
        hasHat = false;
        hasCoffee = false;
        hipLevel = 0;
        
        std::thread a(GetThings);
        std::thread b(CheckCoffeeAndHat);
        
        a.join();
        b.join();
        
        if (hipLevel == 0) {
            std::cout << "Entry denied" << std::endl;
        } else {
            std::cout << "Entry granted with hip level: " << hipLevel << std::endl;
        }
    }
}

#pragma mark lock-based (acquire)

namespace lock {
    std::atomic<bool> hasHat;
    std::atomic<bool> hasCoffee;
    std::atomic<int> hipLevel;
    
    void GetThings()
    {
        hasHat.store(true, std::memory_order_relaxed);
        hasCoffee.store(true, std::memory_order_release);
    }
    
    void CheckCoffeeAndHat()
    {
        while (!hasCoffee.load(std::memory_order_acquire)) {
            /* wait till employee gets a coffee */
        }
        
        if (hasHat.load(std::memory_order_relaxed)) {
            hipLevel++;
        }
    }
    
    void EmployeeEnter()
    {
        hasHat = false;
        hasCoffee = false;
        hipLevel = 0;
        
        std::thread a(GetThings);
        std::thread b(CheckCoffeeAndHat);
        
        a.join();
        b.join();
        
        if (hipLevel == 0) {
            std::cout << "Entry denied" << std::endl;
        } else {
            std::cout << "Entry granted with hip level: " << hipLevel << std::endl;
        }
    }
}

#pragma mark lock-based (consume)

namespace lock2 {
    std::atomic<bool> hasHat;
    std::atomic<bool> hasCoffee;
    std::atomic<int> hipLevel;
    
    void GetThings()
    {
        std::this_thread::sleep_for(std::chrono::seconds(rand()%3+1));
       
        hasHat.store(true, std::memory_order_relaxed);
        hasCoffee.store(true, std::memory_order_release);
    }
    
    void CheckCoffeeAndHat()
    {
        int naps_taken = 0;
        while (!hasCoffee.load(std::memory_order_consume)) {
            /* nap for a while */
            naps_taken++;
            std::this_thread::sleep_for(std::chrono::seconds(rand()%2+1));
        }
        
        std::cout << "Naps: " << naps_taken << std::endl;
        
        if (hasHat.load(std::memory_order_relaxed)) {
            hipLevel++;
        }
    }
    
    void EmployeeEnter()
    {
        hasHat = false;
        hasCoffee = false;
        hipLevel = 0;
        
        std::thread a(GetThings);
        std::thread b(CheckCoffeeAndHat);
        
        a.join();
        b.join();
        
        if (hipLevel == 0) {
            std::cout << "Entry denied" << std::endl;
        } else {
            std::cout << "Entry granted with hip level: " << hipLevel << std::endl;
        }
    }
}


/* This is the main() */
int main()
{
    srand((unsigned int)time(0));
    
    //   defult::EmployeeEnter();
    // unordered::EmployeeEnter();
    lock2::EmployeeEnter();
    
    // DoCustomExchange();
    //   DoFetchAdd(50);
    //   TryExchange();
    //   DoExchangeStrong(50);
    // DoExchangeWeak(100);
    //   DoExchange(100);
    //DoStore(100);
    //Scene1();
    // Scene2();
    return 0;
}