/*
 *
 *  GRACENOTE, INC. PROPRIETARY INFORMATION
 *  This software is supplied under the terms of a license agreement or
 *  nondisclosure agreement with Gracenote, Inc. and may not be copied
 *  or disclosed except in accordance with the terms of that agreement.
 *  Copyright(c) 2000-2014. Gracenote, Inc. All Rights Reserved.
 *
 */
 
#ifndef _GNENUMS_H_
#define _GNENUMS_H_




/***************************************
* GnLookupMode
*/
typedef enum : NSInteger
{
    /**
    * Invalid lookup mode
    */
    kLookupModeInvalid = 0,
    
    /**
    * This mode forces the lookup to be done against the local database only. Local caches created from (online) query
    * results are not queried in this mode.
    * If no local database exists, the query will fail.
    */
    kLookupModeLocal,
    
    /**
    * This is the default lookup mode. If a cache exists, the query checks it first for a match.
    * If a no match is found in the cache, then an online query is performed against Gracenote Service.
    * If a result is found there, it is stored in the local cache.  If no online provider exists, the query will fail.
    * The length of time before cache lookup query expires can be set via the user object.
    */
    kLookupModeOnline,
    
    /**
    * This mode forces the query to be done online only and will not perform a local cache lookup first.
    * If no online provider exists, the query will fail. In this mode online queries and lists are not
    * written to local storage, even if a storage provider has been initialize.
    */
    kLookupModeOnlineNoCache,
    
    /**
    * This mode forces the query to be done online only and will not perform a local cache lookup first.
    * If no online provider exists, the query will fail. If a storage provider has been initialized,
    * queries and lists are immediately written to local storage, but are never read unless the lookup mode is changed.
    */
    kLookupModeOnlineNoCacheRead,
    
    /**
    * This mode forces the query to be done against the online cache only and will not perform a network lookup.
    * If no online provider exists, the query will fail.
    */
    kLookupModeOnlineCacheOnly
    
} GnLookupMode;


/***************************************
* GnLookupData
*/
typedef enum : NSInteger
{
    kLookupDataInvalid = 0,
    
    /**
    * Indicates whether a response should include data for use in fetching content (like images).
    * <p><b>Remarks:</b></p>
    * An application's client ID must be entitled to retrieve this specialized data. Contact your
    *	Gracenote Global Services and Support representative with any questions about this enhanced
    *	functionality.
    */
    kLookupDataContent,
    
    /**
    * Indicates whether a response should include any associated classical music data.
    * <p><b>Remarks:</b></p>
    * An application's license must be entitled to retrieve this specialized data. Contact your
    * Gracenote Global Services and Support representative with any questions about this enhanced functionality.
    */
    kLookupDataClassical,
    
    /**
    * Indicates whether a response should include any associated sonic attribute data.
    * <p><b>Remarks:</b></p>
    * An application's license must be entitled to retrieve this specialized data. Contact your
    * Gracenote Global Services and Support representative with any questions about this enhanced functionality.
    */
    kLookupDataSonicData,
    
    /**
    * Indicates whether a response should include associated attribute data for GNSDK Playlist.
    * <p><b>Remarks:</b></p>
    * An application's license must be entitled to retrieve this specialized data. Contact your
    * Gracenote Global Services and Support representative with any questions about this enhanced functionality.
    */
    kLookupDataPlaylist,
    
    /**
    * Indicates whether a response should include external IDs (third-party IDs).
    * <p><b>Remarks:</b></p>
    * External IDs are third-party IDs associated with the results (such as an Amazon ID),
    *	configured specifically for your application.
    * An application's client ID must be entitled to retrieve this specialized data. Contact your
    * Gracenote Global Services and Support representative with any questions about this enhanced functionality.
    * External IDs can be retrieved from applicable query response objects.
    */
    kLookupDataExternalIds,
    
    /**
    * Indicates whether a response should include global IDs.
    */
    kLookupDataGlobalIds,
    
    /**
    * Indicates whether a response should include additional credits.
    */
    kLookupDataAdditionalCredits
    
} GnLookupData;


