/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnLocaleInfo_h_
#define _GnLocaleInfo_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnEnums.h"




/**
* Encapsulates information about a GnLocale instance. Used where GNSDK delivers a locale description such as
* iterating the available locales or when querying a GnLocale instance for information.
*/ 

@interface GnLocaleInfo : NSObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* Construct GnLocaleInfo object with default values.
*/ 

-(INSTANCE_RETURN_TYPE) initWithGnLocaleGroup: (GnLocaleGroup)group language: (GnLanguage)language region: (GnRegion)region descriptor: (GnDescriptor)descriptor;

/**
* Get locale group
* @return Group
*/ 

-(GnLocaleGroup) group;

/**
* Get locale language
* @return Language
*/ 

-(GnLanguage) language;

/**
* Get locale region
* @return Region
*/ 

-(GnRegion) region;

/**
* Get locale descriptor
* @return Descriptor
*/ 

-(GnDescriptor) descriptor;


@end


@interface GnLocaleInfoEnumerator : NSEnumerator

typedef void (^GnLocaleInfoBlock)(id obj, NSUInteger index, BOOL *stop);

-(void)enumerateObjectsUsingBlock: (GnLocaleInfoBlock)handler;

-(NSUInteger) count;

-(GnLocaleInfo*) nextObject;

-(GnLocaleInfo*) objectAtIndex:(NSUInteger)index;

-(NSArray*) allObjects;


@end



#endif /* _GnLocaleInfo_h_ */

