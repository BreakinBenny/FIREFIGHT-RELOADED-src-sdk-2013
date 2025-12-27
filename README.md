# ![Logo](https://github.com/FIREFIGHT-RELOADED/FIREFIGHT-RELOADED-src-sdk-2013/raw/endgame/Graphics/FR_Logo.png)

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/c1149f5b5fa14522a1bea1daa452bdf4)](https://app.codacy.com/gh/FIREFIGHT-RELOADED/FIREFIGHT-RELOADED-src-sdk-2013/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Project Status: Active – The project has reached a stable, usable state and is being actively developed.](https://www.repostatus.org/badges/latest/active.svg)](https://www.repostatus.org/#active)

FIREFIGHT RELOADED is an open-source Source SDK 2013 modification with the goal to combine gameplay elements from Half-Life 2, SMOD, old-school first-person shooters like Quake and Unreal Tournament, and the RPG-styled horde-mode gameplay that ties this mod together. 

- Steam Store page: http://store.steampowered.com/app/397680
- ModDB Page: http://www.moddb.com/mods/firefight-reloaded

**NOTE: PLEASE NOTE THAT THIS REPOSITORY WILL HAVE UNFINISHED OR BUGGY CODE FROM A NEWER RELEASE OF FIREFIGHT RELOADED. KEEP THIS IN MIND BEFORE COMPILING OR FORKING.**

As this is a Half-Life 2/Source Engine mod, a file named LICENCE-VALVE.md is included which includes Valve's Source 1 SDK licence.\
All code written by Valve and Bitl are covered by this license.\
Code used in FIREFIGHT RELOADED may be used for your Source Engine game/mod, however you must give credit to Bitl if you do use code from this mod into your Source SDK project.

## Compiling

### WINDOWS:

To be able to build FIREFIGHT RELOADED you will need to download Visual Studio 2022 or later and the following tools via the Visual Studio installer:

* MSVC v143 - VS 2022 C++ x64/x86 build tools
* C++ MFC Library for latest v143 build tools (x86 and x64)
* Latest Windows 10 or Windows 11 SDK.
* Microsoft Speech API (Optional)

Creating a solution is as simple as running sp/src/creategameprojects.bat, then opening up the solution in Visual Studio.

### LINUX:
Read this first to install the steam runtime and other necessary components.\
https://developer.valvesoftware.com/wiki/Source_SDK_2013#Source_SDK_2013_on_Linux \
After installing it and other dependencies, do the 3 steps below.

1. CD to your FIREFIGHT RELOADED git directory, I.E.\
```cd ~/projects/FIREFIGHT-RELOADED-src-sdk-2013```

2. Run the steam-runtime. NOTE: Make sure you have the steam runtime installed in /valve.\
```bash ./sandbox_setup.sh```
If you haven't installed the steam-runtime, run:
```bash ./sandbox_install.sh```
After it installs, it will start steam-runtime.

3. Go back to this directory and run:\
```bash ./build_fr_linux.sh```\
This will update the repo automatically. If you would like to update the repo without building it, run:\
```bash ./update_fr_linux.sh```\
If you would like to build the repo without updating it, run:\
```bash ./build_fr_linux_noupdate.sh```

Read [sp/game/firefightreloaded/scripts/credits.txt](https://github.com/FIREFIGHT-RELOADED/FIREFIGHT-RELOADED-src-sdk-2013/blob/endgame/sp/game/firefightreloaded/scripts/credits.txt) for credits and citations.

This mod uses FMOD. FMOD created by Firelight Technologies Pty Ltd.

[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com) [![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