/***************************************
* GnThreadPriority
*/
typedef enum : NSInteger
{
    kThreadPriorityInvalid = 0,
    
    /**
    * Use of default thread priority.
    */
    kThreadPriorityDefault,
    
    /**
    * Use idle thread priority.
    */
    kThreadPriorityIdle,
    
    /**
    * Use low thread priority (default).
    */
    kThreadPriorityLow,
    
    /**
    * Use normal thread priority.
    */
    kThreadPriorityNormal,
    
    /**
    * Use high thread priority.
    */
    kThreadPriorityHigh
    
} GnThreadPriority;


/***************************************
* GnFingerprintType
*/
typedef enum : NSInteger
{
    /**
    * Invalid fingerprint type
    */
    kFingerprintTypeInvalid = 0,
    
    /**
    * Specifies a fingerprint data type for generating fingerprints used with MusicID-File.
    * <p><b>Remarks:</b></p>
    * A MusicID-File fingerprint is a fingerprint of the beginning 16 seconds of the file.
    * <p><b>Note:</b></p>
    * Do not design your application to submit only 16 seconds of a file; the
    * application must submit data until GNSDK indicates it has received enough input.
    * Use this fingerprint type when identifying audio from a file source (MusicID-File).
    */
    kFingerprintTypeFile,
    
    /**
    *  Specifies a fingerprint used for identifying an ~3-second excerpt from an audio stream.
    *  Use this fingerprint type when identifying a continuous stream of audio data and when retrieving
    *  Track Match Position values. The fingerprint represents a
    *  specific point in time of the audio stream as denoted by the audio provided when the fingerprint
    *  is generated.
    *  <p><b>Note:</b></p>
    *  Do not design your application to submit only 3 seconds of audio data; the
    *  application must submit audio data until GNSDK indicates it has received enough input.
    *  You must use this fingerprint or its 6-second counterpart when generating results where match
    *  position is required.
    *  The usage of this type of fingerprint must be configured to your specific User ID, otherwise queries
    *  of this type will not succeed.
    */
    kFingerprintTypeStream3,
    
    /**
    *  Specifies a fingerprint used for identifying an ~6-second excerpt from and audio stream.
    *  This is the same as kFingerprintTypeStream3 but requires more audio data to generate
    *  but could be more accurate.
    *  For additional notes see kFingerprintTypeStream3.
    */
    kFingerprintTypeStream6,
    
    /**
    * @deprecated NB: This key has been marked as deprecated and will be removed from the next major release.
    *      Use kFingerprintTypeFile instead.
    */
    kFingerprintTypeCMX,
    
    /**
    * @deprecated NB: This key has been marked as deprecated and will be removed from the next major release.
    *      Use kFingerprintTypeStream3 or kFingerprintTypeStream6 instead.
    */
    kFingerprintTypeGNFPX
    
} GnFingerprintType;


/***************************************
* GnStatus
*/
typedef enum : NSInteger
{
    /** @internal kStatusUnknown @endinternal
    * Status unknown.
    * @ingroup StatusCallbacks_TypesEnums
    */
    kStatusUnknown = 0,
    
    /* Basic messages */
    
    /** @internal kStatusBegin @endinternal
    * Issued once per application function call, at the beginning of the call; percent_complete = 0.
    * @ingroup StatusCallbacks_TypesEnums
    */
    kStatusBegin,
    
    /** @internal kStatusProgress @endinternal
    * Issued roughly 10 times per application function call; percent_complete values between 1-100.
    * @ingroup StatusCallbacks_TypesEnums
    */
    kStatusProgress,
    
    /** @internal kStatusComplete @endinternal
    * Issued once per application function call, at the end of the call; percent_complete = 100.
    * @ingroup StatusCallbacks_TypesEnums
    */
    kStatusComplete,
    
    /** @internal kStatusErrorInfo @endinternal
    * Issued when an error is encountered. If sent, call #gnsdk_manager_error_info().
    * @ingroup StatusCallbacks_TypesEnums
    */
    kStatusErrorInfo,
    
    /* Advanced messages */
    
    /** @internal kStatusConnecting @endinternal
    * Issued when connecting to network.
    * @ingroup StatusCallbacks_TypesEnums
    */
    kStatusConnecting,
    
    /** @internal kStatusSending @endinternal
    * Issued when uploading.
    * @ingroup StatusCallbacks_TypesEnums
    */
    kStatusSending,
    
    /** @internal kStatusReceiving @endinternal
    * Issued when downloading.
    * @ingroup StatusCallbacks_TypesEnums
    */
    kStatusReceiving,
    
    /** @internal kStatusDisconnected @endinternal
    * Issued when disconnected from network.
    * @ingroup StatusCallbacks_TypesEnums
    */
    kStatusDisconnected,
    
    /** @internal kStatusReading @endinternal
    * Issued when reading from storage.
    * @ingroup StatusCallbacks_TypesEnums
    */
    kStatusReading,
    
    /** @internal kStatusWriting @endinternal
    * Issued when writing to storage.
    * @ingroup StatusCallbacks_TypesEnums
    */
    kStatusWriting,            /* issued whenever writing to storage. */
    
    /** @internal gnsdk_status_cancelled @endinternal
    * Issued when transaction/query is cancelled
    * @ingroup StatusCallbacks_TypesEnums
    */
    kStatusCancelled
    
} GnStatus;


