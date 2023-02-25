#include "MacTheme.h"

#include <Cocoa/Cocoa.h>

// https://stackoverflow.com/questions/55925862/how-can-i-set-my-os-x-application-theme-in-code

void MacTheme::dark()
{
   [NSApp setAppearance:[NSAppearance appearanceNamed:NSAppearanceNameDarkAqua]];
}

void MacTheme::light()
{
   [NSApp setAppearance:[NSAppearance appearanceNamed:NSAppearanceNameAqua]];
}

void MacTheme::system()
{
   [NSApp setAppearance:nil];
}
