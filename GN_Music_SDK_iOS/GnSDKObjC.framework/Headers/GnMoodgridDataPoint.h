/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnMoodgridDataPoint_h_
#define _GnMoodgridDataPoint_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"





/**
** GnMoodgridDataPoint
*/ 

@interface GnMoodgridDataPoint : NSObject

@property NSUInteger x;
@property NSUInteger y;
/**
* GnMoodgridDataPoint
*/
-(INSTANCE_RETURN_TYPE) init;

/**
* GnMoodgridDataPoint
*/
-(INSTANCE_RETURN_TYPE) initWithX: (NSUInteger)x y: (NSUInteger)y;


@end


@interface GnMoodgridDataPointEnumerator : NSEnumerator

typedef void (^GnMoodgridDataPointBlock)(id obj, NSUInteger index, BOOL *stop);

-(void)enumerateObjectsUsingBlock: (GnMoodgridDataPointBlock)handler;

-(NSUInteger) count;

-(GnMoodgridDataPoint*) nextObject;

-(GnMoodgridDataPoint*) objectAtIndex:(NSUInteger)index;

-(NSArray*) allObjects;


@end



#endif /* _GnMoodgridDataPoint_h_ */

