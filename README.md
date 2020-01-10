# Hindsight CAN Drivers
## Use these drivers for Embedded CAN Communications on Hindsight (PY2020) Rover.

### More information
For more information please visit the Slite documentation HUB for Husky Robotics. This documentation is under the Electronics/CAN/CAN Protocol sheet.<br />

### How To Use
_Note: You will need to have the standard C libraries compiled with your code. This is usually done automatically. If you have problems with `stdint.h` let Jaden know._<br /><br />
<ins>*STEP 1*</ins><br />
Within your repository for your firmware create a submodule with this repo in it. You can follow [this guide](https://git-scm.com/book/en/v2/Git-Tools-Submodules) for instructions on how to do that and explanations for what exactly a submodule is.<br />
Usually this command is done as follows: <br />
```git submodule add git@github.com:huskyroboticsteam/HindsightCAN.git path_to_your_source```<br />
If you **put these files in the same folder as your source code**, you can skip the next step.<br />

<ins>*STEP 2*</ins><br />
Add all of the source files to your compiler path within whatever IDE you are using. This will be dependent on what IDE you are using and how you are building your code. Since most people here will not be using a Makefile, we cannot give explicit direction on how to do this. If you have trouble, talk to Jaden, Dylan, or someone else who has done it. Please try Googling it and figuring it out yourself first though.<br /><br />
<ins>*STEP 3*</ins><br />
Import all of the relevant headers into your project. All devices will need to import `CANCommon.h`, which gives you access to the files that are necessary for interpretting and creating Common Mode packets. But if you are working on a Motor Unit for example, you will need to also import `CANMotorUnit.h` to give you access to the functions pertinent to specifically a motor unit. If you are writing software for the Jetson, you will need to import all of them! Yay! /s 