#AsynchUnitTests

AsynchUnitTestVerifier add support for writing asynchronous test cases in the SenTestingKit framework built into Xcode.
 
###Why?
It is  not uncommon for iOS applications to have components that collaborate with the main thread via background threads, e.g. making network requests, performing calculations or other long running tasks that should not block the main thread. Writing unit or integration tests for these components can be difficult since the test case will continue running while the background thread perform its task with no possibilty to evaluate the outcome.

###How does it work?
The framework adds a new macro to the existing assert macros provided by the SenTestingKit framework. This macro adds the possibilty to evaluate a condition in the future, blocking the test case from continuing until either the condition is satisfied or the timeout is reached.
 
The `futureAssert` macro will probe the provided assert macro on a regular interval. If the test evaluates to NO it will be probed again a few seconds later until the timeout is reached. If the timeout is reached the test cases will marked as failed. If the assert marco evaluates to YES during the timeout preriod the test case will continue execution (either with the same or a new test case).
 
###Interface
The following macros are supported:
 
- `futureAssert(assert)` - Evaluate the wrapped `assert` marco during the default time out, 10 seconds.
- `futureAssertWithinTimeout(assert, timeout)` - Evaluate the wrapped `assert` marco during the provided `timeout` value (in seconds).
 
The `assert` marco can by any of the marcos provided by the SenTestingKit framework, e.g. STAssertTrue, STAsseertNotNil etc.
 
###Example
 
    - (void)testNetworkRequest {
      Networker *networker = [[[Networker alloc] init] autorelease];
 
      __block int resposeCode = 0;
      [networker asynchHttpRequestWithCompletionBlock: ^(NSInteger httpResponseCode) {
        resposeCode = httpResponseCode;
      }];
 
      futureAssert(STAssertTrue(resposeCode == 200, @"Server did not return http response code 200"));
    }
 
The example above will test the asynchronous method `asynchHttpRequestWithCompletionBlock` and check that `httpResponseCode` is equal to 200.