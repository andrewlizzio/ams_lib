# AMS Library

[Homepage](http://sketchucation.com/forums/viewtopic.php?f=323&t=55067#p499835)

[GitHub](https://github.com/AntonSynytsia/AMS-Library)

[Documentation](http://www.rubydoc.info/github/AntonSynytsia/AMS-Library/master/index)


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


## Synopsis
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


## Requirements

* Microsoft Windows XP, Vista, 7, 8, 10.
* Mac OS X 10.5+ (Limited)
* SketchUp 6 or later.
