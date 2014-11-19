//
//  GnAudioFileDecoder.h
//  gnsdk_objc
//  Created on 10/2/13
//  Copyright Gracenote Inc. 2011. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GnAudioSourceDelegate.h"



@interface GnAudioFile:NSObject <GnAudioSourceDelegate>


@property(nonatomic, strong) NSURL *audioFileUrl;
@property(nonatomic, strong) NSURL *tempFileUrl;

/**
 * Initializes and returns a GnAudioFile instance.
 * @param audioFileUrl NSURL instance containing URL to an audio file.
 * @return void        A valid GnAudioFile instance if allocation was successful; nil otherwise.
 */
-(instancetype) initWithAudioFileURL:(NSURL*) inAudioFileUrl;


/**
 * Starts decoding the audio file residing at audioFileURL passed in while initilizing GnAudioFile class.
 * @param error    NSError object that will be initialized with error code and description if an exception is caught; nil otherwise.
 * @return void    Returns nothing (void).
 */

-(void) startDecoding:(NSError**) error;


@end
