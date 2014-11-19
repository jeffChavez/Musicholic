/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnResponseDataMatches_h_
#define _GnResponseDataMatches_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnDataMatch.h"
#import "GnDataObject.h"


@class GnDataMatchEnumerator;


/**
* Collection of data match results received in response to a data match query.
*/ 

@interface GnResponseDataMatches : GnDataObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
*  Result count - number of matches returned
*  @return Count
*/ 

-(NSUInteger) resultCount;

/**
*  Range start - ordinal value of first match in range total
*  @return Range start
*/ 

-(NSUInteger) rangeStart;

/**
*  Range end - ordinal value of last match in range total
*  @return Range end
*/ 

-(NSUInteger) rangeEnd;

/**
*  Range total - total number of matches that could be returned
*  @return Range total
*/ 

-(NSUInteger) rangeTotal;

/**
*  Flag indicating if response needs user or app decision - either multiple matches returned or less than perfect single match
*  @return True is user decision needed, false otherwise
*/ 

-(BOOL) needsDecision;

/**
* Iterator for accessing response match(es).
* @return Iterator
*/ 

-(GnDataMatchEnumerator*) dataMatches;


@end



#endif /* _GnResponseDataMatches_h_ */

