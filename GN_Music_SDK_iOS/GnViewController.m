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
#import <CoreLocation/CoreLocation.h>
#import <objc/runtime.h>

#import <GnSDKObjC/Gn.h>

#import "History.h"
#import "CoverArt.h"
#import "Metadata.h"
#import "GnAudioVisualizeAdapter.h"

#import "NetworkController.h"


#define BUTTONWIDTH 150
#define BUTTONHEIGHT 50

#define ALBUMTITLELABELTAG 7000
#define TRACKTITLELABELTAG 7001
#define ARTISTLABELTAG 7002
#define TRACKDURATIONLABELTAG 7003
#define CAPTIONLABELTAG 7004
#define TEXTFIELDTAG 7005
#define ALBUMIDACTIONSHEETTAG 7007
#define ALBUMCOVERARTIMAGETAG 7008
#define ADDITIONALMETADATAVIEWTAG 7009
#define TRACKMATCHPOSITIONLABELTAG 7010
#define ADDITIONALCONTENTVIEWTAG 7011
#define ADDITIONALCONTENTIMAGEVIEWTAG 7012
#define ADDITIONALCONTENTALBUMLABELTAG 7013
#define ADDITIONALCONTENTARTISTLABELTAG 7014
#define ADDITIONALCONTENTTEXTVIEWTAG 7015
#define ADDITIONALCONTENTTITLELABELTAG 7016

#define LABELWIDTHIPHONE  150
#define LABELWIDTHIPAD    420


#define LOCALLOOKUPOPTIONONLY               @"local-lookupoption-only"


static NSString *gnsdkLicenseFilename = @"license.txt";

@interface GnViewController ()<GnMusicIdStreamEventsDelegate, GnMusicIdFileEventsDelegate, UITabBarDelegate, CLLocationManagerDelegate, UIGestureRecognizerDelegate, GnMusicIdFileInfoEventsDelegate, GnAudioVisualizerDelegate, GnLookupLocalStreamIngestEventsDelegate>

/*GnSDK properties*/
@property (strong) GnMusicIdStream *gnMusicIDStream;
@property (strong) GnMic *gnMic;
@property (strong) GnAudioVisualizeAdapter *gnAudioVisualizeAdapter;
@property (strong) GnManager *gnManager;
@property (strong) GnUser *gnUser;
@property (strong) GnUserStore *gnUserStore;
@property (strong) GnStorageSqlite *gnStorageSqlite;
@property (strong) GnLookupLocalStream* gnLookupLocalStream;
@property (strong) CLLocation *currentLocation;
@property (strong) NSArray *history;
@property (strong) NSMutableArray *albumDataMatches;
@property (strong) GnLocale *locale;

/*Sample App properties*/
@property (strong) UISegmentedControl *searchSegmentedControl;
@property (strong) UISegmentedControl *cancelSegmentedControl;
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

@property Mode currentMode;

@property NSIndexPath *currentlySelectedIndexPath;

@end

@implementation GnViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
//    [NetworkController networkController] fetchDrinkForSong:@"Billie%20Jean" withArtist:@"Michael%20Jackson" withCompletionHandler:^(NSString *, NSData *) {
//        
//    }
    
    self.recordingIsPaused = NO;
    __block NSError * error = nil;

    self.currentMode = UnknownMode;
    self.lookupSourceIsLocal = 0;
    self.audioProcessingStarted = 0;
    self.queryBeginTimeInterval = -1;
    self.queryEndTimeInterval = -1;

    self.cancellableObjects = [NSMutableArray arrayWithCapacity:2];
    self.albumDataMatches = [NSMutableArray arrayWithCapacity:2];

    [self setupInterface];
    [self setupLocationServices];

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


