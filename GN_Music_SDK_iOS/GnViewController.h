//
//  GnViewController.h
//  GN_Music_SDK_iOS
//
//  Copyright (c) 2013 Gracenote. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <CoreLocation/CoreLocation.h>

typedef NS_ENUM(NSInteger, Mode)
{
    UnknownMode=-1,
    AdditionalContentMode
};

#define kHeightOfAdditionalMetadataCell 260
#define kHeightOfAdditionalMetadataCellPad 360

#define kHeightOfRowPad 140
#define kHeightOfRow 100

@interface GnViewController : UIViewController<CLLocationManagerDelegate, UITableViewDataSource, UITableViewDelegate, UINavigationBarDelegate, UIActionSheetDelegate>

@property (strong, nonatomic) CLLocationManager *locationManager;
@property (strong, nonatomic) IBOutlet UILabel *statusLabel;
@property (strong, nonatomic) IBOutlet UIView *lineView;
@property (weak, nonatomic) IBOutlet UIView *showOrHideVisualizationButtonView;
@property (weak, nonatomic) IBOutlet UIButton *showOrHideVisualizationButton;
@property (weak, nonatomic) IBOutlet UIView *visualizationView;
@property (weak, nonatomic) IBOutlet UIImageView *coloredRingImageView;
@property (weak, nonatomic) IBOutlet UIImageView *gracenoteLogoImageView;

@property (weak, nonatomic) IBOutlet UILabel *statusCaptionLabel;
@property (weak, nonatomic) IBOutlet UILabel *toolbarBorderCoverupLabel;
@property (weak, nonatomic) IBOutlet UITableView *resultsTableView;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *busyIndicator;
@property (weak, nonatomic) IBOutlet UINavigationItem *titleNavigationItem;
@property (weak, nonatomic) IBOutlet UITabBarItem *historyTabBarItem;
@property (weak, nonatomic) IBOutlet UITableView *searchFieldsTableView;
@property (weak, nonatomic) IBOutlet UIButton *cancelOperationsButton;
@property (weak, nonatomic) IBOutlet UIButton *idNowButton;

- (IBAction) idNow:(id) sender;
- (IBAction)cancelAllOperations:(id)sender;
- (IBAction)showVisualization:(id)sender;

-(void) stopRecording;

@end
