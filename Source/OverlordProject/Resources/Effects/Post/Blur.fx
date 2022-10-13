//BLUR
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
	// Set the TexCoord
	output.Position = float4(input.Position, 1.f);
	output.TexCoord = input.TexCoord;
	
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input) : SV_Target
{
	// Step 1: find the dimensions of the texture (the texture has a method for that)
	float width, height;
	gTexture.GetDimensions(width, height);
	// Step 2: calculate dx and dy (UV space for 1 pixel)
	float dx = 1.f / width;
	float dy = 1.f / height;
	// Step 3: Create a double for loop (5 iterations each)
	//		   Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
	//			Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
	float3 colour = float3(0.f, 0.f, 0.f);
	int passes = 5;
	int halfPass = passes / 2;
	int nrSamples = 0;
	float2 startOffset = float2(input.TexCoord.x - dx * halfPass, input.TexCoord.y - dy * halfPass);
	for (int i = 0; i < passes; ++i)
	{
		for (int j = 0; j < passes; ++j)
		{
			if ((i + j) % 2 == 0)
			{
				float2 offset = float2(dx * i, dy * j);
				colour += gTexture.Sample(samPoint, startOffset + offset);
				++nrSamples;
			}
		}
	}
	// Step 4: Divide the final color by the number of passes (in this case 5*5)
	colour /= nrSamples;
	// Step 5: return the final color

	return float4(colour, 1.0f);
}


//TECHNIQUE
//---------
technique11 Blur
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