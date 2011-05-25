//
//  SlapAMacAppDelegate.h
//  SlapAMac
//
//  Created by Oleksandr Tymoshenko on 10-01-06.
//  Copyright 2010 Bluezbox Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SMSController.h"
@interface SlapAMacAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
	IBOutlet id sensitivityPanel;
	IBOutlet SMSController* smsController;
}

@property (assign) IBOutlet NSWindow *window;
- (NSMenu *) createMenu;
- (void) actionQuit: (id)sender;
- (void) actionAbout: (id)sender;
- (void) actionSensitivity: (id)sender;

@end
