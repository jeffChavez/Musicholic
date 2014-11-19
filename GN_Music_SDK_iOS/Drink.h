//
//  Drink.h
//  Musicholic
//
//  Created by Jeff Chavez on 11/18/14.
//  Copyright (c) 2014 CodeFellows. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Drink : NSObject

@property (nonatomic, strong) NSString *name;
@property (nonatomic, strong) NSString *imageURL;
@property (nonatomic, strong) UIImage *image;

- (instancetype) initWithDictionary: (NSDictionary *) drinkDictionary;
- (NSObject *) parseJSONDataIntoDrink: (NSData *) data;

@end