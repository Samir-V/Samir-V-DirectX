struct VS_INPUT
{
	float3 Position : POSITION;
	float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

float4x4 gWorldViewProj : WorldViewProjection;
Texture2D gDiffuseMap : DiffuseMap;

RasterizerState gRasterizerState
{
	Cullmode = none;
	FrontCounterClockwise = false;
};

BlendState gBlendState
{
	BlendEnable[0] = true;
	SrcBlend = src_alpha;
	DestBlend = inv_src_alpha;
	BlendOp = add;
	SrcBlendAlpha = zero;
	DestBlendAlpha = zero;
	BlendOpAlpha = add;
	RenderTargetWriteMask[0] = 0x0F;
};

SamplerState samPointDefault
{
    Filter = MIN_MAG_MIP_Point;
    AddressU = Wrap;
    AddressV = Wrap;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Position = mul(float4(input.Position, 1.0f), gWorldViewProj);
	output.UV = input.UV;
    output.Normal = normalize(input.Normal);
    output.Tangent = normalize(input.Tangent);
	return output;
}

float4 SampleTexture(VS_OUTPUT input, Texture2D textureToSample) 
{   
    return textureToSample.Sample(samPointDefault, input.UV);
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	return SampleTexture(input, gDiffuseMap);
}

technique11 DefaultTechnique
{
	pass P0
	{
		SetRasterizerState(gRasterizerState);
		SetBlendState(gBlendState, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS() ) );
	}
}