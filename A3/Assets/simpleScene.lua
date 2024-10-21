root = gr.node('root')

-- 定义材料
red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

-- 创建头部（使用suzanne）
head_joint = gr.joint('head_joint', {-45, 0, 45}, {-45, 0, 45})
root:add_child(head_joint)

head = gr.mesh('suzanne', 'head')
head_joint:add_child(head)
head:scale(0.8, 0.8, 0.8)
head:translate(0.0, 1.5, 0.0)  -- 把头放在较高的位置
head:set_material(white)

-- 创建身体（用cube作为身体的主干）
body = gr.mesh('cube', 'body')
root:add_child(body)
body:scale(0.7, 1.5, 0.5)
body:translate(0.0, 0.0, 0.0)  -- 主体向下移
body:set_material(red)

-- 左臂（sphere表示关节，cube表示上臂和前臂）
left_shoulder_joint = gr.joint('left_shoulder_joint', {-45, 0, 45}, {-45, 0, 45})
body:add_child(left_shoulder_joint)
left_shoulder_joint:translate(-0.8, 1.0, 0.0)

left_upper_arm = gr.mesh('cube', 'left_upper_arm')
left_shoulder_joint:add_child(left_upper_arm)
left_upper_arm:scale(0.3, 0.7, 0.3)
left_upper_arm:translate(-0.3, -0.7, 0.0)
left_upper_arm:set_material(blue)

left_elbow_joint = gr.joint('left_elbow_joint', {-45, 0, 45}, {-45, 0, 45})
left_upper_arm:add_child(left_elbow_joint)
left_elbow_joint:translate(0.0, -0.7, 0.0)

left_lower_arm = gr.mesh('cube', 'left_lower_arm')
left_elbow_joint:add_child(left_lower_arm)
left_lower_arm:translate(0.0, -0.7, 0.0)
left_lower_arm:set_material(green)

-- 右臂
right_shoulder_joint = gr.joint('right_shoulder_joint', {-45, 0, 45}, {-45, 0, 45})
body:add_child(right_shoulder_joint)
right_shoulder_joint:translate(0.8, 1.0, 0.0)

right_upper_arm = gr.mesh('cube', 'right_upper_arm')
right_shoulder_joint:add_child(right_upper_arm)
right_upper_arm:scale(0.2, 0.7, 0.2)
right_upper_arm:translate(0.3, -0.7, 0.0)
right_upper_arm:set_material(blue)

right_elbow_joint = gr.joint('right_elbow_joint', {-45, 0, 45}, {-45, 0, 45})
right_upper_arm:add_child(right_elbow_joint)
right_elbow_joint:translate(0.0, -0.7, 0.0)

right_lower_arm = gr.mesh('cube', 'right_lower_arm')
right_elbow_joint:add_child(right_lower_arm)
right_lower_arm:scale(0.2, 0.7, 0.2)
right_lower_arm:translate(0.0, -0.7, 0.0)
right_lower_arm:set_material(green)

-- 左腿
left_hip_joint = gr.joint('left_hip_joint', {-45, 0, 45}, {-45, 0, 45})
body:add_child(left_hip_joint)
left_hip_joint:translate(-0.4, -1.5, 0.0)

left_thigh = gr.mesh('cube', 'left_thigh')
left_hip_joint:add_child(left_thigh)
left_thigh:scale(0.3, 0.8, 0.3)
left_thigh:translate(0.0, -0.8, 0.0)
left_thigh:set_material(blue)

left_knee_joint = gr.joint('left_knee_joint', {-45, 0, 45}, {-45, 0, 45})
left_thigh:add_child(left_knee_joint)
left_knee_joint:translate(0.0, -0.8, 0.0)

left_calf = gr.mesh('cube', 'left_calf')
left_knee_joint:add_child(left_calf)
left_calf:scale(0.2, 0.8, 0.2)
left_calf:translate(0.0, -0.8, 0.0)
left_calf:set_material(green)

-- 右腿
right_hip_joint = gr.joint('right_hip_joint', {-45, 0, 45}, {-45, 0, 45})
body:add_child(right_hip_joint)
right_hip_joint:translate(0.4, -1.5, 0.0)

right_thigh = gr.mesh('cube', 'right_thigh')
right_hip_joint:add_child(right_thigh)
right_thigh:scale(0.3, 0.8, 0.3)
right_thigh:translate(0.0, -0.8, 0.0)
right_thigh:set_material(blue)

right_knee_joint = gr.joint('right_knee_joint', {-45, 0, 45}, {-45, 0, 45})
right_thigh:add_child(right_knee_joint)
right_knee_joint:translate(0.0, -0.8, 0.0)

right_calf = gr.mesh('cube', 'right_calf')
right_knee_joint:add_child(right_calf)
right_calf:scale(0.2, 0.8, 0.2)
right_calf:translate(0.0, -0.8, 0.0)
right_calf:set_material(green)

-- 移动整个模型
root:translate(0.0, 0.0, -10.0)

return root
