# AMS Library

## Description

AMS Library is a collection of functions and utilities for interacting with
SketchUp window and its input procedures via the Microsoft Windows API. Features
include switching SketchUp fullscreen, monitoring window state changes,
monitoring keyboard and mouse events, and preventing the interference of
SketchUp keyboard and mouse shortcut accelerators. Such features provide
extension developers with additional control for their tool. In addition to
utilizing SketchUp tool events, a developer can utilize AMS Library's keyboard
and mouse callback events to receive input of all the messages, including the
mouse wheel. AMS Library's callback events are registered in form of observers
(although they are also modifiers), meaning a tool does not necessarily have to
be an active tool in order to receive input events. This allows for operating
extensions while other extensions are operating. In addition to the observer and
modifier procedures, AMS Library provides Windows API functionality for tweaking
dialogs to a next level, including removing the surrounding window frame and
applying window transparency. Furthermore, AMS Library comes with various
geometry and entity hierarchy manipulation functions. In a way, AMS Library
provides developers with functionality not achievable with SketchUp Ruby API.


## Usage

AMS Library is intended to be a dependency extension for other extensions.
Extensions planning to use AMS Library need to verify AMS Library, of a specific
version, is installed, shown in the following sample:
```ruby
# FILE: main_entry.rb

tload_me = true

# Load and verify AMS Library
begin
  # Attempt to load
  require 'ams_Lib/main'

  # Verify version
  tload_me = false if AMS::Lib::VERSION.to_f < 3.6
  
rescue LoadError
  tload_me = false

end

if tload_me
  # Load the main file
  dir = ::File.expand_path(::File.dirname(__FILE__))
  dir.force_encoding('UTF-8') if RUBY_VERSION !~ /^1.8/
  require File.join(dir, "main")

else
  msg = "[MY_EXTENSION_NAME] requires AMS Library, version 3.6.0 or later! "
  msg << "This extension will not be loaded with the library not installed or outdated. "
  msg << "Would you like to navigate to the library's download page?"
  tload_me = false
  if ::UI.messagebox(msg, MB_YESNO) == IDYES
    ::UI.openURL('http://sketchucation.com/forums/viewtopic.php?f=323&t=55067#p499835')
  end

end
```

When registering your extension, have it load the <tt>main_entry</tt> file:
```ruby
dir = ::File.expand_path(::File.dirname(__FILE__))
dir.force_encoding('UTF-8') if RUBY_VERSION !~ /^1.8/
fpath = :File.join(dir, "MY_EXTENSION_NAME/main_entry")
extension = ::SketchupExtension.new(MY_EXTENSION_NAME, fpath)
```


## Synopsis
```ruby
# FILE: main.rb

require 'ams_Lib/main'

# Get handle to SketchUp window.
AMS::Sketchup.get_main_window

# Setting SketchUp full screen on the monitor SU window is associated to.
AMS::Sketchup.switch_full_screen(true)

# Setting SketchUp full screen on all monitors.
AMS::Sketchup.switch_full_screen(true, 2, 2)

# Monitoring and processing SketchUp window events.
class MySketchupObserver

  def swo_on_switch_full_screen(state)
    if state
      puts 'Main window switched full screen!'
    else
      puts 'Main window switched to original placement.'
    end
  end

  def swp_on_mouse_wheel_rotate(x,y, dir)
    puts "mouse wheel rotated - pos : (#{x}, #{y}), dir : #{dir}"
    # Prevent mouse wheel from interacting with SU window. Returning 1 means
    # mouse wheel zoom in/out operation would be blocked, which might be
    # handy for those seeking more control over SketchUp window. Returning
    # any other value won't block the event.
    return 1
  end

end # class MySketchupObserver

AMS::Sketchup.add_observer(MySketchupObserver.new)
```

## Requirements

* Microsoft Windows XP, Vista, 7, 8, 10.
* Mac OS X 10.6+ (Limited)
* SketchUp 6 or later.


## Installation Instructions

AMS Library releases are available at <i>Extension Warehouse</i> or <i>SketchUcation Extension Store</i>.

To download from the repository, copy <tt>ams_Lib</tt> folder and <tt>ams_Lib.rb</tt>,
located at <tt>RubyExtension/</tt>, to your plugins folder.
