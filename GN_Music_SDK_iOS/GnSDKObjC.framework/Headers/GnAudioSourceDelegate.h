/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnAudioSourceDelegate_h_
#define _GnAudioSourceDelegate_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"





@protocol GnAudioSourceDelegate <NSObject>

@required

/**
* Initialize the audio source. This will be invoked prior to any other methods. If initialization
* fails return a non-zero value. In this case the consumer will not call any other methods
* including that to close the audio source.
* @return 0 indicates initialization was successful, non-zero otherwise.
*/ 

-(NSUInteger) sourceInit;

/**
* Close the audio source. The consumer will not call any other methods after the source has
* been closed
*/ 

-(void) sourceClose;

/**
* Return the number of samples per second of the source audio format. Returns zero if called
* prior to SourceInit.
* @return Samples per second
*/ 

-(NSUInteger) samplesPerSecond;

/**
* Return the number of bits in a sample of the source audio format. Returns zero if called
* prior to SourceInit.
* @return Sample size in bits
*/ 

-(NSUInteger) sampleSizeInBits;

/**
* Return the number of channels of the source audio format. Returns zero if called
* prior to SourceInit.
* @return Number of channels
*/ 

-(NSUInteger) numberOfChannels;

/**
* Get audio data from the audio source. This is a blocking call meaning it should
* not return until there is data available.
* When data is available this method must
* copy data to the provided buffer ensuring not to overflow it. The number of bytes
* copied to the buffer is returned.
* To signal the audio source is unable to deliver anymore data return zero. The
* consumer will then stop requesting data and close the audio source.
* @param dataBuffer 	[out] Buffer to receive audio data
* @param dataSize 		[in]  Size in bytes of buffer
* @return Number of bytes copied to the buffer. Return zero to indicate
* 		   no more data can be delivered via the audio source.
*/ 

-(NSUInteger) getData: (unsigned char*)dataBuffer dataSize: (NSUInteger)dataSize;


@end



#endif /* _GnAudioSourceDelegate_h_ */

