model = Sketchup.active_model
model.start_operation('op', true)
begin
  e = model.selection.first
  res = AMS::Group.split(e, Geom::Point3d.new(0, 0, 110), Z_AXIS, model.entities, e.transformation, true) { |g|
    g.is_a?(Sketchup::ComponentInstance) ? false : true
  }
  model.definitions.purge_unused
  model.commit_operation
rescue Exception => err
  model.abort_operation
  raise err
end


model = Sketchup.active_model
model.start_operation('op', true)
begin
  e = model.selection.first
  res = AMS::Group.copy(e, model.entities, ORIGIN, true) { |g|
    g.is_a?(Sketchup::ComponentInstance) ? false : true
  }
  model.definitions.purge_unused
  model.commit_operation
rescue Exception => err
  model.abort_operation
  raise err
end


e = Sketchup.active_model.selection.first
m = e.transformation.to_a
t = AMS::Geometry.extract_matrix_scale(m)
e.transformation = t


pt1 = Geom::Point3d.new(1,5,10)
pt2 = Geom::Point3d.new(0,0,2)
zaxis = Geom::Vector3d.new(0,0,-5)
x = (pt2 - pt1).dot(zaxis)
pt3 = pt1 + AMS::Geometry.scale_vector(zaxis, x / (zaxis.length * zaxis.length))