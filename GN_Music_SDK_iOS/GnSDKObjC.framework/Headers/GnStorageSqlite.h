/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnStorageSqlite_h_
#define _GnStorageSqlite_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnStorageSqlite.h"




/**
* GNSDK SQLite storage provider
*/ 

@interface GnStorageSqlite : NSObject

/**
* Enable SQLite storage provider
* @return Storage provider instance
*/ 

+(GnStorageSqlite*) enable:(NSError**) error;

/**
*  Retrieves the version string of the Storage SQLite library.
*/ 

+(NSString*) version;

/**
*  Retrieves the build date string of the Storage SQLite library.
*/ 

+(NSString*) buildDate;

/**
*  Retrieves the version string of the internal SQLite database engine.
*/ 

+(NSString*) sqliteVersion;

/**
*  Sets a folder location for GNSDK SQLite storage
*  @param folderPath	[in] Relative path name for storage
* <p><b>Remarks:</b></p>
*  This API sets the folder location for ALL GNSDK SQLite storage - to set specific cache or database
*  locations use StorageLocation methods of the appropriate class.
*/ 

-(void) storageLocationWithFolderPath: (NSString*)folderPath error: (NSError**)error;

/**
* Get storage location folder for GNSDK SQLite storage
*/ 

-(NSString*) storageLocation:(NSError**) error;

/**
* Get the temporary storage location folder for GNSDK SQLite storage
*/ 

-(NSString*) temporaryStorageLocation:(NSError**) error;

/**
*  Sets a temporary folder location for GNSDK SQLite storage
*  @param folderPath	[in] Relative path name for storage
* <p><b>Remarks:</b></p>
*  This API sets the temporary folder location for ALL GNSDK SQLite storage.
*/ 

-(void) temporaryStorageLocationWithFolderPath: (NSString*)folderPath error: (NSError**)error;

/**
* Sets the maximum size the GNSDK cache can grow to; for example '100' for 100 Kb or '1024' for 1
*  MB. This limit applies to each cache that is created.
*
*  The value passed for this option is the maximum number of Kilobytes that the cache files can grow
*  to. For example, '100' sets the maximum to 100 KB, and '1024' sets the maximum to 1 MB.
*
*  If the cache files' current size already exceeds the maximum when this option is set, then the
*  set maximum is not applied.
*
*  When the maximum size is reached, new cache entries are not written to the database.
*  Additionally, a maintenance thread is run that attempts to clean up expired records from the
*  database and create space for new records.
*
*  If this option is not set the cache files default to having no maximum size.
*/ 

-(void) maximumCacheFileSizeWithMaxCacheSize: (NSUInteger)maxCacheSize error: (NSError**)error;

/**
* Get maximum cache file size
*/ 

-(NSUInteger) maximumCacheFileSize:(NSError**) error;

/**
*  Sets the maximum amount of memory SQLite can use to buffer cache data.
*
*  The value passed for this option is the maximum number of Kilobytes of memory that can be used.
*  For example, '100' sets the maximum to 100 KB, and '1024' sets the maximum to 1 MB.
*
*/ 

-(void) maximumCacheMemoryWithMaxMemSize: (NSUInteger)maxMemSize error: (NSError**)error;

/**
* Get maximum cache memory
*/ 

-(NSUInteger) maximumCacheMemory:(NSError**) error;

/**
*  Sets the method that SQLite uses to write to the cache files.
*
*  This option is available for SQLite performance tuning. Valid values for this option are:
*  <ul>
*  <li>OFF (default setting): No synchronous writing; the quickest but least safe method.</li>
*  <li>NORMAL: Synchronous writing only at critical times; the generally quick and safe method.</li>
*  <li>FULL: Always synchronous writing; the slowest and safest method.</li>
*  </ul>
*
*  If the threat of losing a cache file due to hardware failure is high, then set this option in
*  your application to NORMAL or FULL.
*/ 

-(void) synchronousModeWithMode: (NSString*)mode error: (NSError**)error;

/**
* Get synchronous mode setting
*/ 

-(NSString*) synchronousMode:(NSError**) error;

/**
*  Sets how the SQLite journal file is managed for database transactions.
*
*  This option is available for SQLite performance tuning. Valid values for this option are:
*  <ul>
*  <li>DELETE: Journal file is deleted after each transaction.</li>
*  <li>TRUNCATE: Journal file is truncated (but not deleted) after each transaction.</li>
*  <li>PERSIST: Journal file remains after each transaction.</li>
*  <li>MEMORY (default setting): Journal file is only stored in memory for each transaction.</li>
*  <li>OFF: No journaling is performed.</li>
*  </ul>
**/ 

-(void) journalModeWithMode: (NSString*)mode error: (NSError**)error;

/**
* Get journalling mode setting
*/ 

-(NSString*) journalMode:(NSError**) error;


@end



#endif /* _GnStorageSqlite_h_ */

