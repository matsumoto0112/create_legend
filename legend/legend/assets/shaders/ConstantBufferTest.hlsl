struct Color{
    float4 color;
};

ConstantBuffer<Color> g_color : register(b0);

struct PSInput
{
    float4 position : SV_POSITION;
};

float4 main() : SV_TARGET
{
	return g_color.color;
}