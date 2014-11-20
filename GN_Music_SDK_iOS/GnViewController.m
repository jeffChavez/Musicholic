//
//  GnViewController.m
//  GN_Music_SDK_iOS
//
//  Copyright (c) 2013 Gracenote. All rights reserved.
//

#import "GnViewController.h"
#import "GnAppDelegate.h"
#import "GnDataModel.h"

#import <AVFoundation/AVFoundation.h>
#import <CoreData/CoreData.h>
#import <objc/runtime.h>

#import <GnSDKObjC/Gn.h>

#import "History.h"
#import "CoverArt.h"
#import "Metadata.h"
#import "GnAudioVisualizeAdapter.h"

#import "NetworkController.h"
#import "DrinkView.h"




static NSString *gnsdkLicenseFilename = @"license.txt";

@interface GnViewController ()<GnMusicIdStreamEventsDelegate, GnMusicIdFileEventsDelegate, UITabBarDelegate, UIGestureRecognizerDelegate, GnMusicIdFileInfoEventsDelegate, GnAudioVisualizerDelegate, GnLookupLocalStreamIngestEventsDelegate>



//TEST
@property (strong, nonatomic) GnDataModel *currentDataModel;
@property (strong, nonatomic) Drink *currentDrink;


/*GnSDK properties*/
@property (strong) GnMusicIdStream *gnMusicIDStream;
@property (strong) GnMic *gnMic;
@property (strong) GnAudioVisualizeAdapter *gnAudioVisualizeAdapter;
@property (strong) GnManager *gnManager;
@property (strong) GnUser *gnUser;
@property (strong) GnUserStore *gnUserStore;
@property (strong) GnStorageSqlite *gnStorageSqlite;
@property (strong) GnLookupLocalStream* gnLookupLocalStream;
@property (strong) NSArray *history;
@property (strong) NSMutableArray *albumDataMatches;
@property (strong) GnLocale *locale;

/*Sample App properties*/
@property (assign) BOOL lookupSourceIsLocal;
@property (assign) BOOL audioProcessingStarted;
@property (assign) NSTimeInterval queryBeginTimeInterval;
@property (assign) NSTimeInterval queryEndTimeInterval;
@property (strong) NSMutableArray *cancellableObjects;

@property BOOL recordingIsPaused;
@property BOOL visualizationIsVisible;
@property UIDynamicAnimator *dynamicAnimator;
@property dispatch_queue_t internalQueue;
@property (strong) NSMutableArray *results;

@property NSIndexPath *currentlySelectedIndexPath;

@property DrinkView *drinkView;
@end

@implementation GnViewController

-(void) handleTap: (UITapGestureRecognizer *)tapGestureRecognizer {
    
    [UIView animateWithDuration:0.4 animations:^{
        self.drinkView.frame = CGRectMake(self.view.frame.size.width + self.drinkView.frame.size.width, self.view.frame.size.height / 2, self.drinkView.frame.size.width, self.drinkView.frame.size.width);
    }];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.drinkView = [[DrinkView alloc] init];
    self.drinkView = [[[NSBundle mainBundle] loadNibNamed:@"DrinkView" owner:self options:nil] objectAtIndex:0];
    self.drinkView.frame = CGRectMake(self.view.frame.size.width + self.drinkView.frame.size.width, self.view.frame.size.height / 2, self.drinkView.frame.size.width, self.drinkView.frame.size.height);
    self.drinkView.labelView.text = @"Martini";
    self.drinkView.imageView.image = [UIImage imageNamed:@"martini.jpg"];
    self.drinkView.imageView.layer.cornerRadius = self.drinkView.frame.size.width / 4;
    self.drinkView.imageView.layer.masksToBounds = YES;
    
    [UIView animateWithDuration:0.4 delay:0.4 usingSpringWithDamping: 0.8 initialSpringVelocity:0.2 options:0 animations:^{
        self.drinkView.frame = CGRectMake(self.view.frame.size.width / 2 - (self.drinkView.frame.size.width / 2), self.view.frame.size.height / 2 , 200 , 200);
    } completion:^(BOOL finished) {
        NSLog(@"123");
    }];
    
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTap:)];
    [self.drinkView addGestureRecognizer:tapGesture];
    [self.view addSubview:self.drinkView];
    
    self.recordingIsPaused = NO;
    __block NSError * error = nil;

    self.lookupSourceIsLocal = 0;
    self.audioProcessingStarted = 0;
    self.queryBeginTimeInterval = -1;
    self.queryEndTimeInterval = -1;

    self.cancellableObjects = [NSMutableArray arrayWithCapacity:2];
    self.albumDataMatches = [NSMutableArray arrayWithCapacity:2];

    [self setupInterface];

    self.results = [NSMutableArray arrayWithCapacity:2];

    AVAudioSession *session = [AVAudioSession sharedInstance];
    [session setPreferredSampleRate:44100 error:nil];
    [session setInputGain:0.5 error:nil];
    [session setActive:YES error:nil];


    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationResignedActive:)
                                                 name:UIApplicationWillResignActiveNotification
                                               object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationDidBecomeActive:)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];

    // Check if both ClientID and ClientIDTag have been set.
    if ([CLIENTID length]==0 || [CLIENTIDTAG length]==0)
    {
        self.statusLabel.text = @"Please set Client ID and Client Tag.";
        return;
    }

    // Check if license file has been set.
    if (gnsdkLicenseFilename==nil)
    {
        self.statusLabel.text = @"License filename not set.";
        return;
    }
    else if ([[NSBundle mainBundle] pathForResource:gnsdkLicenseFilename ofType:nil] ==nil)
    {
        self.statusLabel.text = [NSString stringWithFormat:@"License file not found:%@", gnsdkLicenseFilename];
        return;
    }

    // -------------------------------------------------------------------------------
    // Initialize GNSDK.
    // -------------------------------------------------------------------------------
    error = [self initializeGNSDKWithClientID: CLIENTID clientIDTag: CLIENTIDTAG];
	if (error)
	{
		NSLog( @"Error: 0x%zx %@ - %@", (long)[error code], [error domain], [error localizedDescription] );
	}
	else
	{
        

		@try
		{
			self.gnStorageSqlite = [GnStorageSqlite enable: &error];

			[self.gnStorageSqlite storageLocationWithFolderPath:[GnAppDelegate applicationDocumentsDirectory] error: &error];
			if (error)
			{
				NSLog( @"Error: 0x%zx %@ - %@", (long)[error code], [error domain], [error localizedDescription] );
			}
			else
			{
				error = [self setupLocalLookup];
				if (error)
				{
					NSLog( @"Error: 0x%zx %@ - %@", (long)[error code], [error domain], [error localizedDescription] );
				}
				else
				{
                    [self downloadLatestBundle];
                    
					// -------------------------------------------------------------------------------
					// Initialize Microphone AudioSource to Start Recording.
					// -------------------------------------------------------------------------------

					// Configure Microphone
					self.gnMic = [[GnMic alloc] initWithSampleRate: 44100 bitsPerChannel:16 numberOfChannels: 1];
                    
                    // configure dispatch queue
                    self.internalQueue = dispatch_queue_create("gnsdk.TaskQueue", NULL);
                    
					// If configuration succeeds, start recording.
					if (self.gnMic)
					{
						[self setupMusicIDStream];
					}
				}
			}
		}
		@catch (NSException *exception)
		{
			NSLog( @"Error: %@ - %@ - %@", [exception name], [exception reason], [exception userInfo] );
			return;
		}
	}
}

