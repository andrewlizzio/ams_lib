module AMS
  class << self

    # Convert a Ruby object to string that can be evaluated back into the same
    # object.
    # @param [nil, Boolean, Fixnum, Bignum, Float, String, Symbol, Array, Hash]
    #   item An object to convert. An Array and a Hash can only contain objects
    #   of +NilClass+, +TrueClass+, +FalseClass+, +Fixnum, +Float+, +Bignum+,
    #   +String+, +Symbol+, +Array+, and +Hash+ types. This, as well, applies to
    #   sub-arrays and sub-hashes.
    # @raise [TypeError] if given item or sub-item type is not supported.
    # @since 3.1.0
    def inspect_element(item)
    end

    # Validate object type.
    # @param [Object] object
    # @param [Class, Array<Class>] types A class or an array of classes to
    #   check against.
    # @return [void]
    # @raise [TypeError] if object class doesn't match with any of the specified
    #   types.
    # @since 2.0.0
    def validate_type(object, *types)
    end

    # Clamp floating value between min and max.
    # @param [Numeric] value
    # @param [Numeric] min
    # @param [Numeric] max
    # @return [Numeric]
    # @since 3.6.0
    def clamp_real(value, min, max)
    end

    # Get sign of a floating value.
    # @param [Numeric] value
    # @return [Fixnum] -1 or 1
    # @since 3.6.0
    def sign_real(value)
    end

    # Get the least of two floating values.
    # @param [Numeric] a
    # @param [Numeric] b
    # @return [Numeric]
    # @since 3.6.0
    def min_real(a, b)
    end

    # Get the greatest of two floating values.
    # @param [Numeric] a
    # @param [Numeric] b
    # @return [Numeric]
    # @since 3.6.0
    def max_real(a, b)
    end

    # Clamp integer value between min and max.
    # @param [Fixnum] value
    # @param [Fixnum] min
    # @param [Fixnum] max
    # @return [Fixnum]
    # @since 3.6.0
    def clamp_int(value, min, max)
    end

    # Get sign of a integer value.
    # @param [Fixnum] value
    # @return [Fixnum] -1 or 1
    # @since 3.6.0
    def sign_int(value)
    end

    # Get the least of two integer values.
    # @param [Fixnum] a
    # @param [Fixnum] b
    # @return [Fixnum]
    # @since 3.6.0
    def min_int(a, b)
    end

    # Get the greatest of two integer values.
    # @param [Fixnum] a
    # @param [Fixnum] b
    # @return [Fixnum]
    # @since 3.6.0
    def max_int(a, b)
    end

    # Round a numeric value to a particular number of decimal places.
    # @param [Numeric] number
    # @param [Fixnum] precision
    # @return [Numeric]
    # @since 3.3.0
    def round(number, precision = 0)
    end

    # Determine whether an object is a +true+ or +false+.
    # @return [Boolean]
    # @since 2.0.0
    def is_boolean?(object)
    end

    alias is_bool? is_boolean?

    # Get entity by entity ID.
    # @param [Fixnum] id
    # @return [Sketchup::Entity, nil]
    # @since 2.0.0
    def get_entity_by_id(id)
    end

    # Get top level entity by entity ID.
    # @param [Fixnum] id
    # @return [Sketchup::Entity, nil]
    # @since 3.0.0
    def get_top_entity_by_id(id)
    end

  end # class << self
end # module AMS
