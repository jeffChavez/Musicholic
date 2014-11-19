/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GnPlaylistCollection_h_
#define _GnPlaylistCollection_h_


#import <Foundation/Foundation.h>
#import "GnDefines.h"

#import "GnAlbum.h"
#import "GnCancellableDelegate.h"
#import "GnContributor.h"
#import "GnDataObject.h"
#import "GnListElement.h"
#import "GnPlaylistAttributes.h"
#import "GnPlaylistCollection.h"
#import "GnPlaylistCollectionSyncEventsDelegate.h"
#import "GnEnums.h"
#import "GnPlaylistIdentifier.h"
#import "GnPlaylistMoreLikeThisOptions.h"
#import "GnPlaylistResult.h"
#import "GnString.h"
#import "GnTrack.h"
#import "GnUser.h"


@class GnPlaylistCollectionEnumerator;
@class GnPlaylistIdentifierEnumerator;
@class GnStringEnumerator;


/**
* <b>Experimental</b>: Playlist Collection Summary that represents
* the media in a user's collection and can be used to generate
* playlists from that media. A Collection Summary is not intended to be
* used as a general database of the user's media collection.
*
* <b>Creating a Collection Summary</b>
*
* GNSDK supports multiple user collections and therefore multiple Collection
* Summaries. Each Collection Summary is identified by a unique name. To
* create a new Collection Summary instantiate a GnPlaylistCollection object
* with a unique name.
*
* <b>Adding User Media</b>
*
* To generate a playlist from user media your application must first
* identify the user's media using Gracenote services (such as MusicID-File)
* and then create a Playlist Collection Summary with the recognized media.
*
* In a simple example your application can use GnMusicIdFile Library ID
* functionality to identify audio tracks. The GnMusicIdFile result provides
* to GnAlbum objects containg GnTrack objects that map back to the original
* audio track; your application can add the GnTrack object <b>and</b> the
* GnAlbum object to the Playlist Collection Summary with a unique identifyer
* for the audio track.
*
* Note: When identifying media intended for inclusion in a Playlist Collection
* Summary you must specify that the result inlcude lookup data kLookupDataPlaylist
* and kLookupDataSonicData.
*
* In some cases an audio track may not contain enough information to match it
* with a single GnTrack object, in such caes you application can use the
* available information to add it to the Collection Summary. For example if only
* album title or artist name information is available GnMusicId could be used to
* match a GnAlbum or GnContributor object which can then be added. Similarly
* if only genre information is available the Lists's subsystem could be used to
* match a GnListElement object which can then be added.
*
* <b>Generating Playlists</b>
*
* Using a GnPlaylistCollection object your application can generate "More Like
* This" playlist or where more control is required a playlist specific via
* Playlist Definition Langauge (PDL).
*
* A MTL (More Like This) playlist can be generated from a seed, which can be any
* Gracenote data object. For example you may use GnMusicId to perform a text
* search for a specific track and receive a GnAlbum object. Your application
* can provide the GnAlbum object as the seed. Note: do not use the GnTrack
* object as the seed, Playlist will use the "matched" GnTrack object to determine
* which track on the album to use.
*
* For advanced playlist definition your application can define Playlist Definition
* Language (PDL) Statements. For more information on creating a PDL Statements consult 
* the PDL Specification.
*
* <b>Synchronizing User's Media Collection</b>
*
* A user's media collection will change over time. To ensure generated playlists
* include new media items and don't include removed media items your application must
* synchronize the user's collection with their Collection Summary.
*
* Synchronization is a two step process. Step one requires your application to use
* SyncProcessAdd to add <b>all</b> unique media identifiers that currently exist in the user's
* collection. Sep two is to process those identifers by calling SyncProcessExecute.
*
* During execution GNSDK reconciles the identifiers within the Collection Summary
* with those added for sychronization. It can determine which identifiers are new, meaning
* they are were added to the user's collection and need to be added to the Collection
* Summary; and those that are old, meaning they were removed from the user's collection
* and need to be removed from the Collection Summary.
*
* <b>Joining Collections</b>
*
* Multiple collection summaries can be joined allowing playlists to be generated that contain
* tracks from multiple collections.
*
* <b>Storing Persistently</b>
*
* Playlist Collection Summaries are stored entirely in heap memory. To avoid
* re-creating them every time your application starts you should store them
* persistently.
*
* Gracenote recommends using managed persistent storage services provided by
* GnPlaylistStorage.
*
* Alternatively a Collection Summary can be serialized into a string that your
* application can store persistently and later use to reconstitute a Collection
* Summary in memory.
*
*/ 

@interface GnPlaylistCollection : NSObject

-(INSTANCE_RETURN_TYPE) init __attribute__((unavailable("init not available, use initWith instead")));

/**
* Constructor for GnPlaylistCollection using char* string name. This creates the collection with the name that is passed in.
* @param name   The name to be used to construct the colleciton.
*/ 

