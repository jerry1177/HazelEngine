#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextCoord;
layout(location = 3) in float a_TextIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

out vec2 v_TextCoord;
out vec4 v_Color;
out float v_TextIndex;
out float v_TilingFactor;

void main() {
	v_TextIndex = a_TextIndex;
	v_Color = a_Color;
	v_TextCoord = a_TextCoord;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

//uniform float u_TilingFactor;
uniform sampler2D u_Textures[32];

in vec2 v_TextCoord;
in vec4 v_Color;
in float v_TextIndex;
in float v_TilingFactor;

void main() {
	
	//vec4 textColor = texture(u_Textures[int(v_TextIndex)], v_TextCoord * v_TilingFactor) * v_Color;
	//if(textColor.a < 0.1)
    //    discard;
	//color = textColor;
	color = texture(u_Textures[int(v_TextIndex)], v_TextCoord * v_TilingFactor) * v_Color;;
}