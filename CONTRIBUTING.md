## Contributing to raylib

Hello contributors! Welcome to raylib! 

This document contains a set of guidelines to contribute to the project. These are mostly guidelines, not rules. 
Use your best judgement, and feel free to propose changes to this document in a pull-request.

### raylib philosophy

 - raylib is a tool to LEARN videogames programming, every single function in raylib should be a tutorial on itself.
 - raylib is SIMPLE and EASY-TO-USE, I tried to keep it compact with a small set of functions, if a function is too complex or is not clearly useful, better not to include it.
 - raylib is open source and free; educators and institutions can use this tool to TEACH videogames programming completely by free.
 - raylib is collaborative; contribution of tutorials / code-examples / bugs-solving / code-comments are highly appreciated.
 - raylib's license (and its external libs respective licenses) allow using it for commercial products.

### Some interesting reads to start with

 - [raylib history](HISTORY.md)
 - [raylib architecture](https://github.com/raysan5/raylib/wiki/raylib-architecture)
 - [raylib license](LICENSE.md)
 - [raylib roadmap](ROADMAP.md)
 
[raylib Wiki](https://github.com/raysan5/raylib/wiki) contains some information about the library and is open to anyone for edit. 
Feel free to review it if required, just take care not to break anything.

### raylib C coding conventions

Despite being written in C, raylib does not follow the standard Hungarian notation for C, 
it [follows Pascal-case/camel-case notation](https://github.com/raysan5/raylib/wiki/raylib-coding-conventions), 
more common on C# language. All code formatting decisions have been carefully taken
to make it easier for students to read, write and understand code. 

Source code is extensively commented for that purpose, raylib primary learning method is:

 > learn by reading code and examples


### Using raylib develop branch

raylib [master branch](https://github.com/raysan5/raylib) contains the latest raylib release but all 
the future work is always done on [develop branch](https://github.com/raysan5/raylib/tree/develop), 
it's far ahead of master and I try to keep it always stable with latest library changes.

Please, if issuing a bug or sending a pull-request, verify that you're using develop branch.

Note that while using develop branch predefined Notepad++ building scripts could not work, in that case,
use provided [Makefile](https://github.com/raysan5/raylib/blob/develop/src/Makefile) or [CMake](https://github.com/raysan5/raylib/blob/develop/src/CMakeLists.txt) building systems.

For detailed information on building raylib and examples, please check [raylib Wiki](https://github.com/raysan5/raylib/wiki).

### Opening new Issues

To open new issue for raylib (bug, enhancement, discussion...), just try to follow these rules:

 - Make sure the issue has not already been reported before by searching on GitHub under Issues.
 - If you're unable to find an open issue addressing the problem, open a new one. Be sure to include a 
 title and clear description, as much relevant information as possible, and a code sample demonstrating the unexpected behavior.
 - If applies, attach some screenshot of the issue and a .zip file with the code sample and required resources.
 - On issue description, add a brackets tag about the raylib module that relates to this issue. 
 If don't know the module, just report the issue, I will review it.
 - You can check other issues to see how is being done!

### Sending a Pull-Request

To send a pull-request, first of all, make sure you're sending it to [develop branch](https://github.com/raysan5/raylib/tree/develop). Here are some other rules:

 - Make sure the PR description clearly describes the problem and solution. Include the relevant issue number if applicable.
 - Don't send big pull-requests (lots of changelists), they are difficult to review. It's better to send small pull-request, one at a time.
 - Verify that changes don't break the build (at least on Windows platform). As many platforms where you can test it, the better, but don't worry
 if you cannot test all the platforms.

### raylib bindings

Some people ported raylib to other languages in form of bindings or wrappers to the library, here is a list with the ones I'm aware of:

 - [raylib C/C++ version (default)](https://github.com/raysan5/raylib)
 - [raylib Lua binding](https://github.com/raysan5/raylib-lua)
 - [raylib Go binding](https://github.com/gen2brain/raylib-go)
 - [raylib Nim binding](https://gitlab.com/define-private-public/raylib-Nim)
 - [raylib Crystal binding](https://gitlab.com/Zatherz/cray) 
 - [raylib Perl wrapper](https://metacpan.org/pod/Graphics::Raylib) 
 - raylib Pascal binding with custom IDE (closed-source)
 
Usually, raylib bindings follow the convention: `raylib-{language}`

Let me know if you're writing a new binding for raylib, I will list it here and I usually 
provide the icon/logo for that new language binding.

### Contact information

If you have any doubt, don't hesitate to [contact me](mailto:ray@raylib.com)!.
You can write me a direct mail but you can also contact me on the following networks:

 - [raylib forum](http://forum.raylib.com/) - A good place for discussions or to ask for help.
 - [raylib gitter](https://gitter.im/raylib) - A direct communication channel for project discussions.
 - [raylib twitter](https://twitter.com/raysan5) - My personal twitter account, I usually post about raylib, you can send me PMs.
 - [raylib web](http://www.raylib.com/) - On top-right corner there is a bunch of networks where you can find me.

Thank you very much for your time! :)

----

Here is a list of raylib contributors, these people have invested part of their time 
contributing (in some way or another) to make the raylib project better. Huge thanks to all of them!

 - [Zopokx](https://github.com/Zopokx) for testing the web.
 - [Elendow](http://www.elendow.com) for testing and helping on web development.
 - Victor Dual for implementing and testing 3D shapes functions.
 - Marc Palau for implementing and testing 3D shapes functions and contribute on camera and gestures modules.
 - Kevin Gato for improving texture internal formats support and helping on raygui development. 
 - Daniel Nicolas for improving texture internal formats support and helping on raygui development. 
 - Marc Agüera for testing and using raylib on a real product ([Koala Seasons](http://www.koalaseasons.com))
 - Daniel Moreno for testing and using raylib on a real product ([Koala Seasons](http://www.koalaseasons.com))
 - Daniel Gomez for testing and using raylib on a real product ([Koala Seasons](http://www.koalaseasons.com))
 - [Sergio Martinez](https://github.com/anidealgift) for helping on raygui development and tools development (raygui_styler).
 - [Victor Fisac](https://github.com/victorfisac) for developing physics raylib module (physac) and implementing PBR materials and lighting systems... among multiple other improvements and multiple tools and games. 
 - Albert Martos for helping on raygui and porting examples and game-templates to Android and HTML5.
 - Ian Eito for helping on raygui and porting examples and game-templates to Android and HTML5.
 - [procedural](https://github.com/procedural) for testing raylib on Linux, correcting some bugs and adding several mouse functions.
 - [Chris Hemingway](https://github.com/cHemingway) for improving raylib on OSX build system.
 - [Emanuele Petriglia](https://github.com/LelixSuper) for working on multiple GNU/Linux improvements and developing [TicTacToe](https://github.com/LelixSuper/TicTacToe) raylib game.
 - [Joshua Reisenauer](https://github.com/kd7tck) for adding audio modules support (XM, MOD) and reviewing audio system.
 - [Marcelo Paez](https://github.com/paezao) for his help on OSX to solve High DPI display issue. Thanks Marcelo!
 - [Ghassan Al-Mashareqa](https://github.com/ghassanpl) for his amazing contribution with raylib Lua module, I just work over his code to implement [rlua](https://github.com/raysan5/raylib/blob/master/src/rlua.h)
 - [Teodor Stoenescu](https://github.com/teodor-stoenescu) for his improvements on OBJ object loading.
 - [RDR8](https://github.com/RDR8) for helping with Linux build improvements
 - [Saggi Mizrahi](https://github.com/ficoos) for multiple fixes on Linux and audio system
 - [Daniel Lemos](https://github.com/xspager) for fixing issues on Linux games building
 - [Joel Davis](https://github.com/joeld42) for adding raycast picking utilities and a [great example](https://github.com/raysan5/raylib/blob/master/examples/models/models_mesh_picking.c)
 - [Richard Goodwin](https://github.com/AudioMorphology) for adding RPI touchscreen support
 - [Milan Nikolic](https://github.com/gen2brain) for adding Android build support with custom standalone toolchain
 - [Michael Vetter](https://github.com/jubalh) for improvements on build system and adding meson support for Linux
 - [Wilhem Barbier](https://github.com/nounoursheureux) for adding Image generation functions and some fixes
 - [Benjamin Summerton](https://github.com/define-private-public) for improving OSX building and his amazing work on CMake build sytem
 - [MartinFX](https://github.com/Martinfx) for adding compilation support for FreeBSD OS
 - [Wilhem Barbier](https://github.com/nounoursheureux) for supporting default shaders on shader loading, if shader not provided
 - [Ahmad Fatoum](https://github.com/a3f) for implementing continuus integration support for raylib (Travis and AppVeyor) and greatly improving build system.
 - [SamNChiet](https://github.com/SamNChiet) for his work on UWP inputs implementation.
 - [David Reid](https://github.com/mackron) for a complete review of audio module to support his amazing mini_al audio library.
 - [Kai](https://github.com/questor) for multiple code reviews and improvements.
 
Please, if I forget someone in this list, excuse me and write me an email to remind me to add you!