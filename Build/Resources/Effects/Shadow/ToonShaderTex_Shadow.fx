//TOON SHADER
//First Name: Pauline
//Last Name: Vanden Heede
//Class: 2DAE06
//Tutorial: https://roystan.net/articles/toon-shader.html

//GLOBAL VARIABLES
//****************
//MATRICES
float4x4 gMWorld : WORLD;
float4x4 gMWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gMViewInverse : VIEWINVERSE;
float4x4 gWorldViewProj_Light;
//COLOURS
float4 gColorAmbient : COLOR = float4(0.4, 0.4, 0.4, 1.0);
float4 gColorLight : COLOR = float4(1.0, 1.0, 1.0, 1.0);
float4 gColorSpecular : COLOR = float4(0.9, 0.9, 0.9, 1.0);
float4 gColorRim : COLOR = float4(1.0, 1.0, 1.0, 1.0);

//LIGHT
float3 gLightDirection :DIRECTION
<
	string UIName = "Light Direction";
	string Object = "TargetLight";
> = float3(0.551f, -0.534f, 0.641f);

//CONSTANTS
float gShininess = 10;
float gRimIntensity = 0.716;
float gRimThreshold = 0.1;
float gShadowMapBias = 0.01f;

//TEXTURE
Texture2D gDiffuseMap;
Texture2D gShadowMap;



//STATES
//******
SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;// or Mirror or Clamp or Border
	AddressV = Wrap;// or Mirror or Clamp or Border
};
SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;// or Mirror or Clamp or Border
	AddressV = Wrap;// or Mirror or Clamp or Border
};
SamplerComparisonState cmpSampler
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};
RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};
RasterizerState NoCulling
{
	CullMode = NONE;
};
DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

//I&O STRUCTS
struct VS_INPUT {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};
struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float4 worldPosition : COLOR1;
	float4 lPos : TEXCOORD1;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};


float2 texOffset(int u, int v)
{
	//TODO: return offseted value (our shadow map has the following dimensions: 1280 * 720)
	float2 output = (float2)0;
	output.x = float(u) / 1280;
	output.y = float(v) / 720;
	return output;
}

float EvaluateShadowMap(float4 lpos)
{
	//TODO: complete
	//re-homogenize position after interpolation
	lpos.xyz = lpos.xyz / lpos.w;

	//position not visible from light -> frustum clipping (should be in range [-1,1])
	if (lpos.x < -1.f || lpos.x > 1.f ||
		lpos.y < -1.f || lpos.y > 1.f ||
		lpos.z < 0.f || lpos.z > 1.f)
		return 0.f;

	//clip space to texture space [-1,1] -> [0, 1]
	lpos.x = lpos.x / 2.f + 0.5f;
	lpos.y = lpos.y / -2.f + 0.5f; //y points in different direction

	//Shadow map bias
	lpos.z = lpos.z - gShadowMapBias;

	//PCF sampling for shadow map
	float sum = 0;

	//sample
	//perform PCF filtering on a 4 x 4 texel neighborhood
	for (float y = -1.5f; y <= 1.5f; y += 1.f)
	{
		for (float x = -1.5f; x <= 1.5f; x += 1.f)
		{
			sum = sum + gShadowMap.SampleCmpLevelZero(cmpSampler, lpos.xy + texOffset(x, y), lpos.z);
		}
	}

	float shadowDepth = sum / 16.0;

	//float shadowDepth = gShadowMap.Sample(samPoint, lpos.xy).r; //only need one float value

	return shadowDepth;
}

//MAIN VERTEX SHADER
//******************
VS_OUTPUT MainVS(VS_INPUT input) {
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(float4(input.position, 1.0f), gMWorldViewProj);
	output.worldPosition = mul(float4(input.position, 1.0f), gMWorld);
	output.lPos = mul(float4(input.position, 1.f), gWorldViewProj_Light);
	output.normal = normalize(mul(input.normal, (float3x3)gMWorld));
	output.texCoord = input.texCoord;
	return output;
}


//MAIN PIXEL SHADER
//*****************
float4 MainPS(VS_OUTPUT input) : SV_TARGET{

	//DIFFUSE
	float4 diffuseColour = gDiffuseMap.Sample(samLinear,input.texCoord);
	float diffuseStrength = dot(input.normal, -gLightDirection);

	//LIGHT
	float lightIntensity = smoothstep(0, 0.01, diffuseStrength);
	float4 lightColour = lightIntensity * gColorLight;

	//SPECULAR - PHONG
	float3 viewDirection = normalize(input.worldPosition.xyz - gMViewInverse[3].xyz);
	float3 reflectDirection = reflect(viewDirection, input.normal);
	float specularStrength = dot(reflectDirection, -gLightDirection);
	float specularIntensity = pow(specularStrength * lightIntensity, gShininess * gShininess);
	specularIntensity = smoothstep(0.005, 0.01, specularIntensity);
	float4 specularColour = specularIntensity * gColorSpecular;

	//RIM
	float rim = 1 - dot(viewDirection, -input.normal);
	float amount = rim * pow(diffuseStrength, gRimThreshold);
	rim = smoothstep(gRimIntensity - 0.01, gRimIntensity + 0.01, amount);
	float4 rimColour = gColorRim * rim;

	float shadowValue = EvaluateShadowMap(input.lPos);
	float colour_a = ((lightColour + specularColour + rimColour + gColorAmbient) * diffuseColour).a;

	return float4(((lightColour + specularColour + rimColour + gColorAmbient) * diffuseColour).rgb, colour_a); //grey values * colour
}


//TECHNIQUES
//**********
technique10 DefaultTechnique {
	pass p0 {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}