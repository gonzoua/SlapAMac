//
//  SlapAMacAppDelegate.m
//  SlapAMac
//
//  Created by Oleksandr Tymoshenko on 10-01-06.
//  Copyright 2010 Bluezbox Software. All rights reserved.
//

#import "SlapAMacAppDelegate.h"

@implementation SlapAMacAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	// Insert code here to initialize your application 
	NSStatusItem *statusItem;
	NSMenu *menu = [self createMenu];
	
	statusItem = [[[NSStatusBar systemStatusBar]
				   statusItemWithLength:NSVariableStatusItemLength] retain];
	[statusItem setMenu:menu];
	[statusItem setHighlightMode:YES];
	[statusItem setToolTip:@"SlapAMac"];
	[statusItem setImage:[NSImage imageNamed:@"glove"]];
	
	[menu release];
}

- (NSMenu *) createMenu {
	NSZone *menuZone = [NSMenu menuZone];
	NSMenu *menu = [[NSMenu allocWithZone:menuZone] init];
	NSMenuItem *menuItem;

	// Add About Action
	menuItem = [menu addItemWithTitle:@"Sensitivity"
							   action:@selector(actionSensitivity:)
						keyEquivalent:@""];
	[menuItem setToolTip:@"Adjust sensitivity level"];
	[menuItem setTarget:self];
	
	
	// Add About Action
	menuItem = [menu addItemWithTitle:@"About"
							   action:@selector(actionAbout:)
						keyEquivalent:@""];
	[menuItem setToolTip:@"Do you want to know a secret?"];
	[menuItem setTarget:self];
	
	// Add Quit Action
	menuItem = [menu addItemWithTitle:@"Quit"
							   action:@selector(actionQuit:)
						keyEquivalent:@""];
	[menuItem setToolTip:@"Click to Quit this App"];
	[menuItem setTarget:self];
	
	return menu;
}	

- (void) actionQuit: (id)sender
{	
	[NSApp terminate:sender];
}

- (void) actionAbout:(id)sender
{
	[NSApp activateIgnoringOtherApps:YES];
	
	[[NSApplication sharedApplication] orderFrontStandardAboutPanel:sender];	
}

- (void) actionSensitivity: (id)sender
{	
	if (![sensitivityPanel isVisible]) {
		[sensitivityPanel makeKeyAndOrderFront:self];
	}
	
}

- (NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication *)sender
{
	[smsController savePrefs];
	return YES;
}
@end
