/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnCredit_h_
#define _GnCredit_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnContributor.h"
#import "GnDataObject.h"
#import "GnName.h"
#import "GnRole.h"




/**
* Lists the contribution of a person (or occasionally a company, such as a record label)
* to a recording.
*/ 

@interface GnCredit : GnDataObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* Credit's name, such as the name of the person or company.
* @return Name
*/ 

-(GnName*) name;

/**
* Credit's role
* @return Role
* <p><b>Remarks:</b></p>
* This is a list-based value requiring that a corresponding locale or list be loaded.
*/ 

-(GnRole*) role;

/**
* Credit's contributor.
* @return Contributor
*/ 

-(GnContributor*) contributor;

/**
* GnType
*/
+(NSString*) gnType;

/**
* From
*/
+(GnCredit*) from: (GnDataObject*)obj error: (NSError**)error;


@end


@interface GnCreditEnumerator : NSEnumerator

typedef void (^GnCreditBlock)(id obj, NSUInteger index, BOOL *stop);

-(void)enumerateObjectsUsingBlock: (GnCreditBlock)handler;

-(NSUInteger) count;

-(GnCredit*) nextObject;

-(GnCredit*) objectAtIndex:(NSUInteger)index;

-(NSArray*) allObjects;


@end



#endif /* _GnCredit_h_ */

