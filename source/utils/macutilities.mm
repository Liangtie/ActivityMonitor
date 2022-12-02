
#include <ApplicationServices/ApplicationServices.h>
#include <Foundation/Foundation.h>
#include <AppKit/AppKit.h>
#include <CoreData/CoreData.h>

#include "macutilities.h"
//#include <QtDebug>

uint64_t MacUtilities::msSinceLastInput()
{
    return 1000*static_cast<uint64_t>(CGEventSourceSecondsSinceLastEventType(kCGEventSourceStateHIDSystemState, kCGAnyInputEventType));
}

std::string MacUtilities::getActiveAppName()
{
    NSRunningApplication *curr_app = [[NSWorkspace sharedWorkspace] frontmostApplication];
    NSString *curr_app_name = [curr_app localizedName];
    return std::string([curr_app_name UTF8String]);
}

std::string MacUtilities::getActiveWindowTitle()
{
    NSRunningApplication *curr_app = [[NSWorkspace sharedWorkspace] frontmostApplication];
    pid_t curr_pid = [curr_app processIdentifier];
    auto options = kCGWindowListOptionAll;
    CFArrayRef windowList = CGWindowListCopyWindowInfo(options, kCGNullWindowID);
    for (int i=0; i<CFArrayGetCount(windowList); ++i)
    {
        NSDictionary *window = (__bridge NSDictionary *)(CFDictionaryRef)CFArrayGetValueAtIndex(windowList, i);
        NSNumber *pid = [window objectForKey:@"kCGWindowOwnerPID"];
        NSString *windowTitle = [window objectForKey:@"kCGWindowName"];
        if(!pid || !windowTitle)
            continue;
        NSString *windowTitleTrimmed = [windowTitle stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
        if(curr_pid == [pid intValue] && windowTitleTrimmed.length!=0 && ![windowTitleTrimmed isEqualToString:@"Item-0"])
        {
            return std::string([windowTitleTrimmed UTF8String]);
        }
    }
    return "Unknown";
}
