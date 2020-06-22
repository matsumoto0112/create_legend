struct PSInput
{
    float4 position : SV_POSITION;
};

PSInput VSMain(float4 pos : POSITION)
{
    PSInput result = (PSInput)0;
    result.position = float4(pos.xyz,1.0);
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return float4(1,0,0,1);
}
