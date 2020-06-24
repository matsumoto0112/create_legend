struct VSInput{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

PSInput main(VSInput v) 
{   
    PSInput result = (PSInput)0;
    result.position = float4(v.pos.xyz,1.0);
    result.uv = v.uv;
    return result;
}