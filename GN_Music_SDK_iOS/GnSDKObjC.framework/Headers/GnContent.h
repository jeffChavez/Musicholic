/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnContent_h_
#define _GnContent_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnAsset.h"
#import "GnEnums.h"
#import "GnDataObject.h"


@class GnAssetEnumerator;


/**
* Provides access to content assets that can be retrieved from Urls.
* Content is only available is content is enabled from the original query's lookup data.
* Use the query object's options to enable content.
* The type of content that is available depends on the parent object. For example
* you can access album cover art from a GnAlbum object, but not an artist image. To
* retrieve an artist image navigate to a GnContributor object and invoke it's GnContent
* instance.
*/ 

@interface GnContent : GnDataObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* Id
*/
-(NSString*) id;

/**
*  Content type - cover art, biography, etc.
*  @return Content type
*/ 

-(GnContentType) contentType;

/**
*  Content's mime type
*  @return Mime type
*/ 

-(NSString*) mimeType;

/**
* Asset by image size (if applicable)
* @param imageSize	[in] Image size
* @return Asset object
*/ 

-(GnAsset*) asset: (GnImageSize)imageSize;

/**
* Iterator for the external third-party IDs (e.g., Amazon's)  associated with this contributor.
* @return Iterator
*/ 

-(GnAssetEnumerator*) assets;


@end


@interface GnContentEnumerator : NSEnumerator

typedef void (^GnContentBlock)(id obj, NSUInteger index, BOOL *stop);

-(void)enumerateObjectsUsingBlock: (GnContentBlock)handler;

-(NSUInteger) count;

-(GnContent*) nextObject;

-(GnContent*) objectAtIndex:(NSUInteger)index;

-(NSArray*) allObjects;


@end



#endif /* _GnContent_h_ */

