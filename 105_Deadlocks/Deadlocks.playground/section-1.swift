// Playground - noun: a place where people can play

import UIKit
import XCPlayground

XCPSetExecutionShouldContinueIndefinitely(continueIndefinitely: true)


let concurrentQueue:dispatch_queue_t = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
let serialQueue:dispatch_queue_t = dispatch_queue_create("deadlockQueue", nil)

/* This will deadlock for sure */
func SimpleDeadlock()
{
    println("Experiment 1: Serial Queue, Sync operations")
    
    dispatch_sync(serialQueue) {
        println("Enter task: 0")
        dispatch_sync(serialQueue) {
            println("Enter task: 1")
            println("Exit task: 1")
        }
        println("Exit task: 0")
    }
}

/* Order LIFO */
func SimpleDeadlock2()
{
    println("Experiment 2: Concurrent Queue, Sync operations")
    
    dispatch_sync(concurrentQueue) {
        println("Enter task: 0")
        dispatch_sync(concurrentQueue) {
            println("Enter task: 1")
            println("Exit task: 1")
        }
        println("Exit task: 0")
    }
}

/* Order FIFO */
func SimpleDeadlock3()
{
    println("Experiment 3: Serial Queue, Async operations")

    dispatch_async(serialQueue) {
        println("Enter task: 0")
        dispatch_async(serialQueue) {
            println("Enter task: 1")
            println("Exit task: 1")
        }
        println("Exit task: 0")
    }
}

//SimpleDeadlock()
SimpleDeadlock2()
SimpleDeadlock3()

println("Exit")
