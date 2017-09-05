#version 330 core
in vec2 texCoord;

uniform sampler2D ourTexture;
uniform vec4 color;
uniform float renderMode;
uniform vec4 spriteCoordinates;

void main()
{
	vec2 spriteUV = spriteCoordinates.xy + (texCoord * spriteCoordinates.zw);
	if( renderMode == 0.0 )
	{
		gl_FragColor = color;
	}
	else if( renderMode == 1.0 )
	{
		gl_FragColor = texture(ourTexture, spriteUV);
	}
	else if( renderMode == 2.0 )
	{
		gl_FragColor = texture(ourTexture, spriteUV) * color;
	}
}