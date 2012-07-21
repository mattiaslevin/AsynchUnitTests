//
//  AsynchUnitTestVerifier.h
//
//  Created by Mattias Levin on 7/19/12.
//  Copyright (c) 2012 Wadpam. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <objc/runtime.h>


// Uncomment the line below to get debug statements
#define PRINT_DEBUG
// Debug macros
#ifdef PRINT_DEBUG
//#  define DLOG(fmt, ...) NSLog( (@"%s [Line %d] " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);
#  define DLOG(fmt, ...) NSLog( (@"" fmt), ##__VA_ARGS__);
#else
#  define DLOG(...)
#endif
// ALog always displays output regardless of the DEBUG setting
#define ALOG(fmt, ...) NSLog( (@"%s [Line %d] " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);


// Default timeout value.
#define DEFAULT_TIMEOUT 10
// Default polling value.
#define POLLING_INTERVAL 2


/**
 The provided test assert marco will be probed at regular interval during the default timeout period, 
 10 seconds.
 */
#define futureAssert(assert) futureAssertWithinTimeout(assert, DEFAULT_TIMEOUT)


/**
 The provided test assert marco will be probed at regular interval during the provided timeout period.
 If the macro evaluates to YES within the timeout the assert will pass and the test cases will countinue 
 the exection.
 If the marco does conitune to evaluate to NO until the timeout the asswert will fail and the test case 
 will be logged as failed.
 */
#define futureAssertWithinTimeout(assert, timeout) \
do { \
NSDate *timeoutDate = [[NSDate date] dateByAddingTimeInterval:(timeout)]; \
NSRunLoop *runLoop = [NSRunLoop currentRunLoop]; \
method_exchangeImplementations(class_getInstanceMethod([self class], @selector(failWithException:)), class_getInstanceMethod([AsynchUnitTestVerifier class], @selector(mockFailWithException:))); \
int i = 1; \
while (YES) { \
[runLoop runUntilDate:[[NSDate date] dateByAddingTimeInterval:POLLING_INTERVAL]]; \
DLOG(@"Running probe iteration %d", i); \
@try { \
assert; \
NSLog(@"Asserts was successful"); \
break; \
} \
@catch(MockException *exception) { \
DLOG(@"Got assert exception"); \
if ([timeoutDate timeIntervalSinceNow] <= 0) \
break; \
} \
i++; \
} \
DLOG(@"Done sleeping, continue running thread"); \
method_exchangeImplementations(class_getInstanceMethod([self class], @selector(failWithException:)), class_getInstanceMethod([AsynchUnitTestVerifier class], @selector(mockFailWithException:))); \
assert; \
} while(NO)



/**
 AsynchUnitTestVerifier add support for writing asynchronous test cases in the SenTestingKit framework 
 built into Xcode.
 
 ###Why?
 It is  not uncommon for iOS applications to have components that collaborate with the main thread via
 background threads, e.g. making network requests, performing calculations or other long running tasks that 
 should not block the main thread. Writing unit or integration tests for these components can be difficult 
 since the test case will continue running while the background thread perform its task with no possibilty 
 to evaluate the outcome.
 
 ###How does it work?
 The framework adds a new macro to the existing assert macros provided by the SenTestingKit framework. 
 This macro adds the possibilty to evaluate a condition in the future, blocking the test case from 
 continuing until either the condition is satisfied or the timeout is reached.
 
 The `futureAssert` macro will probe the provided assert macro on a regular interval. If the test evaluates
 to NO it will be probed again a few seconds later until the timeout is reached. If the timeout is reached 
 the test cases will marked as failed. If the assert marco evaluates to YES during the timeout preriod the 
 test case will continue execution (either with the same or a new test case).
 
 ###Interface
 The following macros are supported:
 
 - `futureAssert(assert)` - Evaluate the wrapped `assert` marco during the default time out, 10 seconds.
 - `futureAssertWithinTimeout(assert, timeout)` - Evaluate the wrapped `assert` marco during the 
 provided `timeout` value (in seconds).
 
 The `assert` marco can by any of the marcos provided by the SenTestingKit framework, e.g. STAssertTrue, 
 STAsseertNotNil etc.
 
 ###Example
 
    - (void)testNetworkRequest {
      Networker *networker = [[[Networker alloc] init] autorelease];
 
      __block int resposeCode = 0;
      [networker asynchHttpRequestWithCompletionBlock: ^(NSInteger httpResponseCode) {
        resposeCode = httpResponseCode;
      }];
 
      futureAssert(STAssertTrue(resposeCode == 200, @"Server did not return http response 200"));
    }
 
 The example above will test the asynchronous method `asynchHttpRequestWithCompletionBlock` and check
 that `httpResponseCode` is equal to 200.
 */
@interface AsynchUnitTestVerifier : NSObject


/**
 The instance method that will replace the implementation of the `failWithException:` method in the
 SenTestingKit testing framework during the probing of the test assert macro. This is need to avoid 
 logging the test cases as failed until the end of the timeout period. 
 @param exception An NSException created by the unit testing framework when the text macro fails.
 */
- (void)mockFailWithException:(NSException*)exception;

@end


/**
 A purely internal exception class used for signalling the success or failure when probing an assert 
 macro until the timeout is reached.
 */
@interface MockException : NSException
@end