- (NSError *) setupLocalLookup
{
	NSError *	error = nil;

	//	Initialize the local lookup so we can do local lookup queries.
	self.gnLookupLocalStream = [GnLookupLocalStream enable: &error];
	if (! error)
	{
		NSString *	docDir = [GnAppDelegate applicationDocumentsDirectory];
		[self.gnLookupLocalStream storageLocation: docDir
											error: &error];
	}

	return error;
}



-(void) downloadLatestBundle
{
    NSError *	error = nil;
    
    //	Look for the 10,000 track bundle and if not found try the little one.
    NSString*	bundlePath = [[NSBundle mainBundle] pathForResource: @"1557.b" ofType: nil];
        
    if (bundlePath)
    {
        [self.gnLookupLocalStream storageClear: &error];
            
        if (! error)
        {
            __block GnLookupLocalStreamIngest *lookupLocalStreamIngest = [[GnLookupLocalStreamIngest alloc] initWithGnLookupLocalStreamIngestEventsDelegate:self];
                
            // Load Bundle in a separate thread to keep the UI responsive. This is required for Large Bundles that can take few minutes to be ingested.
                
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
                    
                NSError *error = nil;
                NSInteger bytesRead = 0;
                double totalBytesRead = 0;
                uint8_t buffer[1024];
                NSInputStream *fileInputStream = [NSInputStream inputStreamWithFileAtPath:bundlePath];
                    
                [fileInputStream open];
                    
                do {
                    bytesRead =  [fileInputStream read:buffer maxLength:1024];
                    [lookupLocalStreamIngest write:buffer dataSize:sizeof(buffer) error:&error];
                        
                    if(error)
                    {
                        NSLog(@"Error during lookupLocalStreamIngest write: %@", [error localizedDescription]);
                    }
                        
                    totalBytesRead+=bytesRead;
                        
                }while (bytesRead>0);
                    
                [lookupLocalStreamIngest flush:&error];
                [fileInputStream close];
                    
            });
        }
    }
}



-(void) setupInterface
{
    
    //Setup Dynamic Animator.
    self.dynamicAnimator = [[UIDynamicAnimator alloc] initWithReferenceView:self.visualizationView];
    
    //Resize Visualization
    CGRect visualizationRect = self.visualizationView.frame;
    visualizationRect.origin.y -= visualizationRect.size.height - (self.showOrHideVisualizationButtonView.frame.size.height + 10);
    self.visualizationView.frame = visualizationRect;
    self.showOrHideVisualizationButton.titleLabel.text = @"Show Visualization";
    self.showOrHideVisualizationButtonView.layer.cornerRadius = 10.0f;
    self.showOrHideVisualizationButtonView.layer.borderWidth = 1.0f;
    self.showOrHideVisualizationButtonView.layer.borderColor = [UIColor whiteColor].CGColor;
    self.visualizationView.layer.cornerRadius = 5.0f;



    [self.idNowButton.layer setShadowColor:[UIColor darkGrayColor].CGColor];
    [self.idNowButton.layer setShadowOffset:CGSizeMake(0, 0)];
    [self.idNowButton.layer setShadowRadius:10.0f];
    [self.idNowButton.layer setShadowOpacity:1];
    [self.idNowButton.layer setMasksToBounds:NO];

    self.currentlySelectedIndexPath = nil;


}

