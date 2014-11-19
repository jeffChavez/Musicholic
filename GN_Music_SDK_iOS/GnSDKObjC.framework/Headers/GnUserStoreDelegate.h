/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnUserStoreDelegate_h_
#define _GnUserStoreDelegate_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"





@protocol GnUserStoreDelegate <NSObject>

@required

/**
* LoadSerializedUser
*/
-(NSString*) loadSerializedUser: (NSString*)clientId;

/**
* StoreSerializedUser
*/
-(BOOL) storeSerializedUser: (NSString*)clientId userData: (NSString*)userData;


@end



#endif /* _GnUserStoreDelegate_h_ */

