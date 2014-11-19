/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnArtist_h_
#define _GnArtist_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnContributor.h"
#import "GnDataObject.h"
#import "GnName.h"




/**
* Person or group primarily responsible for creating the Album or Track.
*/ 

@interface GnArtist : GnDataObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* Artist's official name.
* @return Name
*/ 

-(GnName*) name;

/**
* Contributor object - contributor associated with the work
* @return Contributor
*/ 

-(GnContributor*) contributor;

/**
* GnType
*/
+(NSString*) gnType;

/**
* From
*/
+(GnArtist*) from: (GnDataObject*)obj error: (NSError**)error;


@end



#endif /* _GnArtist_h_ */

