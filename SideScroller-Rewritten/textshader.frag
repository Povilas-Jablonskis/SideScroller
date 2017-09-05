#version 330 core
in vec2 texCoord;

uniform sampler2D ourTexture;
uniform vec4 color;

void main()
{    
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(ourTexture, texCoord).r);
	gl_FragColor = sampled * color;
}