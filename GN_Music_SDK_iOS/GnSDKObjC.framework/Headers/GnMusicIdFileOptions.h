/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnMusicIdFileOptions_h_
#define _GnMusicIdFileOptions_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnEnums.h"




/**
* Configures options for GnMusicIdFile
*/ 

@interface GnMusicIdFileOptions : NSObject

/**
*  Indicates whether the MusicID-File query should be performed against local embedded databases or online.
*  @param lookupMode  [in] One of the GnLookupMode values
*/ 

-(void) lookupMode: (GnLookupMode)lookupMode error: (NSError**)error;

/**
*  Sets the lookup data value for the MusicID-File query.
*  @param val 		[in] Set One of the GnLookupData values
*  @param enable 	[in] True or false to enable or disable
*/ 

-(void) lookupData: (GnLookupData)val enable: (BOOL)enable error: (NSError**)error;

/**
*  Sets the batch size for the MusicID-File query.
*  @param size [in] set String value or one of MusicID-File Option Values that corresponds to BATCH_SIZE
*  <p><b>Remarks:</b></p>
*  The option value provided for batch size must be greater than zero (0).
*/ 

-(void) batchSize: (NSUInteger)size error: (NSError**)error;

/**
*  Indicates whether MusicID-File should Process the responses Online, this may reduce the amount of 
*  resources used by the client. Online processing must be allowed by your license.
*  @param enable [in] True or false to enable or disable
*/ 

-(void) onlineProcessing: (BOOL)enable error: (NSError**)error;

/**
*  Sets the preferred language for the MusicID-File query.
*  @param preferredLangauge [in] One of the GNSDK language values
*/ 

-(void) preferResultLanguage: (GnLanguage)preferredLangauge error: (NSError**)error;

/**
* Use this option to specify an external identifier which MusicID-File should try to include in any responses that are returned.
*  <p><b>Remarks:</b></p>
* This option is currently only supported when online processing is enabled.
*  @param preferredExternalId	[in] The name of an external identifier that should be preferred when selecting matches
*/ 

-(void) preferResultExternalId: (NSString*)preferredExternalId error: (NSError**)error;

/**
*  Sets the thread priority for a given MusicID-File query.
*  @param threadPriority 	[in] Set one of GnThreadPriority values that corresponds to thread priority
*  <p><b>Remarks:</b></p>
*  The option value provided for thread priority must be one of the defined
*  GnThreadPriority values.
*/ 

-(void) threadPriority: (GnThreadPriority)threadPriority error: (NSError**)error;

/**
*  General option setting for custom options
*  @param optionKey [in] set One of the MusicID-File Option Keys
*  @param enable [in opt] set True or false to enable or disable
*/ 

-(void) customWithOptionKey: (NSString*)optionKey enable: (BOOL)enable error: (NSError**)error;

/**
*  Set option using option name
*  @param option   [in] Option name
*  @param value	[in] Option value
*/ 

-(void) customWithOption: (NSString*)option value: (NSString*)value error: (NSError**)error;


@end



#endif /* _GnMusicIdFileOptions_h_ */

