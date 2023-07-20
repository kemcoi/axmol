#version 310 es
precision highp float;
precision highp int;

layout(location = 0) in vec4 v_fragmentColor;
layout(location = 1) in vec2 v_texCoord;

layout(binding = 0) uniform sampler2D u_tex0;

layout(std140) uniform fs_ub {
    vec4 u_textColor;
};

layout(location = 0) out vec4 FragColor;

void main()
{
    float dist = texture(u_tex0, v_texCoord).x;
    float width = fwidth(dist); // ESSL300, GLSL330 support fwidth
    float alpha = smoothstep(0.5-width, 0.5+width, dist) * u_textColor.a;
    FragColor = v_fragmentColor * vec4(u_textColor.rgb,alpha);
}
