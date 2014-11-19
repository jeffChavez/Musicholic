/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnMoodgridResult_h_
#define _GnMoodgridResult_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnMoodgridIdentifier.h"


@class GnMoodgridIdentifierEnumerator;


/**
** <b>Experimental</b>: GnMoodgridResult
*/ 

@interface GnMoodgridResult : NSObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
*  Returns the count of the GnMoodgridIdentifier instances in this result.
*/ 

-(NSUInteger) count:(NSError**) error;

/**
* Retrieves an iterator to access the GnMoodgridIdentifier instances in this result.
*/ 

-(GnMoodgridIdentifierEnumerator*) identifiers;


@end



#endif /* _GnMoodgridResult_h_ */

