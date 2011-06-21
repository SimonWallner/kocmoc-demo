==============================================================================
KOCMOC - a small handwritten OpenGL 3.2 core demo.
==============================================================================


Introduction
------------

This demo was created as part of the "real time rendering" lab course at the Vienna University of Technology (http://www.cg.tuwien.ac.at/courses/Realtime/VU.html)

The video is available at http://vimeo.com/19202087

Any comments/questions are hightly appreciated and you can contact me via email of follow me on twitter @SimonWallner.


Features
--------
- Opengl 3.2 core profile renderer
- Loading and processing of collada files. Not all fomat features are supported and meshes must be set up correctly.
- object/world space normal mapping
- post process color correction with 3D LUT
- 2x FSAA
- Non-planar projection (spherical) achieved by resampling in the post process fragment shader
- simple shadow mapping
- simple key frame animation system
- simple property file system
- fixed/variable timestamp animation with frame export (i.e. to record movies)
- OpenAL audio

Controls
--------
 - Press F1 to see a list of commands.
 - Press 'r' to reset the animation.
 - Press ESC to quit.


How to build
------------
To build the demo, download the sourc files and also additionally dowload the openCollada repo into the ./external/opencollada/ folder. 

CMake is used as a build system, and it should build without problems on Windows 7 with Visual Studio 2008. VS2010 is not yet supported by collada. In order for CMake to use collada as a sub project, apply this (http://code.google.com/p/opencollada/issues/detail?id=140) patch. Executables are put into the bin/ folder so make sure to set the wokring directory accordingly.


Used Libs
---------
 - boost (not really, wanted to but didn't in the end.)
 - devIL
 - freealut
 - gl3w
 - glfw
 - glm
 - libogg
 - libvorbis
 - libxml
 - openal
 - opencollada


License/references
-------
All source code is made available under the MIT license (see below). All media files (textures, sounds, etc...) are available under the creative commons CC BY-NC-SA (Attribution-NonCommercial-ShareAlike) license.

Parts of the source code are documented, but it mostly consists of ugly hacks, so where safety glasses and helmelts if you dive into it.

The image of the milky way is courtesy of 
NASA/Goddard Space Flight Center Scientific Visualization Studio
http://svs.gsfc.nasa.gov/goto?3572


MIT License
-----------
Copyright (c) 2011 Simon Wallner

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


