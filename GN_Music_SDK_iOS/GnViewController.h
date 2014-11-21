//
//  GnViewController.h
//  GN_Music_SDK_iOS
//
//  Copyright (c) 2013 Gracenote. All rights reserved.
//

#import <UIKit/UIKit.h>


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
@property (weak, nonatomic) IBOutlet UIButton *findDrinkButton;
@property (weak, nonatomic) IBOutlet UIButton *echoNestFindDrinkButton;

@property (weak, nonatomic) IBOutlet UIImageView *songAlbumImage;
@property (weak, nonatomic) IBOutlet UILabel *songInfoLabel;
@property (weak, nonatomic) IBOutlet UILabel *statusIdNowLabel;

@property (nonatomic, strong) IBOutlet NSLayoutConstraint *idNowButtonCenterYAlignmentConstraint;

- (IBAction) login:(id) sender;
- (IBAction) idNow:(id) sender;
- (IBAction) findDrink:(id) sender;
- (IBAction) echoNest:(id)sender;
- (IBAction) cancelAllOperations:(id)sender;
- (IBAction) showVisualization:(id)sender;

-(void) stopRecording;

@end
