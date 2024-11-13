-- Define materials
red = gr.material({0.8, 0.1, 0.1}, {0.0, 0.0, 0.0}, 0)
green = gr.material({0.1, 0.8, 0.1}, {0.0, 0.0, 0.0}, 0)
white = gr.material({0.8, 0.8, 0.8}, {0.0, 0.0, 0.0}, 0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)

-- Create the Cornell Box structure

scene = gr.node('scene')

-- Floor
floor = gr.nh_box('floor', {0, 0, 0}, 1)
scene:add_child(floor)
floor:set_material(white)
floor:scale(6, 0.1, 6)
floor:translate(-3, -3, -3)

-- Ceiling
ceiling = gr.nh_box('ceiling', {0, 0, 0}, 1)
scene:add_child(ceiling)
ceiling:set_material(white)
ceiling:scale(6, 0.1, 6)
ceiling:translate(-3, 3, -3)

-- Back Wall
back_wall = gr.nh_box('back_wall', {0, 0, 0}, 1)
scene:add_child(back_wall)
back_wall:set_material(white)
back_wall:scale(6, 6, 0.1)
back_wall:translate(-3, -3, -3)

-- Left Wall
left_wall = gr.nh_box('left_wall', {0, 0, 0}, 1)
scene:add_child(left_wall)
left_wall:set_material(red)
left_wall:scale(0.1, 6, 6)
left_wall:translate(-3, -3, -3)

-- Right Wall
right_wall = gr.nh_box('right_wall', {0, 0, 0}, 1)
scene:add_child(right_wall)
right_wall:set_material(green)
right_wall:scale(0.1, 6, 6)
right_wall:translate(3, -3, -3)

-- Cow Model in the center
cow_poly = gr.mesh('cow', 'cow.obj')
scene:add_child(cow_poly)
cow_poly:set_material(hide)
cow_poly:scale(0.4, 0.4, 0.4)
cow_poly:translate(0, -1, 0)  -- Adjust as needed for cow placement

-- Sphere in front of the cow
sphere = gr.nh_sphere('sphere', {0, 0, 0}, 1)
scene:add_child(sphere)
sphere:set_material(white)
sphere:scale(0.9, 0.9, 0.9)
sphere:translate(-1.5, -2, 1.5)

-- Cube in front of the cow
cube = gr.nh_box('cube', {0, 0, 0}, 1)
scene:add_child(cube)
cube:set_material(white)
cube:scale(0.8, 1.5, 0.8)
cube:translate(0.8, -3, 2)

-- Light source
light_position = {0, 2.9, 2.5}
light_color = {0.8, 0.8, 0.8}
light_falloff = {1, 0, 0}

gr.render(scene,
          'cornell-box-cow.png', 512, 512,
          {0, 0, 10}, {0, 0, -1}, {0, 1, 0}, 50,
          {0.2, 0.2, 0.2}, {gr.light(light_position, light_color, light_falloff)})
