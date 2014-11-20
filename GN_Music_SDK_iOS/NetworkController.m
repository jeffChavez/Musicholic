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


- (void) requestOauthAccessForUser: (User *) user {
    
    // Encode user info
//    NSString *encodedScreenname = [[user.screenname dataUsingEncoding:NSUTF8StringEncoding] base64EncodedStringWithOptions:0];
//    NSString *encodedEmail = [[user.email dataUsingEncoding:NSUTF8StringEncoding] base64EncodedStringWithOptions:0];
//    NSString *encodedPassword = [[user.password dataUsingEncoding:NSUTF8StringEncoding] base64EncodedStringWithOptions:0];

    // Make dictionary
    NSDictionary *dict = [[NSDictionary alloc] initWithObjectsAndKeys:
                          user.screenname, @"screenname",
                          user.email, @"email",
                          user.password, @"password",
                         nil];

    // Make dictionary into JSON
    NSError *error;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dict options:0 error:&error];
    [jsonData base64EncodedDataWithOptions:0];
    
    
    // Set up NSMutableURLRequest
    NSString *herokuURLString = @"https://musicholic.herokuapp.com/login/oauth"; // is this the right url to send the request to?
    NSURL *url = [NSURL URLWithString:herokuURLString];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    request.HTTPMethod = @"POST";
    NSUInteger length = jsonData.length;
    NSString *contentLengthString = [NSString stringWithFormat:@"%ldl", (long)length];
    [request setValue:contentLengthString forHTTPHeaderField: @"Content-Length"];
    [request setValue:@"application/json" forHTTPHeaderField: @"Content-Type"];
    request.HTTPBody = jsonData;
    
    
    NSURLSessionDataTask *dataRequest = [[NSURLSession sharedSession] dataTaskWithRequest:request completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        if (error != nil ) {
            NSLog(@"error in oauth request");
        } else {
            NSString *tokenParsed = @"access_token";
            [[NSUserDefaults standardUserDefaults] setValue:tokenParsed forKey:@"OAuthToken"];
            [[NSUserDefaults standardUserDefaults] synchronize];
        }
    }];
    [dataRequest resume];
}

- (void) fetchDrinkForSong:(NSString *)title withArtist: (NSString *) artist withCompletionHandler:(void (^)(NSString *, Drink *))success; {
    
    // Create request URL
    NSString *songTitleNoSpaces = [title stringByReplacingOccurrencesOfString:@" " withString:@"%20"];
    NSString *songArtistNoSpaces = [artist stringByReplacingOccurrencesOfString:@" " withString:@"%20"];
    NSString *api_key = @"FGSG5VYMGP92BYLA8";
    NSString *urlString = [NSString stringWithFormat: @"http://developer.echonest.com/api/v4/song/search?api_key=%@&artist=%@&title=%@", api_key, songArtistNoSpaces, songTitleNoSpaces];
    NSLog(@"%@", urlString);
    NSDictionary *dict = [[NSDictionary alloc] initWithObjectsAndKeys:
                          urlString, @"url",
                          nil];
    NSError *error;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dict options:0 error:&error];
    [jsonData base64EncodedDataWithOptions:0];
    NSString *herokuURLString = @"https://musicholic.herokuapp.com/api";
    NSURL *url = [NSURL URLWithString:herokuURLString];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    request.HTTPMethod = @"POST";
    NSUInteger length = jsonData.length;
    NSString *contentLengthString = [NSString stringWithFormat:@"%ldl", (long)length];
    [request setValue:contentLengthString forHTTPHeaderField: @"Content-Length"];
    [request setValue:@"application/json" forHTTPHeaderField: @"Content-Type"];
    request.HTTPBody = jsonData;
    NSURLSessionDataTask *dataRequest = [[NSURLSession sharedSession] dataTaskWithRequest:request completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        if ([response isKindOfClass: [NSHTTPURLResponse class]]) {
            NSHTTPURLResponse * httpResponse = (NSHTTPURLResponse *) response;
            NSLog(@"status code is %ld",(long)[httpResponse statusCode]);
            if ([httpResponse statusCode] >= 200 && [httpResponse statusCode] <= 204 ) {
                NSLog(@"status code 200");
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
    [dataRequest resume];
}


- (void) ECHONESTfetchDrinkForSong:(NSString *)title withArtist: (NSString *) artist withCompletionHandler:(void (^)(NSString *, Song *))success; {
    
    // Construct correct URL as a string
    // What is a sample URL I can test?
    NSString *songTitleNoSpaces = [title stringByReplacingOccurrencesOfString:@" " withString:@"%20"];
    NSString *songArtistNoSpaces = [artist stringByReplacingOccurrencesOfString:@" " withString:@"%20"];
    NSString *api_key = @"FGSG5VYMGP92BYLA8";
    NSString *urlString = [NSString stringWithFormat: @"http://developer.echonest.com/api/v4/song/search?api_key=%@&artist=%@&title=%@", api_key, songArtistNoSpaces, songTitleNoSpaces];
    NSURL *requestURL = [NSURL URLWithString:urlString];
    NSURLSessionDataTask *dataTask = [[NSURLSession sharedSession] dataTaskWithURL:requestURL completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        if ([response isKindOfClass: [NSHTTPURLResponse class]]) {
            NSHTTPURLResponse * httpResponse = (NSHTTPURLResponse *) response;
            NSLog(@"status code is %ld",(long)[httpResponse statusCode]);
            if ([httpResponse statusCode] >= 200 && [httpResponse statusCode] <= 204 ) {
                Song *song = [[Song alloc] parseJSONDataIntoSong:data];
                [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                    success(nil, song);
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


- (void) ECHONESTfetchDrinkForSongSummary:(NSString *)songID withCompletionHandler:(void (^)(NSString *, Song *))success; {
    NSString *api_key = @"FGSG5VYMGP92BYLA8";
    NSString *urlString = [NSString stringWithFormat: @"http://developer.echonest.com/api/v4/song/profile?api_key=%@&id=%@&bucket=audio_summary", api_key, songID];
    NSURL *requestURL = [NSURL URLWithString:urlString];
    NSURLSessionDataTask *dataTask = [[NSURLSession sharedSession] dataTaskWithURL:requestURL completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        if ([response isKindOfClass: [NSHTTPURLResponse class]]) {
            NSHTTPURLResponse * httpResponse = (NSHTTPURLResponse *) response;
            NSLog(@"status code is %ld",(long)[httpResponse statusCode]);
            if ([httpResponse statusCode] >= 200 && [httpResponse statusCode] <= 204 ) {
                Song *song = [[Song alloc] parseJSONDataIntoSong:data];
                [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                    success(nil, song);
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