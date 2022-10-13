//BACKDROP
//First Name: Pauline
//Last Name: Vanden Heede
//Class: 2DAE06
//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;
Texture2D gBackDrop;

float4 gClearColour;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState enableDepthWriting
{
    DepthWriteMask = ALL;
};
/// Create Rasterizer State (Backface culling)
RasterizerState backCulling
{
    CullMode = BACK;
};


//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
    output.Position = float4(input.Position, 1.f);
	// Set the TexCoord
    output.TexCoord = input.TexCoord;
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input) : SV_Target
{
    float3 final = gTexture.Sample(samPoint, input.TexCoord).rgb;
    float3 colour = abs(gClearColour - final);
    if (colour.r < 0.1f && colour.g < 0.1f && colour.b < 0.1f)
    {
        final = gBackDrop.Sample(samPoint, input.TexCoord).rgb;
    }
    return float4(final, 1.0f );
}


//TECHNIQUE
//---------
technique11 BackDrop
{
    pass P0
    {          
        // Set states...
        SetRasterizerState(backCulling);
        SetDepthStencilState(enableDepthWriting, 0);
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

