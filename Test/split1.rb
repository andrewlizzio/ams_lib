# Splits group/component instance at plane
def split(group, point, normal, entities, tra, recurse = true)
  # A fast way to split is by creating a copy of two groups
  # and then dividing it at plane
  # All objects behind plane will be deleted
  d = AMS::Group.get_definition(group)

  inst1 = entities.add_instance(d, tra)
  inst1.make_unique
  inst2 = entities.add_instance(d, tra)
  inst2.make_unique
  
  inst1.material = group.material
  inst2.material = group.material
  
  inst1.layer = group.layer
  inst2.layer = group.layer

  ents1 = inst1.definition.entities
  ents2 = inst2.definition.entities

  # Create a plane group for intersecting with
  pg = entities.add_group
  pgt = Geom::Transformation.new(point, normal)
  pg.move!(pgt)
  v = point - tra.origin
  mag = v.dot(normal)
  v -= AMS::Geometry.scale_vector(v, mag)
  x2 = inst1.bounds.diagonal + v.length
  x1 = x2 * 2.0
  pg.entities.add_face(
    Geom::Point3d.new(-x1,  x1, 0),
    Geom::Point3d.new( x1,  x1, 0),
    Geom::Point3d.new( x1, -x1, 0),
    Geom::Point3d.new(-x1, -x1, 0))
  pts = [
    Geom::Point3d.new(-x2,  x2, 0),
    Geom::Point3d.new( x2,  x2, 0),
    Geom::Point3d.new( x2, -x2, 0),
    Geom::Point3d.new(-x2, -x2, 0)]
  tx = tra.inverse * pgt
  pts.each { |pt| pt.transform!(tx) }
  f1 = ents1.add_face(pts)
  f2 = ents2.add_face(pts.reverse)
  f1e = f1 ? f1.edges : []
  f2e = f2 ? f2.edges : []
  ents1.intersect_with(false, tra, ents1, tra, true, pg)
  ents2.intersect_with(false, tra, ents2, tra, true, pg)
  f1e.each { |e| e.erase! if e.valid? }
  f2e.each { |e| e.erase! if e.valid? }
  pg.erase! if pg.valid?

  # Delete edges behind planes
  dir2 = tx.zaxis
  dir1 = dir2.reverse
  tpt = tx.origin
  ents1.grep(::Sketchup::Edge).each { |e|
    next unless e.valid?
    if (e.start.position - tpt).dot(dir1) > 1.0e-6 || (e.end.position - tpt).dot(dir1) > 1.0e-6
      e.erase!
    end
  }
  ents2.grep(::Sketchup::Edge).each { |e|
    next unless e.valid?
    if (e.start.position - tpt).dot(dir2) > 1.0e-6 || (e.end.position - tpt).dot(dir2) > 1.0e-6
      e.erase!
    end
  }

  # Validate
  if ents1.count == 0
    inst1.erase!
    inst1 = nil
  end
  if ents2.count == 0
    inst2.erase!
    inst2 = nil
  end

  if recurse
  # Recurse
  if inst1
    grps = []
    ents1.each { |e|
      if e.is_a?(::Sketchup::Group) || e.is_a?(::Sketchup::ComponentInstance)
        grps << e
      end
    }
    grps.each { |e|
      next unless e.valid?
      split_sub(e, tpt, dir1, ents1, e.transformation)
    }
  end

  if inst2
    grps = []
    ents2.each { |e|
      if e.is_a?(::Sketchup::Group) || e.is_a?(::Sketchup::ComponentInstance)
        grps << e
      end
    }
    grps.each { |e|
      next unless e.valid?
      split_sub(e, tpt, dir2, ents2, e.transformation)
    }
  end
  end

  # Return
  return [inst1, inst2]
end

# Splits group/component instance at plane
def split_sub(group, point, normal, entities, tra)
  # A fast way to split is by creating a copy of two groups
  # and then dividing it at plane
  # All objects behind plane will be deleted
  group.make_unique
  d = AMS::Group.get_definition(group)
  ents = d.entities

  # Create a plane group for intersecting with
  pg = entities.add_group
  pgt = Geom::Transformation.new(point, normal)
  pg.move!(pgt)
  v = point - tra.origin
  mag = v.dot(normal)
  v -= AMS::Geometry.scale_vector(v, mag)
  x2 = group.bounds.diagonal + v.length
  x1 = x2 * 2.0
  pg.entities.add_face(
    Geom::Point3d.new(-x1,  x1, 0),
    Geom::Point3d.new( x1,  x1, 0),
    Geom::Point3d.new( x1, -x1, 0),
    Geom::Point3d.new(-x1, -x1, 0))
  pts = [
    Geom::Point3d.new(-x2,  x2, 0),
    Geom::Point3d.new( x2,  x2, 0),
    Geom::Point3d.new( x2, -x2, 0),
    Geom::Point3d.new(-x2, -x2, 0)]
  tx = tra.inverse * pgt
  pts.each { |pt| pt.transform!(tx) }
  f = ents.add_face(pts.reverse)
  fe = f ? f.edges : []
  ents.intersect_with(false, tra, ents, tra, true, pg)

  fe.each { |e| e.erase! if e.valid? }
  pg.erase! if pg.valid?

  # Delete edges behind planes
  dir = tx.zaxis
  tpt = tx.origin
  ents.grep(::Sketchup::Edge).each { |e|
    next unless e.valid?
    if (e.start.position - tpt).dot(dir) > 1.0e-6 || (e.end.position - tpt).dot(dir) > 1.0e-6
      e.erase!
    end
  }

  # Validate
  if ents.count == 0
    group.erase!
    return
  end

  # Recurse
  grps = []
  ents.each { |e|
    if e.is_a?(::Sketchup::Group) || e.is_a?(::Sketchup::ComponentInstance)
      grps << e
    end
  }
  grps.each { |e|
    next unless e.valid?
    split_sub(e, tpt, dir, ents, e.transformation)
  }

  nil
end


model = Sketchup.active_model
model.start_operation('op', true)
begin
  e = model.selection.first
  a,b = split(e, Geom::Point3d.new(0, 0, 102), Z_AXIS, model.entities, e.transformation, true)
  model.commit_operation
rescue Exception => err
  model.abort_operation
  raise err
end