//
//  Drink.m
//  Musicholic
//
//  Created by Jeff Chavez on 11/18/14.
//  Copyright (c) 2014 CodeFellows. All rights reserved.
//

#import "Drink.h"

@implementation Drink

- (instancetype) initWithDictionary: (NSDictionary *) drinkDictionary {
    self = [super init];
    self.name = drinkDictionary[@"name"];
    self.imageURL = drinkDictionary[@"imageUrl"];
    return self;
}

- (Drink *) parseJSONDataIntoDrink: (NSData *) data {
    NSError *error = nil;
    NSDictionary *jsonDictionary = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];
    Drink *newDrink = [[Drink alloc] initWithDictionary:jsonDictionary];
    return newDrink;
}

@end