/***************************************
* GnLanguage
*/
typedef enum : NSInteger
{
    kLanguageInvalid = 0,
    
    kLanguageArabic,
    kLanguageBulgarian,
    kLanguageChineseSimplified,
    kLanguageChineseTraditional,
    kLanguageCroatian,
    kLanguageCzech,
    kLanguageDanish,
    kLanguageDutch,
    kLanguageEnglish,
    kLanguageFarsi,
    kLanguageFinnish,
    kLanguageFrench,
    kLanguageGerman,
    kLanguageGreek,
    kLanguageHungarian,
    kLanguageIndonesian,
    kLanguageItalian,
    kLanguageJapanese,
    kLanguageKorean,
    kLanguageNorwegian,
    kLanguagePolish,
    kLanguagePortuguese,
    kLanguageRomanian,
    kLanguageRussian,
    kLanguageSerbian,
    kLanguageSlovak,
    kLanguageSpanish,
    kLanguageSwedish,
    kLanguageThai,
    kLanguageTurkish,
    kLanguageVietnamese
} GnLanguage;


/***************************************
* GnRegion
*/
typedef enum : NSInteger
{
    /** Default region. A region will be selected based on what's available. */
    kRegionDefault = 0,
    
    /** Global region */
    kRegionGlobal,
    
    /** United States region */
    kRegionUS,
    
    /** Japan region */
    kRegionJapan,
    
    /** China region */
    kRegionChina,
    
    /** Taiwan region */
    kRegionTaiwan,
    
    /** Korea region */
    kRegionKorea,
    
    /** Europe region */
    kRegionEurope,
    
    /** Deprecated, will be removed in a future release, use kRegionUS. */
    kRegionNorthAmerica,
    
    /** Latin America region */
    kRegionLatinAmerica,
    
    /** India region */
    kRegionIndia
} GnRegion;


/***************************************
* GnDescriptor
*/
typedef enum : NSInteger
{
    kDescriptorDefault = 0,
    
    kDescriptorSimplified,
    kDescriptorDetailed
} GnDescriptor;


/***************************************
* GnContentType
*/
typedef enum : NSInteger
{
    kContentTypeNull = 0,
    kContentTypeUnknown = 1,
    
    kContentTypeImageCover,
    kContentTypeImageArtist,
    kContentTypeImageVideo,
    kContentTypeBiography,
    kContentTypeReview,
    kContentTypeNews,
    kContentTypeArtistNews,
    kContentTypeListenerComments,
    kContentTypeReleaseComments
} GnContentType;


/***************************************
* GnImageSize
*/
typedef enum : NSInteger
{
    kImageSizeUnknown = 0,
    
    kImageSize75,
    kImageSize110,
    kImageSize170,
    kImageSize220,
    kImageSize300,
    kImageSize450,
    kImageSize720,
    kImageSize1080,
    
    /* Size aliases */
    kImageSizeThumbnail = kImageSize75,
    kImageSizeSmall     = kImageSize170,
    kImageSizeMedium    = kImageSize450,
    kImageSizeLarge     = kImageSize720,
    kImageSizeXLarge    = kImageSize1080
    
} GnImageSize;


/***************************************
* GnLicenseInputMode
*/
typedef enum : NSInteger
{
    kLicenseInputModeInvalid = 0,
    
    /**
    * Submit license content as string
    */
    kLicenseInputModeString,
    
    /**
    * Submit license content in file
    */
    kLicenseInputModeFilename,
    
    /**
    * Submit license content from stdin
    */
    kLicenseInputModeStandardIn
} GnLicenseInputMode;


