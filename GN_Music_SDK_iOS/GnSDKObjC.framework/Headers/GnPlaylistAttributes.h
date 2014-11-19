/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnPlaylistAttributes_h_
#define _GnPlaylistAttributes_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnDataObject.h"
#import "GnPlaylistAttributes.h"




/**
* <b>Experimental</b>: GnPlaylistAttributes
*/ 

@interface GnPlaylistAttributes : GnDataObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* GnPlaylistAttributes
*/
-(INSTANCE_RETURN_TYPE) initWithGnPlaylistAttributes: (GnPlaylistAttributes*)other;

/**
* AlbumName
*/
-(NSString*) albumName;

/**
* ArtistName
*/
-(NSString*) artistName;

/**
* ArtistType
*/
-(NSString*) artistType;

/**
* Era
*/
-(NSString*) era;

/**
* Genre
*/
-(NSString*) genre;

/**
* Origin
*/
-(NSString*) origin;

/**
* Mood
*/
-(NSString*) mood;

/**
* Tempo
*/
-(NSString*) tempo;


@end



#endif /* _GnPlaylistAttributes_h_ */

