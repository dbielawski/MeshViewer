#version 330 core

in vec3 vtx_position;
in vec4 vtx_color;

out vec4 color;

uniform mat4 mat_obj;
uniform mat4 mat_view;
uniform mat4 mat_proj;

uniform float alpha_val = 1.0;

void main(void)
{
    gl_Position = mat_proj * mat_view * vec4(vtx_position, 1.f);

    color = vec4(vtx_color.rgb, alpha_val);
}
