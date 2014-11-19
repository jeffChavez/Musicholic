/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnUser_h_
#define _GnUser_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnUserOptions.h"
#import "GnUserStoreDelegate.h"




/**
* Gracenote user
*/ 

@interface GnUser : NSObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* Reconstitutes user from serialized user handle data.
* Use this constructor to reconstitute a previously serialized GnUser. Reconstitution does not
* count towards the user count for your client in Gracenote Service.
* @param serializedUser		[in] String of serialized user handle data
* @param clientIdTest			[in_opt] Serialized user's expected Client ID
*/ 

-(INSTANCE_RETURN_TYPE) initWithSerializedUser: (NSString*)serializedUser clientIdTest: (NSString*)clientIdTest;

/**
* Create a GnUser with the provided Client ID and Client Tag. Check user storage for an
* existing user and if found reconstitutes the user. Reconstitution does not
* count towards the user count for your client in Gracenote Service. If not found in
* user storage a new user is created.
* @param userStore				[in] User store delegate
* @param clientId				[in] Client Identifer
* @param clientTag				[in] Client Tag
* @param applicationVersion	[in] Application version
*/ 

-(INSTANCE_RETURN_TYPE) initWithGnUserStoreDelegate: (id <GnUserStoreDelegate>)userStore clientId: (NSString*)clientId clientTag: (NSString*)clientTag applicationVersion: (NSString*)applicationVersion;

/**
* Gets flag indicating if the current user is only registered for local queries
* @return True if local only user, fals eotherwise
*/ 

-(BOOL) isLocalOnly:(NSError**) error;

/**
* Receive user options object. Use to set user options.
* @return User options object
*/ 

-(GnUserOptions*) options;


@end



#endif /* _GnUser_h_ */

