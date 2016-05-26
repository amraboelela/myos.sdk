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
                NSDateFormatter *dateFormatter=[[NSDateFormatter alloc] init];
                [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
                NSLog(@"Tes suite '%@' started at %@", className, [dateFormatter stringFromDate:[NSDate date]]);
                [classInstance runTest];
                NSLog(@"Tes suite '%@' passed at %@", className, [dateFormatter stringFromDate:[NSDate date]]);
            }
        }
    }
    free(classList);
    [pool release];
    return 0;
}
