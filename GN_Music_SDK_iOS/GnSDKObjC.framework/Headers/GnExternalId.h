/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnExternalId_h_
#define _GnExternalId_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnDataObject.h"




/**
* Third-party identifier used to match identified media to merchandise IDs in online stores and other services
*
*/ 

@interface GnExternalId : GnDataObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
*  External ID source (e.g., Amazon)
*  @return External ID source
*/ 

-(NSString*) source;

/**
*  External ID type
*  @return External ID type
*/ 

-(NSString*) type;

/**
*  External ID value
*  @return External ID value
*/ 

-(NSString*) value;

/**
* GnType
*/
+(NSString*) gnType;

/**
* From
*/
+(GnExternalId*) from: (GnDataObject*)obj error: (NSError**)error;


@end


@interface GnExternalIdEnumerator : NSEnumerator

typedef void (^GnExternalIdBlock)(id obj, NSUInteger index, BOOL *stop);

-(void)enumerateObjectsUsingBlock: (GnExternalIdBlock)handler;

-(NSUInteger) count;

-(GnExternalId*) nextObject;

-(GnExternalId*) objectAtIndex:(NSUInteger)index;

-(NSArray*) allObjects;


@end



#endif /* _GnExternalId_h_ */

