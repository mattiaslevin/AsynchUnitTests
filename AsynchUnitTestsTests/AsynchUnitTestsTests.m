//
//  AsynchUnitTestsTests.m
//  AsynchUnitTestsTests
//
//  Created by Mattias Levin on 7/21/12.
//  Copyright (c) 2012 Wadpam. All rights reserved.
//

#import "AsynchUnitTestsTests.h"
#import "AsynchUnitTestVerifier.h"


@implementation AsynchUnitTestsTests


- (void)setUp {
  [super setUp];    
  // Set-up code here.
}


- (void)tearDown {
  // Tear-down code here.
  [super tearDown];
}


// Test an asynchonous method
- (void)testAsynchMethod {
  
  __block int result = 0;
  
  [self asynchMethodWithCompletionBlock:^(NSInteger value) {
    result = value;
  }];
  
  // Evaluate the value or result until the default timeout
  futureAssert(STAssertTrue(result == 100, @"Result is not equal to 100 as expected"));
}


// A stupid asynchronous method that blocks for 4 seconds
- (void)asynchMethodWithCompletionBlock:(void(^)(NSInteger value))block {
  // Run in a seperate thread
  dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
    
    // Sleep for 4 seconds
    sleep(4);
    
    // Run the completion block
    block(100);
    
  });
}


@end
