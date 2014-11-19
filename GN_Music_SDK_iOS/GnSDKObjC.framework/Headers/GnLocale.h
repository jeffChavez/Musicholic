/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnLocale_h_
#define _GnLocale_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnCancellableDelegate.h"
#import "GnEnums.h"
#import "GnLocaleInfo.h"
#import "GnStatusEventsDelegate.h"
#import "GnUser.h"


@class GnLocaleInfoEnumerator;


/**
* GNSDK internal iterator class
*//**
* Loads Gracenote data for the specified locale
*
* Locales are used by GNSDK for various reasons and it's the best practice to have an appropriate
* locale loaded. Typically an application uses user preferences or device settings to determine the
* region and language of the loaded locale. To determine what locale group and descriptor to load
* work with your Gracenote support representative.
*
* Locale data is only loaded if it not already loaded. It can be loaded from Gracenote Service or from a
* local database. To use a local database a storage provider must be enabled, such as GnStorageSqlite.
* Where a local database is used GNSDK will attempt to load locale data from it, if not found locale data
* will be downloaded from Gracenote Service and written to the local database.
*
* Locale load can be canceled at any time by setting the "canceller" provided in any GnStatusEventsDelegate
* delegate method. No cancel method is provided on GnLocale object because loading can happen on object
* construction.
*
* Loading a locale can be lengthy, so some applications may wish to perform this operation on a background
* thread to avoid stalling the main thread.
*
* Once a locale is loaded it can be set as the group default. The default locale is automatically
* associated with appropriate response objects returned by GNSDK, allowing Gracenote descriptive data,
* such as genres, to be returned according to the locales region and language.
*
* Gracenote data is regularly updated; therefore Locale data should also be updated. GNSDK can detect
* when an application should update a locale and call an GnSystemEventsDelegate delegate method providing the GnLocale
* object. Your application can register for system events via a GnManager instance. Alternatively your
* application can keep the GnLocale object and periodically invoke update. It is Gracenote best practice
* that your application implement a locale update procedure.
*/ 

@interface GnLocale : NSObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* GnLocale
*/
-(INSTANCE_RETURN_TYPE) initWithGnLocaleInfo: (GnLocaleInfo*)localeInfo user: (GnUser*)user statusEventsDelegate: (id <GnStatusEventsDelegate>)pEventHandler;

/**
* Loads the specified locale
* @param group				Locale group specifies which locale data is loaded
* @param language			Language of locale data
* @param region			Region of the locale data where applicable
* @param descriptor		Descriptor, or verbosity, of the locale data where applicable
* @param user				User object
* @param pEventHandler     Status events delegate
* <p><b>Note</b><p> This method blocks the current thread until the load is complete;
* set an GnStatusEventsDelegate delegate to receive progress messages.
*
* Long Running Potential: Network I/O, File system I/O
*/ 

-(INSTANCE_RETURN_TYPE) initWithGnLocaleGroup: (GnLocaleGroup)group language: (GnLanguage)language region: (GnRegion)region descriptor: (GnDescriptor)descriptor user: (GnUser*)user statusEventsDelegate: (id <GnStatusEventsDelegate>)pEventHandler;

/**
* Loads the specified locale
* @param group				Locale group specifies which locale data is loaded
* @param langIsoCode		Language of locale data as an ISO code
* @param region			Region of the locale data where applicable
* @param descriptor		Descriptor, or verbosity, of the locale data where applicable
* @param user				User object
* @param pEventHandler     Status events delegate
* <p><b>Note</b><p> This method blocks the current thread until the load is complete;
* set an GnStatusEventsDelegate delegate to receive progress messages.
*
* Long Running Potential: Network I/O, File system I/O
*/ 

-(INSTANCE_RETURN_TYPE) initWithGnLocaleGroup: (GnLocaleGroup)group langIsoCode: (NSString*)langIsoCode region: (GnRegion)region descriptor: (GnDescriptor)descriptor user: (GnUser*)user statusEventsDelegate: (id <GnStatusEventsDelegate>)pEventHandler;

/**
* Reconstitutes locale from serialized locale data.
* @param serializedLocale	String of serialized locale handle data
*/ 

-(INSTANCE_RETURN_TYPE) initWithSerializedLocale: (NSString*)serializedLocale;

/**
* LocaleInformation
*/
-(GnLocaleInfo*) localeInformation;

/**
* Retrieves this locale's revision string.
* @return Revision
*/ 

-(NSString*) revision:(NSError**) error;

/**
* Sets this locale as the default locale for the 'locale group' (see GnLocaleGroup).
*/ 

-(void) setGroupDefault:(NSError**) error;

/**
* Retrieve iterator for locales available from locale lists cache
* @return Locale info iterator
*/ 

+(GnLocaleInfoEnumerator*) storedLocales;

/**
* Updates a locale with new versions of the locale data, if available.
* The application must ensure Gracenote Service can be contacted to test for a new list version
* by appropriately configuring the user's lookup mode to allow online access.
*
* The application can cancel the update procedure by setting the "canceller" in any method
* called in the status event delegate.
*
* @param user				User requesting the locale update
* @param pEventHandler		Status events delegate
* @return True indicates updates were applied, false indicates no updates are available
*
* <p><b>Note</b></p>
* This methods blocks the current thread until the update procedure is complete;
* set a status events delegate to receive progress messages.
*
* Long Running Potential: Network I/O, File system I/O
*/ 

-(BOOL) update: (GnUser*)user statusEventsDelegate: (id <GnStatusEventsDelegate>)pEventHandler error: (NSError**)error;

/**
* Tests a locale to determine if a newer revision of any locale data is available. If available the new data
* is not downloaded. To download the application must invoke update.
* The application must ensure Gracenote Service can be contacted to test for a new list version
* by appropriately configuring the user's lookup mode to allow online access.
*
* The application can cancel the update check procedure by setting the "canceller" in any method
* called in the status event delegate.
*
* @param user				User requesting the locale update check
* @param pEventHandler		Status event delegate
* @return True indicates updates are available, false otherwise.
*
* <p><b>Remarks:</b></p>
* This method can be invoked periodically to check Gracenote Service for updates to locale data.
*
* <p><b>Note:</b></p>
* You should immediately check for updates after constructing a locale object from a saved serialized locale
* string as it may be out of date.
*
* This methods blocks the current thread until the update procedure is complete;
* set a status events delegate to receive progress messages.
*
* Long Running Potential: Network I/O
*/ 

-(BOOL) updateCheck: (GnUser*)user statusEventsDelegate: (id <GnStatusEventsDelegate>)pEventHandler error: (NSError**)error;

/**
* Serializes locale data into encrypted text string that the application can store locally for later use.
* <p><b>Note:</b></p>
* If the application is using a GNSDK local storage solution locale data is automatically stored
* and retrieved from a local store according to the configuration of user lookup option.
* Applications implementing their own local storage functionality can use this method to
* render a locale into a format that can be stored persistently and restored at a later time using
* the appropriate constructor.
* @return Serialize locale data
*/ 

-(NSString*) serialize:(NSError**) error;


@end



#endif /* _GnLocale_h_ */

