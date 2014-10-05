//
//  ThreadSafeList.cpp
//  TryCPP
//
//  Created by Sid on 05/10/14.
//  Copyright (c) 2014 whackylabs. All rights reserved.
//

#include "ThreadSafeList.h"
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <forward_list>
#include <iostream>
#include <future>

#define MAX_FLIGHTS 10
#define SEARCH_FLIGHT 2

struct DebugCount {
    unsigned int read;
    unsigned int write;
    
    DebugCount() :
    read(0),
    write(0)
    {}
    
    ~DebugCount()
    {
        std::cout << "Flight found after " << write << " writes and " << read << " reads." << std::endl;
    }
};


class StatusBoard {
    std::forward_list<int> statusBoard;
    mutable std::mutex mutex;
    mutable DebugCount debugCount;
    
public:
    void AddFlight()
    {
        std::lock_guard<std::mutex> lock(mutex);
        
        debugCount.write++;

        int newFlight = rand()%MAX_FLIGHTS;
        statusBoard.push_front(newFlight);
    }
    
    bool FindFlight(const int flightNum) const
    {
        std::lock_guard<std::mutex> lock(mutex);
        
        debugCount.read++;
        
        return (std::find(statusBoard.begin(), statusBoard.end(), flightNum) != statusBoard.end());
    }
};

class Airport {
private:
    StatusBoard statusBoard;

    bool StatusReader(const int searchFlightNum)
    {
        return statusBoard.FindFlight(searchFlightNum);
    }
    
    void StatusWriter()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        statusBoard.AddFlight();
    }

public:
    
    void ParallelLoop()
    {
        while (!std::async(std::launch::async, &Airport::StatusReader, this, SEARCH_FLIGHT).get()) {
            /* write operation */
            std::thread writeOp(&Airport::StatusWriter, this);
            writeOp.detach();
        }
    }
    
    void SerialLoop()
    {
        while (!StatusReader(SEARCH_FLIGHT)) {
            StatusWriter();
        }
    }
};

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    Airport airport;
    //SerialLoop();
    airport.ParallelLoop();
}