-(void) enableOrDisableControls:(BOOL) enable
{
    self.idNowButton.enabled = enable && self.audioProcessingStarted;
    self.cancelOperationsButton.enabled = !enable;

}

#pragma mark - Display Overlay View's





- (IBAction)showVisualization:(id)sender {
    
    __block CGRect visualizationFrame = self.visualizationView.frame;
    
   if(!self.visualizationIsVisible)
   {
       visualizationFrame.origin.y += visualizationFrame.size.height - (self.showOrHideVisualizationButtonView.frame.size.height+10);
   }
   else
   {
       visualizationFrame.origin.y -= visualizationFrame.size.height - (self.showOrHideVisualizationButtonView.frame.size.height+10);
   }
    
    [UIView animateWithDuration:0.5 animations:^{
    
        self.visualizationView.frame = visualizationFrame;
    
    } completion:^(BOOL finished){
       
        if(finished)
        {
            self.visualizationIsVisible = !self.visualizationIsVisible;
            self.showOrHideVisualizationButton.titleLabel.text = (self.visualizationIsVisible)?@"       \tClose\t":@"Show Visualization";
            
            [UIView animateWithDuration:0.5 animations:^{
            
               if(self.visualizationIsVisible)
               {
                UIDynamicItemBehavior *spinBehavior = [[UIDynamicItemBehavior alloc] initWithItems:@[self.gracenoteLogoImageView]];
                [spinBehavior addAngularVelocity:5 forItem:self.gracenoteLogoImageView];
                [spinBehavior setAngularResistance:0];
                   // [self.dynamicAnimator addBehavior:spinBehavior];
               }
                else
                {
                    float scale = 1.0f;
                    CGAffineTransform sscale = CGAffineTransformMakeScale (scale, scale);
                    self.coloredRingImageView.transform  = sscale;
                    
                    [self.dynamicAnimator removeAllBehaviors];
                    
                    CATransform3D rotTransform = CATransform3DMakeRotation (0, 0,
                                                                            0, 1);
                    self.gracenoteLogoImageView.layer.transform = rotTransform;

                }
            }];
        }
    
    }];
   
}






#pragma mark - Music ID Stream Setup

-(void) setupMusicIDStream
{
    if (!self.gnUser)
        return;
    self.recordingIsPaused = NO;

    __block NSError *musicIDStreamError = nil;
    @try
	{
		self.gnMusicIDStream = [[GnMusicIdStream alloc] initWithGnUser: self.gnUser preset:kPresetMicrophone locale:self.locale musicIdStreamEventsDelegate: self];

		musicIDStreamError = nil;
		GnMusicIdStreamOptions *options = [self.gnMusicIDStream options];
		[options resultSingle:YES error:&musicIDStreamError];
		[options lookupData:kLookupDataSonicData enable:YES error:&musicIDStreamError];
		[options lookupData:kLookupDataContent enable:YES error:&musicIDStreamError];
		[options preferResultCoverart:YES error:&musicIDStreamError];

		musicIDStreamError = nil;
		dispatch_async(self.internalQueue, ^
        {
            self.gnAudioVisualizeAdapter = [[GnAudioVisualizeAdapter alloc] initWithAudioSource:self.gnMic audioVisualizerDelegate:self];
            
            self.idNowButton.enabled = NO; //disable stream-ID until audio-processing-started callback is received
            
			[self.gnMusicIDStream audioProcessStartWithAudioSource:(id <GnAudioSourceDelegate>)self.gnAudioVisualizeAdapter error:&musicIDStreamError];

			if (musicIDStreamError)
			{
                dispatch_async(dispatch_get_main_queue(), ^
                {
                    NSLog(@"Error while starting Audio Process With AudioSource - %@", [musicIDStreamError localizedDescription]);
                });
            }
        });
	}
	@catch (NSException *exception)
	{
		NSLog( @"Error: %@ - %@ - %@", [exception name], [exception reason], [exception userInfo] );
	}
}

#pragma mark - GnManager, GnUser Initialization

-(NSError *) initializeGNSDKWithClientID: (NSString*)clientID clientIDTag: (NSString*)clientIDTag
{
	NSError*	error = nil;
	NSString*	resourcePath  = [[NSBundle mainBundle] pathForResource: gnsdkLicenseFilename ofType: nil];
	NSString*	licenseString = [NSString stringWithContentsOfFile: resourcePath
														  encoding: NSUTF8StringEncoding
														     error: &error];
	if (error)
	{
		NSLog( @"Error in reading license file %@ at path %@ - %@", gnsdkLicenseFilename, resourcePath, [error localizedDescription] );
	}
	else
	{
		@try
		{
			self.gnManager = [[GnManager alloc] initWithLicense: licenseString licenseInputMode: kLicenseInputModeString];
			self.gnUserStore = [[GnUserStore alloc] init];
			self.gnUser = [[GnUser alloc] initWithGnUserStoreDelegate: self.gnUserStore
															 clientId: clientID
															clientTag: clientIDTag
												   applicationVersion: @"1.0.0.0"];

            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                
                NSError *localeError = nil;
                
                @try
                {
                    self.locale = [[GnLocale alloc] initWithGnLocaleGroup: kLocaleGroupMusic
                                                                 language: kLanguageEnglish
                                                                   region: kRegionGlobal
                                                               descriptor: kDescriptorSimplified
                                                                     user: self.gnUser
                                                     statusEventsDelegate: nil];
                    
                    [self.locale setGroupDefault:&localeError];
                    
                    
                    if (localeError)
                    {
                        NSLog(@"Error while loading Locale - %@", [localeError localizedDescription]);
                    }
                    
                }
                @catch (NSException *exception)
                {
                    NSLog(@"Exception %@", [exception reason]);
                }
                
            });
		}
		@catch (NSException *exception)
		{
			error = [NSError errorWithDomain: [[exception userInfo] objectForKey: @"domain"]
										code: [[[exception userInfo] objectForKey: @"code"] integerValue]
									userInfo: [NSDictionary dictionaryWithObject: [exception reason] forKey: NSLocalizedDescriptionKey]];
			self.gnManager  = nil;
			self.gnUser = nil;
		}
	}

	return error;
}


