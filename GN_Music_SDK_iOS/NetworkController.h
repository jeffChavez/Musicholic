//
//  NetworkController.h
//  Musicholic
//
//  Created by Jeff Chavez on 11/18/14.
//  Copyright (c) 2014 CodeFellows. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "Drink.h"
#import "Song.h"
#import "User.h"

@interface NetworkController : NSObject

+ (id) networkController;

- (void) requestOauthAccessForUser: (User *) user;

- (void) fetchDrinkForSong: (NSString *)title withArtist:(NSString *)artist withCompletionHandler:(void (^)(NSString *, Drink *))success;
- (void) ECHONESTfetchDrinkForSong:(NSString *)title withArtist: (NSString *) artist withCompletionHandler:(void (^)(NSString *, Song *))success;
- (void) ECHONESTfetchDrinkForSongSummary:(NSString *)songID withCompletionHandler:(void (^)(NSString *, Song *))success;

- (void) fetchImageForDrink: (Drink *)drink withCompletionHandler:(void (^)(UIImage *)) success;


@end