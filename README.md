gtk_shader_ide
==============
- minimalist portable boilerplate GUI [gtk+-2.0](http://www.gtk.org)/[libglade](http://developer.gnome.org/libglade) project (fully functional example/demo)
- [GNU autotools](http://en.wikipedia.org/wiki/GNU_build_system) & [pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config)
- with (optional) use of [GtkGLExt](http://projects.gnome.org/gtkglext), if you want to use [OpenGL](http://www.opengl.org) [!!ARBvp1.0](http://www.opengl.org/registry/specs/ARB/vertex_program.txt)/[!!ARBfp1.0](http://www.opengl.org/registry/specs/ARB/fragment_program.txt)

![Alt text](https://github.com/mbohun/gtk_shader_ide/raw/master/doc/small_gsc_screenshot_20041008-shadowsoft.png "2004-10-08")![Alt text](https://github.com/mbohun/gtk_shader_ide/raw/master/doc/small_gsc_screenshot_20041019-shadowsoft.png "2004-10-19")

build prerequisites
-------------------
- gtk+-2.0
- gtkglext
- pkg-config
- GNU autotools
- nVidia / AMD / Intel gfx card

build
-----
$ ./configure

$ make

run & test
----------
$ cd src

$ ./gtk_shader_ide

copy & paste the test !!ARBvp1.0 program [doc/test.arbvp](https://raw.github.com/mbohun/gtk_shader_ide/master/doc/test.arbvp) into the "Vertex Shader" editor

![Alt text](https://github.com/mbohun/gtk_shader_ide/raw/master/doc/small_gsc_screenshot_20041008-shadowsoft.png "2004-10-08")

switch to the "Fragment Shader" editor and copy & paste in the !!ARBfp1.0 program [doc/basic.arbfp](https://raw.github.com/mbohun/gtk_shader_ide/master/doc/basic.arbfp)

![Alt text](https://github.com/mbohun/gtk_shader_ide/raw/master/doc/small_gsc_screenshot_20041008-shadowsoft.png "2004-10-08")

press the "Compile and Execute Shaders" toolbar button

![Alt text](https://github.com/mbohun/gtk_shader_ide/raw/master/doc/small_gsc_screenshot_20041008-shadowsoft.png "2004-10-08")

delete the fragment program from the "Fragment Shader" editor and copy & paste in [doc/grayscale.arbfp](https://raw.github.com/mbohun/gtk_shader_ide/master/doc/grayscale.arbfp) and press the "Compile and Execute Shaders" toolbar button again

![Alt text](https://github.com/mbohun/gtk_shader_ide/raw/master/doc/small_gsc_screenshot_20041008-shadowsoft.png "2004-10-08")

have fun 

TODO:
-----
- embed the XML GUI (src/gtk_shader_ide.glade) into the executable
- replace the handwritten syntax highlighting function with [gtksourceview](http://projects.gnome.org/gtksourceview) (if possible - as in not going to pull in whole GNOME project :-) as a _dependency_ )
- expose OpenGL state to the IDE
- add features like loading textures, etc.