#pragma mark - Status Update Methods

- (void) setStatus:(NSString*)status showStatusPrefix:(BOOL)showStatusPrefix
{
	NSString *statusToDisplay;

	if (showStatusPrefix) {
		NSMutableString *mstr = [NSMutableString string];
		[mstr appendString:@"Status: "];
		[mstr appendString:status];
		statusToDisplay = [NSString stringWithString:mstr];
	} else {
		statusToDisplay = status;
	}

	self.statusLabel.text = statusToDisplay;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark - Id Now


- (IBAction) findDrink:(id) sender {
    NSLog(@"FIND DRINK!!");

    // MAKE URL
    [[NetworkController networkController] fetchDrinkForSong:@"Billie Jean" withArtist:@"Michael Jackson" withCompletionHandler:^(NSString *errorString, Drink *drink) {

        // Set the currentDrink property with the result from the mongodb
        self.currentDrink = drink;
        [self loadSongDataIntoViews];
        
    }];
}




-(void) idNow:(id) sender
{
    if(self.gnMusicIDStream)
    {
        self.cancelOperationsButton.enabled = YES;
        [self enableOrDisableControls:NO];
        [self.results removeAllObjects];

        self.currentlySelectedIndexPath = nil;

        NSError *error = nil;
        [self.cancellableObjects addObject: self.gnMusicIDStream];
        [self.gnMusicIDStream identifyAlbumAsync:&error];
        [self updateStatus: @"Identifying"];

        if (error)
        {
            NSLog(@"Identify Error = %@", [error localizedDescription]);
            self.queryBeginTimeInterval = -1;
        }
        else
        {
            self.queryBeginTimeInterval = [[NSDate date] timeIntervalSince1970];
        }
    }
}

- (IBAction)cancelAllOperations:(id)sender
{
    for(id obj in self.cancellableObjects)
    {
        if([obj isKindOfClass:[GnMusicIdStream class]])
        {
            NSError *error = nil;
            [obj identifyCancel:&error];
            if(error)
            {
                NSLog(@"MusicIDStream Cancel Error = %@", [error localizedDescription]);
            }
        }
        else if ([obj isKindOfClass:[GnMusicIdFile class]])
        {
            [obj cancel];
        }
        else
        {
            [obj setCancel:YES];
        }
    }
    
}


#pragma mark - Do Operation Methods






- (NSString*)stringWithPercentEscape:(NSString*) refStr
{
    return (NSString *) CFBridgingRelease(CFURLCreateStringByAddingPercentEscapes(NULL, (CFStringRef)refStr, NULL, CFSTR("￼=,!$&'()*+;@?\n\"<>#\t :/"),kCFStringEncodingUTF8));
}

#pragma mark - Process Album Response

-(void) processAlbumResponseAndUpdateResultsTable:(id) responseAlbums
{
    // THIS POPULATES THE DATA MODEL OBJECT
    // Fire the network request to get drink info
    
    
    id albums = nil;
    
    if([responseAlbums isKindOfClass:[GnResponseAlbums class]]) {
        albums = [responseAlbums albums];
        NSLog(@"responseAlbums is Kind of Class GnResponseAlbums");
    }
    else {
        albums = responseAlbums;
    }
    
    
    for(GnAlbum* album in albums)
    {
        NSLog(@"going through for loop");
		GnTrackEnumerator *tracksMatched  = [album tracksMatched];
        NSString *albumArtist = [[[album artist] name] display];
        NSString *albumTitle = [[album title] display];
        NSString *albumGenre = [album genre:kDataLevel_1] ;
        NSString *albumID = [NSString stringWithFormat:@"%@-%@", [album tui], [album tuiTag]];
        GnExternalId *externalID  =  nil;
        if ([album externalIds] && [[album externalIds] allObjects].count)
            externalID = (GnExternalId *) [[album externalIds] nextObject];

        NSString *albumXID = [externalID source];
        NSString *albumYear = [album year];
        NSString *albumTrackCount = [NSString stringWithFormat:@"%lu", (unsigned long)[album trackCount]];
        NSString *albumLanguage = [album language];
        
        __block GnDataModel *gnDataModelObject = [[GnDataModel alloc] init];
        gnDataModelObject.albumArtist = albumArtist;
        gnDataModelObject.albumGenre = albumGenre;
        gnDataModelObject.albumID = albumID;
        gnDataModelObject.albumXID = albumXID;
        gnDataModelObject.albumYear = albumYear;
        gnDataModelObject.albumTitle = albumTitle;
        gnDataModelObject.albumTrackCount = albumTrackCount;
        gnDataModelObject.albumLanguage = albumLanguage;


        
        
        /* Get CoverArt */
        GnContent *coverArtContent = [album coverArt];
        GnAsset *coverArtAsset = [coverArtContent asset:kImageSizeSmall];
        NSString *URLString = [NSString stringWithFormat:@"http://%@", [coverArtAsset url]];
        
        NSURLRequest* request = [NSURLRequest requestWithURL:[NSURL URLWithString:URLString]];
        [NSURLConnection sendAsynchronousRequest:request queue:[NSOperationQueue mainQueue] completionHandler: ^(NSURLResponse *response, NSData* data, NSError* error) {
            if(data && !error) {
               gnDataModelObject.albumImageData = data;
                [self loadSongDataIntoViews];
            }
        }];

        
        GnContent *artistImageContent = [[[album artist] contributor] image];
        GnAsset *artistImageAsset = [artistImageContent asset:kImageSizeSmall];
        NSString *artistImageURLString = [NSString stringWithFormat:@"http://%@", [artistImageAsset url]];
        
        NSURLRequest *artistImageFetchRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:artistImageURLString]];
        [NSURLConnection sendAsynchronousRequest:artistImageFetchRequest queue:[NSOperationQueue mainQueue] completionHandler: ^(NSURLResponse *response, NSData* data, NSError* error){

            if(data && !error)
            {
                gnDataModelObject.artistImageData = data;
            }
        }];
        
        
        
        
        
        
        
        NSLog(@"Matched Album = %@", [[album title]display]);
        
        for(GnTrack *track in tracksMatched)
        {
            
            NSLog(@"  Matched Track = %@", [[track title]display]);

            NSString *trackArtist =  [[[track artist] name] display];
            NSString *trackMood = [track mood:kDataLevel_1] ;
            NSString *trackOrigin = [[[track artist] contributor] origin:kDataLevel_1];
            NSString *trackTempo = [track tempo:kDataLevel_1];
            NSString *trackGenre =  [track genre:kDataLevel_1];
            NSString *trackID =[NSString stringWithFormat:@"%@-%@", [track tui], [track tuiTag]];
            NSString *trackDuration = [NSString stringWithFormat:@"%lu",(unsigned long) ( [track duration]/1000)];
            NSString *currentPosition = [NSString stringWithFormat:@"%lu", (NSUInteger) [track currentPosition]/1000];
            NSString *matchPosition = [NSString stringWithFormat:@"%lu", (NSUInteger) [track matchPosition]/1000];


            if ([track externalIds] && [[track externalIds] allObjects].count)
                externalID = (GnExternalId *) [[track externalIds] nextObject];

            NSString *trackXID = [externalID source];
            NSString* trackNumber = [track trackNumber];
            NSString* trackTitle = [[track title] display];
            NSString* trackArtistType = [[[track artist] contributor] artistType:kDataLevel_1];

            //Allocate GnDataModel.
            gnDataModelObject.trackArtist = trackArtist;
            gnDataModelObject.trackMood = trackMood;
            gnDataModelObject.trackTempo = trackTempo;
            gnDataModelObject.trackOrigin = trackOrigin;
            gnDataModelObject.trackGenre = trackGenre;
            gnDataModelObject.trackID = trackID;
            gnDataModelObject.trackXID = trackXID;
            gnDataModelObject.trackNumber = trackNumber;
            gnDataModelObject.trackTitle = trackTitle;
            gnDataModelObject.trackArtistType = trackArtistType;
            gnDataModelObject.trackMatchPosition = matchPosition;
            gnDataModelObject.trackDuration = trackDuration;
            gnDataModelObject.currentPosition = currentPosition;
        }

        [self.results addObject:gnDataModelObject];
        self.currentDataModel = gnDataModelObject;
//        [self loadTestViews];

        
    }

    [self performSelectorOnMainThread:@selector(refreshResults) withObject:nil waitUntilDone:NO];
}



