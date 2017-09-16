#version 330 core

in vec3 vtx_position;
in vec3 vtx_color;

out vec3 color;

uniform mat4 mat_obj;
uniform mat4 mat_view;
uniform mat4 mat_proj;

void main(void)
{
    gl_Position = mat_proj * mat_view * vec4(vtx_position, 1.f);

    color = vtx_color;
}
