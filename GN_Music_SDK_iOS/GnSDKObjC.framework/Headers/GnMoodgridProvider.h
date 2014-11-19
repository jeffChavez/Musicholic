/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnMoodgridProvider_h_
#define _GnMoodgridProvider_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"





/**
** <b>Experimental</b>: GnMoodgridProvider
*/ 

@interface GnMoodgridProvider : NSObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* Name
*/
-(NSString*) name:(NSError**) error;

/**
* Type
*/
-(NSString*) type:(NSError**) error;

/**
* RequiresNetwork
*/
-(BOOL) requiresNetwork:(NSError**) error;


@end


@interface GnMoodgridProviderEnumerator : NSEnumerator

typedef void (^GnMoodgridProviderBlock)(id obj, NSUInteger index, BOOL *stop);

-(void)enumerateObjectsUsingBlock: (GnMoodgridProviderBlock)handler;

-(NSUInteger) count;

-(GnMoodgridProvider*) nextObject;

-(GnMoodgridProvider*) objectAtIndex:(NSUInteger)index;

-(NSArray*) allObjects;


@end



#endif /* _GnMoodgridProvider_h_ */

