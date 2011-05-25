//
//  SMSController.m
//  SlapAMac
//
//  Created by Oleksandr Tymoshenko on 10-01-06.
//  Copyright 2010 Bluezbox Software. All rights reserved.
//

#import "SMSController.h"

void changeSpace(int direction)
{
	CFRelease(CGEventCreate(NULL));
	int arrowKey;
	int modifierKey;
	
	if (direction == PREV_SPACE)
		arrowKey = KEY_LEFT;
	else 
		arrowKey = KEY_RIGHT;

	CFPreferencesAppSynchronize(CFSTR("com.apple.symbolichotkeys"));
	
	// Just get one of space control keys
	NSDictionary *hotKeysInfo = 
	(NSDictionary *)CFPreferencesCopyAppValue(CFSTR("AppleSymbolicHotKeys"), 
											  CFSTR("com.apple.symbolichotkeys"));
	NSArray *hotKey = [hotKeysInfo valueForKeyPath:@"81.value.parameters"];
	int isEnabled = [[hotKeysInfo valueForKeyPath:@"81.enabled"] intValue];
	
	long modifiers = [[hotKey objectAtIndex:2] longValue];
	[hotKeysInfo release];
	
	if (!isEnabled)
	{
		// Ouch!
		NSAlert *alert = [NSAlert alertWithMessageText:@"Ouch! It hurts!" 
										 defaultButton:@"OK"
									   alternateButton:nil 
										   otherButton:nil
							 informativeTextWithFormat:@"Spaces are disabled"];
		
		[alert runModal];
		return;		
	}
	
	CGEventRef eventDown = CGEventCreateKeyboardEvent(NULL, arrowKey, true);
	CGEventRef eventUp = CGEventCreateKeyboardEvent(NULL, arrowKey, false);
	
	if (modifiers & NSCommandKeyMask)
	{
		NSLog(@"mod key: command");
		modifierKey = KEY_COMMAND;
		CGEventSetFlags(eventDown, kCGEventFlagMaskCommand);
		CGEventSetFlags(eventUp, kCGEventFlagMaskCommand);

	}
	else if (modifiers & NSAlternateKeyMask)
	{
		NSLog(@"mod key: alt");
		modifierKey = KEY_ALT;
		CGEventSetFlags(eventDown, kCGEventFlagMaskAlternate);
		CGEventSetFlags(eventUp, kCGEventFlagMaskAlternate);

	}
	else if (modifiers & NSControlKeyMask)
	{
		NSLog(@"mod key: control");
		modifierKey = KEY_CONTROL;
		CGEventSetFlags(eventDown, kCGEventFlagMaskControl);
		CGEventSetFlags(eventUp, kCGEventFlagMaskControl);
	}
	else {
		// Ouch!
		NSAlert *alert = [NSAlert alertWithMessageText:@"Ouch! It hurts!" 
												defaultButton:@"OK"
											  alternateButton:nil 
												  otherButton:nil
									informativeTextWithFormat:@"Switching between spaces is disabled"];
		
		[alert runModal];
		CFRelease(eventDown);
		CFRelease(eventUp);
		return;
	}

	
	CGEventPost(kCGHIDEventTap, eventDown);
	CGEventPost(kCGHIDEventTap, eventUp);
	
	// just to make sure spaces icon is gone
	CGEventRef eventModifierKeyDown = CGEventCreateKeyboardEvent(NULL, modifierKey, true);
	CGEventRef eventModifierKeyUp = CGEventCreateKeyboardEvent(NULL, modifierKey, false);
	usleep(10);
	CGEventPost(kCGHIDEventTap, eventModifierKeyDown);		
	CGEventPost(kCGHIDEventTap, eventModifierKeyUp);	
	
	CFRelease(eventDown);
	CFRelease(eventUp);
	CFRelease(eventModifierKeyDown);
	CFRelease(eventModifierKeyUp);
}

@implementation SMSController

@synthesize sensitivity_level_;

- (void)awakeFromNib
{
	if (smsOpen(&sms_))
	{		
		NSAlert *       alert = [NSAlert alertWithMessageText:@"smsOpen" 
												defaultButton:@"OK"
											  alternateButton:nil 
												  otherButton:nil
									informativeTextWithFormat:@"Failed to open SMS IOService !"];
		
		[alert runModal];
		return;
	}
	
	stable_ticks_ = 0;
	[self loadPrefs];
	[NSTimer scheduledTimerWithTimeInterval:0.01
									 target:self
								   selector:@selector(timerSmsRotation:)
								   userInfo:nil
									repeats:YES];


}

- (void)dealloc
{
	smsClose(&sms_);
    [super dealloc];
}

- (void)timerSmsRotation:(NSTimer *)aTimer
{

	sms_data_t data;
	if (! smsGetData(&sms_, &data))
	{
		int threshold = data.x;

		if (abs(threshold) < sensitivity_level_ - 10)
			stable_ticks_++;
		
		if ((abs(threshold) > sensitivity_level_) && 
			(stable_ticks_ > MIN_STABLE_TICKS)) 
		{			
			stable_ticks_ = 0;
			if (threshold < 0) 
			{
				NSLog(@"Hit: Prev\n");
				changeSpace(PREV_SPACE);
			}
			else 
			{
				NSLog(@"Hit: Next\n");
				changeSpace(NEXT_SPACE);
			}
		}
	}
}

- (void) savePrefs
{
	NSUserDefaults *prefs = [NSUserDefaults standardUserDefaults];
	[prefs setInteger:self.sensitivity_level_ forKey:@"sensitivity"];
	[prefs synchronize];
}

- (void) loadPrefs
{
	NSUserDefaults *prefs = [NSUserDefaults standardUserDefaults];
	NSInteger level = [prefs integerForKey:@"sensitivity"];
	if (level == 0)
		level = HIT_THRESHOLD;
	
	self.sensitivity_level_ = level;
}

@end
