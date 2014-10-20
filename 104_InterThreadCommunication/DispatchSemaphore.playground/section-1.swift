// Playground - noun: a place where people can play

import UIKit

let sema:dispatch_semaphore_t = dispatch_semaphore_create(0);
let concurrentQueue:dispatch_queue_t = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)

func StartConsumer()
{
    println("Consumer sleeping")
    dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER)
    println("Consumer working")
}

func StartProducer()
{
    dispatch_async(concurrentQueue) {
        println("Producer working")
        sleep(1)
        println("Producer sleeping")
        dispatch_semaphore_signal(sema)
    }
}

func StartProcess()
{
    StartProducer()
    StartConsumer();
}

StartProcess()

var str = "Exit"
