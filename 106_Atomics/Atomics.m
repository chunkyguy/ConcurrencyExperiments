#import <Foundation/Foundation.h>

@interface Employee : NSObject

@property (copy) NSString *firstName;
@property (copy) NSString *lastName;
@property (readwrite) int coffeeConsumed; // in litres

- (NSString *)description;

@end

@implementation Employee

- (id)init
{
    self = [super init];
    if (!self) {
        return nil;
    }
    
    _firstName = [@"Monty" copy];
    _lastName = [@"Burns" copy];
    _coffeeConsumed = 9235;
    
    return self;
}

- (void)dealloc
{
    self.firstName = nil;
    self.lastName = nil;
    [super dealloc];
}

- (NSString *)description;
{
    return [NSString stringWithFormat:@"%@ %@: %@ L",
            _firstName,
            _lastName,
            @(_coffeeConsumed)];
}
@end

void updateRecord()
{
    dispatch_group_t wait = dispatch_group_create();
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    
    Employee *emp = [[Employee alloc] init];
    
    dispatch_group_enter(wait);
    dispatch_async(queue, ^{
        [NSThread sleepForTimeInterval:rand()/(NSTimeInterval)RAND_MAX];
        emp.firstName = @"Homer";
        [NSThread sleepForTimeInterval:rand()/(NSTimeInterval)RAND_MAX];
        emp.lastName = @"Simpson";
        [NSThread sleepForTimeInterval:rand()/(NSTimeInterval)RAND_MAX];
        emp.coffeeConsumed = 2045;
        dispatch_group_leave(wait);
    });
    
    
    dispatch_group_enter(wait);
    dispatch_async(queue, ^{
        [NSThread sleepForTimeInterval:rand()/(NSTimeInterval)RAND_MAX];
        emp.firstName = @"Lenny";
        [NSThread sleepForTimeInterval:rand()/(NSTimeInterval)RAND_MAX];
        emp.lastName = @"Leonard";
        [NSThread sleepForTimeInterval:rand()/(NSTimeInterval)RAND_MAX];
        emp.coffeeConsumed = 127;
        dispatch_group_leave(wait);
    });
    
    dispatch_group_enter(wait);
    dispatch_async(queue, ^{
        [NSThread sleepForTimeInterval:rand()/(NSTimeInterval)RAND_MAX];
        emp.firstName = @"Carl";
        [NSThread sleepForTimeInterval:rand()/(NSTimeInterval)RAND_MAX];
        emp.lastName = @"Carlson";
        [NSThread sleepForTimeInterval:rand()/(NSTimeInterval)RAND_MAX];
        emp.coffeeConsumed = 598;
        dispatch_group_leave(wait);
    });
    
    dispatch_group_enter(wait);
    dispatch_async(queue, ^{
        [NSThread sleepForTimeInterval:rand()/(NSTimeInterval)RAND_MAX];
        NSLog(@"%@", emp);
        dispatch_group_leave(wait);
    });
    
    dispatch_group_wait(wait, DISPATCH_TIME_FOREVER);
    [emp release];
}

int main(int argc, char * argv[]) {
    @autoreleasepool {
        for (int i = 0; i < 3; ++i) {
            srand(time(0));
            updateRecord();
        }
        NSLog(@"Done");
    }
    return 0;
}
