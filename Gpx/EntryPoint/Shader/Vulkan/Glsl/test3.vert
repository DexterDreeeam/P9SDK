
#version 450

layout(location = 0)
in vec3 in_Position;

layout(location = 1)
in vec3 in_Color;

layout(location = 0)
out vec3 out_Color;

//==========================================

layout(binding = 0)
uniform transfer_matrix_group
{
    mat4 model;
    mat4 view;
    mat4 proj;
} trans_mat;

//==========================================

void main()
{
    vec4 pos = vec4(in_Position, 1.0);
    pos = trans_mat.model * pos;
    pos = trans_mat.view * pos;
    pos = trans_mat.proj * pos;

    gl_Position = pos;
    out_Color = in_Color;
}