#pragma mark - Recording Interruptions

-(void) startRecording
{
    if (self.gnMusicIDStream)
    {
        NSError *error = nil;
        [self.gnMusicIDStream audioProcessStartWithAudioSource:self.gnMic error:&error];
        
        NSLog(@"Error while starting audio Process %@", [error localizedDescription]);
    }
}

-(void) stopRecording
{
    NSError *error = nil;
    [self.gnMusicIDStream audioProcessStop:&error];
    [[AVAudioSession sharedInstance] setActive:NO error:nil];
}


-(void) updateStatus: (NSString *)status
{
	//	The text view must be updated from the main thread or it throws an exception...
	dispatch_async( dispatch_get_main_queue(), ^{
		self.statusLabel.text = [NSString stringWithFormat: @"%@\n", status];
	});
}

-(void) stopBusyIndicator
{
    dispatch_async( dispatch_get_main_queue(), ^{
        
        [self enableOrDisableControls:YES];
        
	});
}


#pragma mark - GnMusicIDStreamEventsDelegate Methods

-(void) musicIdStreamIdentifyingStatusEvent: (GnMusicIdStreamIdentifyingStatus)status cancellableDelegate: (id <GnCancellableDelegate>)canceller
{
    NSString *statusString = nil;

    switch (status)
    {
        case kStatusIdentifyingInvalid:
            statusString = @"Error";
            break;

        case kStatusIdentifyingStarted:
			statusString = @"Identifying";
            break;

        case kStatusIdentifyingFpGenerated:
			statusString = @"Fingerprint Generated";
            break;

        case kStatusIdentifyingLocalQueryStarted:
			statusString = @"Local Query Started";
            self.lookupSourceIsLocal = 1;
            self.queryBeginTimeInterval = [[NSDate date] timeIntervalSince1970];
            break;

        case kStatusIdentifyingLocalQueryEnded:
			statusString = @"Local Query Ended";
            self.lookupSourceIsLocal = 1;
            self.queryEndTimeInterval = [[NSDate date] timeIntervalSince1970];
            break;

        case kStatusIdentifyingOnlineQueryStarted:
			statusString = @"Online Query Started";
            self.lookupSourceIsLocal = 0;
            break;

        case kStatusIdentifyingOnlineQueryEnded:
			statusString = @"Online Query Ended";
            self.queryEndTimeInterval = [[NSDate date] timeIntervalSince1970];
			break;
            
        case kStatusIdentifyingEnded:
            statusString = @"Identification Ended";
            break;
     }

	if (statusString)
	{
		/*	Don't update status unless we have something to show.	*/
		[self updateStatus: statusString];
	}
}

