/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnLog_h_
#define _GnLog_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnLog.h"
#import "GnLogColumns.h"
#import "GnLogEventsDelegate.h"
#import "GnLogFilters.h"
#import "GnEnums.h"
#import "GnLogOptions.h"




/**
* Configures and enables GNSDK logging including registering custom logging packages
* and writing your own logging message to the GNSDK log
*/ 

@interface GnLog : NSObject

/**
* Instantiate a GNSDK logging instance
* @param logFilePath		[in] File path of the logging file
* @param pLoggingDelegate	[in] Delegate to receive logging messages
*/ 

-(INSTANCE_RETURN_TYPE) initWithLogFilePath: (NSString*)logFilePath logEventsDelegate: (id <GnLogEventsDelegate>)pLoggingDelegate;

/**
* Instantiate a GNSDK logging instance
* <p><b>Remarks:</b></p>
* Logging is not started until GnLog::Enable() is called
* @param logFilePath		[in] File path of the logging file
* @param filters			[in] Logging filters
* @param columns			[in] Logging columns
* @param options			[in] Logging options
* @param pLoggingDelegate	[in] Delegate to receive logging messages
*/ 

-(INSTANCE_RETURN_TYPE) initWithLogFilePath: (NSString*)logFilePath filters: (GnLogFilters*)filters columns: (GnLogColumns*)columns options: (GnLogOptions*)options logEventsDelegate: (id <GnLogEventsDelegate>)pLoggingDelegate;

/**
* Set logger instance options
*/ 

-(void) options: (GnLogOptions*)options;

/**
* Set logger instance filters
*/ 

-(void) filters: (GnLogFilters*)filters;

/**
* Set logger instance columns
*/ 

-(void) columns: (GnLogColumns*)columns;

/**
* Enable logging for the given package with the current logging options and filters.
* Enable can be called multiple times to enable logging of multiple packages to the same log.
* <p><b>Remarks:</b></p>
* Changes to logging options and filters do not take affect until the logger is next enabled.
* @param package [in] GnLogPackage to enable for this log.
* @return This GnLog object to allow method chaining.
*/ 

-(GnLog*) enableWithPackage: (GnLogPackageType)package error: (NSError**)error;

/**
* Enable logging for the custom package id with the current logging options and filters.
* Enable can be called multiple times to enable logging of multiple packages to the same log.
* <p><b>Remarks:</b></p>
* Changes to logging options and filters do not take affect until the logger is next enabled.
* @param package [in] GnLogPackage to enable for this log.
* @return This GnLog object to allow method chaining.
*/ 

-(GnLog*) enableWithCustomPackageId: (NSUInteger)customPackageId error: (NSError**)error;

/**
* Disable logging for the given package for the current log.
* If no other packages are enabled for the log, the log will be closed
* @param package [in] GnLogPackage to disable for this log.
* @return This GnLog object to allow method chaining.
*/ 

-(GnLog*) disableWithPackage: (GnLogPackageType)package error: (NSError**)error;

/**
* Disable logging for the custom package id for the current log.
* If no other packages are enabled for the log, the log will be closed
* @param package [in] GnLogPackage to disable for this log.
* @return This GnLog object to allow method chaining.
*/ 

-(GnLog*) disableWithCustomPackageId: (NSUInteger)customPackageId error: (NSError**)error;

/**
* Register an application with a unique packageid and package name 
* @param customPackageId		[in] Custom Package ID : must be unique and be within the custom package range.
* @param customPackageName     [in] Custom tag to identfiy each log message. e.g. "MYAPP" .
* @return This GnLog object to allow method chaining.
* <p><b>Remarks:</b></p>
* This function enables an application register a custom package Id and package Name for writing to the log
* system. 
*/ 

-(GnLog*) register: (NSUInteger)customPackageId customPackageName: (NSString*)customPackageName error: (NSError**)error;

/**
* Write a message to the custom registered log 
* @param line              [in] Source line number of this call
* @param fileName          [in] Source file name of this call
* @param customPackageId   [in] Custom Package ID of application making call
* @param messageType       [in] Logging message type
* @param format            [in] Log message format
* <p><b>Remarks:</b></p>
* This function enables an application to write a logging message to the GNSDK logging
* system. If a logging events delegate is provided that handles the given package ID and enabled log filters,
* then this message is written to those log(s). The customer Package ID must be one previously registered by the
* application.
*/ 

+(void) write: (NSInteger)line fileName: (NSString*)fileName customPackageId: (NSUInteger)customPackageId messageType: (GnLogMessageType)messageType format: (NSString*)format;


@end



#endif /* _GnLog_h_ */

