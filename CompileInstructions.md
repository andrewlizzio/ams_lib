# Windows
Refer to the following instructions for compiling <tt>ams_lib.so</tt> with Visual Studio:

1. Set build configuration to <tt>Release (X.Y)</tt>.
2. Set build platform to <tt>x86</tt> or <tt>x64</tt>.
2. Build Solution.
3. Copy <tt>.../VS/[Win32/x64]/Release (X.Y)/ams_lib/ams_lib.so</tt>
   to the appropriate binary folder:
   <tt>RubyExtension/ams_Lib/libraries/stage/[win32/win64]/[X.Y]/</tt>

# Max OS X
Refer to the following instructions for compiling <tt>ams_lib.bundle</tt> with xCode:

1. Set active scheme to <tt>Ruby (X.Y) - [32/64]</tt>
2. Execute <tt>(Menu) Product > Archive</tt>
3. Export the built archive to your documents.
4. Locate <tt>ams_lib.bundle</tt> within the exported archive.
5. Copy <tt>ams_lib.bundle</tt> to the appropriate binary folder:
   <tt>RubyExtension/ams_Lib/libraries/stage/[osx32/osx64]/[X.Y]/</tt>