-(void) musicIdStreamProcessingStatusEvent: (GnMusicIdStreamProcessingStatus)status cancellableDelegate: (id <GnCancellableDelegate>)canceller
{
    switch (status)
    {
        case  kStatusProcessingInvalid:
            break;
        case   kStatusProcessingAudioNone:
            break;
        case kStatusProcessingAudioStarted:
            {
                dispatch_async(dispatch_get_main_queue(), ^
                       {
                           self.audioProcessingStarted = YES;
                           self.idNowButton.enabled = YES;
                       });
                break;
            }
        case   kStatusProcessingAudioEnded:
            break;
        case  kStatusProcessingAudioSilence:
            break;
        case  kStatusProcessingAudioNoise:
            break;
        case kStatusProcessingAudioSpeech:
            break;
        case  kStatusProcessingAudioMusic:
            break;
        case  kStatusProcessingTransitionNone:
            break;
        case  kStatusProcessingTransitionChannelChange:
            break;
        case  kStatusProcessingTransitionContentToContent:
            break;
        case kStatusProcessingErrorNoClassifier:
            break;
    }
}

-(void) statusEvent: (GnStatus) status
	percentComplete: (NSUInteger)percentComplete
	 bytesTotalSent: (NSUInteger) bytesTotalSent
 bytesTotalReceived: (NSUInteger) bytesTotalReceived
cancellableDelegate: (id <GnCancellableDelegate>) canceller
{
    NSString *statusString = nil;

    switch (status)
    {
        case kStatusUnknown:
            statusString = @"Status Unknown";
            break;

        case  kStatusBegin:
            statusString = @"Status Begin";
            break;

        case kStatusProgress:
            break;

        case  kStatusComplete:
            statusString = @"Status Complete";
            NSLog(@"FOUND MATCH - STATUS COMPLETE");
            break;

        case kStatusErrorInfo:
            statusString = @"No Match";
            [self stopBusyIndicator];
            break;

        case kStatusConnecting:
            statusString = @"Status Connecting";
            break;

        case kStatusSending:
            statusString = @"Status Sending";
            break;

        case kStatusReceiving:
            statusString = @"Status Receiving";
            break;

        case kStatusDisconnected:
            statusString = @"Status Disconnected";
            break;

        case kStatusReading:
            statusString = @"Status Reading";
            break;

        case kStatusWriting:
            statusString = @"Status Writing";
            break;
            
        case kStatusCancelled:
            statusString = @"Status Cancelled";
            break;
    }

	[self updateStatus: [NSString stringWithFormat:@"%@ [%zu%%]", statusString?statusString:@"", (unsigned long)percentComplete]];
}

-(void) musicIdStreamAlbumResult: (GnResponseAlbums*)result cancellableDelegate: (id <GnCancellableDelegate>)canceller
{
    // A RESULT WAS FOUND. processAlbumResponseAndUpdateResultsTable called
    NSLog(@"musicIdStreamAlbumResult fired.");
    [self.cancellableObjects removeObject:self.gnMusicIDStream];

    if(self.cancellableObjects.count==0)
    {
        self.cancelOperationsButton.enabled = NO;
    }

   [self stopBusyIndicator];
   [self processAlbumResponseAndUpdateResultsTable:result];
}

-(void) musicIdStreamIdentifyCompletedWithError: (NSError*)completeError
{
    NSString *statusString = [NSString stringWithFormat:@"%@ - [%zx]", [completeError localizedDescription], (long)[completeError code] ];
    
    [self.cancellableObjects removeObject:self.gnMusicIDStream];
    
    if(self.cancellableObjects.count==0)
    {
        self.cancelOperationsButton.enabled = NO;
    }
    
    [self updateStatus: statusString];
    [self stopBusyIndicator];
}

-(BOOL) cancelIdentify
{
    return NO;
}


#pragma mark - Other Methods


