//
//  Song.h
//  Musicholic
//
//  Created by Jeff Chavez on 11/19/14.
//  Copyright (c) 2014 CodeFellows. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Song : NSObject

@property (nonatomic, strong) NSString *artist;
@property (nonatomic, strong) NSString *title;
@property (nonatomic, strong) NSString *songId;
@property (nonatomic, assign) float danceability;
@property (nonatomic, assign) float energy;
@property (nonatomic, assign) float tempo;


- (instancetype) initWithDictionary: (NSDictionary *) songDictionary;
- (Song *) parseJSONDataIntoSong: (NSData *) data;

@end
