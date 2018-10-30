cbuffer ShaderBuffer
{
    matrix worldViewProj;
    float4 color;
    float4 sizeScale;
};

struct VertexInput
{
	float4 position : POSITION;
    float4 color : COLOR0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
    float4 color : COLOR0;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;

	input.position.w = 1.0f;

    output.position = mul(input.position, worldViewProj);
    output.color = input.color;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return input.color;
}