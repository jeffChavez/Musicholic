/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnRole_h_
#define _GnRole_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnDataObject.h"




/**
* Represents the role that a contributor played in a music or video production;
* for example, singing, playing an instrument, acting, directing, and so on.
* <p><b>Note:</b></p>
* For music credits, the absence of a role for a person indicates that person is the primary
* artist, who may have performed multiple roles.
*/ 

@interface GnRole : GnDataObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* Role category, such as string instruments or brass instruments.
* @return Category
* <p><b>Remarks:</b></p>
* This is a list-based value requiring that a corresponding locale or list be loaded.
*/ 

-(NSString*) category;

/**
* Role's display string.
* @return Role
*/ 

-(NSString*) role;


@end



#endif /* _GnRole_h_ */

