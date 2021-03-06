/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnPlaylistMoreLikeThisOptions_h_
#define _GnPlaylistMoreLikeThisOptions_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"





/**
* <b>Experimental</b>: Playlist "more like this" options.
*/ 

@interface GnPlaylistMoreLikeThisOptions : NSObject

/**
*  Retrieves an option for Maximum number of Tracks for MoreLikeThis queries on a given collection.
*  Please note that these options are not serialized or stored.
*  Option for querying/specifying the maximum number of tracks in the result to be returned.
*  0 is not a valid value
*  @return Maximum tracks
*
**/ 

-(NSUInteger) maxTracks:(NSError**) error;

/**
* Sets an option for Maximum number of Tracks for MoreLikeThis queries on a given collection.
* Please note that these options are not serialized or stored.
* @param  value [in] The maximum number of tracks to be returned. 0 is not a valid value.
**/ 

-(void) maxTracksWithValue: (NSUInteger)value error: (NSError**)error;

/**
* Retrieves an option for maximum number of tracks Per artist  for MoreLikeThis queries on a given collection.
* Please note that these options are not serialized or stored.
* 0 is not a valid value
* @return Maximum tracks per artist
*
**/ 

-(NSUInteger) maxPerArtist:(NSError**) error;

/**
* Sets an option for maximum number of tracks per artist for MoreLikeThis queries on a given collection.
* Please note that these options are not serialized or stored.
* @param  value [in] The maximum number of tracks per artist to be returned. 0 is not a valid value.
*
**/ 

-(void) maxPerArtistWithValue: (NSUInteger)value error: (NSError**)error;

/**
* Retrieves an option for maximum number of tracks per album for MoreLikeThis queries on a given collection.
* Please note that these options are not serialized or stored.
* 0 is not a valid value
* @return Maximum tracks per album
*
**/ 

-(NSUInteger) maxPerAlbum:(NSError**) error;

/**
* Sets an option for maximum number of tracks per album for MoreLikeThis queries on a given collection.
* Please note that these options are not serialized or stored.
* @param  value    [in] The maximum number of tracks per album to be returned. 0 is not a valid value.
**/ 

-(void) maxPerAlbumWithValue: (NSUInteger)value error: (NSError**)error;

/**
* Retrieves the RandomSeed option for "More Like This" for a given collection.
* Please note that these options are not serialized or stored.
* Option for querying/specifying the seed value for the random number generator used in calculating a
* "More Like This" playlist.
* @return Seed value
*/ 

-(NSUInteger) randomSeed:(NSError**) error;

/**
* Sets the RandomSeed option for "More Like This" for a given collection.
* Please note that these options are not serialized or stored.
* Option for querying/specifying the seed value for the random number generator used in calculating a
* "More Like This" playlist. Using the same number for a seed will generate the same 'random' sequence, thus allowing
* the same playlist ordering to be recreated. Choosing different numbers will create different playlists. Setting
* this value to "0" will disable using a random seed.
* @param value		[in] An unsigned int random seed .
*/ 

-(void) randomSeedWithValue: (NSUInteger)value error: (NSError**)error;


@end



#endif /* _GnPlaylistMoreLikeThisOptions_h_ */