-(INSTANCE_RETURN_TYPE) initWithName: (NSString*)name;

/**
* Constructor for GnPlaylistCollection using a buffer to deserialize it from  and the corresponding buffer size .
* This created the collection from the buffer that is passed in if it is successfully deserialized.
* @param collData  [in] buffer that contains the serialized buffer.
* @param dataSize	[in] buffer size .
*/ 

-(INSTANCE_RETURN_TYPE) initWithCollData: (unsigned char*)collData dataSize: (NSUInteger)dataSize;

/**
* Copy Constructor for GnPlaylistCollection.
*/ 

-(INSTANCE_RETURN_TYPE) initWithGnPlaylistCollection: (GnPlaylistCollection*)other;

/**
* Get the collection name
* @return Name
*/ 

-(NSString*) name:(NSError**) error;

/**
* Change the collection name
* @param updatedName	New collection name
*/ 

-(void) nameWithUpdatedName: (NSString*)updatedName error: (NSError**)error;

/**
* Add a identifier with no metadata to a Collection Summary
*
* @param mediaIdentifier   [in] Media identifier
*/ 

-(void) addWithMediaIdentifier: (NSString*)mediaIdentifier error: (NSError**)error;

/**
* Add a GnAlbum object and its metadata to a Collection Summary.
*
* @param mediaIdentifier   [in] Media identifier
* @param album             [in] GnAlbum object
*/ 

-(void) addWithMediaIdentifier: (NSString*)mediaIdentifier album: (GnAlbum*)album error: (NSError**)error;

/**
* Add a GnTrack object and its metadata to a Collection Summary
*
* @param mediaIdentifier   [in] Media identifier
* @param track             [in] GnTrack object
*/ 

-(void) addWithMediaIdentifier: (NSString*)mediaIdentifier track: (GnTrack*)track error: (NSError**)error;

/**
* Add a GnContributor object and its metadata to a Collection Summary
*
* @param mediaIdentifier   [in] Media identifier
* @param contributor       [in] GnContributor object
*/ 

-(void) addWithMediaIdentifier: (NSString*)mediaIdentifier contributor: (GnContributor*)contributor error: (NSError**)error;

/**
* Add a GnPlaylistAttributes object to a Collection Summary.
*
* @param mediaIdentifier       [in] Media identifier
* @param playlistAttributes	[in] Playlist attributes
*/ 

-(void) addWithMediaIdentifier: (NSString*)mediaIdentifier playlistAttributes: (GnPlaylistAttributes*)playlistAttributes error: (NSError**)error;

/**
* Add a GnListElement object to a Collection Summary.
*
* @param mediaIdentifier   [in] Media identifier
* @param listElement       [in] GnListElement object
*/ 

-(void) addWithMediaIdentifier: (NSString*)mediaIdentifier listElement: (GnListElement*)listElement error: (NSError**)error;

/**
* Remove a media element from a Collection Summary.
*
* @param mediaIdentifier  [in] Media identifier
*/ 

-(void) remove: (NSString*)mediaIdentifier error: (NSError**)error;

/**
* Test if a media element is in a Collection Summary.
*
* @param mediaIdentifier  [in] Media identifier
*/ 

-(BOOL) contains: (NSString*)mediaIdentifier error: (NSError**)error;

/**
* Return metadata from a playlist using a playlist identifier
*
* @param user             [in] Gracenote user
* @param mediaIdentifier  [in] Playlist identifier
*/ 

-(GnPlaylistAttributes*) attributesWithUser: (GnUser*)user mediaIdentifier: (GnPlaylistIdentifier*)mediaIdentifier error: (NSError**)error;

/**
* Return attributes from a playlist using a Collection Summary name
*
* @param user             [in] GN User object
* @param mediaIdentifier  [in] Playlist identifier
*/ 

-(GnPlaylistAttributes*) attributesWithUser: (GnUser*)user mediaIdentifier: (NSString*)mediaIdentifier joinedCollectionName: (NSString*)joinedCollectionName error: (NSError**)error;

/**
* Validate a Playlist Definitioon Statement
* @param pdlStatement	Playlist Definition Statment
* @return Validation result
*/ 

-(NSError*) statementValidate: (NSString*)pdlStatement;

/**
* Determine if a Playlist Definition Statement requires a seed
* to generate a playlist
* @param pdlStatement	Playlist Definition Statment
* @return True if a seed is required, false otherwise
*/ 

-(BOOL) statementRequiresSeed: (NSString*)pdlStatement error: (NSError**)error;

/**
* <b>Experimental</b>. Analyzes the given PDL Statement as applied to the given media identifier.
* Used for debugging and analyzing playlist generation with PDL statements this method
* analyzes the given PDL as applied to the given media identifier.
* The output is a formatted logical tree of the PDL statement and the outcome applied
* for each operation.
* For more information on creating PDL Statements consult the PDL Specification.
* @param pdlStatment		PDL Statment being debugged
* @param mediaIdentifier	Unique identifier of a media item within the collection summary
*/ 

