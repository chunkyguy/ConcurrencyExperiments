// Playground - noun: a place where people can play

import UIKit

// MARK: Common

func isSorted(array:[Int]) -> Bool
{
    for (var i = 1; i < array.count; ++i) {
        if (array[i] < array[i-1]) {
            println("Fail case: \(array[i]) > \(array[i-1])")
            return false
        }
    }
    return true
}

//MARK: Quick Sort

func quickSort(inout array:[Int], left:Int, right:Int)
{
    if (left >= right) {
        return;
    }
    
    /* swap pivot to front */
    swap(&array[left], &array[(left+right)/2])
    var last:Int = left;
    
    /* swap lesser to front */
    for (var i = left+1; i <= right; ++i) {
        if (array[i] < array[left]) {
            swap(&array[i], &array[++last])
        }
    }
    
    /* swap pivot back */
    swap(&array[left], &array[last])
    
    /* sort subarrays */
    quickSort(&array, left, last)
    quickSort(&array, last+1, right)
}

func useQuickSort(var array:[Int])
{
    let start:clock_t = clock();
    quickSort(&array, 0, array.count-1)
    let end:clock_t = clock();
    
    println("quickSort: \((end-start)*1000/clock_t(CLOCKS_PER_SEC)) ms")
    assert(isSorted(array), "quickSort: Array not sorted");
}

//MARK: Quicker Sort

let sortQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0)


func quickerSort(inout array:[Int], left:Int, right:Int)
{
    if (left >= right) {
        return;
    }
    
    /* swap pivot to front */
    swap(&array[left], &array[(left+right)/2])
    var last:Int = left;
    
    /* swap lesser to front */
    for (var i = left+1; i <= right; ++i) {
        if (array[i] < array[left]) {
            swap(&array[i], &array[++last])
        }
    }
    
    /* swap pivot back */
    swap(&array[left], &array[last])
    
    /* sort subarrays */
    dispatch_sync(sortQueue, { () -> Void in
        quickSort(&array, left, last)
    })
    dispatch_sync(sortQueue, { () -> Void in
        quickSort(&array, last+1, right)
    })
}

func useQuickerSort(var array:[Int])
{

    let start:clock_t = clock();

    dispatch_sync(sortQueue, { () -> Void in
        quickerSort(&array, 0, array.count-1)

        let end:clock_t = clock();
        println("quickerSort: \((end-start)*1000/clock_t(CLOCKS_PER_SEC)) ms")
        
        assert(isSorted(array), "quickerSort: Array not sorted");

    })
}

// MARK: Main

var arr:[Int] = []
for (var i = 0; i < 20; ++i) {
    arr.append(Int(rand())%100);
}


useQuickSort(arr)
useQuickerSort(arr)

