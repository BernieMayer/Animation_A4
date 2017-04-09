#version 430 core


uniform vec3 lightColor;
out vec4 color;

void main(void)
{    
    color = vec4(lightColor, 1.0);
}
