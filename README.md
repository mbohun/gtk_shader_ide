# gtk_shader_ide
- minimalist portable boilerplate GUI [gtk+-2.0](http://www.gtk.org)/[libglade](http://developer.gnome.org/libglade) project (fully functional example/demo)
- [GNU autotools](http://en.wikipedia.org/wiki/GNU_build_system) & [pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config)
- with (optional) use of [GtkGLExt](http://projects.gnome.org/gtkglext), if you want to use [OpenGL](http://www.opengl.org) [!!ARBvp1.0](http://www.opengl.org/registry/specs/ARB/vertex_program.txt)/[!!ARBfp1.0](http://www.opengl.org/registry/specs/ARB/fragment_program.txt)

![Alt text](https://github.com/mbohun/gtk_shader_ide/raw/master/doc/small_gsc_screenshot_20041008-shadowsoft.png "2004-10-08, GUI mockup with GLSL code")![Alt text](https://github.com/mbohun/gtk_shader_ide/raw/master/doc/small_gsc_screenshot_20041019-shadowsoft.png "2004-10-19, one of the first real sessions")

## build prerequisites
- gtk+-2.0
- gtkglext (not required if you don't want OpenGL and remove the gtkglext/OpenGL specific parts)
- pkg-config
- GNU autotools
- nVidia / AMD / Intel gfx card

NOTES:

1. I tested (built & run) the project on linux (slackware 12 - 13, Fedora 16 32bit, Fedora 17 64bit, saw it built & run on Ubuntu 64 bit, MS Windows 2000, MS Windows XP)
2. If you want to build on windoze you will need mingw (gcc, make), pkg-config/gtk+-2.0/glade-2.0/gtkglext-1.0 for windoze like those available in an one-click install from [gladewin32.sourceforge.net](http://gladewin32.sourceforge.net)

3. Some linux distros separate packages into 'runtime' and 'development' and the development part is often not installed by default, you will need those installed (for example: 'sudo yum install libglade2-devel.i686' to install libglade2  devel package on a Fedora 16 32bit), to give you an idea what everything is required here is a sample ldd output (while the long list might look scary most of these libs are already installed on most 'modern' linux distros):

```
martin@yobbo:~/src/gtk_shader_ide/src$ ldd ./gtk_shader_composer
        linux-gate.so.1 =>  (0xffffe000)
        libm.so.6 => /lib/libm.so.6 (0x77731000)
        libgtk-x11-2.0.so.0 => /usr/lib/libgtk-x11-2.0.so.0 (0x773a5000)
        libgdk-x11-2.0.so.0 => /usr/lib/libgdk-x11-2.0.so.0 (0x7731c000)
        libatk-1.0.so.0 => /usr/local/lib/libatk-1.0.so.0 (0x772f9000)
        libgio-2.0.so.0 => /usr/local/lib/libgio-2.0.so.0 (0x7716e000)
        libpangoft2-1.0.so.0 => /usr/local/lib/libpangoft2-1.0.so.0 (0x7713c000)
        libgdk_pixbuf-2.0.so.0 => /usr/local/lib/libgdk_pixbuf-2.0.so.0 (0x77119000)
        libpangocairo-1.0.so.0 => /usr/local/lib/libpangocairo-1.0.so.0 (0x7710a000)
        libcairo.so.2 => /usr/local/lib/libcairo.so.2 (0x76fc3000)
        libpango-1.0.so.0 => /usr/local/lib/libpango-1.0.so.0 (0x76f6b000)
        libfreetype.so.6 => /usr/lib/libfreetype.so.6 (0x76eed000)
        libfontconfig.so.1 => /usr/lib/libfontconfig.so.1 (0x76ec4000)
        libgobject-2.0.so.0 => /usr/local/lib/libgobject-2.0.so.0 (0x76e75000)
        libglib-2.0.so.0 => /usr/local/lib/libglib-2.0.so.0 (0x76d2e000)
        libgtkglext-x11-1.0.so.0 => /usr/local/lib/libgtkglext-x11-1.0.so.0 (0x76d2b000)
        libgdkglext-x11-1.0.so.0 => /usr/local/lib/libgdkglext-x11-1.0.so.0 (0x76cd9000)
        libGLU.so.1 => /usr/lib/libGLU.so.1 (0x76c69000)
        libGL.so.1 => /usr/lib/libGL.so.1 (0x76b8d000)
        libXmu.so.6 => /usr/lib/libXmu.so.6 (0x76b77000)
        libXt.so.6 => /usr/lib/libXt.so.6 (0x76b25000)
        libSM.so.6 => /usr/lib/libSM.so.6 (0x76b1d000)
        libICE.so.6 => /usr/lib/libICE.so.6 (0x76b04000)
        libpangox-1.0.so.0 => /usr/local/lib/libpangox-1.0.so.0 (0x76af5000)
        libX11.so.6 => /usr/lib/libX11.so.6 (0x769da000)
        libgmodule-2.0.so.0 => /usr/local/lib/libgmodule-2.0.so.0 (0x769d6000)
        librt.so.1 => /lib/librt.so.1 (0x769cd000)
        libglade-2.0.so.0 => /usr/lib/libglade-2.0.so.0 (0x769b6000)
        libxml2.so.2 => /usr/lib/libxml2.so.2 (0x7687f000)
        libpthread.so.0 => /lib/libpthread.so.0 (0x76867000)
        libc.so.6 => /lib/libc.so.6 (0x76707000)
        /lib/ld-linux.so.2 (0x7777c000)
        libXcomposite.so.1 => /usr/lib/libXcomposite.so.1 (0x76704000)
        libXdamage.so.1 => /usr/lib/libXdamage.so.1 (0x76701000)
        libXfixes.so.3 => /usr/lib/libXfixes.so.3 (0x766fb000)
        libXext.so.6 => /usr/lib/libXext.so.6 (0x766ed000)
        libXrender.so.1 => /usr/lib/libXrender.so.1 (0x766e4000)
        libXinerama.so.1 => /usr/lib/libXinerama.so.1 (0x766e1000)
        libXi.so.6 => /usr/lib/libXi.so.6 (0x766d8000)
        libXrandr.so.2 => /usr/lib/libXrandr.so.2 (0x766d1000)
        libXcursor.so.1 => /usr/lib/libXcursor.so.1 (0x766c7000)
        libgthread-2.0.so.0 => /usr/local/lib/libgthread-2.0.so.0 (0x766c5000)
        libffi.so.6 => /usr/lib/libffi.so.6 (0x766bb000)
        libdl.so.2 => /lib/libdl.so.2 (0x766b7000)
        libz.so.1 => /usr/lib/libz.so.1 (0x766a3000)
        libresolv.so.2 => /lib/libresolv.so.2 (0x7668c000)
        libstdc++.so.6 => /usr/local/gcc-4.7.2/lib/libstdc++.so.6 (0x7659d000)
        libbz2.so.1 => /lib/libbz2.so.1 (0x7658c000)
        libexpat.so.1 => /usr/lib/libexpat.so.1 (0x76566000)
        libpng12.so.0 => /usr/lib/libpng12.so.0 (0x76526000)
        libpixman-1.so.0 => /usr/local/lib/libpixman-1.so.0 (0x76220000)
        libxcb.so.1 => /usr/lib/libxcb.so.1 (0x76208000)
        libXau.so.6 => /usr/lib/libXau.so.6 (0x76205000)
        libXdmcp.so.6 => /usr/lib/libXdmcp.so.6 (0x76200000)
        libgcc_s.so.1 => /usr/local/gcc-4.7.2/lib/libgcc_s.so.1 (0x761e5000)
        libnvidia-tls.so.304.64 => /usr/lib/tls/libnvidia-tls.so.304.64 (0x761e1000)
        libnvidia-glcore.so.304.64 => /usr/lib/libnvidia-glcore.so.304.64 (0x744db000)
        libuuid.so.1 => /lib/libuuid.so.1 (0x744d7000)
```

## build

$ git clone git://github.com/mbohun/gtk_shader_ide.git

$ ./autogen.sh

$ ./configure

$ make

if you do not want to build it grab prebuilt binary from the [release](https://github.com/mbohun/gtk_shader_ide/tree/master/release) subdir or from my [fileserver](http://users.on.net/~mbohun/src/2004-10-19-demo_shader_ide) (note you need both the binary and the .glade file)

## run & test
$ cd src

$ ./gtk_shader_ide

Copy & paste the test !!ARBvp1.0 program [doc/test.arbvp](https://raw.github.com/mbohun/gtk_shader_ide/master/doc/test.arbvp) into the "Vertex Shader" editor

![Alt text](https://raw.github.com/mbohun/gtk_shader_ide/master/doc/session/00-arbvp.png "test.arbvp")

Switch to the "Fragment Shader" editor and copy & paste in the !!ARBfp1.0 program [doc/basic.arbfp](https://raw.github.com/mbohun/gtk_shader_ide/master/doc/basic.arbfp) and press the "Compile and Execute Shaders" toolbar button

![Alt text](https://raw.github.com/mbohun/gtk_shader_ide/master/doc/session/01-arbfp-basic.png "basic.arbfp")

Delete the fragment program from the "Fragment Shader" editor and copy & paste in [doc/grayscale.arbfp](https://raw.github.com/mbohun/gtk_shader_ide/master/doc/grayscale.arbfp) and press the "Compile and Execute Shaders" toolbar button again

![Alt text](https://raw.github.com/mbohun/gtk_shader_ide/master/doc/session/02-arbfp-grayscale.png "grayscale.arbfp")

Some of my (old) notes on OpenGL !!ARBvp1.0/!!ARBfp1.0 programming are available from [http://users.on.net/~mbohun/src/3D/bump_mapping/doc](http://users.on.net/~mbohun/src/3D/bump_mapping/doc), have fun ...

## TODO:
-----
- add proper signal handling, etc.
- add support for some configuration file (~/.gtk_shader_iderc or ~/.config/gtk_shader_ide/iderc)
- embed the XML GUI (src/gtk_shader_ide.glade) into the executable
- replace the handwritten syntax highlighting function with [gtksourceview](http://projects.gnome.org/gtksourceview) (if possible - as in not going to pull in whole GNOME project :-) as a _dependency_ )
- expose OpenGL state to the IDE
- add features like loading textures, etc.
