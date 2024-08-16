# RNR's Not Roblox
[![GitHub CI Status](https://img.shields.io/github/actions/workflow/status/lrre-foss/rnr/build.yml?branch=trunk&label=builds)](https://github.com/lrre-foss/rnr/actions)
[![Discord](https://img.shields.io/discord/1130992923329175552?style=social&logo=discord)](https://discord.gg/2tj4TREby3)
[![Star](https://img.shields.io/github/stars/lrre-foss/RNR?style=social)](https://github.com/lrre-foss/RNR/stargazers)

RNR's Not Roblox (*RNR*) is a project that aims to recreate the look and feel of classic Roblox with new features while remaining fully compatible with clients from that era. It is built upon an engine that closely resembles Roblox's own at the time, referencing decompilations of legacy client binaries.

Interested in contributing? Feel free to [make a pull request](https://github.com/lrre-foss/RNR/pulls), [create a new issue](https://github.com/lrre-foss/rnr/issues) for a feature request or to report a bug, [join the Discord server](https://discord.gg/2tj4TREby3) to report bugs and communicate one-on-one with the developers, or check out the [RNR GitHub Project](https://github.com/orgs/lrre-foss/projects/1) to see what we're working on and what we have done so far.

We also have some short demos on YouTube:
- [Block Town showcase](https://www.youtube.com/watch?v=-V2VUjxpNLs)
- [Doomspires 3.4k parts physics demo](https://www.youtube.com/watch?v=M0nn658uZ34)
- [Angel of Truth 8k+ parts physics demo](https://www.youtube.com/watch?v=EW6G_R6lx_Q)

Additionally, builds are automatically generated for Windows and Linux for each commit. You can browse packaged RNR builds at our [GitHub actions page](https://github.com/lrre-foss/rnr/actions).

## Features and Goals
There are several goals that RNR seeks to accomplish, them being;
- Native Windows and Linux support
- Easy-to-use (simple command line options to launch and host games, as well as a level editor with a modern UI)
- Fully compatible with Roblox versions up to 0.3.744.0 (dated April 2008) in areas such as hosting, joining, level file serialization, etc.
- Incorporates all the various facets of the Roblox engine with a little bit extra (e.g. a network replication whitelist, fancy shader support, etc.)
- Made using clean-room reverse engineering
- Uses Roblox's [Luau](https://luau-lang.org/) as its scripting language while remaining fully compatible with classic Roblox scripts written using Lua 5.1
- As free and open-source as possible (with client code licensed under the GPL and the engine itself being released into the public domain, void of any copyright)
- Patches all the security vulnerabilities and fixing bugs/inefficiencies that legacy Roblox clients had

## Building
<!-- TODO: this should be rewritten entirely, perhaps with a entry on the wiki alongside it. -->
RNR uses [CMake](https://cmake.org/) as its build system. To build RNR, you must first have the following packages installed:
- [Boost](https://www.boost.org/)
- [OGRE](https://github.com/OGRECave/ogre)
- [Bullet](https://github.com/bulletphysics/bullet3)
- [pugixml](https://github.com/zeux/pugixml)
- [Qt 6](https://www.qt.io/product/qt6) (if building the player or studio projects)

For Windows:
- If you're building ***for*** Windows, [MinGW-w64](https://www.mingw-w64.org/) is the preferred toolset of choice.
- If you're building ***on*** Windows, you may use a platform such as [MSYS2](https://www.msys2.org/), which provides an all-in-one environment for running MinGW or GCC.

Additionally, you must also acquire the content folder of the Roblox client you would like to use its resources from and place it into `Content/RNR`. Proprietary Roblox assets are not included with RNR.

Finally, you may run CMake in the path of the folder you've cloned the repository to so that you may configure and then finally build RNR.

# License
RNR is licensed under two separate licenses:
- All of RNR, with the sole exception of the engine, is licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.txt).
- The RNR engine itself is licensed under the [Creative Commons Zero v1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/legalcode.txt) license.

Copies of both licenses have been bundled with RNR.

RNR uses the [Luau](https://luau-lang.org/) language and code, developed by Roblox Corporation. Luau is copyright (c) 2019-2022 Roblox Corporation and copyright (c) 1994–2019 Lua.org, PUC-Rio.

This repository hosts no proprietary Roblox assets. Neither Legacy Roblox Reverse Engineers nor RNR are not associated with Roblox Corporation in any way, shape, or form.