import UIKit
import XCPlayground

XCPSetExecutionShouldContinueIndefinitely(continueIndefinitely: true)

func printUsername(uname:String)
{
        println(uname)
}

let chatQueue:NSOperationQueue = NSOperationQueue()

func multiThreadRoom()
{
    for (var i = 0; i < 5; ++i) {
        chatQueue.addOperation(NSBlockOperation(block: { () -> Void in
            printUsername("X")
        }))
        
        chatQueue.addOperation(NSBlockOperation(block: { () -> Void in
            printUsername("Y")
        }))
    }
}

multiThreadRoom()

println("Main Thread Quit")