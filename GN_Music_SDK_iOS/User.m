//
//  User.m
//  Musicholic
//
//  Created by Parker Lewis on 11/20/14.
//  Copyright (c) 2014 CodeFellows. All rights reserved.
//

#import "User.h"

@implementation User


- (instancetype) initWithName: (NSString *)screenname email:(NSString *)email password:(NSString *)password {
    self = [super init];
    self.screenname = screenname;
    self.email = email;
    self.password = password;
    return self;
}


@end
