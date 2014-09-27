// Concurreny Experiments: Shared data

import UIKit

/* mutable array */
var airportStatusBoard: [Int] = []

func addFlightEntry()
{
    let newEntry:Int = random() % 10
    airportStatusBoard.append(newEntry)
}

/* concurrent write */
let writeQueue:dispatch_queue_t = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0)
/* sequential read */
let readQueue:dispatch_queue_t = dispatch_queue_create("com.whackylabs.concurrency.readQ", nil)

var writeCount:Int = 0
var readCount:Int = 0

func statusWriter()
{
    NSThread.sleepForTimeInterval(2.0)

    println("writing begin: \(writeCount)")
    addFlightEntry()
    println("writing ends: \(writeCount)")
    writeCount++
}

func statusReader(searchFlighNum:Int) -> Bool
{
    println("reading begin: \(readCount)")
    
    var found:Bool = false
    for flightNum in airportStatusBoard {
        if flightNum == searchFlighNum {
            found = true
            break
        }
    }
    println("reading ends: \(readCount)")
    
    readCount++
    
    return found
}

func loop()
{
    srandom(UInt32(time(nil)))

    var done:Bool = false
    while(!done) {

        /* perform a write op */
        dispatch_async(writeQueue, { () -> Void in
            statusWriter();
        })

        /* perform a read op */
        dispatch_async(readQueue, { () -> Void in
            if (statusReader(7)) {
                done = true
            }
        })
    }
}

loop()
let result = "Flight found after \(writeCount) writes and \(readCount) reads"
println(result)
