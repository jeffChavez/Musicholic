/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnLogOptions_h_
#define _GnLogOptions_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnLogOptions.h"




/**
*  Logging options specifies what options are applied to the GNSDK log
*/ 

@interface GnLogOptions : NSObject

/**
* GnLogOptions
*/
-(INSTANCE_RETURN_TYPE) init;

/**
* Specify the log to be written synchronously (on the same thread).
* By default logs are written to asynchronously. No internal logging
* thread is created if all GnLog instances are specified for synchronous
* writing.
*/ 

-(GnLogOptions*) synchronous: (BOOL)bSyncWrite;

/**
* Specify for the archive to retain and rename old logs, default behavior
* is to delete old logs
*/ 

-(GnLogOptions*) archive: (BOOL)bArchive;

/**
* Specify that when archive is also specified the logs to archive (roll)
* at the start of each day (12:00 midnight). Archiving by the given size
* parameter will still occur normally as well.
*/ 

-(GnLogOptions*) archiveDaily;

/**
* Specify the maximum size of log before new log is created. Enter a value of zero (0) to
* always create new log on open
*/ 

-(GnLogOptions*) maxSize: (NSUInteger)maxSize;


@end



#endif /* _GnLogOptions_h_ */

