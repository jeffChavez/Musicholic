/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnAsset_h_
#define _GnAsset_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnDataObject.h"
#import "GnEnums.h"




/**
* Assets for content (cover art, biography etc)
*/ 

@interface GnAsset : GnDataObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
*  Asset dimension
*  @return Dimention string
*/ 

-(NSString*) dimension;

/**
*  Size of content asset in bytes
*  @return Size in bytes
*/ 

-(NSInteger) bytes;

/**
*  Pixel image size of asset as defined with a GnImageSize enum, e.g., kImageSize110 (110x110)
*  @return Image size
*/ 

-(GnImageSize) size;

/**
*  Url for retrieval of asset from Gracenote
*  @return URL
*/ 

-(NSString*) url;


@end


@interface GnAssetEnumerator : NSEnumerator

typedef void (^GnAssetBlock)(id obj, NSUInteger index, BOOL *stop);

-(void)enumerateObjectsUsingBlock: (GnAssetBlock)handler;

-(NSUInteger) count;

-(GnAsset*) nextObject;

-(GnAsset*) objectAtIndex:(NSUInteger)index;

-(NSArray*) allObjects;


@end



#endif /* _GnAsset_h_ */