-(BOOL)is4InchRetina
{
    if ((![UIApplication sharedApplication].statusBarHidden && (int)[[UIScreen mainScreen] applicationFrame].size.height == 548 )
	||  ( [UIApplication sharedApplication].statusBarHidden && (int)[[UIScreen mainScreen] applicationFrame].size.height == 568))
	{
        return YES;
	}

    return NO;
}






#pragma mark -

-(BOOL)isFileFormatSupported:(NSURL*)filePath
{
	NSArray *supportedFileFormats = [NSArray arrayWithObjects:@".mp3",@".mp4",@".wav",@".m4a",@".aac",@".caf",@".aiff",nil];
	BOOL result = NO;
	NSString *fileNameWithExtension = [NSString stringWithString:[filePath description]];
	NSString *extension = [[NSString stringWithFormat:@".%@",[[fileNameWithExtension componentsSeparatedByString: @"."] lastObject]] lowercaseString];
	if ([supportedFileFormats containsObject:extension])
	{
		result = YES;
	}
	else
	{
		result = NO;
	}
	return result;
}




#pragma mark - MusicIdFileEventsDelegate Methods

-(void) musicIdFileAlbumResult: (GnResponseAlbums*)albumResult currentAlbum: (NSUInteger)currentAlbum totalAlbums: (NSUInteger)totalAlbums cancellableDelegate: (id <GnCancellableDelegate>)canceller
{
    NSLog(@"MusicIdFileEventsDelegate fired.");

    
    [self processAlbumResponseAndUpdateResultsTable:albumResult];
}



-(void) refreshResults
{
    if (self.results.count==0)
	{
		[self updateStatus: @"No Match"];
	}
    else
	{
		[self updateStatus: [NSString stringWithFormat: @"Found %lu", (unsigned long)self.results.count]];
	}

    [self loadSongDataIntoViews];
}

-(void) loadSongDataIntoViews {
    UIImage *songAlbumImage = [UIImage imageWithData: self.currentDataModel.albumImageData];
    self.songAlbumImage.image = songAlbumImage;
    
    self.songInfoLabel.text = [NSString stringWithFormat:@"%@",self.currentDataModel.trackTitle];
    
    
    [[NetworkController networkController] fetchImageForDrink:self.currentDrink withCompletionHandler:^(UIImage *drinkImage) {
        self.drinkView.imageView.image = drinkImage;
    }];
}


-(void) gatherFingerprint: (GnMusicIdFileInfo*) fileInfo
			  currentFile: (NSUInteger)currentFile
			   totalFiles: (NSUInteger) totalFiles
	  cancellableDelegate: (id <GnCancellableDelegate>) canceller
{
    NSError *error = nil;
    GnAudioFile *gnAudioFile = [[GnAudioFile alloc] initWithAudioFileURL:[NSURL URLWithString:[fileInfo identifier:&error]]];
    
    if(!error)
    {
        [fileInfo fingerprintFromSource:gnAudioFile error:&error];
        
        if(error)
        {
            NSLog(@"Fingerprint error - %@", [error localizedDescription]);
        }
    }
    else
        NSLog(@"GnAudioFile Error - %@", [error localizedDescription]);
    
}

-(void) musicIdFileComplete:(NSError*) completeError
{
    [self performSelectorOnMainThread:@selector(refreshResults) withObject:nil waitUntilDone:NO];

    // mechanism assumes app only has one GnMusicIdFile operation at a time, so it
    // can remove the GnMusicIdFile object is finds in the cancellable objects
    for(id obj in self.cancellableObjects)
    {
        if ([obj isKindOfClass:[GnMusicIdFile class]])
        {
            [self.cancellableObjects removeObject:obj];
            break;
        }
    }
    
    [self stopBusyIndicator];    
}


-(void) musicIdFileMatchResult: (GnResponseDataMatches*)matchesResult currentAlbum: (NSUInteger)currentAlbum totalAlbums: (NSUInteger)totalAlbums cancellableDelegate: (id <GnCancellableDelegate>)canceller;

{
    NSLog(@"musicIdFileMatchResult fired.");

    
    GnDataMatchEnumerator *matches = [matchesResult dataMatches];
    
    for (GnDataMatch * match in matches)
    {
        if ([match isAlbum] == YES)
        {
            GnAlbum  * album       = [match getAsAlbum];
            if(!album)
                continue;
            
            [self.albumDataMatches addObject:album];
        }
    }
    
    if(currentAlbum>=totalAlbums)
        [self processAlbumResponseAndUpdateResultsTable:self.albumDataMatches];

    [self stopBusyIndicator];
}

-(void) musicIdFileResultNotFound: (GnMusicIdFileInfo*) fileinfo
					  currentFile: (NSUInteger) currentFile
					   totalFiles: (NSUInteger) totalFiles
			  cancellableDelegate: (id <GnCancellableDelegate>) canceller
{
	[self updateStatus: @"No Match"];
}

