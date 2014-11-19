/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnMusicIdStreamEventsDelegate_h_
#define _GnMusicIdStreamEventsDelegate_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnCancellableDelegate.h"
#import "GnEnums.h"
#import "GnResponseAlbums.h"
#import "GnStatusEventsDelegate.h"


@class GnResponseAlbums;


@protocol GnMusicIdStreamEventsDelegate <GnStatusEventsDelegate>

@required

/**
* MusicIdStreamProcessingStatusEvent is currently considered to be experimental.
* An application should only use this option if it is advised by Gracenote Global Services and Support representative.
* Contact your Gracenote Global Services and Support representative with any questions about this enhanced functionality.
*/ 

-(void) musicIdStreamProcessingStatusEvent: (GnMusicIdStreamProcessingStatus)status cancellableDelegate: (id <GnCancellableDelegate>)canceller;

/**
* Provides GnMusicIdStream identifying status notification
* @param status		Status
* @param canceller		Cancellable that can be used to cancel this identification operation
*/ 

-(void) musicIdStreamIdentifyingStatusEvent: (GnMusicIdStreamIdentifyingStatus)status cancellableDelegate: (id <GnCancellableDelegate>)canceller;

/**
* A result is available for a GnMusicIdStream identification request
* @param result		Result
* @param canceller		Cancellable that can be used to cancel this identification operation
*/ 

-(void) musicIdStreamAlbumResult: (GnResponseAlbums*)result cancellableDelegate: (id <GnCancellableDelegate>)canceller;

/**
* Identifying request could not be completed due to the reported error condition
* @param pErrorInfo	Error condition information
*/ 

-(void) musicIdStreamIdentifyCompletedWithError: (NSError*)completeError;


@end



#endif /* _GnMusicIdStreamEventsDelegate_h_ */

