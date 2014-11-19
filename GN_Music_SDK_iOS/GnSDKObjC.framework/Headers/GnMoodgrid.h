/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnMoodgrid_h_
#define _GnMoodgrid_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnEnums.h"
#import "GnMoodgridDataPoint.h"
#import "GnMoodgridPresentation.h"
#import "GnMoodgridProvider.h"
#import "GnUser.h"


@class GnMoodgridProviderEnumerator;


/**
* <b>Experimental</b>: GnMoodgrid
*/ 

@interface GnMoodgrid : NSObject

/**
* GnMoodgrid
*/
-(INSTANCE_RETURN_TYPE) init;

/**
* Version
*/
+(NSString*) version;

/**
* BuildDate
*/
+(NSString*) buildDate;

/**
* Providers
*/
-(GnMoodgridProviderEnumerator*) providers;

/**
* CreatePresentation
*/
-(GnMoodgridPresentation*) createPresentation: (GnUser*)user type: (GnMoodgridPresentationType)type coordinate: (GnMoodgridCoordinateType)coordinate error: (NSError**)error;

/**
* Dimensions
*/
-(GnMoodgridDataPoint*) dimensions: (GnMoodgridPresentationType)type error: (NSError**)error;


@end



#endif /* _GnMoodgrid_h_ */

