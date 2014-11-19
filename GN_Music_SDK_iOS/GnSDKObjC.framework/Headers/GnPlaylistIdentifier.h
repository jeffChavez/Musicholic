/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnPlaylistIdentifier_h_
#define _GnPlaylistIdentifier_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"





/**
* <b>Experimental</b>: GnPlaylistIdentifier
*/ 

@interface GnPlaylistIdentifier : NSObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* MediaIdentifier
*/
-(NSString*) mediaIdentifier;

/**
* CollectionName
*/
-(NSString*) collectionName;


@end


@interface GnPlaylistIdentifierEnumerator : NSEnumerator

typedef void (^GnPlaylistIdentifierBlock)(id obj, NSUInteger index, BOOL *stop);

-(void)enumerateObjectsUsingBlock: (GnPlaylistIdentifierBlock)handler;

-(NSUInteger) count;

-(GnPlaylistIdentifier*) nextObject;

-(GnPlaylistIdentifier*) objectAtIndex:(NSUInteger)index;

-(NSArray*) allObjects;


@end



#endif /* _GnPlaylistIdentifier_h_ */

