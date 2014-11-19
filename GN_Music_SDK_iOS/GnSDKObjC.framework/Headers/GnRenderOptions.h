/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnRenderOptions_h_
#define _GnRenderOptions_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnEnums.h"
#import "GnRenderOptions.h"




/**
* \class GnRenderOptions
* Rendering options (e.g., JSON, XML)
*/ 

@interface GnRenderOptions : NSObject

/**
* Construct GnRenderOptions object
*/ 

-(INSTANCE_RETURN_TYPE) init;

/**
* Specify render format of XML
* @return Render options object
*/ 

-(GnRenderOptions*) xml;

/**
* Specify render format of JSON
* @return Render options object
*/ 

-(GnRenderOptions*) json;

/**
* Standard
*/
-(GnRenderOptions*) standard;

/**
* Specify rendered output include Credits
* @return Render options object
*/ 

-(GnRenderOptions*) credits;

/**
* Specify rendered output include Sortable information
* @return Render options object
*/ 

-(GnRenderOptions*) sortable;

/**
* SerialGdos
*/
-(GnRenderOptions*) serialGdos;

/**
* Specify rendered output include Product IDs
* @return Render options object
*/ 

-(GnRenderOptions*) productIds;

/**
* Specify rendered output include raw TUIs
* @return Render options object
*/ 

-(GnRenderOptions*) rawTuis;

/**
* Specify rendered output include Gracenote IDs
* @return Render options object
*/ 

-(GnRenderOptions*) gnIds;

/**
* GnUIds
*/
-(GnRenderOptions*) gnUIds;

/**
* Specify rendered output include Genre descriptors for specified level
* @param level	[in] Data level
* @return Render options object
*/ 

-(GnRenderOptions*) genres: (GnDataLevel)level;

/**
* Default
*/
-(GnRenderOptions*) default;

/**
* Full
*/
-(GnRenderOptions*) full;

/**
* Clear render options
* @return Render options object
*/ 

-(GnRenderOptions*) clear;


@end



#endif /* _GnRenderOptions_h_ */

