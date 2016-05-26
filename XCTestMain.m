//
//  main.m
//
//  Created by Amr on 5/25/16.
//  Copyright © 2016 Ayesha. All rights reserved.
//

#import <XCTest/XCTest.h>

int main(void)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    int numberOfClasses = objc_getClassList(NULL, 0);
    Class *classList = malloc(numberOfClasses * sizeof(Class));
    numberOfClasses = objc_getClassList(classList, numberOfClasses);
    for (int idx = 0; idx < numberOfClasses; idx++) {
        Class class = classList[idx];
        NSString *className = NSStringFromClass(class);
        if ([className rangeOfString:@"Tests"].length > 0) {
            id classInstance = [[[class alloc] init] autorelease];
            if ([classInstance isKindOfClass:[XCTestCase class]]) {
                //NSLog(@"Test Suite '%@' started.", className);
                [classInstance runTest];
                //NSLog(@"Test Suite '%@' passed.", className);
            }
        }
    }
    free(classList);
    [pool release];
    return 0;
}
