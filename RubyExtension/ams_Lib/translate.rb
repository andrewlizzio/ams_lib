module AMS

  # Translate allows loading and controlling localization of strings.
  # @since 2.0.0
  class Translate

    # Load translation strings.
    # @param [String] toolname Base name of the translation file.
    # @param [String] dir Path to the directory with translation files
    # @param [String, nil] locale Specific language to look for or
    #   <tt>nil</tt> to relent on current localization.
    # @note The extension of a translation file must be <tt>.strings</tt>.
    #   Toolname and locale must be separated with an underscore.
    #   Example: <tt>commands_ru.strings</tt> or <tt>commands_pt-BR.strings</tt>.
    #   File names are not case sensitive.
    # @note The commands within a translate file must following the
    #   <tt>"base lang" = "locale lang"</tt> syntax.
    #   A key-value pair cannot expand to multiple lines.
    #   Line comments (<tt>//</tt>) are supported too. If, however, comments are
    #   found within a string, they will be interpreted as a string.
    #   Spaces and newlines don't affect the syntax. Escape characters within
    #   strings don't affect the syntax either.
    # @note No errors will be raised if a translation file is invalid.
    def initialize(toolname, dir, locale = nil)
      @strings = {}
      locale = ::Sketchup.get_locale unless locale
      # Configure path
      fpath = ::File.join(dir.to_s, toolname.to_s + '_' + locale.to_s + '.strings')
      fpath.force_encoding("UTF-8") unless AMS::IS_RUBY_VERSION_18
      return unless ::File.exists?(fpath)
      # Parse from the file
      ::File.open(fpath, 'r') { |fp|
        fp.each { |line|
          ri = []
          besc = false
          in_str = false
          fslash = false
          for i in 0...line.length
            x = line[i, 1]
            if in_str
              if besc
                besc = false
              elsif x == "\\"
                besc = true
              elsif x == "\""
                ri << i
                in_str = false
              end
            elsif x == "\""
              ri << i + 1
              in_str = true
              fslash = false
            elsif x == "/"
              if fslash
                break
              else
                fslash = true
              end
            end
          end
          if ri.size == 4
            key = line[ri[0], ri[1] - ri[0]]
            val = line[ri[2], ri[3] - ri[2]]
            @strings[key] = val
          end
        }
      }
    end

    # Get a single translation.
    # @param [String] key A string to translate
    # @param [*String] si Optional strings for substitution of %0 ... %n.
    # @return [String] Translated string
    def get(key, *si)
      value = (@strings[key.to_s] || key.to_s).dup
      # Substitute additional strings
      si.compact.each_with_index { |s, i|
        value.gsub!(/\%#{i}/, s.to_s)
      }
      return value
    end

    alias_method(:[], :get)

    # Get all translations as hash.
    # @return [Hash{String => String}] key/value pairs of original and
    #   translated strings.
    def get_all
      return @strings
    end

    # Replace all <tt>TRANSLATE[some string]</tt> within a text to the
    # associated translation.
    # @note If a translation doesn't exist, the expression is replaced with the
    #   string itself.
    # @param [String] text
    # @param [*String] si Optional strings for substitution of %0 ... %n.
    # @return [String]
    # @since 3.6.0
    def convert_text(text, *si)
      text2 = text.dup
      text2.force_encoding("UTF-8") unless AMS::IS_RUBY_VERSION_18
      i = nil
      offset = 0
      while (i = text2.index("TRANSLATE[", offset))
        j = i + 10
        while true
          x = text2[j, 1]
          if x == "]"
            break
          elsif x.nil?
            raise(SyntaxError, "The \"TRANSLATE\" string, starting at index #{i}, is missing a closing bracket!")
          end
          j += 1
        end
        key = text2[i + 10, j - i - 10]
        val = (@strings[key] || key)
        text2[i, j - i + 1] = val
        offset = i + val.length
      end
      # Substitute additional strings
      si.compact.each_with_index { |s, i|
        text2.gsub!(/\%#{i}/, s.to_s)
      }
      return text2
    end

  end # class Translate
end # module AMS
