//
//  IAAppDelegate.h
//  InfinitApplication
//
//  Created by infinit on 10/17/12.
//  Copyright (c) 2012 infinit. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface IAAppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;

@property (assign) IBOutlet NSMenu* status_menu;

@property (retain) NSStatusItem* status_item;
@property (retain) NSImage* status_icon;
@property (retain) NSImage* default_status_icon;

@end
