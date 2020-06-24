struct Color{
    float4 color;
};

ConstantBuffer<Color> g_color : register(b0);
Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float4 main(PSInput i) : SV_TARGET
{
    float4 col = g_texture.Sample(g_sampler,i.uv);
	return col;
}