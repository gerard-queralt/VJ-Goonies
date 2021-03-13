#version 330

uniform vec4 color;
uniform sampler2D tex;

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color
	vec4 texColor = texture(tex, texCoordFrag);
	if(texColor.a < 0.5f)
		discard;
	if(texColor.r == (153.f/255.f) && texColor.g == (217.f/255.f) && texColor.b == (234.f/255.f))
		discard;
	outColor = color * texColor;
}

