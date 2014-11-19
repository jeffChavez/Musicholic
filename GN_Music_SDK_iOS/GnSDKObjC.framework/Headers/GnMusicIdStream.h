/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnMusicIdStream_h_
#define _GnMusicIdStream_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnAudioSourceDelegate.h"
#import "GnCancellableDelegate.h"
#import "GnLocale.h"
#import "GnEnums.h"
#import "GnMusicIdStreamEventsDelegate.h"
#import "GnMusicIdStreamOptions.h"
#import "GnResponseAlbums.h"
#import "GnUser.h"
#import "GnStatusEventsDelegate.h"




/**
* Identifies raw audio received in a continuous stream.
*
* GnMusicIdStream provides services for identifying music within a continuous audio stream.
* As data is received from the audio stream it is provided to GnMusicIdStream, when the application
* wishes to identify the audio it initializes an identification. The results of the identification
* are delivered asynchronously to a delegate object.
*
* GnMusicIdStream is a long-life object and, for a single audio stream, a single instance should
* be kept for as long as the application wishes to identify that audio stream. Where multiple audio
* streams require identification multiple instances of GnMusicIdStream are also required.
*
* GnMusicIdStream can be started and stopped as the audio stream starts and stops. There is no need
* to destroy and recreate a GnMusicIdStream instance due to breaks in the audio stream.
*
* <b>Audio Processing</b>
*
* After instantiating a GnMusicIdStream object the next step is to start audio processing and provide
* raw audio. Raw audio can be provided automatically or manually.
*
* To provide audio automatically your audio stream must be represented by an object that implements
* GnAudioSourceDelegate interface. Gracenote provides GnMic class on some platforms which is a representation
* of the device microphone, though you are encouraged to provide custom implementations representing
* any audio stream source your application needs.
*
* Internally GnMusicIdStream pulls data from the audio source interface in a loop, so some applications
* may wish to start automatic audio processing in a background thread to avoid stalling the main thread.
*
* Raw audio can also be manually provided to GnMusicIdStream. Audio processing must be started and the
* audio format provided, this allows GnMusicIdStream to establish internal buffers and audio
* processing modules. Once audio processing is started audio can then be written for processing as
* it is received from the stream.
*
* At any point audio processing can be stopped. When stopped automatic data fetching ceases or
* if audio data is being provided manually attempts to write data for processing
* manually will fail. Internally GnMusicIdStream clears and releases all buffers and audio
* processing modules. Audio processing can be restarted after it is stopped.
*
* If an error occurs during manual audio processing an exception is thrown during the audio write API
* call. If an error occurs during automatic audio processing the internal loop is exited and an exception
* thrown from the audio processing start API.
*
* <b>Identification</b>
*
* At any point the application can trigger an identification of the audio stream. The identification
* process identifies buffered audio. Only up to ~7 seconds of the most recent audio is buffered.
* If there isn't enough audio buffered for identification it will wait until enough audio is received.
*
* The identification process spawns a thread and completes asynchronously. However two identify methods
* are provided, one for synchronous and one for asynchronous. Where synchronous identification is invoked the
* identification is still performed asynchronously and results delivered via delegate implementing
* GnMusicIdStreamEventsDelegate, but the identify method does not return until the identification is complete.
* Requests to identify audio while a previous pending identification request is pending will be ignored.
*
* Audio can be identified against Gracenote's online database or a local database. The default
* behavior is to attempt a match against the local database and if a match isn't found try the
* online database. Local matches are only possible if GnLookupLocalStream object has been
* instantiated and a MusicID-Stream fingerprint bundle ingested. See GnLookupLocalStream for
* more information on bundle ingestion.
*
* An identification process can be canceled. In this case the identification
* process stops and, if synchronous identification was invoked, the identify method returns.
* The identify error delegate method will be called with an "aborted" error when cancelled.
* Note cancelling does not cease audio processing and application can invoke identify again after
* cancelling.
*
* If an error occurs during identification it is reported in one of two ways.
* If the error occurs in the asynchronous identifying thread the error information is provided
* via identifying error delegate method and the result available delegate method will not be called.
* Where this delegate method is invoked no result available delegate method is invoked.
* If the error occurs prior to the asynchronous identifying thread being launched an exception
* is thrown from the identifying API.
*
* <b>Configuration</b>
*
* GnMusicIdStream is configurable via it's options object. The configuration determines, among
* other things, if identification is performed against local database, online database or both;
* the content returned with results (images, external IDs, etc.)
* See GnMusicIdStreamOptions for more information.
*
*
* Note: Customers must be licensed to implement use of a MusicID product in an application.
* Contact your Gracenote support representative with questions about product licensing and
* entitlement.
*/ 

@interface GnMusicIdStream : NSObject

/**
*  Establishes an audio stream identification object with locale. The locale is used determine
*  the preferred language and script of stream identification results.
*  Note: Results are only returned in preferred language and script where available.
*  @param user 			[in] Gracenote user
*  @param locale 			[in] Gracenote locale
*  @param pEventDelegate 	[in] Audio processing and identification query events handler
*/ 

-(INSTANCE_RETURN_TYPE) initWithGnUser: (GnUser*)user preset: (GnMusicIdStreamPreset)preset locale: (GnLocale*)locale musicIdStreamEventsDelegate: (id <GnMusicIdStreamEventsDelegate>)pEventDelegate;

