/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnLookupLocalStream_h_
#define _GnLookupLocalStream_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnEnums.h"
#import "GnLookupLocalStream.h"




/**
* Provides services for local lookup of MusicID-Stream queries
*
* GnLookupLocalStream is a MusicID-Stream local lookup provider, providing a local
* database of MusicID-Stream tracks suitable for lookup via GnMusicIdStream or GnMusicId.
*
* The local database is a Gracenote storage; therefore a storage provider, such as GnStorageSqlite,
* must be enabled as a pre-requisite to successfully using this class.
*
* <b>Bundle Ingestion</b>
*
* The MusicID-Stream local database is constructed from "bundles" provided periodically by Gracenote.
* Your application must ingest the bundle, a process that adds the tracks from the bundle to the local
* database making them available for local recognition.
*
* To ingest a bundle create an instance of GnLookupLocalStreamIngest and write the bundle bytes
* as received when streaming the bundle from your online service.
*
* Multiple bundles can be ingested by the same application. If the same track exists in multiple
* ingested bundles it is added to the local database only once and only the most recent/up-to-date track
* information is written to the database.
*
* The process of ingestion can be lengthy. Some applications may wish to perform ingestion on a
* background thread to avoid stalling the main application thread.
*
* <b>Bundle Management</b>
*
* Bundles are regularly generated for multiple global regions and in multiple flavors being
* incremental (only differences from previous bundle for that region) or full (all records for the
* region).
*
* Incremental bundles are intended for applications that have installed the previous bundles in the
* sequence. If your application skips a bundle it should clear it's MusicID-Stream local database
* and install the full bundle.
*
* Your bundle management system must provide the means to regularly distribute the correct bundle to
* your application, ensuring it provides the bundle for the correct region and, as appropriate, the
* full or incremental bundle.
*
* A typical bundle management system is:<br>
* - retrieve bundles as produced by Gracenote (manual step)<br>
* - place bundle in online location where application can fetch it<br>
* - application downloads and ingests the bundle
*
* Note: Bundles should be retrieved from an online source. Gracenote recommends when your application
* is installed/initialized it download and ingest the latest bundle rather than ship with a bundle as
* part of the application binaries.
*/ 

@interface GnLookupLocalStream : NSObject

/**
* Enable Lookup local Stream provider.
* @return Storage provider instance
*/ 

+(GnLookupLocalStream*) enable:(NSError**) error;

/**
*  Retrieves GnLookupLocalStream SDK version string.
*  This API can be called at any time, after getting instance of GnManager successfully. The returned
*  string is a constant. Do not attempt to modify or delete.
*  Example: 1.2.3.123 (Major.Minor.Improvement.Build)
*  Major: New functionality
*  Minor: New or changed features
*  Improvement: Improvements and fixes
*  Build: Internal build number
*/ 

+(NSString*) version;

/**
*  Retrieves the GnLookupLocalStream SDK's build date string.
*  @return gnsdk_cstr_t Build date string of the format: YYYY-MM-DD hh:mm UTC
*  <p><b>Remarks:</b></p>
*  This API can be called at any time, after getting instance of GnManager successfully. The returned
* string is a constant. Do not attempt to modify or delete.
*  Example build date string: 2008-02-12 00:41 UTC
*/ 

+(NSString*) buildDate;

/**
*  Sets a folder location for GNSDK LookupLocalStream
*  @param folderPath	[in] Relative path name for storage
* <p><b>Remarks:</b></p>
*  This API overrides the (default or explicit) folder location of GNSDK SQLite storage - for this database.
*/ 

-(void) storageLocation: (NSString*)location error: (NSError**)error;

/**
* EngineType
*/
-(void) engineTypeWithEngineType: (GnLocalStreamEngineType)engineType error: (NSError**)error;

/**
* EngineType
*/
-(GnLocalStreamEngineType) engineType:(NSError**) error;

/**
* Clear all tracks from the MusicID-Stream local database storage file
*/ 

-(void) storageClear:(NSError**) error;

/**
* Remove an item from the MusicID-Stream local database identified by
* Bundle Item ID.
* @param	bundleItemId	[in] Bundle Item ID
*/ 

-(void) storageRemove: (NSString*)bundleItemId error: (NSError**)error;


@end



#endif /* _GnLookupLocalStream_h_ */