/***************************************
* GnUserRegisterMode
*/
typedef enum : NSInteger
{
    /**
    * Register a user via a netowrk connection with Gracenote Service. A user
    * must be registered online before any online queries can be made against
    * Gracenote Service.
    */
    kUserRegisterModeOnline = 1,
    
    /**
    * Register a user for local use only. User's registered locally can only
    * perform queries against local databases and not against Gracenote
    * Service.
    */
    kUserRegisterModeLocalOnly
} GnUserRegisterMode;


/***************************************
* GnListType
*/
typedef enum : NSInteger
{
    kListTypeInvalid = 0,
    
    /**
    * This list contains languages that are supported by Gracenote, and are typically used to indicate
    * the original language of an item.
    */
    kListTypeLanguages,
    
    /**
    * This list contains scripts that are supported by Gracenote
    */
    kListTypeScripts,
    
    /**
    * The list of supported music genres.
    * <p><b>Remarks:</b></p>
    * The genre list contains a hierarchy of genres available from Gracenote strictly for music data.
    */
    kListTypeGenres,
    
    /**
    * The list of supported geographic origins for artists.
    */
    kListTypeOrigins,
    
    /**
    * The list of supported music era categories.
    */
    kListTypeEras,
    
    /**
    * The list of supported artist type categories.
    */
    kListTypeArtistTypes,
    
    /**
    * This list contains role list elements supported Gracenote for album data, such as Vocalist and Bass Guitar.
    */
    kListTypeRoles,
    
    /**
    *  This list contains a hierarchy of genre list elements available from Gracenote, strictly for
    * video data.
    */
    kListTypeGenreVideos,
    
    /**
    * This list contains movie rating list elements supported by Gracenote.
    */
    kListTypeRatings,
    
    /**
    * This list contains film content rating list elements supported by Gracenote.
    */
    kListTypeRatingTypes,
    
    /**
    * This list contains contributor role list elements available from Gracenote, such as Actor or
    * Costume Design. These apply to video data.
    */
    kListTypeContributors,
    
    /**
    * The list of supported feature types for video data.
    */
    kListTypeFeatureTypes,
    
    /**
    *  The list of supported video regions.
    */
    kListTypeVideoRegions,
    
    /**
    * The list of supported video types, such as Documentary, Sporting Event, or Motion Picture.
    */
    kListTypeVideoTypes,
    
    /**
    * The list of supported media types for music and video, such as Audio CD, Blu-ray, DVD, or HD DVD.
    */
    kListTypeMediaTypes,
    
    /**
    * The list of supported video serial types, such as Series or Episode.
    */
    kListTypeVideoSerialTypes,
    
    /**
    * The list of supported work types for video data, such as Musical or Image.
    */
    kListTypeWorkTypes,
    
    /**
    * The list of supported media spaces for video data, such as Music, Film, or Stage.
    */
    kListTypeMediaSpaces,
    
    /**
    * The list of supported moods for music data. Moods are categorized into levels, from more general
    * (Level 1, such as Blue) to more specific (Level 2, such as Gritty/Earthy/Soulful).
    */
    kListTypeMoods,
    
    /**
    * The list of supported tempos for music data, has three levels of granularity.
    * The tempos are categorized in levels in increasing order of granularity.
    * Level 1: The meta level, such as Fast Tempo.
    * Level 2: The sub tempo level, such as Very Fast.
    * Level 3: The micro level, which may be displayed as a numeric tempo range, such as 240-249, or a
    * descriptive phrase.
    */
    kListTypeTempos,
    
    /**
    * The list of supported composition forms for classical music.
    */
    kListTypeCompostionForm,
    
    /**
    * The list of supported instrumentation for classical music.
    */
    kListTypeInstrumentation,
    
    /**
    * The list of supported overall story types for video data, such as Love Story.
    * It includes general theme classifications such as such as Love Story, Family Saga, Road Trip,
    * and Rags to Riches.
    */
    kListTypeVideoStoryType,
    
    /**
    * The list of supported audience types for video data.
    * It includes general audience classifications by age, ethnicity, gender, and spiritual beliefs,
    * such as Kids & Family, African-American, Female, Gay & Lesbian, and Buddhist.
    */
    kListTypeVideoAudience,
    
    /**
    * The list of supported moods for video data, such as Offbeat.
    * It includes general classifications such as such as Offbeat, Uplifting, Mystical, and Sarcastic.
    */
    kListTypeVideoMood,
    
    /**
    * The list of supported film reputation types for video data, such as Classic.
    * It includes general classifications such as such as Classic, Chick Flick, and Cult.
    */
    kListTypeVideoReputation,
    
    /**
    * The list of supported scenarios for video data. It
    * includes general classifications such as such as Action, Comedy, and Drama.
    */
    kListTypeVideoScenario,
    
    /**
    * The language of the list is determined by the language value given to
    */
    kListTypeVideoSettingEnv,
    
    /**
    * The list of supported historical time settings for video data, such as Elizabethan Era,
    * 1558-1603, or Jazz Age, 1919-1929.
    */
    kListTypeVideoSettingPeriod,
    
    /**
    * The list of supported story concept sources for video data, such as Fairy Tales & Nursery Rhymes.
    * It includes story source classifications such as Novel, Video Game, and True Story.
    */
    kListTypeVideoSource,
    
    /**
    * The list of supported film style types for video data, such as Film Noir.It
    * includes general style classifications such as Art House, Film Noir, and Silent.
    */
    kListTypeVideoStyle,
    
    /**
    * The list of supported film topics for video data, such as Racing or Teen Angst. It includes a diverse
    * range of film topics, such as Politics, Floods, Mercenaries, Surfing, and Adventure. It also includes
    * some list elements that can be considered sub-topics of a broader topic. For example, the list element Aliens (the broad topic),
    * and Nice Aliens and Bad Aliens (the more defined topics).
    */
    kListTypeVideoTopic,
    
    /**
    * The list of supported viewing types for EPG data, such as live and rerun.
    */
    kListTypeEpgViewingTypes,
    
    /**
    * The list of supported audio types for EPG data, such as stereo and dolby.
    */
    kListTypeEpgAudioTypes,
    
    /**
    * The list of supported video types for EPG data, such as HDTV and PAL30.
    */
    kListTypeEpgVideoTypes,
    
    /**
    * The list of supported video types for EPG data, such as closed caption.
    */
    kListTypeEpgCaptionTypes,
    
    /**
    * The list of supported categories for IPG data, such as movie and TV series.
    */
    kListTypeIpgCategoriesL1,
    
    /**
    * The list of supported categories for IPG data, such as action and adventure.
    */
    kListTypeIpgCategoriesL2,
    
    /**
    * The list of supported production types for EPG data, such as news and documentary.
    */
    kListTypeEpgProductionTypes,
    
    /**
    * The list of supported device types for EPG data.
    */
    kListTypeEpgDeviceTypes
} GnListType;


