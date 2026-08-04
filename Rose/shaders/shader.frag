#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec4 colorOverride;
uniform int useTexture;

void main()
{
    vec4 baseColor = colorOverride;
    if (useTexture == 1)
        baseColor = texture(texture1, TexCoord);

    FragColor = baseColor * vec4(lightColor * objectColor, 1.0);
}
