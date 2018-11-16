# Windows
Refer to the following instructions for compiling ams_lib.so with Visual Studio:

1. Set build configuration to Release (X.Y).
2. Set build platform to x86 or x64.
2. Build Solution.
3. Copy .../vs/[Win32/x64]/Release (X.Y)/ams_lib/ams_lib.so
   to the appropriate binary folder:
   RubyExtension/ams_Lib/libraries/stage/[win32/win64]/[X.Y]/

# Max OS X
Refer to instructions below for compiling ams_lib.bundle with xCode:

1. Set active scheme to "Ruby (X.Y) - [32/64]"
2. Execute (Menu) Product > Archive
3. Export the built archive to your documents.
4. Locate ams_lib.bundle within the exported archive.
5. Copy ams_lib.bundle to the appropriate binary folder:
   RubyExtension/ams_Lib/libraries/stage/[osx32/osx64]/[X.Y]/