/***************************************
* GnListRenderFlags
*/
typedef enum : NSInteger
{
    kRenderDefault,
    
    /**
    * Minimal information included.
    */
    kRenderMinimal,
    
    /**
    * All information included.
    */
    kRenderFull
    
} GnListRenderFlags;


/***************************************
* GnLocaleGroup
*/
typedef enum : NSInteger
{
    kLocaleGroupInvalid = 0,
    
    kLocaleGroupMusic,
    kLocaleGroupVideo,
    kLocaleGroupPlaylist,
    kLocaleGroupEpg
} GnLocaleGroup;


/***************************************
* GnLogMessageType
*/
typedef enum : NSInteger
{
    kLoggingMessageTypeError = 1,
    kLoggingMessageTypeWarning,
    kLoggingMessageTypeInfo,
    kLoggingMessageTypeDebug
    
} GnLogMessageType;


/***************************************
* GnLogPackageType
*/
typedef enum : NSInteger
{
    kLogPackageInternal	=  1,
    kLogPackageManager		,
    kLogPackageMusicID		,
    kLogPackageMusicIDFile	,
    kLogPackageLink			,
    kLogPackageVideoID		,
    kLogPackageSubmit		,
    kLogPackagePlaylist		,
    kLogPackageStorageSqlite,
    kLogPackageDsp			,
    kLogPackageMusicIdMatch	,
    kLogPackageAcr			,
    kLogPackageLookupLocal	,
    kLogPackageEDBInstall	,
    kLogPackageEPG			,
    kLogPackageMoodGrid		,
    kLogPackageStorageQNX	,
    kLogPackageLookupFPLocal,
    kLogPackageCorrelates	,
    kLogPackageTaste		,
    kLogPackageMusicIDStream,
    kLogPackageLookupLocalStream,
    kLogPackageRhythm,
    kLogPackageAllGNSDK,
    kLogPackageAll
} GnLogPackageType;


