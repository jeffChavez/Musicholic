/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnLogEventsDelegate_h_
#define _GnLogEventsDelegate_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"





@protocol GnLogEventsDelegate <NSObject>

@required

/**
* Implement to receive logging messages from GNSDK
*/ 

-(BOOL) logMessage: (NSUInteger)packageId filterMask: (NSUInteger)filterMask errorCode: (NSUInteger)errorCode message: (NSString*)message;


@end



#endif /* _GnLogEventsDelegate_h_ */

