# Overview

TobiiProNano is a sublibrary for LSL. It provides the information from the Eye-Tracker Tobii Pro Nano

# Usage

When the application is launched, you can choose the channel you want to extract from the Eye-Tracker. Then, you need to reference the tracker model. 
To finshed just click on "Connect" to link the application to the Eye-Tracker.

# Build instructions

## Build LSL

To start, you must install the LSL library by following each step mentionned on this link: [LSL build](https://github.com/sccn/labstreaminglayer/blob/master/doc/BUILD.md).

## Build TrignoWireless library

Before building this library, be sure you have downloaded the TobiiPron library. You can find it here: [Tobii Library](https://www.tobiipro.com/product-listing/tobii-pro-sdk/#Download).

### Windows users

Step to follow:<BR/>
* Create the "build" foledr at the root
* Open a terminal and move into this one
* Run this command line :  `build> cmake .. -G "Visual Studio 16 2019" -DQt5_DIR="C:\Qt\version\msvc_version\cmake\Qt5" -DBOOST_DIR="C:\local\boost_version" -DTOBII_LIB="path\to\tobii\lib"`
* Run this command line :  `build> cmake --build . --config Release --target install`

### Linux and OSX users

Step to follow:<BR/>
* Open a terminal at the root of the library
* Run this command line :  `TW> mkdir build & cd build`
* Run this command line :  `TW> ccmake ..`
* Press "c" to configure
* Fill the path for the LSL library, Qt5 and Boost if there're missing and press c again
* Press "q" to quit
* Run this command line :  `TW> cmake --build . --target install`

Note: You probably need to move the *.so near the apllication