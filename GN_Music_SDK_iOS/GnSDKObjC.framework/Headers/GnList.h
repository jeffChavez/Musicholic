/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnList_h_
#define _GnList_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnCancellableDelegate.h"
#import "GnDataObject.h"
#import "GnEnums.h"
#import "GnListElement.h"
#import "GnLocale.h"
#import "GnStatusEventsDelegate.h"
#import "GnUser.h"


@class GnListElementEnumerator;


/**
* Gracenote list.
*/ 

@interface GnList : NSObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* GnList
*/
-(INSTANCE_RETURN_TYPE) initWithGnListType: (GnListType)listType language: (GnLanguage)language region: (GnRegion)region descriptor: (GnDescriptor)descriptor user: (GnUser*)user statusEventsDelegate: (id <GnStatusEventsDelegate>)pEventHandler;

/**
* GnList
*/
-(INSTANCE_RETURN_TYPE) initWithGnListType: (GnListType)listType locale: (GnLocale*)locale user: (GnUser*)user statusEventsDelegate: (id <GnStatusEventsDelegate>)pEventHandler;

/**
* GnList
*/
-(INSTANCE_RETURN_TYPE) initWithSerializedList: (NSString*)serializedList;

/**
* Tests an existing list for updates and downloads a new list, if available. The application must
* ensure the List module can contact the Gracenote Service to test for a new list version, by
* appropriately configuring the user lookup option.
* Note: This function blocks the current thread until the download is complete; set a status callback function to receive progress messages.
* @param user				User making the list update request
* @return True if an update is available, false otherwise.
* <p><b>Remarks:</b></p>
* Use this function to periodically update a list. The list will be updated if an update is available.
* Optionally an application can check if an update is available before calling this method.
*
* Long Running Potential: Network I/O, File system I/O (for online query cache or local lookup)
*/ 

-(BOOL) update: (GnUser*)user statusEventsDelegate: (id <GnStatusEventsDelegate>)pEventHandler error: (NSError**)error;

/**
* Tests an existing list to determine if a newer revision is available. If available, the new revision is not downloaded. To download
* the new revision the application must call GnList.Update().
* The application must ensure the List module can contact the Gracenote Service to test for a new list version, by
* appropriately configuring the user lookup option.
* Note: This function blocks the current thread until the check is complete; set a status callback function to receive progress messages.
* @param user				User making the list update check request
* @return True if an update is available, false otherwise.
* <p><b>Remarks:</b></p>
* Use this function to periodically check Gracenote Service for updates to an existing list handle.
* <p><b>Note:</b></p>
* You should configure application(s) to automatically check for list updates to ensure use of the
* most current data.
* You should immediately check for updates after constructing a list object from a saved serialized list string as it may
* be out of date.
* This function blocks the current thread until the download is complete;
* set a status callback function to receive progress messages.
*
* Long Running Potential: Network I/O
*/ 

-(BOOL) updateCheck: (GnUser*)user statusEventsDelegate: (id <GnStatusEventsDelegate>)pEventHandler error: (NSError**)error;

/**
* Serializes a list into encrypted text, so the application can store it for later use.
* <p><b>Note:</b></p>
* If you application is using a GNSDK local storage solution, lists are automatically stored
* and retrieved from a local store according to the configuration of user option.
* Applications implementing their own local storage functionality can use this method
* to render a list into a format that can be stored persistently and restored at a later time using
* the appropriate constructor.
*/ 

-(NSString*) serialize:(NSError**) error;

/**
*  Renders list data to XML.
*  @param levels			List level values to render
*  @param renderFlags		Flags configuring rendering output
*/ 

-(NSString*) renderToXml: (NSUInteger)levels renderFlags: (GnListRenderFlags)renderFlags error: (NSError**)error;

/**
* Retrieves this list's type.
*/ 

-(GnListType) type:(NSError**) error;

/**
* Retrieves this list's descriptor.
*/ 

-(GnDescriptor) descriptor:(NSError**) error;

/**
* Retrieves this list's language.
*/ 

-(GnLanguage) language:(NSError**) error;

/**
* Retrieves this list's region.
*/ 

-(GnRegion) region:(NSError**) error;

/**
* Retrieves this list's revision string.
*/ 

-(NSString*) revision:(NSError**) error;

/**
* Retrieves a maximum number of levels in a hierarchy for a given list.
* <p><b>Remarks:</b></p>
* When this function succeeds, the returned parameter contains the number of levels in the given
* list's hierarchy. This level count value is needed when determining which level to access when
* retrieving elements or data from a list.
* Lists can be flat or hierarchical. A flat list has only one level. A hierarchical list has a
* parent-child relationship, where the parent's value is broad enough to encompass its child values
* (for example, a level 1 Rock genre is a parent to level 2 Country Rock and Punk Rock genres). You
* can configure an application to use a single level or the entire hierarchy.
* Level 1 indicates the top level of the list, which usually contains the more general data. The
* highest level value for a list contains the most fine-grained data.
*/ 

-(NSUInteger) levelCount:(NSError**) error;

/**
* ListElements
*/
-(GnListElementEnumerator*) listElements: (NSUInteger)level;

/**
* Retrieves a list element from a list using a specific list element ID.
* If no list element with the ID is found a null list element object is returned.
* @param itemId		List element item ID
*/ 

-(GnListElement*) elementById: (NSUInteger)itemId error: (NSError**)error;

/**
* Retrieves list element whose range includes the specified value
* If no list element matching the range is found a null list element object is returned.
* @param range		Value for range comparison
*/ 

-(GnListElement*) elementByRange: (NSUInteger)range error: (NSError**)error;

/**
* Retrieves list element whose string matches the specified value.	
* If no list element with the string is found a null list element object is returned.
* @param strEquality		Value of string to look up
*/ 

-(GnListElement*) elementByString: (NSString*)strEquality error: (NSError**)error;

/**
* Retrieves list element corresponding to the data object.
* If no list element matching the data object is found a null list element object is returned.
* @param dataObject		Gracenote data object
* @param ordinal			Ordinal
* @param level				List level value
*/ 

-(GnListElement*) elementByGnDataObject: (GnDataObject*)dataObject ordinal: (NSUInteger)ordinal level: (NSUInteger)level error: (NSError**)error;


@end



#endif /* _GnList_h_ */

