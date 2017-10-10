#version 330 core

in vec4 color;
in vec3 normal;

out vec4 out_color;


struct LightInfo
{
    vec3 position;
    vec3 intensity;
    vec3 direction;
};

uniform float alpha_val = 1.0;

uniform LightInfo lights[4];
int MAX_LIGHT = 4;

vec3 shade(vec3 n, vec3 light_dir, vec3 diffuse)
{
    vec3 result = vec3(0, 0, 0);
    float cos_term = max(0.f, dot(n, -light_dir));

    result = diffuse * cos_term;
    return result;
}

void main(void)
{
    //    vec3 c = vec3(0.f);

    //    for (int i = 0; i < MAX_LIGHT; ++i)
    //    {
    //        c += shade(normal, lights[i].direction, color.rgb);
    //    }

    //    out_color = vec4(lights[0].intensity.rgb, alpha_val);

    //    out_color = vec4(lights[0].intensity.rgb, alpha_val);

    //    out_color = vec4(lights[0].direction.x, lights[0].direction.y, lights[0].direction.z, alpha_val);

    out_color = vec4(color.rgb, alpha_val);
    //    out_color = vec4(normal, 1);
    //    out_color = vec4(shade(normal, l_dir, color.rgb), color.a);
}
