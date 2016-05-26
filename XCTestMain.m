//
//  main.m
//
//  Created by Amr on 5/25/16.
//  Copyright © 2016 Ayesha. All rights reserved.
//

//#import <RengoFoundation/RengoFoundation.h>
#import <XCTest/XCTest.h>
//#import "LevelDBTests.h"

int main(void)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
//    DLog();
    
    
    int numberOfClasses = objc_getClassList(NULL, 0);
    Class *classList = malloc(numberOfClasses * sizeof(Class));
    numberOfClasses = objc_getClassList(classList, numberOfClasses);
    
    for (int idx = 0; idx < numberOfClasses; idx++)
    {
        Class class = classList[idx];
        NSString *className = NSStringFromClass(class);
        //NSLog(@"%@", className);
        if ([className rangeOfString:@"Tests"].length > 0) {
            //if (-1 > 0) {
            //Class classFromName = NSClassFromString(className);
            //NSLog(@"%d", [className rangeOfString:@"Tests"].location);
            id classInstance = [[[class alloc] init] autorelease];
            if ([classInstance isKindOfClass:[XCTestCase class]]) {
                NSLog(@"[class isKindOfClass:XCTestCase]");
            
                //class *testsClass = [[[class alloc] init] autorelease];
                [classInstance runTest];
            }
        }
    }
    free(classList);
    
    /*
    LevelDBTests *levelDBTests = [[[LevelDBTests alloc] init] autorelease];
    [levelDBTests runTest];*/
    //testClasses = IOPipeRunCommand(@"echo ${OBJECTS}", YES);
    //DLog(@"testClasses: %@", testClasses);
    [pool release];
    return 0;
}
