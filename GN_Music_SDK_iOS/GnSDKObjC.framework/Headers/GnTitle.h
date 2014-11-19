/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnTitle_h_
#define _GnTitle_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnDataObject.h"




/**
* Title of a work or product
*/ 

@interface GnTitle : GnDataObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
*  Title display language
*  @return Language string
*/ 

-(NSString*) language;

/**
*  3 letter ISO code for display language
*  @return Language code
*/ 

-(NSString*) languageCode;

/**
*  Title display string
*  @return Strng suitable for displaying to end user
*/ 

-(NSString*) display;

/**
*  Title prefix, e.g., The
*  @return Prefix
*/ 

-(NSString*) prefix;

/**
*  Sortable title
*  @return Sortable string
*/ 

-(NSString*) sortable;

/**
*  Sortable title scheme
*  @return Sortable scheme
*/ 

-(NSString*) sortableScheme;

/**
* Main title
* @return Title
*/ 

-(NSString*) mainTitle;

/**
* Title edition
* @return Edition
*/ 

-(NSString*) edition;

/**
* GnType
*/
+(NSString*) gnType;

/**
* From
*/
+(GnTitle*) from: (GnDataObject*)obj error: (NSError**)error;


@end



#endif /* _GnTitle_h_ */

