/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnPlaylistCollectionSyncEventsDelegate_h_
#define _GnPlaylistCollectionSyncEventsDelegate_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnCancellableDelegate.h"
#import "GnPlaylistCollection.h"
#import "GnEnums.h"
#import "GnPlaylistIdentifier.h"


@class GnPlaylistCollection;
@class GnPlaylistIdentifier;


@protocol GnPlaylistCollectionSyncEventsDelegate <NSObject>

@required

/**
* OnUpdate
*/
-(void) onUpdate: (GnPlaylistCollection*)collection identifier: (GnPlaylistIdentifier*)identifier status: (GnPlaylistEventsIdentiferStatus)status cancellableDelegate: (id <GnCancellableDelegate>)canceller;


@end



#endif /* _GnPlaylistCollectionSyncEventsDelegate_h_ */

