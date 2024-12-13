
struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Color : COLOR;
	float2 UV : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Color : COLOR;
	float2 UV : TEXCOORD;
};

float4x4 gWorldViewProj : WorldViewProjection;

Texture2D gDiffuseMap : DiffuseMap;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_Point;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 16;
    AddressU = Wrap;
    AddressV = Wrap;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Position = mul(float4(input.Position,1.0f), gWorldViewProj);
	output.Color = input.Color;
	output.UV = input.UV;
	return output;
}

float4 SampleTexture(VS_OUTPUT input, SamplerState samplerState) 
{
    return gDiffuseMap.Sample(samplerState, input.UV);
}


float4 PS_Point(VS_OUTPUT input) : SV_TARGET
{
	float4 texColor = SampleTexture(input, samPoint);
	return texColor * float4(input.Color, 1.0f);
}

float4 PS_Linear(VS_OUTPUT input) : SV_TARGET
{
	float4 texColor = SampleTexture(input, samLinear);
	return texColor * float4(input.Color, 1.0f);
}

float4 PS_Anisotropic(VS_OUTPUT input) : SV_TARGET
{
	float4 texColor = SampleTexture(input, samAnisotropic);
	return texColor * float4(input.Color, 1.0f);
}

technique11 PointTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_Point()));
    }
}

technique11 LinearTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_Linear()));
    }
}

technique11 AnisotropicTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_Anisotropic()));
    }
}