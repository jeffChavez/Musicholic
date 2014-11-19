/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnName_h_
#define _GnName_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnDataObject.h"




/**
* Name
*/ 

@interface GnName : GnDataObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
*  Name display language
*  @return Langauge string
*/ 

-(NSString*) language;

/**
*  3-letter ISO code for display langauge
*  @return Language code
*/ 

-(NSString*) languageCode;

/**
*  Display name string
*  @return Name suitable for displaying to the end user
*/ 

-(NSString*) display;

/**
*  Sortable name
*  @return Sortable string
*/ 

-(NSString*) sortable;

/**
*  Sortable scheme
*  @return Sortable Scheme
*/ 

-(NSString*) sortableScheme;

/**
*  Name prefix, e.g., "The"
*  @return Prefix
*/ 

-(NSString*) prefix;

/**
*  Family name
*  @return Name
*/ 

-(NSString*) family;

/**
*  Given name
*  @return name
*/ 

-(NSString*) given;

/**
*  Name global ID - used for transcriptions
*  @return Gracenote Global ID
*/ 

-(NSString*) globalId;

/**
* GnType
*/
+(NSString*) gnType;

/**
* From
*/
+(GnName*) from: (GnDataObject*)obj error: (NSError**)error;


@end


@interface GnNameEnumerator : NSEnumerator

typedef void (^GnNameBlock)(id obj, NSUInteger index, BOOL *stop);

-(void)enumerateObjectsUsingBlock: (GnNameBlock)handler;

-(NSUInteger) count;

-(GnName*) nextObject;

-(GnName*) objectAtIndex:(NSUInteger)index;

-(NSArray*) allObjects;


@end



#endif /* _GnName_h_ */

