#pragma once


#include <xwl/platforms/osx.h>

#if __OBJC__
#import <Cocoa/Cocoa.h>
@interface xwlDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>

@end
#endif
