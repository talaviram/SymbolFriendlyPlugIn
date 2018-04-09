#!/bin/sh

xcodebuild -project "./Builds/MacOSX/SymbolFriendlyPlugin.xcodeproj" -configuration "Release" GCC_GENERATE_DEBUGGING_SYMBOLS=YES STRIP_INSTALLED_PRODUCT=YES DEBUG_INFORMATION_FORMAT=dwarf-with-dsym