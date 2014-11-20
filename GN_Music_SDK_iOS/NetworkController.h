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

@interface NetworkController : NSObject

+ (id) networkController;

- (void) fetchDrinkForSong: (NSString *)title withArtist:(NSString *)artist withCompletionHandler:(void (^)(NSString *, Drink *))success;

- (void) fetchImageForDrink: (Drink *)drink withCompletionHandler:(void (^)(UIImage *)) success;


@end