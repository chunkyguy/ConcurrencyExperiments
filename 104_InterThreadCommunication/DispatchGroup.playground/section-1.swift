// Playground - noun: a place where people can play

import UIKit

let concurrentQueue:dispatch_queue_t = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
let group:dispatch_group_t = dispatch_group_create()


func StartConsumer()
{
    println("Consumer sleeping")
    dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
    println("Consumer working")
}

func StartProducer()
{
    dispatch_group_async(group, concurrentQueue) { () -> Void in
        println("Producer working")
        sleep(1)
        println("Producer sleeping")
    }
}

func StartProcess()
{
    StartProducer()
    StartConsumer();
}

StartProcess()
var str = "Exit"
