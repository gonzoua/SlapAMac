//
//  SMSController.h
//  SlapAMac
//
//  Created by Oleksandr Tymoshenko on 10-01-06.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "smsutils.h"

enum dir_t {
	PREV_SPACE,
	NEXT_SPACE,
};

#define N_DX 30
#define HIT_THRESHOLD 35
#define	STABLE_THRESHOLD 20
#define	MIN_STABLE_TICKS 30

// Keycodes
#define	KEY_LEFT	123
#define	KEY_RIGHT	124
#define	KEY_COMMAND	55
#define KEY_ALT		58
#define KEY_CONTROL	59
@interface SMSController : NSObject {
	sms_t sms_;
	int stable_ticks_;
	NSInteger sensitivity_level_;
};

@property NSInteger sensitivity_level_;
- (void) savePrefs;
- (void) loadPrefs;
@end