/***************************************
* GnLocalStreamEngineType
*/
typedef enum : NSInteger
{
    kLocalStreamEngineInvalid = 0,
    
    kLocalStreamEngineMMap,
    kLocalStreamEngineInMemory
} GnLocalStreamEngineType;


/***************************************
* GnLookupLocalStreamIngestStatus
*/
typedef enum : NSInteger
{
    kIngestStatusInvalid = 0,
    kIngestStatusItemBegin,
    kIngestStatusItemAdd,
    kIngestStatusItemDelete
    
} GnLookupLocalStreamIngestStatus;


/***************************************
* GnMoodgridPresentationType
*/
typedef enum : NSInteger
{
    kMoodgridPresentationType5x5   = 0x0505,
    kMoodgridPresentationType10x10 = 0x0A0A
    
} GnMoodgridPresentationType;


/***************************************
* GnMoodgridCoordinateType
*/
typedef enum : NSInteger
{
    kMoodgridCoordinateTopLeft,
    kMoodgridCoordinateBottomLeft
    
} GnMoodgridCoordinateType;


/***************************************
* GnMoodgridFilterConditionType
*/
typedef enum : NSInteger
{
    kConditionTypeInclude = 1,
    kConditionTypeExclude
    
} GnMoodgridFilterConditionType;


/***************************************
* GnMoodgridFilterListType
*/
typedef enum : NSInteger
{
    kMoodgridListTypeGenre = 1,
    kMoodgridListTypeOrigins,
    kMoodgridListTypeEras
    
} GnMoodgridFilterListType;


/***************************************
* GnMusicIdFileInfoStatus
*/
typedef enum : NSInteger
{
    /**
    * FileInfo has not been processed.
    * @ingroup Music_MusicIDFile_TypesEnums
    */
    kMusicIdFileInfoStatusUnprocessed  = 0,
    
    
    /**
    * FileInfo is currently being processed.
    * @ingroup Music_MusicIDFile_TypesEnums
    */
    kMusicIdFileInfoStatusProcessing   = 1,
    
    
    /**
    * An error occurred while processing the FileInfo.
    * @ingroup Music_MusicIDFile_TypesEnums
    */
    kMusicIdFileInfoStatusError      = 2,
    
    
    /**
    * No results were found for FileInfo.
    * @ingroup Music_MusicIDFile_TypesEnums
    */
    kMusicIdFileInfoStatusResultNone   = 3,
    
    
    /**
    * Single preferred response available for FileInfo.
    * @ingroup Music_MusicIDFile_TypesEnums
    */
    kMusicIdFileInfoStatusResultSingle = 4,
    
    
    /**
    * All retrieved results available for FileInfo.
    * @ingroup Music_MusicIDFile_TypesEnums
    */
    kMusicIdFileInfoStatusResultAll  = 5
    
} GnMusicIdFileInfoStatus;


/***************************************
* GnMusicIdFileCallbackStatus
*/
typedef enum : NSInteger
{
    /**
    *
    *   MusicID-File processing is beginning for a given FileInfo.
    * @ingroup Music_MusicIDFile_TypesEnums
    */
    kMusicIdFileCallbackStatusProcessingBegin    = 0x100,
    
    
    /**
    *
    *   Performing a Gracenote query for given FileInfo.
    * @ingroup Music_MusicIDFile_TypesEnums
    */
    kMusicIdFileCallbackStatusFileInfoQuery      = 0x150,
    
    
    /**
    *
    *   MusicID-File processing is complete for a given FileInfo.
    * @ingroup Music_MusicIDFile_TypesEnums
    */
    kMusicIdFileCallbackStatusProcessingComplete = 0x199,
    
    /**
    *
    *   MusicID-File processing for a given FileInfo has encountered an
    *   error.
    * @ingroup Music_MusicIDFile_TypesEnums
    */
    kMusicIdFileCallbackStatusProcessingError    = 0x299,
    
    
    /**
    *
    *   An error in MusicID-File querying or processing has occurred.
    * @ingroup Music_MusicIDFile_TypesEnums
    */
    kMusicIdFileCallbackStatusError              = 0x999
    
} GnMusicIdFileCallbackStatus;


