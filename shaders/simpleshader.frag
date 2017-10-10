#version 330 core

in vec4 color;

out vec4 out_color;

void main(void)
{
    out_color = vec4(color.rgb, 1.f);
}
