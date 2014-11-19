/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnMoodgridPresentation_h_
#define _GnMoodgridPresentation_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnEnums.h"
#import "GnMoodgridDataPoint.h"
#import "GnMoodgridProvider.h"
#import "GnMoodgridResult.h"


@class GnMoodgridDataPointEnumerator;


/**
** <b>Experimental</b>: GnMoodgridPresentation
*/ 

@interface GnMoodgridPresentation : NSObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* Moods
*/
-(GnMoodgridDataPointEnumerator*) moods:(NSError**) error;

/**
* LayoutType
*/
-(GnMoodgridPresentationType) layoutType:(NSError**) error;

/**
* CoordinateType
*/
-(GnMoodgridCoordinateType) coordinateType;

/**
* AddFilter
*/
-(void) addFilter: (NSString*)uniqueIndentfier elistType: (GnMoodgridFilterListType)elistType strValueId: (NSString*)strValueId eConditionType: (GnMoodgridFilterConditionType)eConditionType error: (NSError**)error;

/**
* RemoveFilter
*/
-(void) removeFilter: (NSString*)uniqueIndentfier error: (NSError**)error;

/**
* RemoveAllFilters
*/
-(void) removeAllFilters:(NSError**) error;

/**
* MoodName
*/
-(NSString*) moodName: (GnMoodgridDataPoint*)position error: (NSError**)error;

/**
* MoodId
*/
-(NSString*) moodId: (GnMoodgridDataPoint*)position error: (NSError**)error;

/**
* FindRecommendations
*/
-(GnMoodgridResult*) findRecommendations: (GnMoodgridProvider*)provider position: (GnMoodgridDataPoint*)position error: (NSError**)error;

/**
* FindRecommendationsEstimate
*/
-(NSUInteger) findRecommendationsEstimate: (GnMoodgridProvider*)provider position: (GnMoodgridDataPoint*)position error: (NSError**)error;


@end



#endif /* _GnMoodgridPresentation_h_ */

