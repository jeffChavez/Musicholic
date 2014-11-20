//
//  UserSignIn.h
//  Musicholic
//
//  Created by Jeff Chavez on 11/20/14.
//  Copyright (c) 2014 CodeFellows. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GnViewController.h"

@interface UserSignInView : UIView

@property (weak, nonatomic) IBOutlet UITextField *usernameTextField;
@property (weak, nonatomic) IBOutlet UITextField *emailTextField;
@property (weak, nonatomic) IBOutlet UITextField *passwordTextField;
@property (weak, nonatomic) IBOutlet UIButton *signInButton;
@property (strong, nonatomic) GnViewController *GnViewController;
@property (strong, nonatomic) UserSignInView *userSignInView;


@end
