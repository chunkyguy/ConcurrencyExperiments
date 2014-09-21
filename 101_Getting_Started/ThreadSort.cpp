//
//  ThreadSort.cpp
//  TryCPP
//
//  Created by Sid on 13/09/14.
//  Copyright (c) 2014 whackylabs. All rights reserved.
//

#include "ThreadSort.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace {
    
#ifdef DEBUG
    template<typename T>
    void PrintArr(const std::vector<T> &arr, const std::string &pre = "")
    {
        std::cout << pre;
        std::copy(arr.begin(), arr.end(), std::ostream_iterator<T>(std::cout, " "));
        std::cout << std::endl;
    }
#endif

    template<typename T>
    void QuickSort(std::vector<T> &arr, const size_t left, const size_t right)
    {
        if (left >= right) {
            return;
        }
        
        /* move pivot to left */
        std::swap(arr[left], arr[(left+right)/2]);
        size_t last = left;
        
        /* swap all lesser elements */
        for (size_t i = left+1; i <= right; ++i) {
            if (arr[i] < arr[left]) {
                std::swap(arr[++last], arr[i]);
            }
        }
        
        /* move pivot back */
        std::swap(arr[left], arr[last]);

        /* sort subarrays */
        QuickSort(arr, left, last);
        QuickSort(arr, last+1, right);
    }

    template<typename T>
    void QuickerSort(std::vector<T> &arr, const size_t left, const size_t right)
    {
        if (left >= right) {
            return;
        }
        
        /* move pivot to left */
        std::swap(arr[left], arr[(left+right)/2]);
        size_t last = left;
        
        /* swap all lesser elements */
        for (size_t i = left+1; i <= right; ++i) {
            if (arr[i] < arr[left]) {
                std::swap(arr[++last], arr[i]);
            }
        }
        
        /* move pivot back */
        std::swap(arr[left], arr[last]);
        
        /* sort subarrays */
        auto taskLeft = std::async(QuickerSort<T>, std::ref(arr), left, last);
        auto taskRight = std::async(QuickerSort<T>, std::ref(arr), last+1, right);
        
        /* wait */
//        if (taskLeft.wait_for(std::chrono::seconds(0)) != std::future_status::deferred ||
//            taskRight.wait_for(std::chrono::seconds(0)) != std::future_status::deferred) {
//            while (taskLeft.wait_for(std::chrono::seconds(0)) != std::future_status::ready &&
//                taskRight.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
//                std::this_thread::yield();
//            }
//        }
    }

    template<typename T>
    bool IsSorted(const std::vector<T> &arr)
    {
        for (int i = 1; i < arr.size(); ++i) {
            if (arr[i-1] > arr[i]) {
                return false;
            }
        }
        return true;
    }
    
}

template <typename T>
void UseQuickSort(std::vector<T> arr)
{
    clock_t start, end;
    start = clock();
    QuickSort(arr, 0, arr.size() - 1);
    end = clock();
    std::cout << "QuickSort: " << (end - start) * 1000.0/double(CLOCKS_PER_SEC) << "ms" << std::endl;
    assert(IsSorted(arr));
}

template <typename T>
void UseQuickerSort(std::vector<T> arr)
{
    clock_t start, end;
    start = clock();
    auto bgTask = std::async(QuickerSort<T>, std::ref(arr), 0, arr.size()-1);
    if (bgTask.wait_for(std::chrono::seconds(0)) != std::future_status::deferred) {
        while (bgTask.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            std::this_thread::yield();
        }
        assert(IsSorted(arr));
        end = clock();
        std::cout << "QuickerSort: " << (end - start) * 1000.0/double(CLOCKS_PER_SEC) << "ms" << std::endl;
    }
}

void ThreadSort_main()
{
    
//    std::vector<int> arr;
//    for (int i = 0; i < 100; ++i) {
//        arr.push_back(rand()%10000);
//    }

    std::vector<int> arr = {7, 49, 73, 58, 30, 72, 44, 78, 23, 9};
#ifdef DEBUG
    PrintArr(arr, "Bef: ");
#endif

    UseQuickSort(arr);
    //UseQuickerSort(arr);
    

#ifdef DEBUG
    PrintArr(arr, "Aft: ");
#endif

    //    assert(IsSorted(arr));
    
    //    std::cout << "Time: " << (end - start) * 1000.0/double(CLOCKS_PER_SEC) << "ms" << std::endl;
}
