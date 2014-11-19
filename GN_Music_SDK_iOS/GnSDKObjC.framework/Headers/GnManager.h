/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnManager_h_
#define _GnManager_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnEnums.h"
#import "GnList.h"
#import "GnLocale.h"
#import "GnStoreOps.h"
#import "GnSystemEventsDelegate.h"
#import "GnUser.h"




/**
* Provides core functionality necessary to all Gracenote objects. This class must
* be instantiated prior to any other Gracenote objects.
*/ 

@interface GnManager : NSObject

/**
* Initializes GNSDK. You must initialize the GNSDK prior to instantiating any other GNSDK objects.
* Valid license data must be provided. Gracenote Global Services and Support team provides a license with the SDK.
* @param license			[in] license data
* @param licenseInputMode	[in] how the license data is being provided
*/ 

-(INSTANCE_RETURN_TYPE) initWithLicense: (NSString*)license licenseInputMode: (GnLicenseInputMode)licenseInputMode;

/**
* Retrieves the GNSDK version string.
* This API can be called at any time after GnManager instance is constructed successfully. The returned
* string is a constant. Do not attempt to modify or delete.
* Example: 1.2.3.123 (Major.Minor.Improvement.Build)
* Major: New functionality
* Minor: New or changed features
* Improvement: Improvements and fixes
*/ 

+(NSString*) version;

/**
* Retrieves the product version string.
* This API can be called at any time after GnManager instance is constructed successfully. The returned
* string is a constant. Do not attempt to modify or delete.
* Example: 1.2.3.123 (Major.Minor.Improvement.Build)
* Major: New functionality
* Minor: New or changed features
* Improvement: Improvements and fixes
* Build: Internal build number
*/ 

+(NSString*) productVersion;

/**
* Retrieves the GNSDK's build date string.
* This API can be called at any time after GnManager instance is constructed successfully. The returned
* string is a constant. Do not attempt to modify or delete.
* Example: 2008-02-12 00:41 UTC
*/ 

+(NSString*) buildDate;

/**
* GlobalIdMagic
*/
+(NSString*) globalIdMagic;

/**
* Creates a new Serialized User and also increments the user's Client ID user count with Gracenote Service.
* Use this constructor to create a new user; when successful, this call registers a new user for
* a specified client in Gracenote Service. Once the new user is registered and the user count
* incremented in Gracenote Service, the count cannot be reduced, nor can the same user be
* again retrieved.
* Newly registered user handles must be serialized and stored locally for that user to be used
* again for future queries; failing to do this quickly depletes the client's allotted user quota
* within the Gracenote Service.
* @param registerMode  		[in] register as online or local only user
* @param clientId     			[in] client ID that initiates requests with this handle; value provided by
*                      		Gracenote
* @param clientTag    			[in] client ID tag value that matches client ID; value provided by Gracenote
* @param applicationVersion  	[in] client application version; numeric value provided by application, and
*                      		this value is required
*/ 

-(NSString*) userRegister: (GnUserRegisterMode)registerMode clientId: (NSString*)clientId clientTag: (NSString*)clientTag applicationVersion: (NSString*)applicationVersion error: (NSError**)error;

/**
* Get query cache storage operations instance
*/ 

-(GnStoreOps*) queryCacheStore;

/**
* Get content cache storage operations instance
*/ 

-(GnStoreOps*) contentCacheStore;

/**
* Get locales storage operations instance
*/ 

-(GnStoreOps*) localesStore;

/**
* Provide a delegate to receive system events
* @param pEventHandler		[in] event handler delegate
*/ 

-(void) systemEventHandler: (id <GnSystemEventsDelegate>)pEventHandler;

/**
* SystemMemoryEvent
*/
-(void) systemMemoryEvent: (NSUInteger)memoryWarnSize;

/**
* SystemMemoryCurrent
*/
-(NSUInteger) systemMemoryCurrent;

/**
* SystemMemoryHighWater
*/
-(NSUInteger) systemMemoryHighWater: (BOOL)bResetHighWater;

/**
* TestGracenoteConnection
*/
-(void) testGracenoteConnection: (GnUser*)user error: (NSError**)error;


@end



#endif /* _GnManager_h_ */