-(NSString*) statementAnalyzeIdent: (NSString*)pdlStatement mediaIdentifier: (NSString*)mediaIdentifier error: (NSError**)error;

/**
* Generate a playlist from a GnDataObject
*
* @param user             [in] Gracenote user
* @param pdlStatement     [in] Playlist Description Language statement
* @param playlistSeed     [in] GN data object to use as seed for playlist
*/ 

-(GnPlaylistResult*) generatePlaylistWithUser: (GnUser*)user pdlStatement: (NSString*)pdlStatement playlistSeed: (GnDataObject*)playlistSeed error: (NSError**)error;

/**
* Generate a playlist using a Playlist Definition Language (PDL) Statement from this object's Collection Summary.
* For more information on creating PDL Statements consult the PDL Specification.
*
* @param user             [in] Gracenote user
* @param pdlStatement     [in] Playlist Description Language statement
*
*/ 

-(GnPlaylistResult*) generatePlaylistWithUser: (GnUser*)user pdlStatement: (NSString*)pdlStatement error: (NSError**)error;

/**
* Generate a playlist from a GnDataObject
*
* @param user             [in] Gracenote user
* @param musicDataObj     [in] Gracenote data object
*
*/ 

-(GnPlaylistResult*) generateMoreLikeThis: (GnUser*)user musicDataObj: (GnDataObject*)musicDataObj error: (NSError**)error;

/**
*  Returns an iterable container for all the playlist attributes supported by this collection.
*
*/ 

-(GnStringEnumerator*) attributesSupported;

/**
* Returns an iterable container for all the media identifiers in the collection
*/ 

-(GnPlaylistIdentifierEnumerator*) mediaIdentifiers;

/**
*  Returns an iterable container for all the joined collections .
*
*/ 

-(GnPlaylistCollectionEnumerator*) joins;

/**
* Join a playlist collection by collection name
*
* @param collectionName   [in] Collection Summary name
*/ 

-(GnPlaylistCollection*) joinFindByName: (NSString*)collectionName error: (NSError**)error;

/**
* Join a playlist collection by playlist collection object
*
* @param toJoin   [in] GnPlaylistCollection object for join
*/ 

-(void) join: (GnPlaylistCollection*)toJoin error: (NSError**)error;

/**
* Remove a join with another playlist
*
* @param toRemove   [in] GnPlaylistCollection object to remove join
*/ 

-(void) joinRemove: (GnPlaylistCollection*)toRemove error: (NSError**)error;

/**
* Serialize a Collection Summary into buffer
*
* @param serializedData    [out] Buffer to receive serialized data
* @param dataSize          [in] Buffer size
* @return buffer size
*/ 

-(NSUInteger) serialize: (unsigned char*)serializedData dataSize: (NSUInteger)dataSize error: (NSError**)error;

/**
* Returns the size of a serialized playlist collection object
* @return serialized buffer size
*/ 

-(NSUInteger) serializeSize:(NSError**) error;

/**
* Add an identifier as part of synchronizing a user's media collection with the
* corresponding Collection Summary.
*
* Collection sychronization is a two step process, step one is to use this method to
* add all unique identifiers that currently exist in the user's collection. Step two
* is to execute the synchronization process.
* @param mediaIdentifier [in] unique media identifier used in you application e.g. file path
*/ 

-(void) syncProcessAdd: (NSString*)mediaIdentifier error: (NSError**)error;

/**
* Process synchronize identifiers previously added to the Collection Summary to complete
* synchronizing a user's media collection with the corresponding Collection Summary.
*
* As step two of the synchronization process call this method after adding all unique
* identifiers using SyncProcessAdd. This call will determine which identifiers added for
* synchronization need to be also be added to the Collection Summary; and those that need to be
* removed from the Collection Summary because they no longer exist in the user's collection.
* This information is delivered via the synchronization delegate.
* @param syncEvents [in] Synchronizations events delegate
*/ 

-(void) syncProcessExecute: (id <GnPlaylistCollectionSyncEventsDelegate>)syncEvents error: (NSError**)error;

/**
* Get object for setting "more like this" options
* @return	More like this options object
*/ 

-(GnPlaylistMoreLikeThisOptions*) moreLikeThisOptions;


@end


@interface GnPlaylistCollectionEnumerator : NSEnumerator

typedef void (^GnPlaylistCollectionBlock)(id obj, NSUInteger index, BOOL *stop);

-(void)enumerateObjectsUsingBlock: (GnPlaylistCollectionBlock)handler;

-(NSUInteger) count;

-(GnPlaylistCollection*) nextObject;

-(GnPlaylistCollection*) objectAtIndex:(NSUInteger)index;

-(NSArray*) allObjects;


@end



#endif /* _GnPlaylistCollection_h_ */

