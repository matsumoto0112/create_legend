struct SceneConstant{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

ConstantBuffer<SceneConstant> g_scene : register(b1);


struct VSInput{
    float4 pos : POSITION;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

PSInput main(VSInput v) 
{   
    float4 pos = float4(v.pos.xyz,1.0);
    pos = mul(pos,g_scene.world);
    pos = mul(pos,g_scene.view);
    pos = mul(pos,g_scene.proj);
    PSInput result = (PSInput)0;
    result.position =pos;
    result.uv = float2(0.0,0.0);
    return result;
}