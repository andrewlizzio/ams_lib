# @since 3.3.0
module AMS::Group
  class << self

    # Get group/component instance definition.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @return [Sketchup::ComponentDefinition]
    def get_definition(object)
    end

    # Get group/component instance entities.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @return [Sketchup::Entities]
    def get_entities(object)
    end

    # Compute group/component-instance axes-aligned bounding-box from edges.
    # @note This function was revised in version 3.6.0.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @param [Geom::Transformation, nil] transformation A coordinate system in
    #   which the computation is to be performed. Usually this parameter is set
    #   to the coordinate system +object+ is associated to. Pass +nil+ to have
    #   the computation performed in +object+'s local space.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Geom::BoundingBox]
    # @since 3.5.0
    def get_bounding_box_from_edges(object, recurse = true, transformation = nil, &entity_validation)
    end

    # Compute group/component-instance axes-aligned bounding-box from faces.
    # @note This function was revised in version 3.6.0.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @param [Geom::Transformation, nil] transformation A coordinate system in
    #   which the computation is to be performed. Usually this parameter is set
    #   to the coordinate system +object+ is associated to. Pass +nil+ to have
    #   the computation performed in +object+'s local space.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Geom::BoundingBox]
    def get_bounding_box_from_faces(object, recurse = true, transformation = nil, &entity_validation)
    end

    # Get group/component-instance edges.
    # @note This function was revised in version 3.6.0.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @param [Geom::Transformation, nil] transformation A coordinate system in
    #   which the computation is to be performed. Usually this parameter is set
    #   to the coordinate system +object+ is associated to. Pass +nil+ to have
    #   the computation performed in +object+'s local space.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Array<Array<Geom::Point3d>>] An array of edges. Each edge
    #   represents an array of two points.
    def get_edges(object, recurse = true, transformation = nil, &entity_validation)
    end

    # Get group/component-instance edge vertices.
    # @note This function was revised in version 3.6.0.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @param [Geom::Transformation, nil] transformation A coordinate system in
    #   which the computation is to be performed. Usually this parameter is set
    #   to the coordinate system +object+ is associated to. Pass +nil+ to have
    #   the computation performed in +object+'s local space.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Array<Geom::Point3d>] An array of points.
    def get_vertices_from_edges(object, recurse = true, transformation = nil, &entity_validation)
    end

    # Get group/component-instance faces.
    # @note This function was revised in version 3.6.0.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @param [Geom::Transformation, nil] transformation A coordinate system in
    #   which the computation is to be performed. Usually this parameter is set
    #   to the coordinate system +object+ is associated to. Pass +nil+ to have
    #   the computation performed in +object+'s local space.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Array<Array<Geom::Point3d>>] An array of faces. Each face
    #   represents an array of points.
    def get_faces(object, recurse = true, transformation = nil, &entity_validation)
    end

    # Get group/component-instance face vertices.
    # @note This function was revised in version 3.6.0.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @param [Geom::Transformation, nil] transformation A coordinate system in
    #   which the computation is to be performed. Usually this parameter is set
    #   to the coordinate system +object+ is associated to. Pass +nil+ to have
    #   the computation performed in +object+'s local space.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Array<Geom::Point3d>] An array of points.
    def get_vertices_from_faces(object, recurse = true, transformation = nil, &entity_validation)
    end

    # Get group/component-instance face vertices collections.
    # @note This function was revised in version 3.6.0.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @param [Geom::Transformation, nil] transformation A coordinate system in
    #   which the computation is to be performed. Usually this parameter is set
    #   to the coordinate system +object+ is associated to. Pass +nil+ to have
    #   the computation performed in +object+'s local space.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Array<Array<Geom::Point3d>>] An array of point collections
    #   from every included sub-group/sub-component instance. Each point
    #   collection represents an array of points.
    def get_vertices_from_faces2(object, recurse = true, transformation = nil, &entity_validation)
    end

    # Get group/component-instance construction point and line positions.
    # @note This function was revised in version 3.6.0.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @param [Geom::Transformation, nil] transformation A coordinate system in
    #   which the computation is to be performed. Usually this parameter is set
    #   to the coordinate system +object+ is associated to. Pass +nil+ to have
    #   the computation performed in +object+'s local space.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Array<Geom::Point3d>] An array of points.
    def get_construction(object, recurse = true, transformation = nil, &entity_validation)
    end

    # Get group/component-instance face triplets.
    # @note This function was revised in version 3.6.0.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @param [Geom::Transformation, nil] transformation A coordinate system in
    #   which the computation is to be performed. Usually this parameter is set
    #   to the coordinate system +object+ is associated to. Pass +nil+ to have
    #   the computation performed in +object+'s local space.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Array<Array<Geom::Point3d>>] An array of polygons. Each polygon
    #   represents an array of three points - a triplet.
    def get_polygons_from_faces(object, recurse = true, transformation = nil, &entity_validation)
    end

    # Get group/component-instance triangular mesh.
    # @note This function was revised in version 3.6.0.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @param [Geom::Transformation, nil] transformation A coordinate system in
    #   which the computation is to be performed. Usually this parameter is set
    #   to the coordinate system +object+ is associated to. Pass +nil+ to have
    #   the computation performed in +object+'s local space.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Geom::PolygonMesh] Everything merged into one mesh.
    def get_triangular_mesh(object, recurse = true, transformation = nil, &entity_validation)
    end

    # Get group/component-instance triangular mesh collections. Each sub-group,
    # sub-component, a collection of connected faces is reserved its own mesh.
    # @note This function was revised in version 3.6.0.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @param [Geom::Transformation, nil] transformation A coordinate system in
    #   which the computation is to be performed. Usually this parameter is set
    #   to the coordinate system +object+ is associated to. Pass +nil+ to have
    #   the computation performed in +object+'s local space.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Array<Geom::PolygonMesh>] An array of meshes.
    def get_triangular_meshes(object, recurse = true, transformation = nil, &entity_validation)
    end

    # Calculate group/component-instance centre of mass from faces.
    # @note This method only works with convex groups.
    # @note This function was revised in version 3.6.0.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @param [Geom::Transformation, nil] transformation A coordinate system in
    #   which the computation is to be performed. Usually this parameter is set
    #   to the coordinate system +object+ is associated to. Pass +nil+ to have
    #   the computation performed in +object+'s local space.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Geom::Point3d]
    def calc_centre_of_mass(object, recurse = true, transformation = nil, &entity_validation)
    end

    # Copy group/component-instance without including the undesired entities.
    # @note This function was revised in version 3.6.0.
    # @note Make sure to wrap this function with a start/commit operation to
    #   avoid polluting the undo stack.
    # @note The original +object+ is not modified in any way.
    # @note If a block is passed, copied object is made unique.
    # @note The main, copied group is assigned its original name, layer,
    #   material, visibility state, casts/receives shadows state; attributes
    #   are not copied.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Sketchup::Entities] context The entities context to paste into.
    # @param [Geom::Transformation, nil] transformation A transformation matrix,
    #   relative to +context+, to apply to the resulting group. Pass +nil+
    #   to have the resulting group be applied the matrix of +object+.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Sketchup::Group, Sketchup::ComponentInstance, nil] A copied group
    #   or +nil+ if nothing copied.
    def copy(object, context, transformation = nil, recurse = true, &entity_validation)
    end

    # Split group/component-instance at plane.
    # @note Make sure to wrap this function with a start/commit operation to
    #   avoid polluting the undo stack.
    # @note The original +object+ is not modified in any way.
    # @note Copied groups are assigned their original name, layer,
    #   material, visibility state, casts/receives shadows state; attributes
    #   are not copied.
    # @param [Sketchup::Group, Sketchup::ComponentInstance] object A group or a
    #   component instance.
    # @param [Geom::Point3d] point A point on plane in +context+ coordinates.
    # @param [Geom::Vector3d] normal Plane normal in +context+ coordinates.
    # @param [Sketchup::Entities] context The entities context to paste into.
    # @param [Geom::Transformation, nil] transformation A transformation matrix,
    #   relative to +context+, to apply to the resulting groups. Pass +nil+
    #   to have the resulting groups be applied the matrix of +object+.
    # @param [Boolean] recurse Whether to process sub-groups and sub-component
    #   instances.
    # @yield A procedure for determining whether a particular sub-group or a
    #   sub-component-instance is to be considered a part of the operation.
    # @yieldparam [Sketchup::Group, Sketchup::ComponentInstance] sub_entity
    # @yieldreturn [Boolean] Pass +true+ to have +sub_entity+ included in the
    #   operation; pass +false+ to have +sub_entity+ ignored.
    # @return [Array<Sketchup::Group, Sketchup::ComponentInstance, nil>] An
    #   array of two items. Each item is either a group, a component-instance,
    #   or nil if turned out empty, during the split operation.
    # @since 3.6.0
    def split(object, point, normal, context, transformation = nil, recurse = true, &entity_validation)
    end

  end # class << self
end # module AMS::Group
