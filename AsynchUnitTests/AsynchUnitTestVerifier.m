//
//  AsynchUnitTestVerifier.m
//
//  Created by Mattias Levin on 7/19/12.
//  Copyright (c) 2012 Wadpam. All rights reserved.
//

#import "AsynchUnitTestVerifier.h"


// Implementation
@implementation AsynchUnitTestVerifier


// The replacement method used during the probing period to reporting failurs in the SenTestingKit testing framework
- (void)mockFailWithException:(NSException*)exception {
  DLOG(@"Probing test assert marco failed");
  @throw [MockException exceptionWithName:@"MockException" 
                                   reason:@"Assert failed during probing" 
                                 userInfo:nil];
}


@end


// Implementaiton of MockException
@implementation MockException
@end
