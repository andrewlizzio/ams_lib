def split(group, point, normal, parent_entities, transformation, recurse = true)
  d = AMS::Group.get_definition(group)

  inst1 = parent_entities.add_instance(d, transformation)
  inst2 = parent_entities.add_instance(d, transformation)

  inst1.material = group.material
  inst1.layer = group.layer
  inst1.name = group.name
  inst1.visible = group.visible?

  inst2.material = group.material
  inst2.layer = group.layer
  inst2.name = group.name
  inst2.visible = group.visible?

  res1 = split_sub(inst1, point, normal, parent_entities, recurse)
  res2 = split_sub(inst2, point, normal.reverse, parent_entities, recurse)

  return [res1, res2]
end

# Splits group/component instance at plane
def split_sub(group, point, normal, parent_entities, recurse)
  # A fast way to split is by creating a copy of two groups
  # and then dividing it at plane
  # All objects behind plane will be deleted
  group.make_unique
  gents = AMS::Group.get_entities(group)
  transformation = group.transformation

  # Shift point so that it is aligned with the group's center relative to plane normal
  x = (point - group.bounds.center).dot(normal)
  point2 = group.bounds.center + AMS::Geometry.scale_vector(normal, x)

  # Create a plane group for intersecting with
  pg = parent_entities.add_group
  pg_tra = Geom::Transformation.new(point2, normal)
  pg.move!(pg_tra)
  # Compute local transformation of group
  pg_tra_loc = transformation.inverse * pg_tra
  # Compute face dimensions
  v = point2 - transformation.origin
  mag = v.dot(normal)
  v -= AMS::Geometry.scale_vector(normal, mag)
  x1 = (group.bounds.diagonal + v.length) * 0.5
  x2 = x1 * 2.0
  # Add face to group
  f = gents.add_face(
    Geom::Point3d.new(-x1, -x1, 0).transform(pg_tra_loc),
    Geom::Point3d.new( x1, -x1, 0).transform(pg_tra_loc),
    Geom::Point3d.new( x1,  x1, 0).transform(pg_tra_loc),
    Geom::Point3d.new(-x1,  x1, 0).transform(pg_tra_loc))
  # Add face to group
  pg.entities.add_face(
    Geom::Point3d.new(-x2, -x2, 0),
    Geom::Point3d.new( x2, -x2, 0),
    Geom::Point3d.new( x2,  x2, 0),
    Geom::Point3d.new(-x2,  x2, 0))
  # Obtain face outer edges for erasing them in the end
  fe = f ? f.edges : []
  # Intersect group with plane
  gents.intersect_with(false, transformation, gents, transformation, true, pg)
  # Erase the outer edges
  fe.each { |e| e.erase! if e.valid? }
  # Erase the plane group
  pg.erase! if pg.valid?

  # Delete edges behind planes
  tpt = pg_tra_loc.origin
  dir = pg_tra_loc.zaxis
  gents.grep(::Sketchup::Edge).each { |e|
    next unless e.valid?
    if (e.start.position - tpt).dot(dir).to_f > 1.0e-4 || (e.end.position - tpt).dot(dir).to_f > 1.0e-4
      e.erase!
    end
  }

  # Recurse
  if recurse
    sub_groups = []
    gents.each { |e|
      if e.is_a?(::Sketchup::Group) || e.is_a?(::Sketchup::ComponentInstance)
        sub_groups << e
      end
    }
    sub_groups.each { |e|
      next unless e.valid?
      split_sub(e, tpt, dir, gents, true)
    }
  end

  # Validate and return
  if gents.count > 0
    return group
  else
    group.erase!
    return nil
  end
end

model = Sketchup.active_model
model.start_operation('op', true)
begin
  e = model.selection.first
  a,b = split(e, Geom::Point3d.new(0, 0, 110), Z_AXIS, model.entities, e.transformation, true)
  model.definitions.purge_unused
  model.commit_operation
rescue Exception => err
  model.abort_operation
  raise err
end