-(void) gatherMetadata: (GnMusicIdFileInfo*) fileInfo
		   currentFile: (NSUInteger) currentFile
			totalFiles: (NSUInteger) totalFiles
   cancellableDelegate: (id <GnCancellableDelegate>) canceller
{
    NSError *error = nil;
    NSString* filePath = [fileInfo identifier:&error];
    
    if (error)
    {
        NSLog(@"Error while retrieving filename %@ ", [error localizedDescription]);
    }
    else
    {
        AVAsset *asset = [AVAsset assetWithURL:[NSURL URLWithString:filePath]];
        if (asset)
        {
            NSString * supportedMetaDataFormatStr = AVMetadataFormatID3Metadata;
            
            for (NSString * metaDataFormatStr in [asset availableMetadataFormats] ) {
                if ([metaDataFormatStr isEqualToString:AVMetadataFormatiTunesMetadata] == YES)
                {
                    supportedMetaDataFormatStr = AVMetadataFormatiTunesMetadata;
                    break;
                }
                else if ([metaDataFormatStr isEqualToString:AVMetadataFormatID3Metadata] == YES)
                {
                    supportedMetaDataFormatStr = AVMetadataFormatID3Metadata;
                    break;
                }
                
            }
            
            NSArray *metadataArray =  [asset metadataForFormat:supportedMetaDataFormatStr];
            
            NSMutableString *metadataKeys = [NSMutableString stringWithFormat:@""];
            
            for(AVMetadataItem* item in metadataArray)
            {
                // NSLog(@"AVMetadataItem Key = %@ Value = %@",item.key, item.value );
                
                if([[item commonKey] isEqualToString:@"title"])
                {
                    [fileInfo trackTitleWithValue:(NSString*) [item value] error:nil];
                    [metadataKeys appendString: (NSString*)[item value]];
                    [metadataKeys appendString:@","];
                }
                else if([[item commonKey] isEqualToString:@"albumName"])
                {
                    [fileInfo albumTitleWithValue:(NSString*) [item value] error:nil];
                    [metadataKeys appendString: (NSString*)[item value]];
                    [metadataKeys appendString:@","];
                }
                else if([[item commonKey] isEqualToString:@"artist"])
                {
                    [fileInfo trackArtistWithValue:(NSString*) [item value] error:nil];
                    [metadataKeys appendString: (NSString*)[item value]];
                    [metadataKeys appendString:@","];
                }
            }
            
        }
    }
}

-(void) musicIdFileStatusEvent: (GnMusicIdFileInfo*) fileinfo
						status: (GnMusicIdFileCallbackStatus) status
				   currentFile: (NSUInteger) currentFile
					totalFiles: (NSUInteger) totalFiles
		   cancellableDelegate: (id <GnCancellableDelegate>) canceller
{
    NSString *statusString = nil;

    switch (status)
    {
        case kMusicIdFileCallbackStatusProcessingBegin:
            statusString = @"Processing Begin";
            break;
        case kMusicIdFileCallbackStatusFileInfoQuery:
            statusString = @"File Info Query";
            break;
        case kMusicIdFileCallbackStatusProcessingComplete:
            statusString = @"Processing Complete";
            break;
        case kMusicIdFileCallbackStatusProcessingError:
            statusString = @"Processing Error";
            break;
        case kMusicIdFileCallbackStatusError:
            statusString = @"Error";
            break;
    }

	[self updateStatus: statusString];
}


#pragma mark - NavigationBar delegate methods

- (UIBarPosition)positionForBar:(id <UIBarPositioning>)bar
{
    return UIBarPositionTopAttached;
}


#pragma mark - Application Notifications

-(void) applicationResignedActive:(NSNotification*) notification
{
    // to ensure no pending identifications deliver results while your app is
    // not active it is good practice to call cancel
    // it is safe to call identifyCancel if no identify is pending
    [self.gnMusicIDStream identifyCancel:NULL];
    
    // stopping audio processing while the app is inactive to release the
    // microphone for other apps to use
    [self stopRecording];
    dispatch_sync(self.internalQueue, ^
        {
              self.recordingIsPaused = YES;
                    
    });
}

-(void) applicationDidBecomeActive:(NSNotification*) notification
{
   if(self.recordingIsPaused)
   {
       self.recordingIsPaused = NO;
       __block NSError *musicIDStreamError = nil;
       
       [[AVAudioSession sharedInstance] setActive:YES error:nil];
       dispatch_async(self.internalQueue, ^
       {
           [self.gnMusicIDStream audioProcessStartWithAudioSource:(id <GnAudioSourceDelegate>)self.gnAudioVisualizeAdapter error:&musicIDStreamError];
                          
           if (musicIDStreamError)
           {
                dispatch_async(dispatch_get_main_queue(), ^{
                                                 
                   NSLog(@"Error while starting Audio Process With AudioSource - %@", [musicIDStreamError localizedDescription]);
                });
            }
        });
   }
}


-(void) dealloc
{
    [self stopRecording];
    [self.cancellableObjects removeAllObjects];
    self.cancellableObjects = nil;

    [[NSNotificationCenter defaultCenter] removeObserver:self];
}






#pragma mark - GnLookupLocalStreamIngestEventsDelegate

-(void) statusEvent: (GnLookupLocalStreamIngestStatus)status bundleId: (NSString*)bundleId cancellableDelegate: (id <GnCancellableDelegate>)canceller
{
    NSLog(@"status = %ld", status);
}

#pragma mark - GnAudioVisualizerDelegate Methods

-(void) RMSDidUpdateByValue:(float) value
{
   if(self.visualizationIsVisible)
   {
    dispatch_async(dispatch_get_main_queue(), ^{
    
        float scale =  value + 1.0f;
        CGAffineTransform sscale = CGAffineTransformMakeScale (scale, scale);
        self.coloredRingImageView.transform  = sscale;
        
    });
   }
}

@end
