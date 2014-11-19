/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnDataMatch_h_
#define _GnDataMatch_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnAlbum.h"
#import "GnContributor.h"
#import "GnDataObject.h"




/**
*  Represents a match to query where any type of match is desired, album or contributor.
*/ 

@interface GnDataMatch : GnDataObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
*  Flag indicating if match is album
*  @return True if album is a match, false otherwise
*/ 

-(BOOL) isAlbum;

/**
*  Flag indicating if match is contributor
*  @return True if result is a contributor, false otherwise
*/ 

-(BOOL) isContributor;

/**
*  If album, get match as album object
*  @return Album
*/ 

-(GnAlbum*) getAsAlbum;

/**
*  If contributor, get match as contributor object
*  @return Contributor
*/ 

-(GnContributor*) getAsContributor;


@end


@interface GnDataMatchEnumerator : NSEnumerator

typedef void (^GnDataMatchBlock)(id obj, NSUInteger index, BOOL *stop);

-(void)enumerateObjectsUsingBlock: (GnDataMatchBlock)handler;

-(NSUInteger) count;

-(GnDataMatch*) nextObject;

-(GnDataMatch*) objectAtIndex:(NSUInteger)index;

-(NSArray*) allObjects;


@end



#endif /* _GnDataMatch_h_ */

