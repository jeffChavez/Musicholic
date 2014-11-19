/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnUserOptions_h_
#define _GnUserOptions_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnEnums.h"




/**
* Configuration options for GnUser
*/ 

@interface GnUserOptions : NSObject

/**
* Constructs empty GnUserOptions object. It is not associated with any GnUser object.
*/ 

-(INSTANCE_RETURN_TYPE) init;

/**
* Get lookup mode.
* @return Lookup mode
*/ 

-(GnLookupMode) lookupMode:(NSError**) error;

/**
* Set lookup mode.
* @param lookupMode	[in] Lookup mode
*/ 

-(void) lookupModeWithLookupMode: (GnLookupMode)lookupMode error: (NSError**)error;

/**
* Get network proxy hostname
* @return Network proxy hostname
*/ 

-(NSString*) networkProxy:(NSError**) error;

/**
* Sets host name, username and password for proxy to route GNSDK queries through
* @param hostname	[in] Fully qualified host name with optional port number. If no port number
*                  is given the default port number is assumed to be 80.
*                  Example values are "http://proxy.mycompany.com:8080/", "proxy.mycompany.com:8080" and
*                  "proxy.mycompany.com"
* @param username  [in] Valid user name for the proxy server. Do not set this option if a username is not required.
* @param password  [in] Valid password for the proxy server. Do not set this option if a password is not required.
*/ 

-(void) networkProxyWithHostname: (NSString*)hostname username: (NSString*)username password: (NSString*)password error: (NSError**)error;

/**
* Gets the network time-out for all GNSDK queries
* @return Netwrk timeout
*/ 

-(NSUInteger) networkTimeout:(NSError**) error;

/**
* Sets the network time-out for all GNSDK queries in milliseconds.
* Value for this option is a string with a numeric value that indicates the number of milliseconds
* to set for network time-outs. For example for a 30-second time-out set option value as 30000
* @param timeout_ms	[in] Time-out in milliseconds
*/ 

-(void) networkTimeoutWithTimeout_ms: (NSUInteger)timeout_ms error: (NSError**)error;

/**
* Get network local balance state
* @return Network load balance state
*/ 

-(BOOL) networkLoadBalance:(NSError**) error;

/**
* Enables/Disable distributing queries across multiple Gracenote co-location facilities
* When not enabled, queries will generally resolve to a single co-location.
* To implement load balancing, enable this option.
* Value to enable this option must be a boolean value that indicates true.
* @param bEnable  [in] True to enable load balancing, false otherwise.
* <p><b>Note:</b></p>
* Ensure that any security settings (such as a firewall) in your network infrastructure do not
* affect outbound access and prevent GNSDK from transmitting queries to various hosts with unique IP
* addresses.
*/ 

-(void) networkLoadBalanceWithBEnable: (BOOL)bEnable error: (NSError**)error;

/**
* Get the IP address of the network interface as configured for this User object
* @return IP address
*/ 

-(NSString*) networkInterface:(NSError**) error;

/**
* Set a specific network interface to be used with connections made by GNSDK utilizing this GnUser object.
* Choosing which interface to use can be beneficial for systems with multiple network interfaces. Without
* setting this option, connections will be made of the default network interface as decided by the
* operating system. To enable this option a valid local IP address of the desired network interface must be
* provided.
* @param nicIpAddress	IP address of a local network interface
*/ 

-(void) networkInterfaceWithNicIpAddress: (NSString*)nicIpAddress error: (NSError**)error;

/**
* Set information about this user
* @param location_id	[in] Set an IP address or country code to represent the location of user performing requests.
*                      Value for this parameter is a string with the IP address, or a 3-character country code
*                      for the client making the request. This is generally required when setting a proxy for
*                      GNSDK queries. Example values are "192.168.1.1", "usa" and "jpn".
* @param mfg			[in] The manufacturer of the device running the SDK. Used mostly by Gracenote Service to collect
*                      runtime statistics.
* @param os			[in] The OS version of the device running the SDK. Used mostly by Gracenote Service to collect
*                      runtime statistics.
* @param uid			[in] Unique ID of the device running the SDK, such as ESN. Used mostly by Gracenote Service to
*                      collect runtime statistics.
*/ 

-(void) userInfo: (NSString*)location_id mfg: (NSString*)mfg os: (NSString*)os uid: (NSString*)uid error: (NSError**)error;

/**
* Gets cache expiration time in seconds
* @return Cache expiration time
*/ 

-(NSUInteger) cacheExpiration:(NSError**) error;

/**
* Sets the maximum duration for which an item in the GNSDK query cache is valid. This duration is in
* seconds, and must exceed one day.
* The value set for this option is a string with a numeric value that indicates the number of
* seconds to set for the expiration of cached queries. The maximum duration is set by Gracenote and
* varies by requirements.
* @param durationSec		[in] Expiration duration in seconds. For example, for a one day expiration
* 							set an option value of "86400" (60 seconds * 60 minutes * 24 hours); for a
* 							seven day expiration set an option value of "604800"
* 							(60 seconds * 60 minutes * 24 hours * 7 days).
* <p><b>Note:</b></p>
* Setting this option to a zero value (0) causes the cache to start deleting records upon cache
* hit, and not write new or updated records to the cache; in short, the cache effectively flushes
* itself. The cache will start caching records again once this option is set to a value greater than
* 0. Setting this option to a value less than 0 (for example: -1) causes the cache to use default
* expiration values.
*
* For mobile platforms Android, iOS and Windows Mobile the default cache expiration is zero.
*
* Cache expiration only has an effect if the application initializes a GNSDK storage provider such as
* GnStorageSqlite. Without a storage provider no cache can be created.
*
*/ 

-(void) cacheExpirationWithDurationSec: (NSUInteger)durationSec error: (NSError**)error;

/**
* Gets user option
* @param key	[in] Option key
* @return Option value
*/ 

-(NSString*) customWithKey: (NSString*)key error: (NSError**)error;

/**
* Sets User option
* @param key	[in] Option key
* @param value	[in] Option value
*/ 

-(void) customWithKey: (NSString*)key value: (NSString*)value error: (NSError**)error;


@end



#endif /* _GnUserOptions_h_ */

