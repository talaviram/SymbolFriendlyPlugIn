SymbolFriendlyPlugin
====================

WHY?
----
Some people on [JUCE forum](https://forum.juce.com/t/generating-osx-dsyms-and-still-stripping-symbols/23731) had hard-time getting release builds while keeping symbols.

To be honest, I've also had some learning curve until we got it right...

This basic project tries to show how to properly compile release builds without exposing symbols, while keeping symbols exported in-order to be able and symbolicate a user crashlog or dump afterwards.

HOW
---

Compiling / Building ** ALL OSes **
===================================

- Get latest [JUCE](https://github.com/WeAreROLI/JUCE/) (I'm using develop branch),- Compile Projucer
- Open the `SymbolFriendlyPlugIn.jucer` from Projucer.
- Save it.


macOS
-----

the most important part is the shell script.
Notice the extra arguments passed to `xcodebuild`.
It is always better to have proper scripts that are less prune to user errors.

I didn't add additional stages, but on a build system it's strongly advised you'd gather all symbols during the build script(s) and package them with the specific release.
you should also try to keep some identification such as a build integer or unique value to allow you matching a release binaries and their appropriate symbols.

    1. make sure you've built Projucer.
    2. open the project with Projucer and save it with the appropriate JUCE modules locations.
    3. after you've saved it run the following shell script:

`sh build_macos.sh`

    4. once compiled, keep symbols and binaries.

Windows
-------

Visual Studio produces PDBs most of the time while the actual binaries won't include too much unintended data. unlike macOS there's pretty much no possibility of both within the final binary.
Usually binaries that got proper PDB would include a path to their PDB with the final release. it's not exposing much and needed to link a PDB to the binary.

It is always better to use `MSBuild` but while `Xcodebuild` has no additional requriments,  MSBuild __does__ require some prior steps **missing** within the example batch. (%PATH% and other fun Windows stuff ;) ).
I didn't use MSBuild though.

For our example, simply load your Visual Studio project, and compile in Release.
I've only added Visual Studio 2017 Exporter but you can easily make an exporter to Visual Studio 2015 or even older that would act pretty much the same.

**TIP!:**
---------
One thing to remember, with JUCE at least, PDBs can have the same name regardless to architecture and format (VST/VST3/AAX). so they'll only symbolicate if the PDB filename corrosponds to the one expected. even if they've got the same GUID. apperantly that's not enough.

For example:
`SymbolFriendlyPlugIn.dll` and `SymboFriendlyPlugIn.exe` will use different PDBs but when in-use __MUST__ be with the same name, `SymbolFriendlyPlugIn.pdb`


How To Verify Symbols (macOS/Linux)
=============================
For Windows, please read above about PDB.

llvm-nm displays the names list (symbol table nlist). Expected symbols are the ones to be used by plug-in hosts such as VST / AudioUnit / VST3/ AAX, etc...

nm should be used on the binary file (not folder! as macOS creates a bundle...)

Try the following on Debug vs Release:
`nm SymbolFriendlyPlugIn.app/Contents/MacOS/SymbolFriendlyPlugIn -s __TEXT __text`


Pre-Compiled Binaries and How to symbolicate crashes
====================================================

Our dream as developers is to get crashlogs easily. Some distributing channels (Apple Store, Google Developer Console, etc..) integrates crashlog gathering for us. but with DAWs it's most of the time rare to get them easily.
Other alternative we'd hope, is to have is a crash-reporter baked into our product. I wish we had such solution and I promise to update this repo if I stumble simple solution as such. But for now, we use simple crashlogs or dumps we can get from the OS with some user motivation.

I've included some pre-compiled binaries if you'd just like to try and test the symbolication process.

macOS
-----

On Mac, there's the crash reporter that jumps-in most of time or additional crash handler provided by host (Pro Tools got one, Ableton Live got one, etc...)
You'd usually ask the user kindly to send you those crash files.

The ones from the hosts would also be helpful and can be symbolicated with atos shown below.

So...
I've provided a file named __SymbolFriendlyPlugIn_2018-04-09-175024.crash__ it containes a non-symbolicated crash on my machine.

There are scripts you'd might like to use for symbolication, but the basic way is AddressToSymbol (atos) included with Apple's Developer Tools.

With the included crash and symbols, use the Standalone version dSYM.
Basically we need to know where our project was first loaded to memory and where it crashed.
(or other memory address within the stack). in our example it's __0x10a0e7000__ (can be found within the crashlog __Binary Images:__ addresses.

-o : the executable.
-l : the load address

So in-order to symbolicate and address here is an example:

    atos -o /Users/dev/Developer/dev/SymbolFriendlyPlugIn/Builds/Prebuilt\ Binaries/macOS/Symbols/SymbolFriendlyPlugIn.app.dSYM/Contents/Resources/DWARF/SymbolFriendlyPlugIn -l 0x10a0e7000 0x000000010a11751f 0x000000010a0f3909

Would result:

    juce::AudioProcessorPlayer::audioDeviceIOCallback(float const**, int, float**, int, int) (in SymbolFriendlyPlugIn) (juce_AudioProcessorPlayer.cpp:163)
    
    SymbolFriendlyPlugInAudioProcessor::processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) (in SymbolFriendlyPlugIn) (PluginProcessor.cpp:135)

One thing our example doesn't include is the architecture. we run this example on Intel/AMD x64 arch. However, if our user (and product) was from another architecture (such as iOS ARM x64 or i386/32-bit x86) then we would also mention the architecture which is also important. using the `--arch` .

Windows
-------

With Microsoft our life are better or worse, depend how you look at it.
In order to obtain a crashlog (or in MS terms - dump or minidump) is by the crash reporter within your plug-in host or... the Task Manager.

It requires more work from the user but will be very useful.

1. Make sure not to close the crashed process (application... your dll host).
2. Open Task Manager (ctrl+alt+del)
3. Select the process that got crashed and right click on it.
4. Select `Create dump file`.

This is the fail-safe way obtaining logs from users. but, it could make HUGE files unlike minidumps.