/**
*  Establishes an audio stream identification object with locale. The locale is used determine
*  the preferred language and script of stream identification results.
*  Note: Results are only returned in preferred language and script where available.
*  @param user 			[in] Gracenote user
*  @param preset type		[in] Gracenote musicID stream preset
*  @param pEventDelegate 	[in] Audio processing and identification query events handler
*/ 

-(INSTANCE_RETURN_TYPE) initWithGnUser: (GnUser*)user preset: (GnMusicIdStreamPreset)preset musicIdStreamEventsDelegate: (id <GnMusicIdStreamEventsDelegate>)pEventDelegate;

/**
*  Retrieves the MusicID-Stream SDK's version string.
*  @return Version string if successful
*  <p><b>Remarks:</b></p>
*  This API can be called at any time, after successfully establishing a MusicID-Stream audio channel.
*  The returned string is a constant. Do not attempt to modify or delete.
*  Example version string: 1.2.3.123 (Major.Minor.Improvement.Build)
*  Major: New functionality
*  Minor: New or changed features
*  Improvement: Improvements and fixes
*  Build: Internal build number
*/ 

+(NSString*) version;

/**
*  Retrieves the MusicID-Stream SDK's build date string.
*  @return Build date string of the format: YYYY-MM-DD hh:mm UTC
*  <p><b>Remarks:</b></p>
*  This API can be called at any time, after successfully establishing a MusicID-Stream audio channel.
*  The returned string is a constant. Do not attempt to modify or delete.
*  Example build date string: 2008-02-12 00:41 UTC
*/ 

+(NSString*) buildDate;

/**
* Get GnMusicIdStream options object. Use to configure your GnMusicIdStream instance.
* @return Options objects
*/ 

-(GnMusicIdStreamOptions*) options;

/**
* Commence retrieving and processing audio from an object implementing the audio source interface.
* This is an alternate and often simpler method for providing audio to GnMusicIdStream
* instead of calling AudioProcessStart (audio format overload) and AudioProcess.
* To use this method the audio source to be identified must be accessible via an GnAudioSourceDelegate
* implementation. Custom implementations of GnAudioSourceDelegate are encouraged.
* Note: audio is retrieved from the audio source in a loop so some applications may wish to start
* automatic audio processing in a background thread to avoid stalling the main thread.
* @param audioSource	[in] Audio source to be identified
*/ 

-(void) audioProcessStartWithAudioSource: (id <GnAudioSourceDelegate>)audioSource error: (NSError**)error;

/**
* Initialize manual delivery of audio stream audio to GnMusicIdStream. GnMusicIdStream
* establishes buffers and audio processing modules, readying itself to receive audio.
* @param samplesPerSecond	[in] Number of samples per second
* @param bitsPerSample		[in] Number of bits per sample
* @param numberOfChannels	[in] Number of channels
*/ 

-(void) audioProcessStartWithSamplesPerSecond: (NSUInteger)samplesPerSecond bitsPerSample: (NSUInteger)bitsPerSample numberOfChannels: (NSUInteger)numberOfChannels error: (NSError**)error;

/**
* <b>Experimental</b> Stops audio processing. If audio processing was started with an object implementing GnAudioSourceDelegate
* the audio source is closed and data is no longer retrieved from that source. If manual audio processing
* was used future attempts to write audio data for processing will fail.
*/ 

-(void) audioProcessStop:(NSError**) error;

/**
* Provides audio manually for processing by GnMusicIdStream. This should not be called if
* audio processing was started with an object implementing the audio source interface.
* @param audioData			[in] Buffer containing sample audio
* @param audioDataLength	[in] Number of bytes of audio in pAudioBuffer
*/ 

-(void) audioProcess: (unsigned char*)audioData audioDataLength: (NSUInteger)audioDataLength error: (NSError**)error;

/**
* @deprecated Will be removed next release, use IdentifyAlbumAsync and WaitForIdentify instead.
* Identifying the audio in the audio stream and blocks until identification is
* complete. Results are delivered asynchronously via GnMusicIdStreamEventsDelegate delegate.
*/ 

-(void) identifyAlbum:(NSError**) error;

/**
* Identifying the audio in the audio stream. Results are delivered
* asynchronously via GnMusicIdStreamEventsDelegate delegate.
*/ 

-(void) identifyAlbumAsync:(NSError**) error;

/**
* Wait for currently running identify call to complete (up to timeout_ms milliseconds).
* Returns true if identification completed in the timeout period, false if not
* @param timeout_ms	[in] Timeout in milliseconds
* @return true			True if completed, false if timed out
*/ 

-(BOOL) waitForIdentify: (NSUInteger)timeout_ms error: (NSError**)error;

/**
* Cancel the current identify operation blocking until the identification has stopped. 
* Cannot be called from within a GnMusicIdStream delegate callback, use the canceller provided 
* in the callback instead. This will throw an exception if it is used with the automatic mode 
* on.
*/ 

-(void) identifyCancel:(NSError**) error;

/**
* Event
*/
-(void) event: (GnMusicIdStreamEvent)event error: (NSError**)error;

/**
* BroadcastMetadata
*/
-(void) broadcastMetadata: (NSString*)broadcastMetadataKey broadcastMetadataValue: (NSString*)broadcastMetadataValue error: (NSError**)error;


@end



#endif /* _GnMusicIdStream_h_ */

