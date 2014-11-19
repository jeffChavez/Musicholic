//
//  GnMic.h
//
//  iOSMobileSDK
//  Copyright Gracenote Inc. 2013. All rights reserved.
//
//
//

/*
 *  Gracenote recording helper wrapper class, provides audio recording capability in your application.
 *  Using a GnMic you can
 *    - Record until user stops the recording.
 *    - Pause and resume a recording.
 */

#import <Foundation/Foundation.h>
#import "GnAudioSourceDelegate.h"

@class GnAudioSourceInterface;

@interface GnMic:NSObject <GnAudioSourceDelegate>

/**
 * Acquires audio input hardware resource and initializes it as specified by audioConfig.
 * @param sampleRate       Preferred Sample rate to be used for recording audio.
 * @param bitsPerChannel   The number of bits per channel to use for recording audio.
 * @param numberOfChannels The number of channels used for recording audio. For example: 2 for stereo; 1 for mono.
 * @param delegate         An instance of a class conforming to GnMicDelegate protocol.
 * @return GnMic*          A valid GnMic instance if audio configuration was valid and allocation was successful; nil otherwise.
 */

- (instancetype) initWithSampleRate:(Float64) sampleRate bitsPerChannel:(UInt32) bitsPerChannel numberOfChannels:(UInt32)  numberOfChannels;

@end



