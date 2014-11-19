/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnMusicIdFileInfo_h_
#define _GnMusicIdFileInfo_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnAudioSourceDelegate.h"
#import "GnEnums.h"
#import "GnResponseAlbums.h"
#import "GnResponseDataMatches.h"




/**
*  Container class for providing media file information to MusicID-File.
*/ 

@interface GnMusicIdFileInfo : NSObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
*  Retrieves the identifier string from a FileInfo object.
*  @return gnsdk_cstr_t Pointer to receive the data value defined for the identifier
*/ 

-(NSString*) identifier:(NSError**) error;

/**
*  Get the file name
*  @return File name
*/ 

-(NSString*) fileName:(NSError**) error;

/**
*  Set the file name
*/ 

-(void) fileNameWithValue: (NSString*)value error: (NSError**)error;

/**
*  Gets the Gracenote CDDB ID value from a FileInfo object.
*  @return Identifier
*/ 

-(NSString*) cddbId:(NSError**) error;

/**
*  Sets the Gracenote CDDB ID value from a FileInfo object.
*/ 

-(void) cddbIdWithValue: (NSString*)value error: (NSError**)error;

/**
*  Gets the album artist
*  @return Artist name
*/ 

-(NSString*) albumArtist:(NSError**) error;

/**
*  Sets the album artist
*/ 

-(void) albumArtistWithValue: (NSString*)value error: (NSError**)error;

/**
*  Gets the album title
*  @return Album title
*/ 

-(NSString*) albumTitle:(NSError**) error;

/**
*  Sets the album title
*/ 

-(void) albumTitleWithValue: (NSString*)value error: (NSError**)error;

/**
*  Gets the track artist
*  @return Track artist
*/ 

-(NSString*) trackArtist:(NSError**) error;

/**
*  Sets the track artist
*/ 

-(void) trackArtistWithValue: (NSString*)value error: (NSError**)error;

/**
*  Gets the track title
*  @return Track title
*/ 

-(NSString*) trackTitle:(NSError**) error;

/**
*  Sets the track title
*/ 

-(void) trackTitleWithValue: (NSString*)value error: (NSError**)error;

/**
*  Gets the track number
*  @return Track number
*/ 

-(NSUInteger) trackNumber:(NSError**) error;

/**
*  Sets the track number
*/ 

-(void) trackNumberWithTrackNumber: (NSUInteger)trackNumber error: (NSError**)error;

/**
*  Gets the disc number
*  @return Disc number
*/ 

-(NSUInteger) discNumber:(NSError**) error;

/**
*  Sets the disc number
*/ 

-(void) discNumberWithDiscNumber: (NSUInteger)discNumber error: (NSError**)error;

/**
*  Gets the tag ID
*  @return Tag identifier
*/ 

-(NSString*) tagId:(NSError**) error;

/**
*  Sets the tag ID
*/ 

-(void) tagIdWithValue: (NSString*)value error: (NSError**)error;

/**
*  Gets the fingerprint data
*  @return Fingerprint data
*/ 

-(NSString*) fingerprint:(NSError**) error;

/**
*  Sets the fingerprint
*/ 

-(void) fingerprintWithValue: (NSString*)value error: (NSError**)error;

/**
*  Gets the media ID
*  @return Media identifier
*/ 

-(NSString*) mediaId:(NSError**) error;

/**
*  Sets the media ID
*/ 

-(void) mediaIdWithValue: (NSString*)value error: (NSError**)error;

/**
*  Gets the Media Unique ID (MUI)
*  @return Media unique identifier
*/ 

-(NSString*) mui:(NSError**) error;

/**
*  Sets the Media Unique ID (MUI)
*/ 

-(void) muiWithValue: (NSString*)value error: (NSError**)error;

/**
*  Gets the CDTOC value
*  @return CDTOC value
*/ 

-(NSString*) cdToc:(NSError**) error;

/**
*  Sets the CDTOC value
*/ 

-(void) cdTocWithValue: (NSString*)value error: (NSError**)error;

/**
*  Gets the Title Unique Identifier (Tui)
*  @return Title unique identifier
*/ 

-(NSString*) tui:(NSError**) error;

/**
*  Sets the Title Unique Identifier (Tui)
*/ 

-(void) tuiWithValue: (NSString*)value error: (NSError**)error;

/**
*  Gets the Tui tag
*  @return Tui tag
*/ 

-(NSString*) tuiTag:(NSError**) error;

/**
*  Sets the Tui tag
*/ 

-(void) tuiTagWithValue: (NSString*)value error: (NSError**)error;

