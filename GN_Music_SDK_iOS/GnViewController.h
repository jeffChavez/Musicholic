//
//  GnViewController.h
//  GN_Music_SDK_iOS
//
//  Copyright (c) 2013 Gracenote. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface GnViewController : UIViewController<UINavigationBarDelegate, UIActionSheetDelegate, UITextFieldDelegate>

@property (strong, nonatomic) IBOutlet UILabel *statusLabel;

@property (weak, nonatomic) IBOutlet UILabel *statusCaptionLabel;
@property (weak, nonatomic) IBOutlet UIButton *cancelOperationsButton;
@property (weak, nonatomic) IBOutlet UIButton *idNowButton;
@property (weak, nonatomic) IBOutlet UIButton *findDrinkButton;
@property (weak, nonatomic) IBOutlet UIButton *echoNestFindDrinkButton;

@property (weak, nonatomic) IBOutlet UIImageView *songAlbumImage;
@property (weak, nonatomic) IBOutlet UILabel *songInfoLabel;
@property (weak, nonatomic) IBOutlet UILabel *statusIdNowLabel;

@property (nonatomic, strong) IBOutlet NSLayoutConstraint *idNowButtonCenterYAlignmentConstraint;

- (IBAction) idNow:(id) sender;
- (IBAction) findDrink:(id) sender;
- (IBAction) echoNest:(id)sender;
- (IBAction) cancelAllOperations:(id)sender;

-(void) stopRecording;

@end
