#version 330 core

in vec3 vtx_position;
in vec4 vtx_color;
in vec3 vtx_normal;

out vec4 color;
out vec3 normal;
out vec3 l_dir;
out vec4 l_intensity;

uniform mat4 mat_obj;
uniform mat4 mat_view;
uniform mat4 mat_proj;

uniform float alpha_val = 1.0;
uniform vec3 light_dir;
uniform vec4 light_intensity;

void main(void)
{
    gl_Position = mat_proj * mat_view * vec4(vtx_position, 1.f);

    normal = vtx_normal;
    l_dir = light_dir;
    l_intensity = light_intensity;

    color = vec4(vtx_color.rgb, alpha_val);
}