-(void) setupLocationServices
{
    //Create locationManager to get latitude,longitude for search history record.
	self.locationManager = [[CLLocationManager alloc] init];
	self.locationManager.delegate = self; // Tells the location manager to send updates to this object
	self.locationManager.desiredAccuracy = kCLLocationAccuracyBest;
	self.locationManager.distanceFilter = 20.0;
	[self.locationManager startUpdatingLocation];
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

	//Add Search and Cancel Buttons.
    self.searchSegmentedControl = [[UISegmentedControl alloc] initWithItems:@[@"Search"]];

    NSInteger offset = 20;
    NSInteger width = 100;
    NSInteger height = 35;

    if (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)
    {
        offset = 100;
        width = 200;
        height = 50;
    }

    self.searchSegmentedControl.frame = CGRectMake(self.searchFieldsTableView.frame.origin.x+offset,  self.searchFieldsTableView.frame.origin.y+self.searchFieldsTableView.frame.size.height+10, width, height);
    [self.searchSegmentedControl.layer setMasksToBounds:YES];
    [self.searchSegmentedControl setMomentary:YES];
    [self.searchSegmentedControl setBackgroundColor:[UIColor redColor]];
    self.searchSegmentedControl.tintColor = [UIColor whiteColor];
    self.searchSegmentedControl.layer.cornerRadius = (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?5.0f:5.0f;
    self.searchSegmentedControl.layer.borderColor = [UIColor greenColor].CGColor;
//    [self.searchSegmentedControl addTarget:self action:@selector(doTextSearch:) forControlEvents:UIControlEventValueChanged];


    self.cancelSegmentedControl = [[UISegmentedControl alloc] initWithItems:@[@"Cancel"]];
    self.cancelSegmentedControl.frame = CGRectMake(self.searchSegmentedControl.frame.origin.x+self.searchSegmentedControl.frame.size.width+((UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?80:40),  self.searchSegmentedControl.frame.origin.y, width,height);
    [self.cancelSegmentedControl setMomentary:YES];
    [self.cancelSegmentedControl setBackgroundColor:[UIColor blackColor]];
    self.cancelSegmentedControl.tintColor = [UIColor whiteColor];
    self.cancelSegmentedControl.layer.borderColor = [UIColor greenColor].CGColor;
    self.cancelSegmentedControl.layer.cornerRadius = (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?5.0f:5.0f;


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

    self.resultsTableView.userInteractionEnabled = enable;
    self.resultsTableView.scrollEnabled = enable;

}

#pragma mark - Display Overlay View's



-(void) closeAdditionalContentView:(id) sender
{
    if(self.currentlySelectedIndexPath)
    {
        UITableViewCell *cell = [self.resultsTableView cellForRowAtIndexPath:self.currentlySelectedIndexPath];

        [UIView animateWithDuration:0.5 animations:^{
            UIView *additionalContentView = (UIView*) [cell.contentView viewWithTag:ADDITIONALCONTENTVIEWTAG];
            CGRect frame = additionalContentView.frame;
            frame.origin.x = (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad?768:320);
            additionalContentView.frame = frame;
       }];
    }
}








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


-(void) showArtistBiography:(id) sender
{
   UITableViewCell *cell = [self.resultsTableView cellForRowAtIndexPath:self.currentlySelectedIndexPath];
   __block UIView *additionalContentView = (UIView*) [cell.contentView viewWithTag:ADDITIONALCONTENTVIEWTAG];
    UIImageView *additionalContentImageView = (UIImageView*) [additionalContentView viewWithTag:ADDITIONALCONTENTIMAGEVIEWTAG];
    UILabel *additionalContentAlbumLabel = (UILabel*) [additionalContentView viewWithTag:ADDITIONALCONTENTALBUMLABELTAG];
    UILabel *additionalContentArtistLabel = (UILabel*) [additionalContentView viewWithTag:ADDITIONALCONTENTARTISTLABELTAG];
    UITextView *additionalContentTextView = (UITextView*) [additionalContentView viewWithTag:ADDITIONALCONTENTTEXTVIEWTAG];
    UILabel *additionalContentTitleLabel = (UILabel*) [additionalContentView viewWithTag:ADDITIONALCONTENTTITLELABELTAG];
    additionalContentTitleLabel.text = @"Artist Biography";

    [cell.contentView  bringSubviewToFront:additionalContentView];

    GnDataModel *datamodelObject = nil;
    
    if(self.results && self.results.count> (NSUInteger) self.currentlySelectedIndexPath.row)
        datamodelObject =  (GnDataModel *)[self.results objectAtIndex:(NSUInteger) self.currentlySelectedIndexPath.row];
    
    UIImage *artistImage = [UIImage imageWithData:datamodelObject.artistImageData];

    if(artistImage)
    {
        additionalContentImageView.image = artistImage;
    }
    else
    {
        additionalContentImageView.image = [UIImage imageNamed:@"emptyImage.png"];
    }

    additionalContentAlbumLabel.text = datamodelObject.albumTitle;
    additionalContentArtistLabel.text = datamodelObject.albumArtist?datamodelObject.albumArtist:datamodelObject.trackArtist;
    additionalContentTextView.text = datamodelObject.artistBiography?datamodelObject.artistBiography:@"Artist Biography is not currently available.";

    [UIView animateWithDuration:0.5 animations:^{

        CGRect frame = additionalContentView.frame;
        frame.origin.x = 0;
        additionalContentView.frame = frame;
    }];

}

-(void) refreshArtistImage
{
    UITableViewCell *cell = [self.resultsTableView cellForRowAtIndexPath:self.currentlySelectedIndexPath];
    __block UIView *additionalContentView = (UIView*) [cell.contentView viewWithTag:ADDITIONALCONTENTVIEWTAG];
    UIImageView *additionalContentImageView = (UIImageView*) [additionalContentView viewWithTag:ADDITIONALCONTENTIMAGEVIEWTAG];
    
    GnDataModel *datamodelObject = nil;
    
    if(self.results && self.results.count> (NSUInteger) self.currentlySelectedIndexPath.row)
    {
        datamodelObject = (GnDataModel *)[self.results objectAtIndex:(NSUInteger) self.currentlySelectedIndexPath.row];
        
        
        UIImage *artistImage = [UIImage imageWithData:datamodelObject.artistImageData];
        
        if(artistImage)
        {
            additionalContentImageView.image = artistImage;
        }
        else
        {
            additionalContentImageView.image = [UIImage imageNamed:@"emptyImage.png"];
        }
    }
}

-(void) showAlbumReview:(id) sender
{
    UITableViewCell *cell = [self.resultsTableView cellForRowAtIndexPath:self.currentlySelectedIndexPath];
    __block UIView *additionalContentView = (UIView*) [cell.contentView viewWithTag:ADDITIONALCONTENTVIEWTAG];
    UIImageView *additionalContentImageView = (UIImageView*) [additionalContentView viewWithTag:ADDITIONALCONTENTIMAGEVIEWTAG];
    UILabel *additionalContentAlbumLabel = (UILabel*) [additionalContentView viewWithTag:ADDITIONALCONTENTALBUMLABELTAG];
    UILabel *additionalContentArtistLabel = (UILabel*) [additionalContentView viewWithTag:ADDITIONALCONTENTARTISTLABELTAG];
    UITextView *additionalContentTextView = (UITextView*) [additionalContentView viewWithTag:ADDITIONALCONTENTTEXTVIEWTAG];
    UILabel *additionalContentTitleLabel = (UILabel*) [additionalContentView viewWithTag:ADDITIONALCONTENTTITLELABELTAG];
    additionalContentTitleLabel.text = @"Album Review";

    [cell.contentView  bringSubviewToFront:additionalContentView];

    GnDataModel *datamodelObject =  nil;
    
    if(self.results && self.results.count> (NSUInteger) self.currentlySelectedIndexPath.row)
        datamodelObject = (GnDataModel *)[self.results objectAtIndex:(NSUInteger) self.currentlySelectedIndexPath.row];
    
    UIImage *artistImage = [UIImage imageWithData:datamodelObject.albumImageData];

    if(artistImage)
    {
        additionalContentImageView.image = artistImage;
    }
    else
    {
        additionalContentImageView.image = [UIImage imageNamed:@"emptyImage.png"];
    }

    additionalContentAlbumLabel.text = datamodelObject.albumTitle;
    additionalContentArtistLabel.text = datamodelObject.albumArtist?datamodelObject.albumArtist:datamodelObject.trackArtist;
    additionalContentTextView.text = datamodelObject.albumReview?datamodelObject.albumReview:@"Album Review is not currently available.";

    [UIView animateWithDuration:0.5 animations:^{

        CGRect frame = additionalContentView.frame;
        frame.origin.x = 0;
        additionalContentView.frame = frame;
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
        [self.busyIndicator startAnimating];

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
    
    [self stopBusyIndicator];
}


#pragma mark - Do Operation Methods






- (NSString*)stringWithPercentEscape:(NSString*) refStr
{
    return (NSString *) CFBridgingRelease(CFURLCreateStringByAddingPercentEscapes(NULL, (CFStringRef)refStr, NULL, CFSTR("￼=,!$&'()*+;@?\n\"<>#\t :/"),kCFStringEncodingUTF8));
}

#pragma mark - Process Album Response

-(void) processAlbumResponseAndUpdateResultsTable:(id) responseAlbums
{
    id albums = nil;
    
    if([responseAlbums isKindOfClass:[GnResponseAlbums class]])
        albums = [responseAlbums albums];
    else
        albums = responseAlbums;
    
    for(GnAlbum* album in albums)
    {
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
        
        /* Get CoverArt */
        GnContent *coverArtContent = [album coverArt];
        GnAsset *coverArtAsset = [coverArtContent asset:kImageSizeSmall];
        NSString *URLString = [NSString stringWithFormat:@"http://%@", [coverArtAsset url]];

        GnContent *artistImageContent = [[[album artist] contributor] image];
        GnAsset *artistImageAsset = [artistImageContent asset:kImageSizeSmall];
        NSString *artistImageURLString = [NSString stringWithFormat:@"http://%@", [artistImageAsset url]];

        GnContent *artistBiographyContent = [[[album artist] contributor] biography];
        NSString *artistBiographyURLString = [NSString stringWithFormat:@"http://%@", [[[artistBiographyContent assets] nextObject] url]];

        GnContent *albumReviewContent = [album review];
        NSString *albumReviewURLString = [NSString stringWithFormat:@"http://%@", [[[albumReviewContent assets] nextObject] url]];

        __block GnDataModel *gnDataModelObject = [[GnDataModel alloc] init];
        gnDataModelObject.albumArtist = albumArtist;
        gnDataModelObject.albumGenre = albumGenre;
        gnDataModelObject.albumID = albumID;
        gnDataModelObject.albumXID = albumXID;
        gnDataModelObject.albumYear = albumYear;
        gnDataModelObject.albumTitle = albumTitle;
        gnDataModelObject.albumTrackCount = albumTrackCount;
        gnDataModelObject.albumLanguage = albumLanguage;

        NSURLRequest* request = [NSURLRequest requestWithURL:[NSURL URLWithString:URLString]];
        __weak GnViewController *weakSelf = self;
        [NSURLConnection sendAsynchronousRequest:request queue:[NSOperationQueue mainQueue] completionHandler: ^(NSURLResponse *response, NSData* data, NSError* error)
        {

            if(data && !error)
            {
               gnDataModelObject.albumImageData = data;
               [weakSelf.resultsTableView reloadData];
            }
        }];

        NSURLRequest *artistImageFetchRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:artistImageURLString]];
        [NSURLConnection sendAsynchronousRequest:artistImageFetchRequest queue:[NSOperationQueue mainQueue] completionHandler: ^(NSURLResponse *response, NSData* data, NSError* error){

            if(data && !error)
            {
                gnDataModelObject.artistImageData = data;
                [weakSelf.resultsTableView reloadData];
                [self refreshArtistImage];
            }
        }];

        NSURLRequest *artistBiographyFetchRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:artistBiographyURLString]];
        [NSURLConnection sendAsynchronousRequest:artistBiographyFetchRequest queue:[NSOperationQueue mainQueue] completionHandler: ^(NSURLResponse *response, NSData* data, NSError* error){

            if(data && !error)
            {
                gnDataModelObject.artistBiography = [[NSString alloc] initWithBytes:data.bytes length:data.length encoding:NSUTF8StringEncoding];

            }
        }];

        NSURLRequest *albumReviewFetchRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:albumReviewURLString]];
        [NSURLConnection sendAsynchronousRequest:albumReviewFetchRequest queue:[NSOperationQueue mainQueue] completionHandler: ^(NSURLResponse *response, NSData* data, NSError* error){

            if(data && !error)
            {
                gnDataModelObject.albumReview = [[NSString alloc] initWithBytes:data.bytes length:data.length encoding:NSUTF8StringEncoding];

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
            NSString *currentPosition = [NSString stringWithFormat:@"%u", (NSUInteger) [track currentPosition]/1000];
            NSString *matchPosition = [NSString stringWithFormat:@"%u", (NSUInteger) [track matchPosition]/1000];


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
        
    }

    [self  performSelectorOnMainThread:@selector(refreshResults) withObject:nil waitUntilDone:NO];
    [self stopBusyIndicator];
    
    if ([[albums allObjects] count])
    {
        [self performSelectorOnMainThread:@selector(saveResultsForHistory:) withObject:responseAlbums waitUntilDone:YES];
    }
    
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
        [self.busyIndicator stopAnimating];
        
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



#pragma mark - Table View Data Source Methods

- (NSInteger) tableView:(UITableView *) tableView numberOfRowsInSection:(NSInteger)section
{
	if (tableView==self.resultsTableView)
	{
		if (self.results.count==0)
			tableView.separatorColor = [UIColor clearColor];
		else
			tableView.separatorColor = [UIColor lightGrayColor];

		return (NSInteger)self.results.count;
	}

    return 0;
}

// Row display. Implementers should *always* try to reuse cells by setting each cell's reuseIdentifier and querying for available reusable cells with dequeueReusableCellWithIdentifier:
// Cell gets various attributes set automatically based on table (separators) and data source (accessory views, editing controls)

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellIdentifier = @"CellID";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];

    if (!cell)
    {
		cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellIdentifier];

		if (tableView==self.resultsTableView)
		{
			UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(5, 10,  (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?112:56, (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?112:56)];
			imageView.tag = ALBUMCOVERARTIMAGETAG;

			UILabel *albumTitleLabel = [[UILabel alloc] initWithFrame:CGRectMake(imageView.frame.origin.x+imageView.frame.size.width+12, 5, (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?LABELWIDTHIPAD:LABELWIDTHIPHONE, 40)];
			albumTitleLabel.font = [UIFont boldSystemFontOfSize:16];
			albumTitleLabel.textColor = [UIColor colorWithRed:0.7f green:0 blue:0.7f alpha:1];
			albumTitleLabel.tag=ALBUMTITLELABELTAG;
			albumTitleLabel.numberOfLines = 2;
			albumTitleLabel.lineBreakMode = NSLineBreakByCharWrapping;
			[albumTitleLabel setBackgroundColor:[UIColor clearColor]];

			UILabel *trackTitleLabel = [[UILabel alloc] initWithFrame:CGRectMake(albumTitleLabel.frame.origin.x+albumTitleLabel.frame.size.width+5, albumTitleLabel.frame.origin.y, 120, albumTitleLabel.frame.size.height)];
			trackTitleLabel.font = [UIFont systemFontOfSize:12];
			trackTitleLabel.textColor = [UIColor darkGrayColor];
			trackTitleLabel.tag = TRACKTITLELABELTAG;
			trackTitleLabel.numberOfLines = 2;
			trackTitleLabel.lineBreakMode = NSLineBreakByCharWrapping;
			[trackTitleLabel setBackgroundColor:[UIColor clearColor]];

			UILabel *artistLabel = [[UILabel alloc] initWithFrame:CGRectMake(albumTitleLabel.frame.origin.x, albumTitleLabel.frame.origin.y+albumTitleLabel.frame.size.height, (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?LABELWIDTHIPAD:LABELWIDTHIPHONE, 40)];
			artistLabel.font = [UIFont systemFontOfSize:12];
			artistLabel.textColor = [UIColor darkGrayColor];
			artistLabel.tag = ARTISTLABELTAG;
			artistLabel.numberOfLines = 2;
			artistLabel.lineBreakMode = NSLineBreakByCharWrapping;
			[artistLabel setBackgroundColor:[UIColor clearColor]];

			UILabel *trackDurationLabel = [[UILabel alloc] initWithFrame:CGRectMake(artistLabel.frame.origin.x+artistLabel.frame.size.width+5, artistLabel.frame.origin.y, 120, 40)];
			trackDurationLabel.font = [UIFont boldSystemFontOfSize:10];
			trackDurationLabel.textColor = [UIColor grayColor];
			trackDurationLabel.tag = TRACKDURATIONLABELTAG;
			[trackDurationLabel setBackgroundColor:[UIColor clearColor]];

			UIView *additionalContentView = [[UIView alloc] initWithFrame:CGRectMake((UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?768:320, 0, (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?768:cell.contentView.bounds.size.width,  (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?kHeightOfAdditionalMetadataCellPad:kHeightOfAdditionalMetadataCell)];

			additionalContentView.tag = ADDITIONALCONTENTVIEWTAG;
			additionalContentView.backgroundColor = [UIColor blueColor];

			//Add content views to additional content view
			UILabel *additionalContentTitleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, additionalContentView.frame.size.width, 20)];
			additionalContentTitleLabel.tag = ADDITIONALCONTENTTITLELABELTAG;
			additionalContentTitleLabel.layer.cornerRadius = 2.0f;
			additionalContentTitleLabel.backgroundColor = [UIColor darkGrayColor];
			additionalContentTitleLabel.textColor = [UIColor blackColor];
			additionalContentTitleLabel.textAlignment = NSTextAlignmentCenter;
			additionalContentTitleLabel.font = [UIFont boldSystemFontOfSize:16.0f];
			[additionalContentTitleLabel setUserInteractionEnabled:YES];

			UISegmentedControl *closeAdditionalSegment = [[UISegmentedControl alloc] initWithItems:[NSArray arrayWithObject:@"Close"]];
			[closeAdditionalSegment setMomentary:YES];
			[closeAdditionalSegment setUserInteractionEnabled:YES];
			[closeAdditionalSegment setFrame:CGRectMake(additionalContentTitleLabel.frame.origin.x+additionalContentTitleLabel.frame.size.width-50, 0, 50, 20)];
			[closeAdditionalSegment setBackgroundColor:[UIColor redColor]];
			closeAdditionalSegment.tintColor = [UIColor whiteColor];
			[closeAdditionalSegment addTarget:self action:@selector(closeAdditionalContentView:) forControlEvents:UIControlEventValueChanged];
			[additionalContentTitleLabel addSubview:closeAdditionalSegment];

			UIImageView *additionalContentImageView = [[UIImageView alloc] initWithFrame:CGRectMake(5, 25, 56, 56)];
			additionalContentImageView.tag = ADDITIONALCONTENTIMAGEVIEWTAG;


			UILabel *additionalContentAlbumLabel = [[UILabel alloc] initWithFrame:CGRectMake(additionalContentImageView.frame.origin.x+additionalContentImageView.frame.size.width+5, additionalContentImageView.frame.origin.y, additionalContentView.bounds.size.width-(additionalContentImageView.frame.size.width+additionalContentImageView.frame.origin.x+5), 40)];
			additionalContentAlbumLabel.numberOfLines = 2;
			additionalContentAlbumLabel.lineBreakMode = NSLineBreakByCharWrapping;
			[additionalContentAlbumLabel setBackgroundColor:[UIColor clearColor]];
			additionalContentAlbumLabel.tag = ADDITIONALCONTENTALBUMLABELTAG;
			additionalContentAlbumLabel.textColor = [UIColor whiteColor];
			additionalContentAlbumLabel.backgroundColor = [UIColor clearColor];

			UILabel *additionalContentArtistLabel = [[UILabel alloc] initWithFrame:CGRectMake(additionalContentAlbumLabel.frame.origin.x, additionalContentAlbumLabel.frame.origin.y+additionalContentAlbumLabel.frame.size.height+5, additionalContentView.bounds.size.width-(additionalContentImageView.frame.size.width+additionalContentImageView.frame.origin.x+5), 40)];
			additionalContentArtistLabel.numberOfLines = 2;
			additionalContentArtistLabel.lineBreakMode = NSLineBreakByCharWrapping;
			[additionalContentArtistLabel setBackgroundColor:[UIColor clearColor]];
			additionalContentArtistLabel.tag = ADDITIONALCONTENTARTISTLABELTAG;
			additionalContentArtistLabel.textColor = [UIColor whiteColor];

			UITextView *additionalContentTextView = [[UITextView alloc] initWithFrame:CGRectMake(additionalContentImageView.frame.origin.x, additionalContentArtistLabel.frame.origin.y+additionalContentArtistLabel.frame.size.height+5, additionalContentView.bounds.size.width-10, additionalContentView.bounds.size.height - (additionalContentArtistLabel.frame.origin.y+additionalContentArtistLabel.frame.size.height + 10) )];
			[additionalContentTextView setEditable:NO];
			additionalContentTextView.text = nil;
			additionalContentTextView.tag = ADDITIONALCONTENTTEXTVIEWTAG;
			additionalContentTextView.layer.cornerRadius = 5.0f;
			additionalContentTextView.font = [UIFont italicSystemFontOfSize:14.0f];
			additionalContentTextView.textColor = [UIColor darkGrayColor];
			additionalContentTextView.showsVerticalScrollIndicator = YES;

			[additionalContentView addSubview:additionalContentTitleLabel];
			[additionalContentView addSubview:additionalContentImageView];
			[additionalContentView addSubview:additionalContentAlbumLabel];
			[additionalContentView addSubview:additionalContentArtistLabel];
			[additionalContentView addSubview:additionalContentTextView];
			//

			[cell.contentView addSubview:imageView];
			[cell.contentView addSubview:albumTitleLabel];
			[cell.contentView addSubview:trackTitleLabel];
			[cell.contentView addSubview:artistLabel];
			[cell.contentView addSubview:trackDurationLabel];
			[cell.contentView addSubview:additionalContentView];
		}
    }

	if (tableView==self.resultsTableView)
	{
		GnDataModel *datamodelObject = nil;
        
        if(self.results && [self.results count])
        datamodelObject = [self.results objectAtIndex:(NSUInteger) indexPath.row];

		UILabel *albumTitleLabel = (UILabel *)[cell.contentView viewWithTag:ALBUMTITLELABELTAG];
		UILabel *trackTitleLabel = (UILabel *)[cell.contentView viewWithTag:TRACKTITLELABELTAG];
		UILabel *artistLabel = (UILabel *)[cell.contentView viewWithTag:ARTISTLABELTAG];
		UILabel *trackDurationLabel = (UILabel *)[cell.contentView viewWithTag:TRACKDURATIONLABELTAG];

		UIImageView *imageView = (UIImageView*) [cell.contentView viewWithTag:ALBUMCOVERARTIMAGETAG];

		albumTitleLabel.text = datamodelObject.albumTitle;
		trackTitleLabel.text = datamodelObject.trackTitle;
		artistLabel.text = datamodelObject.albumArtist?datamodelObject.albumArtist:datamodelObject.trackArtist;
        NSString *durationText = datamodelObject.trackDuration;
        if ( (nil == durationText) || ([durationText isEqualToString:@"0"]))
            trackDurationLabel.text = @"";
        else
            trackDurationLabel.text = [NSString stringWithFormat:@"Duration: %@s", durationText];

		if (datamodelObject.albumImageData)
		{
			imageView.image = [UIImage imageWithData:datamodelObject.albumImageData];
		}
		else
		{
			imageView.image = [UIImage imageNamed:@"emptyImage.png"];
		}

		if(self.currentlySelectedIndexPath && indexPath.row!=self.currentlySelectedIndexPath.row )
		{
			[[cell.contentView viewWithTag:ADDITIONALMETADATAVIEWTAG] removeFromSuperview];

			[albumTitleLabel setEnabled:NO];
			[trackTitleLabel setEnabled:NO];
			[artistLabel setEnabled:NO];
			[trackDurationLabel setEnabled:NO];
			[imageView setAlpha:0.5];
		}
		else
		{
            if(!self.currentlySelectedIndexPath)
			{
				[cell.contentView setBackgroundColor: [UIColor whiteColor]];
				[[cell.contentView viewWithTag:ADDITIONALMETADATAVIEWTAG] removeFromSuperview];
			}
			else if(self.currentlySelectedIndexPath && indexPath.row==self.currentlySelectedIndexPath.row)
			{
				[cell.contentView setBackgroundColor: [UIColor whiteColor]];

				//Add Playback and sharing view.
				UIView *additionalMetadataView = [cell.contentView viewWithTag:ADDITIONALMETADATAVIEWTAG];

				if(!additionalMetadataView)
				{
					additionalMetadataView = [[UIView alloc] initWithFrame:CGRectMake(cell.contentView.frame.origin.x, imageView.frame.origin.y+imageView.frame.size.height+12, cell.contentView.frame.size.width-10, kHeightOfAdditionalMetadataCell - (artistLabel.frame.origin.y+artistLabel.frame.size.height+12) )];


					additionalMetadataView.tag = ADDITIONALMETADATAVIEWTAG;

					//Add additional metadata labels.
					UILabel *trackMatchPositionLabel = [[UILabel alloc] initWithFrame:CGRectMake(5, 5, (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?LABELWIDTHIPAD:LABELWIDTHIPHONE, 20)];
					trackMatchPositionLabel.text = [NSString stringWithFormat:@"Match Pos: %@s", datamodelObject.trackMatchPosition];
					trackMatchPositionLabel.font = [UIFont systemFontOfSize:12];


					UILabel *lookupSourceLabel = [[UILabel alloc] initWithFrame:CGRectMake(trackMatchPositionLabel.frame.origin.x+trackMatchPositionLabel.frame.size.width+5, trackMatchPositionLabel.frame.origin.y, (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?LABELWIDTHIPAD:LABELWIDTHIPHONE, trackMatchPositionLabel.frame.size.height)];
					lookupSourceLabel.text = [NSString stringWithFormat:@"Lookup Source:%@", self.lookupSourceIsLocal?@"Local":@"Online"];
					lookupSourceLabel.font = [UIFont systemFontOfSize:12];


					UILabel *currentPositionLabel = [[UILabel alloc] initWithFrame:CGRectMake(trackMatchPositionLabel.frame.origin.x, trackMatchPositionLabel.frame.origin.y+trackMatchPositionLabel.frame.size.height+5, (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?LABELWIDTHIPAD:LABELWIDTHIPHONE, 20)];
					currentPositionLabel.text = [NSString stringWithFormat:@"Current Pos:%@s", datamodelObject.currentPosition ];
					currentPositionLabel.font = [UIFont systemFontOfSize:12];


					UILabel *genreLabel = [[UILabel alloc] initWithFrame:CGRectMake(currentPositionLabel.frame.origin.x+currentPositionLabel.frame.size.width+5, currentPositionLabel.frame.origin.y, (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?LABELWIDTHIPAD:LABELWIDTHIPHONE, 20)];
					genreLabel.text = [NSString stringWithFormat:@"Genre: %@", (datamodelObject.trackGenre && datamodelObject.trackGenre.length>0)?datamodelObject.trackGenre:datamodelObject.albumGenre];
					genreLabel.font = [UIFont systemFontOfSize:12];

					NSTimeInterval diff = self.queryEndTimeInterval - self.queryBeginTimeInterval;

					UILabel* timeToMatchLabel = [[UILabel alloc] initWithFrame:CGRectMake(currentPositionLabel.frame.origin.x, currentPositionLabel.frame.origin.y+currentPositionLabel.frame.size.height+5, (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?LABELWIDTHIPAD:LABELWIDTHIPHONE, 20)];
					timeToMatchLabel.text = [NSString stringWithFormat:@"Time to Match(ms): %0.4lf",  diff];
					timeToMatchLabel.font = [UIFont systemFontOfSize:12];

					UILabel *tempoLabel = [[UILabel alloc] initWithFrame:CGRectMake(timeToMatchLabel.frame.origin.x, timeToMatchLabel.frame.origin.y+timeToMatchLabel.frame.size.height+5, (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?LABELWIDTHIPAD:LABELWIDTHIPHONE, 20)];
					tempoLabel.text = [NSString stringWithFormat:@"Tempo: %@", datamodelObject.trackTempo];
					tempoLabel.font = [UIFont systemFontOfSize:12];

					UILabel *originLabel = [[UILabel alloc] initWithFrame:CGRectMake(timeToMatchLabel.frame.origin.x+timeToMatchLabel.frame.size.width+5, timeToMatchLabel.frame.origin.y+timeToMatchLabel.frame.size.height+5, (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?LABELWIDTHIPAD:LABELWIDTHIPHONE, 40)];
					originLabel.text = [NSString stringWithFormat:@"Origin: %@", (datamodelObject.trackOrigin)];
					originLabel.font = [UIFont systemFontOfSize:12];

					UILabel *moodLabel = [[UILabel alloc] initWithFrame:CGRectMake(tempoLabel.frame.origin.x, tempoLabel.frame.origin.y+tempoLabel.frame.size.height+5, (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?LABELWIDTHIPAD:LABELWIDTHIPHONE, 20)];
					moodLabel.text = [NSString stringWithFormat:@"Mood: %@", datamodelObject.trackMood];
					moodLabel.font = [UIFont systemFontOfSize:12];

					UISegmentedControl *artistBiographyControl = [[UISegmentedControl alloc] initWithItems:[NSArray arrayWithObject:@"Artist Biography"]];
					artistBiographyControl.frame = CGRectMake(moodLabel.frame.origin.x+(UI_USER_INTERFACE_IDIOM()?100:0),  moodLabel.frame.origin.y+moodLabel.frame.size.height+10, 150, 30);
					[artistBiographyControl.layer setMasksToBounds:YES];
					[artistBiographyControl setMomentary:YES];
					[artistBiographyControl setBackgroundColor:[UIColor darkGrayColor]];
					artistBiographyControl.tintColor = [UIColor whiteColor];
					artistBiographyControl.layer.cornerRadius = (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?5.0f:5.0f;
					artistBiographyControl.layer.borderColor = [UIColor greenColor].CGColor;
					[artistBiographyControl addTarget:self action:@selector(showArtistBiography:) forControlEvents:UIControlEventValueChanged];


					UISegmentedControl *albumReviewControl = [[UISegmentedControl alloc] initWithItems:[NSArray arrayWithObject:@"Album Review"]];
					albumReviewControl.frame = CGRectMake(artistBiographyControl.frame.origin.x+artistBiographyControl.frame.size.width+(UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad?150:10),  artistBiographyControl.frame.origin.y, 150, 30);
					[albumReviewControl.layer setMasksToBounds:YES];
					[albumReviewControl setMomentary:YES];
					[albumReviewControl setBackgroundColor:[UIColor darkGrayColor]];
					albumReviewControl.tintColor = [UIColor whiteColor];
					albumReviewControl.layer.cornerRadius = (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?5.0f:5.0f;
					albumReviewControl.layer.borderColor = [UIColor greenColor].CGColor;
					[albumReviewControl addTarget:self action:@selector(showAlbumReview:) forControlEvents:UIControlEventValueChanged];

					[additionalMetadataView addSubview:trackMatchPositionLabel];
					[additionalMetadataView addSubview:lookupSourceLabel];
					[additionalMetadataView addSubview:currentPositionLabel];
					[additionalMetadataView addSubview:genreLabel];
					[additionalMetadataView addSubview:timeToMatchLabel];
					[additionalMetadataView addSubview:tempoLabel];
					[additionalMetadataView addSubview:originLabel];
					[additionalMetadataView addSubview:moodLabel];
					[additionalMetadataView addSubview:artistBiographyControl];
					[additionalMetadataView addSubview:albumReviewControl];

					[cell.contentView addSubview:additionalMetadataView];
				}
			}

			[albumTitleLabel setEnabled:YES];
			[trackTitleLabel setEnabled:YES];
			[artistLabel setEnabled:YES];
			[trackDurationLabel setEnabled:YES];
			[imageView setAlpha:1.0f];
		}

		[cell setSelectionStyle:UITableViewCellSelectionStyleGray];
	}
	
	

    return cell;
}


- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
	if(tableView==self.resultsTableView && self.currentlySelectedIndexPath!=nil && self.currentlySelectedIndexPath.row==indexPath.row)
		return (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad)?kHeightOfAdditionalMetadataCellPad : kHeightOfAdditionalMetadataCell;

	if (tableView==self.resultsTableView)
		return 100.0f;

    return 45;
}


//- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
//{
//
//    else
//    {
//        UITableViewCell* cell = [tableView cellForRowAtIndexPath:self.currentlySelectedIndexPath];
//
//        self.currentlySelectedIndexPath = nil;
//
//		UIView *additionalContentView = (UIView*) [cell.contentView viewWithTag:ADDITIONALCONTENTVIEWTAG];
//        CGRect frame = additionalContentView.frame;
//        frame.origin.x = (UI_USER_INTERFACE_IDIOM()==UIUserInterfaceIdiomPad?768:320);
//        additionalContentView.frame = frame;
//    }
//
//    [tableView deselectRowAtIndexPath:indexPath animated:NO];
//    [tableView beginUpdates];
//    [tableView reloadData];
//
//    [self performSelector:@selector(scrollPlaybackViewToVisibleRect) withObject:nil afterDelay:0.1 ];
//    [tableView endUpdates];
//}

#pragma mark - Table View Positioning

-(void) scrollPlaybackViewToVisibleRect
{
    UITableViewCell *cell = [self.resultsTableView cellForRowAtIndexPath:self.currentlySelectedIndexPath];

    [self.resultsTableView scrollRectToVisible:cell.frame animated:YES];
}

#pragma mark - Action Methods







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

    [self.resultsTableView reloadData];
    [self.busyIndicator stopAnimating];
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





#pragma mark - Save History

    // Save the user's MusicID-Stream query incase of search by
    // RecognizeFromMic or RecognizeFromPCM or Fingerprint

-(void)saveResultsForHistory:(GnResponseAlbums*) responseAlbums
{
    NSError *error = nil;
	NSDate *currentDate = [NSDate date];
	NSDateFormatter *gmtDF = [[NSDateFormatter alloc] init];
	[gmtDF setDateFormat:@"yyyy-MM-dd HH:mm:ss zzz"];
	[gmtDF setTimeZone:[NSTimeZone timeZoneWithName:@"GMT"]];
	NSString *dateStr = [gmtDF stringFromDate:currentDate];
	NSDate *gmtDate = [gmtDF dateFromString:dateStr];

    NSManagedObjectContext *context = [GnAppDelegate sharedContext];
    
    for(GnAlbum* album in [responseAlbums albums])
    {
        Metadata *metadata = [NSEntityDescription insertNewObjectForEntityForName:@"Metadata" inManagedObjectContext:context];
        CoverArt *coverArt = [NSEntityDescription insertNewObjectForEntityForName:@"CoverArt" inManagedObjectContext:context];
        History *history = [NSEntityDescription insertNewObjectForEntityForName:@"History" inManagedObjectContext:context];

        //Setup metadata entity attributes.
        metadata.albumTitle = [[album title] display];
        metadata.albumTitleYomi = [[album title] display];
        metadata.artist = [[[album artist] name] display];
        metadata.artistYomi = [[[album artist] name] display];
        metadata.artistBetsumei = [[[album artist] name] display];
        metadata.trackTitle = [[[album trackMatched:1] title] display];
        metadata.trackTitleYomi = [[[album trackMatched:1] title] display];
        metadata.albumId = [album gnUId];
        metadata.albumTrackCount = [NSNumber numberWithUnsignedInteger:[album trackCount]];
        metadata.trackNumber = [NSNumber numberWithUnsignedInteger:[album trackMatchNumber:1]];

        GnContent *coverArtContent = [album content:kContentTypeImageCover];
        GnAsset *coverArtAsset = [coverArtContent asset:kImageSizeSmall];
        NSString *URLString = [NSString stringWithFormat:@"http://%@", [coverArtAsset url]];
    
        NSURLRequest* request = [NSURLRequest requestWithURL:[NSURL URLWithString:URLString]];
        [NSURLConnection sendAsynchronousRequest:request queue:[NSOperationQueue mainQueue] completionHandler: ^(NSURLResponse *response, NSData* data, NSError* error)
         {
         
             if(data && !error)
             {
                 coverArt.size = @"170";
                 coverArt.mimeType = [URLString lastPathComponent];
                 coverArt.data = data;
             
             }
         }];


        //Setup history entity attributes
        history.auto_id = [NSNumber numberWithUnsignedInt:[GnAppDelegate getLastInsertedAuto_id]];
        history.current_date = gmtDate;

        CLLocation *location = [self currentLocation];

        if (location)
        {
            history.latitude = [NSNumber numberWithDouble:location.coordinate.latitude];
            history.longitude = [NSNumber numberWithDouble:location.coordinate.longitude];
        }
        else
        {
            history.latitude = [NSNumber numberWithDouble:0.0];
            history.longitude = [NSNumber numberWithDouble:0.0];
        }

        // Setup relationships
        metadata.coverArt = coverArt;
        coverArt.metaData = metadata;
        history.metadata = metadata;
    
        // Save new record.
        unsigned int newId = (unsigned int)[GnAppDelegate getLastInsertedAuto_id]+1;

        [GnAppDelegate setLastInsertedAuto_id:newId];
        [context save:&error];

        if (error)
        {
            NSLog(@"Could not save the data after inserting.");
        }

        error = nil;
        NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
        NSEntityDescription *entity = [NSEntityDescription
								   entityForName:@"History" inManagedObjectContext:context];
        [fetchRequest setEntity:entity];

        NSSortDescriptor *dateSortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"current_date" ascending:YES];

        [fetchRequest setSortDescriptors:[NSArray arrayWithObject:dateSortDescriptor]];

        NSArray *fetchedObjects = [context executeFetchRequest:fetchRequest error:&error];

        if (error)
        {
            NSLog(@"Could not fetch objects to delete");
        }

        error = nil;

        if (fetchedObjects.count>CAP_LIMIT)
        {
            NSUInteger noOfExtraObjects = fetchedObjects.count - CAP_LIMIT;
            for(NSUInteger i = 0;i<noOfExtraObjects;i++)
            {
                History *historyToDelete = (History *)[fetchedObjects objectAtIndex:i];
                [context deleteObject:historyToDelete];
            }

            [context save:&error];

            if (error)
            {
                NSLog(@"Could not save the context after deleting.");
            }
        }
    }
}

#pragma mark  - CLLocationManagerDelegate Mrethods

- (void)locationManager: (CLLocationManager *)manager
	didUpdateToLocation: (CLLocation *)newLocation
		   fromLocation: (CLLocation *)oldLocation
{
	NSComparisonResult result = [newLocation.timestamp compare:[NSDate dateWithTimeIntervalSinceNow:-10]];
	if (result == NSOrderedDescending || result == NSOrderedSame)
	{
		[manager stopUpdatingLocation];
		@synchronized(self)
		{
			self.currentLocation = [[CLLocation alloc] initWithLatitude: newLocation.coordinate.latitude
															  longitude:newLocation.coordinate.longitude];
		}
	}
}

- (void)locationManager: (CLLocationManager *)manager
	   didFailWithError: (NSError *)error
{
	[manager stopUpdatingLocation];
	@synchronized(self)
	{

            //Failed to get correct location, store that entry as(0,0) and at the time of display show proper message
		self.currentLocation = [[CLLocation alloc] initWithLatitude:0.0 longitude:0.0];
	}
	switch([error code])
	{
		case kCLErrorNetwork: // general, network-related error
		{
			UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error" message:@"Please check your network connection or that you are not in airplane mode" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
			[alert show];

			break;
		}
		case kCLErrorDenied:
		{

			break;
		}
		case kCLErrorLocationUnknown:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}




#pragma mark - GnLookupLocalStreamIngestEventsDelegate

-(void) statusEvent: (GnLookupLocalStreamIngestStatus)status bundleId: (NSString*)bundleId cancellableDelegate: (id <GnCancellableDelegate>)canceller
{
    NSLog(@"status = %d", status);
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
