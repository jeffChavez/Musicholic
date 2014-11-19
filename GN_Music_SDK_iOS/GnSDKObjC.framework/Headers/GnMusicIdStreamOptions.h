/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnMusicIdStreamOptions_h_
#define _GnMusicIdStreamOptions_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnEnums.h"




/**
* Configures options for GnMusicIdStream
*/ 

@interface GnMusicIdStreamOptions : NSObject

/**
*  Specifies whether identification should be performed against local embedded databases or online.
*  @param lookupMode  [in] One of the GnLookupMode values
*/ 

-(void) lookupMode: (GnLookupMode)lookupMode error: (NSError**)error;

/**
*  Specifies which data should be included in the response
*  @param val 		[in] Set One of the GnLookupData values
*  @param enable 	[in] True or false to enable or disable
*/ 

-(void) lookupData: (GnLookupData)val enable: (BOOL)enable error: (NSError**)error;

/**
*  @deprecated To set language provide appropriate GnLocale object with GnMusicIdStream constructor
*/ 

-(void) preferResultLanguage: (GnLanguage)preferredLanguage error: (NSError**)error;

/**
* Specifies preference for results that contain the provided external identifier
* <p><b>Remarks:</b></p>
* This option is currently only supported when online processing is enabled and single
* result is specified.
* @param preferredExternalId 	[in] The name of an external identifier that should be preferred when selecting matches
*/ 

-(void) preferResultExternalId: (NSString*)preferredExternalId error: (NSError**)error;

/**
*  Specifies preference for results that have cover art associated
*  @param bEnable 	[in] Set prefer cover art.
*/ 

-(void) preferResultCoverart: (BOOL)bEnable error: (NSError**)error;

/**
*  Specifies whether a response must return only the single best result.
*  When enabled a single full result is returned, when disabled multiple partial results may be returned.
*  @param bEnable 	[in] Option, default is true. True to enable, false to disable.
*/ 

-(void) resultSingle: (BOOL)bEnable error: (NSError**)error;

/**
*  Specifies whether a response must return a range of results that begin at the specified values
*  @param resultStart  [in] Result range start value
*  <p><b>Remarks:</b></p>
*  This Option is useful for paging through results.
*  <p><b>Note:</b></p>
*  Gracenote Service enforces that the range start value must be less than or equal to the total
*  number of results. If you specify a range start value that is greater than the total number of
*  results, no results are returned.
*/ 

-(void) resultRangeStart: (NSUInteger)resultStart error: (NSError**)error;

/**
* Specifies the number of results to return in the response
* @param resultCount 	[in] Number of results
*
*/ 

-(void) resultCount: (NSUInteger)resultCount error: (NSError**)error;

/**
*  Specifies if revision check should be performed.
*  @param bEnable 	[in] Optional, default is true. True to enable, false to disable.
*//**
*  General option setting for custom string options
*  @param optionKey   [in] Option name
*  @param value	   [in] Option value
*/ 

-(void) custom: (NSString*)optionKey value: (NSString*)value error: (NSError**)error;


@end



#endif /* _GnMusicIdStreamOptions_h_ */

