/**
 * Concurreny Experiments: Shared data
 * Compile: clang SharedData.m -framework Foundation && ./a.out
 */

#import <Foundation/Foundation.h>

#define MAX_FLIGHTS 10
#define SEARCH_FLIGHT 2

//#define WRITE_LOGS
//#define READ_LOGS

@interface Airport : NSObject {
    NSMutableArray *airportStatusBoard;
    NSUInteger writeCount;
    NSUInteger readCount;
    dispatch_queue_t writeQueue;
    dispatch_queue_t readQueue;
}

@property (atomic, copy) NSArray *airportStatusBoardCopy;
@property (atomic) BOOL breakCondition;
@end

@implementation Airport

- (id)init
{
    self = [super init];
    if (!self) {
        return nil;
    }
    
    airportStatusBoard = [[NSMutableArray alloc] init];
    
    writeCount = 0;
    readCount = 0;
    
    readQueue = dispatch_queue_create("com.whackylabs.concurrency.readQ", DISPATCH_QUEUE_SERIAL);
    writeQueue = dispatch_queue_create("com.whackylabs.concurrency.writeQ", DISPATCH_QUEUE_CONCURRENT);

    self.breakCondition = NO;
    
    return self;
}

- (void)dealloc
{
    dispatch_release(readQueue);
    dispatch_release(writeQueue);
    [airportStatusBoard release];
    [super dealloc];
}

- (void)addFlightEntry
{
    NSNumber *newEntry = @(random() % MAX_FLIGHTS);
    
    if ([newEntry isEqualToNumber:@(SEARCH_FLIGHT)]) {
        
        if (self.breakCondition) {
            NSAssert(0, @"Flight %@ should've been searched by now", newEntry);
        }
        
        self.breakCondition = YES;
    }

    [airportStatusBoard addObject:newEntry];
}

- (void) statusWriter
{
    dispatch_barrier_async(writeQueue, ^{
        
        [NSThread sleepForTimeInterval:0.1];

        writeCount++;

#ifdef WRITE_LOGS
        NSLog(@"writing begin: %@", @(writeCount));
#endif
        
        [self addFlightEntry];
        
#ifdef WRITE_LOGS
        NSLog(@"writing ends: %@", @(writeCount));
#endif
    });
}

- (void) statusReaderWithFlightNumber:(NSNumber *)searchFlightNum
                    completionHandler:(void(^)(BOOL success))completion
{

    readCount++;

#ifdef READ_LOGS
    NSLog(@"reading begin: %@", @(readCount));
#endif

    self.airportStatusBoardCopy = airportStatusBoard;
    
    BOOL found = NO;
    for (NSNumber *flightNum in _airportStatusBoardCopy) {
        if ([flightNum isEqualToNumber:searchFlightNum]) {
            found = YES;
            break;
        }
    }

    //   NSLog(@"airportStatusBoardCopy: %@", airportStatusBoardCopy);
    
#ifdef READ_LOGS
    NSLog(@"reading ends: %@", @(readCount));
#endif
    
    
    completion(found);
}

//- (void) serialLoop
//{
//    srandom(time(0));
//    while (![self statusReaderWithFlightNumber:@(7)]) {
//        [self statusWriter];
//    }
//    
//    NSLog(@"Flight found after %@ writes and %@ reads", @(writeCount), @(readCount));
//}

- (void)parallelLoop
{
    srandom(time(0));
    
    __block BOOL done = NO;
    while(!done) {
        
        /* perform a write op */
        dispatch_async(writeQueue, ^{
            [self statusWriter];
        });
        
        /* perform a read op */
        dispatch_async(readQueue, ^{
            [self statusReaderWithFlightNumber:@(SEARCH_FLIGHT) completionHandler:^(BOOL success) {
                done = success;
            }];
        });
    }
    
    NSLog(@"Flight found after %@ writes and %@ reads", @(writeCount), @(readCount));
}

@end

int main()
{
    @autoreleasepool {
        Airport *airport = [[Airport alloc] init];
        [airport parallelLoop];
    }
    
    return 0;
}