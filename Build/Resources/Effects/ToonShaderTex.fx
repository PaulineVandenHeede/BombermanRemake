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

//TEXTURE
Texture2D gDiffuseMap;



//STATES
//******
RasterizerState gRS_NoCulling { CullMode = NONE; };
SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;// or Mirror or Clamp or Border
	AddressV = Wrap;// or Mirror or Clamp or Border
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
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};


//MAIN VERTEX SHADER
//******************
VS_OUTPUT MainVS(VS_INPUT input) {
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.worldPosition = mul(float4(input.position, 1.0f), gMWorld);
	output.position = mul(float4(input.position, 1.0f), gMWorldViewProj);
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

	return (lightColour + specularColour + rimColour + gColorAmbient) * diffuseColour; //grey values * colour
}


//TECHNIQUES
//**********
technique10 DefaultTechnique {
	pass p0 {
		SetRasterizerState(gRS_NoCulling);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}