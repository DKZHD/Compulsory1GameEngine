#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;

out vec4 Color;
out vec3 Normal;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;
uniform bool ThreeDimensional;

void main(){
	if(ThreeDimensional){
		gl_Position = proj * view * model*vec4(aPos,1);
	}
	else{
		gl_Position = proj * model * vec4(aPos,1);
	}
	Color = vec4(aColor,1);
	Normal = aNormal;
};
#shader fragment
#version 330 core
out vec4 fragColor;

in vec4 Color;
in vec3 Normal;

void main(){
	fragColor = Color;
};
