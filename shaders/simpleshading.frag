#version 330 core

#define MAX_LIGHTS 5

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


vec3 shade(vec3 n, vec3 light_dir, vec3 diffuse, vec3 lightIntensity)
{
    vec3 result = vec3(0, 0, 0);
    float cos_term = max(0.f, dot(n, -light_dir));

    result += diffuse * cos_term * lightIntensity;
    return result;
}

void main(void)
{
    vec3 c = vec3(0.f);

    for (int i = 0; i < 4; ++i)
    {
        c += shade(normal, lights[i].direction, color.rgb, lights[i].intensity);
    }

    out_color = vec4(c, alpha_val);
}
