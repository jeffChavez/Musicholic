//
//  NetworkController.m
//  Musicholic
//
//  Created by Jeff Chavez on 11/18/14.
//  Copyright (c) 2014 CodeFellows. All rights reserved.
//

#import "NetworkController.h"


@interface NetworkController()

@property NSOperationQueue *imageQueue;

@end




@implementation NetworkController


// Makes a singleton
+ (id)networkController {
    static NetworkController *networkController = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        networkController = [[self alloc] init];
    });
    
    return networkController;
}



- (void) fetchDrinkForSong:(NSString *)title withArtist: (NSString *) artist withCompletionHandler:(void (^)(NSString *, Drink *))success; {
    
    // Construct correct URL as a string
    // What is a sample URL I can test?
    NSString *songInfo = [NSString stringWithFormat: @"ARTIST=%@&TITLE=%@", title, artist];
    
    //    var echoURL = 'http://developer.echonest.com/api/v4/song/identify?api_key=' +
    //    key +
    //    '&artist=' +
    //    artist +
    //    '&title=' +
    //    title +
    //    '&bucket=audio_summary' +
    //    '&format=json';
    
    
    NSURL *requestURL = [NSURL URLWithString:songInfo];
    
    NSURLSessionDataTask *dataTask = [[NSURLSession sharedSession] dataTaskWithURL:requestURL completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        if ([response isKindOfClass: [NSHTTPURLResponse class]]) {
            NSHTTPURLResponse * httpResponse = (NSHTTPURLResponse *) response;
            NSLog(@"status code is %ld",(long)[httpResponse statusCode]);
            if ([httpResponse statusCode] >= 200 && [httpResponse statusCode] <= 204 ) {
                Drink *drink = [[Drink alloc] parseJSONDataIntoDrink:data];
                [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                    success(nil, drink);
                }];
            } else {
                [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                    NSLog(@"There was an error: %@",error.localizedDescription);
                    success(error.localizedDescription, nil);
                }];
            }
        }
    }];
    [dataTask resume];
}




- (void) fetchImageForDrink: (Drink *)drink withCompletionHandler:(void (^)(UIImage *)) success; {
    self.imageQueue = [[NSOperationQueue alloc] init];
    [self.imageQueue addOperationWithBlock:^{
        // Make URL from Drink's urlString
        // What is a sample URL I can test?
        NSURL *url = [NSURL URLWithString: drink.imageURL];
        NSData *data = [NSData dataWithContentsOfURL:url];
        UIImage *drinkImageToReturn = [UIImage imageWithData:data];
        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
            success(drinkImageToReturn);
        }];
    }];
}


@end