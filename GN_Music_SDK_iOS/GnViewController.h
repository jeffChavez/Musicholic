//
//  GnViewController.h
//  GN_Music_SDK_iOS
//
//  Copyright (c) 2013 Gracenote. All rights reserved.
//

#import <UIKit/UIKit.h>


typedef NS_ENUM(NSInteger, Mode)
{
    UnknownMode=-1,
    AdditionalContentMode
};

#define kHeightOfAdditionalMetadataCell 260
#define kHeightOfAdditionalMetadataCellPad 360

#define kHeightOfRowPad 140
#define kHeightOfRow 100

@interface GnViewController : UIViewController<UINavigationBarDelegate, UIActionSheetDelegate>

@property (strong, nonatomic) IBOutlet UILabel *statusLabel;
@property (weak, nonatomic) IBOutlet UIView *showOrHideVisualizationButtonView;
@property (weak, nonatomic) IBOutlet UIButton *showOrHideVisualizationButton;
@property (weak, nonatomic) IBOutlet UIView *visualizationView;
@property (weak, nonatomic) IBOutlet UIImageView *coloredRingImageView;
@property (weak, nonatomic) IBOutlet UIImageView *gracenoteLogoImageView;

@property (weak, nonatomic) IBOutlet UILabel *statusCaptionLabel;
@property (weak, nonatomic) IBOutlet UIButton *cancelOperationsButton;
@property (weak, nonatomic) IBOutlet UIButton *idNowButton;

@property (weak, nonatomic) IBOutlet UIImageView *testImage;
@property (weak, nonatomic) IBOutlet UILabel *testLABEL;




- (IBAction) idNow:(id) sender;
- (IBAction) findDrink:(id) sender;
- (IBAction)cancelAllOperations:(id)sender;
- (IBAction)showVisualization:(id)sender;

-(void) stopRecording;

@end
