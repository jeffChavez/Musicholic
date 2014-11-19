/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnLookupLocalStreamIngest_h_
#define _GnLookupLocalStreamIngest_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnCancellableDelegate.h"
#import "GnLookupLocalStreamIngestEventsDelegate.h"
#import "GnEnums.h"




/**
* GnLookupLocalStreamIngest
*/
@interface GnLookupLocalStreamIngest : NSObject

/**
*  Constructor for creating a GnLookupLocalStreamIngest Object
*  @param pEventDelegate           [in] Delegate to receive events .
*  @param callbackData             [in] Optional user data that will be sent to the Delegate
*/ 

-(INSTANCE_RETURN_TYPE) initWithGnLookupLocalStreamIngestEventsDelegate: (id <GnLookupLocalStreamIngestEventsDelegate>)pEventDelegate;

/**
* Write to the ingestion process . This can be called multiple times to ensure that data is written as and when it is available.
*  @param data             [in] data to ingest
*  @param dataLength       [in] size of data being written
*/ 

-(void) write: (unsigned char*)bundleData dataSize: (NSUInteger)dataSize error: (NSError**)error;

/**
* Flushes the memory cache to the file storage and commits the changes. This call will result in IO .
* Use this method to ensure that everything written is commited to the file system.
* Note: This is an optional call as internally data is flushed when it exceed the cache size and when the object goes out of scope..
*/ 

-(void) flush:(NSError**) error;


@end



#endif /* _GnLookupLocalStreamIngest_h_ */

