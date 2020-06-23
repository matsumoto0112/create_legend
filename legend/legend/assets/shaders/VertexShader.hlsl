struct PSInput
{
    float4 position : SV_POSITION;
};

PSInput main( float4 pos : POSITION ) 
{   
    PSInput result = (PSInput)0;
    result.position = float4(pos.xyz,1.0);
    return result;
}