/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnSystemEventsDelegate_h_
#define _GnSystemEventsDelegate_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnList.h"
#import "GnLocale.h"


@class GnList;
@class GnLocale;


@protocol GnSystemEventsDelegate <NSObject>

@required

/**
* Notification event that the give Locale should be updated.
* @param locale	[in] Locale detected as out of date
*/ 

-(void) localeUpdateNeeded: (GnLocale*)locale;

/**
* Notification event that the given List should be updated.
* @param list		[in] List detected as out of date
*/ 

-(void) listUpdateNeeded: (GnList*)list;

/**
* Notifcation that GNSDK memory usage has gone over the set warning limit
* @param curMemSize		[in] Current memory size
* @param memoryWarnSize	[in] Memory warning size
*/ 

-(void) systemMemoryWarning: (NSUInteger)curMemSize memoryWarnSize: (NSUInteger)memoryWarnSize;


@end



#endif /* _GnSystemEventsDelegate_h_ */

