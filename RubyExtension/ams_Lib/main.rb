require 'ams_Lib'

# AMS is a top level namespace of AMS Library.
# @since 1.0.0
module AMS

  # @since 2.0.0
  IS_PLATFORM_WINDOWS = (RUBY_PLATFORM =~ /mswin|mingw/i ? true : false)

  # @since 2.0.0
  IS_PLATFORM_OSX = (RUBY_PLATFORM =~ /darwin/i ? true : false)

  # @since 2.0.0
  IS_PLATFORM_LINUX = (RUBY_PLATFORM =~ /linux/i ? true : false)

  # @since 3.5.0
  IS_RUBY_VERSION_18 = (RUBY_VERSION =~ /^1.8/ ? true : false)

  # @since 3.5.0
  IS_RUBY_VERSION_20 = (RUBY_VERSION =~ /^2.0/ ? true : false)

  # @since 3.5.0
  IS_RUBY_VERSION_22 = (RUBY_VERSION =~ /^2.2/ ? true : false)

  # @since 3.5.0
  IS_SKETCHUP_64BIT = ((::Sketchup.respond_to?('is_64bit?') && ::Sketchup.is_64bit?) ? true : false)

  # @since 3.5.0
  IS_SKETCHUP_32BIT = !IS_SKETCHUP_64BIT

  # @since 3.6.0
  SU_MAJOR_VERSION = ::Sketchup.version.to_i


  # Path to directory where temporary files can be created and destroyed.
  if IS_PLATFORM_WINDOWS
    if ENV['LOCALAPPDATA']
      dir = ::File.join(::File.expand_path(ENV['LOCALAPPDATA']), 'Temp')
    else
      dir = ::File.expand_path(ENV['TEMP'])
    end
  else
    dir = ::File.expand_path(ENV['TMPDIR'])
  end
  dir.force_encoding('UTF-8') unless IS_RUBY_VERSION_18
  # @since 2.0.0
  TEMP_DIR = dir.freeze


  dir = ::Sketchup.find_support_file("Plugins")
  if dir.is_a?(::String)
    dir = ::File.expand_path(dir).freeze
  else
    dir = nil
  end
  # @since 3.6.0
  SU_PLUGINS_LOCATION = dir.freeze

  class << self

    # Clamp value between minimum and maximum limits.
    # @param [Numeric] val
    # @param [Numeric, nil] min_val Pass +nil+ to have no min limit.
    # @param [Numeric, nil] max_val Pass +nil+ to have no max limit.
    # @return [Numeric]
    # @since 2.0.0
    def clamp(val, min_val, max_val)
      if (min_val && val < min_val)
        min_val
      elsif (max_val && val > max_val)
        max_val
      else
        val
      end
    end

    # Get the minimum of two values
    # @param [Numeric] a
    # @param [Numeric] b
    # @return [Numeric]
    # @since 2.0.0
    def min(a, b)
      (a < b) ? a : b
    end

    # Get the maximum of two values
    # @param [Numeric] a
    # @param [Numeric] b
    # @return [Numeric]
    # @since 2.0.0
    def max(a, b)
      (a > b) ? a : b
    end

    # Get sign of a numeric value.
    # @param [Numeric] val
    # @return [Fixnum] -1, 0, or 1
    # @since 2.0.0
    def sign(val)
      if val > 0
        return 1
      elsif val < 0
        return -1
      else
        return 0
      end
    end

if AMS::IS_PLATFORM_WINDOWS

    # @since 3.6.0
    def refresh_toolbars
    end

elsif AMS::SU_MAJOR_VERSION >= 18

    def refresh_toolbars
      ::UI.refresh_toolbars
    end

else

    def refresh_toolbars
      model = ::Sketchup.active_model
      return unless model
      model.tools.push_tool(nil)
      model.tools.pop_tool
    end

end

  end # class << self
end # module AMS

unless file_loaded?(__FILE__)
  file_loaded(__FILE__)

  dir = File.dirname(__FILE__)
  dir.force_encoding('UTF-8') unless AMS::IS_RUBY_VERSION_18

  Sketchup.require(File.join(dir, 'extension_manager'))

  ext_manager = AMS::ExtensionManager.new(dir, AMS::Lib::VERSION, false)
  ext_manager.add_c_extension('ams_lib')
  ext_manager.add_ruby_no_require('main')
  ext_manager.add_ruby_no_require('extension_manager')
  ext_manager.add_ruby('translate')
  ext_manager.require_all
  ext_manager.clean_up(true)
end
