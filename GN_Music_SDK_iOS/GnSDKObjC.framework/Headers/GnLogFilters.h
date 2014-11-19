/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnLogFilters_h_
#define _GnLogFilters_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnLogFilters.h"




/**
* Logging Filters
*/ 

@interface GnLogFilters : NSObject

/**
* GnLogFilters
*/
-(INSTANCE_RETURN_TYPE) init;

/** Include error logging messages */ 

-(GnLogFilters*) clear;

/** Include error logging messages */ 

-(GnLogFilters*) error;

/** Include warning logging messages */ 

-(GnLogFilters*) warning;

/** Include informative logging messages */ 

-(GnLogFilters*) info;

/** Include debugging logging messages */ 

-(GnLogFilters*) debug;

/** Include all logging messages */ 

-(GnLogFilters*) all;


@end



#endif /* _GnLogFilters_h_ */

