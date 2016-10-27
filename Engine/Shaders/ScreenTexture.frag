in VS_OUT {
    vec2 uv;
} vs_out;

out vec4 color;

uniform sampler2D screenTexture;

void main()
{
    color = texture(screenTexture, vs_out.uv);
}