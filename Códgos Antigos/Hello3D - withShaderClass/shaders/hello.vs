#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 finalColor;
  
uniform mat4 model;

void main()
{
    gl_Position = model * vec4(position, 1.0f);
    finalColor = vec4(color,1.0);
    
}
