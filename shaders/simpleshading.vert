#version 400 core

in vec3 vtx_position;
in vec4 vtx_color;
in vec3 vtx_normal;

out vec4 color;
out vec3 normal;

uniform mat4 mat_obj;
uniform mat4 mat_view;
uniform mat4 mat_proj;


void main(void)
{
    gl_Position = mat_proj * mat_view * vec4(vtx_position, 1.f);

    normal = vtx_normal;

    color = vtx_color;
}
