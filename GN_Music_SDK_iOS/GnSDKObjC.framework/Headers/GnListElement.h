/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnListElement_h_
#define _GnListElement_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnListElement.h"


@class GnListElementEnumerator;


/**
* Element of a Gracenote list.
*/ 

@interface GnListElement : NSObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* Retrieves a display string for a given list element.
* <p><b>Remarks:</b></p>
* Use this function to directly retrieve the display string from a list element.
*/ 

-(NSString*) displayString:(NSError**) error;

/**
* Retrieves a specified list element ID for a given list element.
* <p><b>Remarks:</b></p>
* Use this function to retrieve the ID of a list element.
*/ 

-(NSUInteger) id:(NSError**) error;

/**
* Retrieves a list element ID for use in submitting parcels.
*/ 

-(NSUInteger) idForSubmit:(NSError**) error;

/**
* Retrieves the list element's description.
*/ 

-(NSString*) description:(NSError**) error;

/**
* The list element's Rating Type ID (available in content ratings list).
*/ 

-(NSString*) ratingTypeId:(NSError**) error;

/**
* Retrieves the parent element of the given list element.
* <p><b>Remarks:</b></p>
* When GNSDK Manager throws an error exception with error code SDKMGRERR_NotFound,
* then the given element is the top-most parent element.
*/ 

-(GnListElement*) parent:(NSError**) error;

/**
* Retrieves the hierarchy level for a given list element.
*/ 

-(NSUInteger) level:(NSError**) error;

/**
* Children
*/
-(GnListElementEnumerator*) children;


@end


@interface GnListElementEnumerator : NSEnumerator

typedef void (^GnListElementBlock)(id obj, NSUInteger index, BOOL *stop);

-(void)enumerateObjectsUsingBlock: (GnListElementBlock)handler;

-(NSUInteger) count;

-(GnListElement*) nextObject;

-(GnListElement*) objectAtIndex:(NSUInteger)index;

-(NSArray*) allObjects;


@end



#endif /* _GnListElement_h_ */

