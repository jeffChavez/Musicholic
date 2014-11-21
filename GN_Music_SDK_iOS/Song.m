//
//  Song.m
//  Musicholic
//
//  Created by Jeff Chavez on 11/19/14.
//  Copyright (c) 2014 CodeFellows. All rights reserved.
//

#import "Song.h"

@implementation Song

- (instancetype) initWithDictionary: (NSDictionary *) songDictionary {
    self = [super init];
    self.artist = songDictionary[@"artist_name"];
    self.title = songDictionary[@"title"];
    self.songId = songDictionary[@"id"];
    self.energy = [[songDictionary valueForKeyPath:@"audio_summary.energy"] floatValue];
    self.danceability = [[songDictionary valueForKeyPath:@"audio_summary.danceability"] floatValue];
    self.tempo = [[songDictionary valueForKeyPath:@"audio_summary.tempo"] floatValue];
    return self;
}

- (Song *) parseJSONDataIntoSong: (NSData *) data {
    NSError *error = nil;
    NSDictionary *jsonDictionary = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];
    NSDictionary *responseDictionary = jsonDictionary[@"response"];
    NSArray *songsArray = responseDictionary[@"songs"];
    NSDictionary *songDictionary = songsArray.firstObject;
    Song *newSong = [[Song alloc] initWithDictionary:songDictionary];
    return newSong;
}

@end
