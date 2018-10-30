cbuffer ShaderBuffer
{
    matrix worldViewProj;
    float4 color;
    float4 sizeScale;
};

struct VertexInput
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

Texture2D shaderTexture;
SamplerState sampleType;

PixelInput VS(VertexInput input)
{
	PixelInput output;

	input.position.w = 1.0f;

    output.position = mul(input.position, worldViewProj);

	output.tex = input.tex;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = shaderTexture.Sample(sampleType, input.tex);
    return albedo * color;
}