#version 330 core

in vec4 color;
in vec3 normal;

in vec3 l_dir;
in vec3 l_intensity;

out vec4 out_color;


vec3 shade(vec3 n, vec3 light_dir, vec3 diffuse)
{
    vec3 result = vec3(0, 0, 0);
    float cos_term = max(0.f, dot(n, -light_dir));

    result = diffuse * cos_term;
    return result;
}

void main(void)
{
    out_color = color;
    //out_color = vec4(normal, 1);
//    out_color = vec4(shade(normal, l_dir, color.rgb), color.a);
}