/***************************************
* GnMusicIdFileProcessType
*/
typedef enum : NSInteger
{
    kQueryReturnSingle = 1,
    kQueryReturnAll
    
} GnMusicIdFileProcessType;


/***************************************
* GnMusicIdFileResponseType
*/
typedef enum : NSInteger
{
    kResponseAlbums = 1,
    kResponseMatches
    
} GnMusicIdFileResponseType;


/***************************************
* GnMusicIdStreamProcessingStatus
*/
typedef enum : NSInteger
{
    kStatusProcessingInvalid = 0,
    
    kStatusProcessingAudioNone,
    kStatusProcessingAudioSilence,
    kStatusProcessingAudioNoise,
    kStatusProcessingAudioSpeech,
    kStatusProcessingAudioMusic,
    
    kStatusProcessingTransitionNone,
    kStatusProcessingTransitionChannelChange,
    kStatusProcessingTransitionContentToContent,
    
    kStatusProcessingErrorNoClassifier,
    
    kStatusProcessingAudioStarted,
    kStatusProcessingAudioEnded
} GnMusicIdStreamProcessingStatus;


/***************************************
* GnMusicIdStreamIdentifyingStatus
*/
typedef enum : NSInteger
{
    /** Invalid status
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kStatusIdentifyingInvalid = 0,
    
    
    /** Identification query started
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kStatusIdentifyingStarted,
    
    /** Fingerprint generated for sample audio
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kStatusIdentifyingFpGenerated,
    
    /** Local query started for identification
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kStatusIdentifyingLocalQueryStarted,
    
    /** Local query ended for identification
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kStatusIdentifyingLocalQueryEnded,
    
    /** Online query started for identification
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kStatusIdentifyingOnlineQueryStarted,
    
    /** Online query ended for identification
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kStatusIdentifyingOnlineQueryEnded,
    
    /** Identification query ended
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kStatusIdentifyingEnded
    
} GnMusicIdStreamIdentifyingStatus;


/***************************************
* GnMusicIdStreamApplicationEventType
*/
typedef enum : NSInteger
{
    /** No event
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kApplicationEventNone = 0,
    
    /** Metadata change event
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kApplicationEventMetadataChange,
    
    /** Broadcast channel change event
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kApplicationEventBroadcastChange,
    
    /** Broadcast pause event
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kApplicationEventPause,
    
    /** Broadcast resume event
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kApplicationEventResume
} GnMusicIdStreamApplicationEventType;


/***************************************
* GnMusicIdStreamPreset
*/
typedef enum : NSInteger
{
    kPresetInvalid = 0,
    
    /** Application type mobile, i.e. audio is captured by microphone
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kPresetMicrophone,
    
    /** Application type radio, i.e. audio is captured at source (e.g. speaker)
    * @ingroup Music_MusicIDStream_TypesEnums
    */
    kPresetRadio
} GnMusicIdStreamPreset;


/***************************************
* GnMusicIdStreamEvent
*/
typedef enum : NSInteger
{
    kEventInvalid = 0,
    
    kEventBroadcastMetadataChange
} GnMusicIdStreamEvent;


/***************************************
* GnPlaylistEventsIdentiferStatus
*/
typedef enum : NSInteger
{
    /**
    * The corresponding identifier's status is unknown, the default state
    */
    kIdentifierStatusUnknown = 0,
    
    /**
    * The corresponding identifier is new, meaning it has been added to the
    * user's media collection and needs to be added to the Collection Summary
    */
    kIdentifierStatusNew     = 10,
    
    /**
    * The corresponding identifier is old, meaning it has been deleted from
    * the user's media collection and needs to be removed from the
    * Collection Summary
    */
    kIdentifierStatusOld     = 20
    
} GnPlaylistEventsIdentiferStatus;


/***************************************
* GnDataLevel
*/
typedef enum : NSInteger
{
    kDataLevelInvalid = 0,
    
    kDataLevel_1      = 1,  /* least granular */
    kDataLevel_2,
    kDataLevel_3,
    kDataLevel_4            /* most granular */
    
} GnDataLevel;


#endif /* _GNENUMS_H_ */

