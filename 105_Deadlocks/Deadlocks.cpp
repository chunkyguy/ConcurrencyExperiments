//
//  Deadlocks.cpp
//
//  Created by Sid on 26/10/14.
//  Copyright (c) 2014 whackylabs. All rights reserved.
//

#include <string>
#include <thread>
#include <mutex>
#include <iostream>

namespace DiningPhilosophers {
    
    const int MAX_FORKS = 2;
    
    std::mutex fork[MAX_FORKS];
    
    class Philosopher {
    private:
        std::string name_;
        int holdForkIndex_;
        
        void Think()
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
    public:
        Philosopher(const std::string &name, const int startForkIndex) :
        name_(name),
        holdForkIndex_(startForkIndex)
        {}
        
        void Eat()
        {
            std::cout << name_ << ": Begin eating" << std::endl;
            
            int lockAIndex = holdForkIndex_;
            int lockBIndex = (holdForkIndex_ + 1) % MAX_FORKS;
            
            std::lock(fork[lockAIndex], fork[lockBIndex]);
            
            Think();
            
            std::lock_guard<std::mutex> locka(fork[lockAIndex], std::adopt_lock);
            std::cout << name_ << ": Hold fork: " << lockAIndex << std::endl;
            
            Think();
            
            std::lock_guard<std::mutex> lockb(fork[lockBIndex], std::adopt_lock);
            std::cout << name_ << ": Hold fork: " << lockBIndex << std::endl;
            
            // eating
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            
            std::cout << name_ << " End eating" << std::endl;
        }
    };
    
    void Dine()
    {
        Philosopher s("Socrates", 0);
        Philosopher n("Nietzsche", 1);
        
        std::thread sEat(&Philosopher::Eat, &s);
        std::thread nEat(&Philosopher::Eat, &n);
        
        sEat.join();
        nEat.join();
    }
}

namespace Sector7G {
    
    
    std::mutex m;
    
    void SwipeCard()
    {
        std::cout << "Card swiping ... " << std::endl;
        std::lock_guard<std::mutex> waterCoolerLock(m);
        std::cout << "Card swiped" << std::endl;
    }
    
    void EnterChamber()
    {
        std::cout << "Chamber unlocking ... " << std::endl;
        std::lock_guard<std::mutex> waterCoolerLock(m);
        std::cout << "Chamber unlocked" << std::endl;
    }
    
    void GetWater()
    {
        EnterChamber();
        SwipeCard();
        
        std::cout << "Water pouring" << std::endl;
    }
    
    void Situation()
    {
        GetWater();
    }
}

/* this is the main() */
void main()
{
    // DiningPhilosophers::Dine();
    Sector7G::Situation();
}