/**
*  Initializes fingerprint generation.
*  @param audioSampleRate [in] set Sample frequency of audio to be provided: 11 kHz, 22 kHz, or 44 kHz
*  @param audioSampleSize [in] set Sample rate of audio to be provided (in 8-bit, 16-bit, or
*  32-bit bytes per sample)
*  @param audioChannels [in] set Number of channels for audio to be provided (1 or 2)
*  <p><b>Remarks:</b></p>
*  The MusicID-File fingerprinting APIs allow applications to provide audio data as a method of
*  identification. This enables MusicID-File to perform identification based on the audio itself, as
*  opposed to performing identification using only the associated metadata.
*  Use the MusicID-File fingerprinting APIs either before processing has begun, or during a
*  GnMusicIdFileEventsDelegate or GnMusicIdFileInfoEventsDelegate delegate method call.
*/ 

-(void) fingerprintBegin: (NSUInteger)audioSampleRate audioSampleSize: (NSUInteger)audioSampleSize audioChannels: (NSUInteger)audioChannels error: (NSError**)error;

/**
*  Provides uncompressed audio data for fingerprint generation.
*  @param audioData [in] pointer to audio data buffer that matches the audio format described in FingerprintBegin().
*  @param audioDataSize [in] size of audio data buffer (in bytes)
*  @return bool Checks whether the fingerprint generation has received enough audio data
*  <p><b>Remarks:</b></p>
*  The provided audio data must be uncompressed PCM data and must match the format given to fingerprintBegin().
*  Returns true value when the fingerprinting process has received
*  enough audio data to perform its processing. Any further provided audio data is ignored.
*  The application must provide audio data until true is returned
*  to successfully generate an audio fingerprint.
*  The application must complete the fingerprinting process by calling
*  FingerprintEnd() when either the audio data terminates, or after true is returned.
*  Use the MusicID-File fingerprinting APIs either before processing has begun, or during a
*  GnMusicIdFileEventsDelegate or GnMusicIdFileInfoEventsDelegate delegate method call.
*/ 

-(BOOL) fingerprintWrite: (unsigned char*)audioData audioDataSize: (NSUInteger)audioDataSize error: (NSError**)error;

/**
*  Finalizes fingerprint generation.
*  <p><b>Remarks:</b></p>
*  The application must complete the fingerprinting process by calling
*  fingerprint_end when either the audio data terminates, or after receiving
*  a GNSDK_TRUE value.
*  Use the MusicID-File fingerprinting APIs either before processing has begun, or during a
*  GnMusicIdFileEventsDelegate or GnMusicIdFileInfoEventsDelegate delegate method call.
*/ 

-(void) fingerprintEnd:(NSError**) error;

/**
* Generate a fingerprint from audio pulled from the provided audio source
* @param audioSource [in] audio source representing the file being identified
*/ 

-(void) fingerprintFromSource: (id <GnAudioSourceDelegate>)audioSource error: (NSError**)error;

/**
*  Retrieves the current status for a specific FileInfo object.
*  <p><b>Remarks:</b></p>
*  The File Info object's state value indicates what kind of response is available for a FileInfo object after
*  MusicID-File. In the case of an error, error info can be retrieved from the FileInfo object.
*/ 

-(GnMusicIdFileInfoStatus) status:(NSError**) error;

/**
*  Retrieves the error information for a FileInfo object. This is related to the status returned.
*  If the status is error, this call returns the extended error information.
*  <p><b>Remarks:</b></p>
*  An error object is returned representing the FileInfo error condition. An error object exception
*  may be thrown if an error occurred retrieving the FileInfo object's error object.
*/ 

-(NSError*) errorInformation:(NSError**) error;

/**
*  Retrieves the album response if available
*  @return Album response
*  <p><b>Remarks:</b></p>
*  This function retrieves the album response object of the match for this file information object if available.
*  Use Status() to determine if a response is available for this file information object.
*/ 

-(GnResponseAlbums*) albumResponse:(NSError**) error;

/**
*  Retrieves the data match response if available
*  @return Data match response
*  <p><b>Remarks:</b></p>
*  This function retrieves the data match response object of the match for this file information object if available.
*  Data match responses mean the match could be an album or contributor or a mix thereof depending on the query options.
*  Use Status() to determine if a response is available for this file information object.
*/ 

-(GnResponseDataMatches*) dataMatchResponse:(NSError**) error;


@end


@interface GnMusicIdFileInfoEnumerator : NSEnumerator

typedef void (^GnMusicIdFileInfoBlock)(id obj, NSUInteger index, BOOL *stop);

-(void)enumerateObjectsUsingBlock: (GnMusicIdFileInfoBlock)handler;

-(NSUInteger) count;

-(GnMusicIdFileInfo*) nextObject;

-(GnMusicIdFileInfo*) objectAtIndex:(NSUInteger)index;

-(NSArray*) allObjects;


@end



#endif /* _GnMusicIdFileInfo_h_ */

