//
//  ConditionVars.cpp
//  TryCPP
//
//  Created by Sid on 20/10/14.
//  Copyright (c) 2014 whackylabs. All rights reserved.
//

#include "ConditionVars.h"
#include <iostream>
#include <string>
#include <thread>
#include <functional>

namespace PCP {
    std::mutex mutex;
    std::condition_variable condition;

    void StartConsumer()
    {
        std::cout << std::this_thread::get_id() << " Consumer sleeping" << std::endl;
        std::unique_lock<std::mutex> consumerLock(mutex);
        condition.wait(consumerLock);
        consumerLock.unlock();
        
        std::cout << std::this_thread::get_id() << " Consumer working" << std::endl;
    }
    
    void StartProducer()
    {
        std::cout << std::this_thread::get_id() << " Producer working" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> producerLock(mutex);
        condition.notify_one();
        std::cout << std::this_thread::get_id() << " Producer sleeping" << std::endl;
    }
    
    void StartProcess()
    {
        std::thread consumerThread(StartConsumer);
        std::thread producerThread(StartProducer);
        
        consumerThread.join();
        producerThread.join();
    }
}

void main()
{
    PCP::StartProcess();
}
