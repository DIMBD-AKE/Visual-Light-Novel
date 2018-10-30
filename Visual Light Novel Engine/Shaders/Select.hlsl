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

    float2 pixel = float2(1.0 / (sizeScale.x * sizeScale.z), 1.0 / (sizeScale.y * sizeScale.w));

    float4 outline = float4(0, 1, 0, 1);
    float4 maxline = float4(0, 1, 0, 1);

    if (input.tex.x < pixel.x * 2 ||
        input.tex.y < pixel.y * 2 ||
        input.tex.x + pixel.x * 2 > 1 ||
        input.tex.y + pixel.y * 2 > 1)
        return maxline;

    float alphaUp = shaderTexture.Sample(sampleType, input.tex + float2(0, pixel.y)).a;
    float alphaDown = shaderTexture.Sample(sampleType, input.tex - float2(0, pixel.y)).a;
    float alphaRight = shaderTexture.Sample(sampleType, input.tex + float2(pixel.x, 0)).a;
    float alphaLeft = shaderTexture.Sample(sampleType, input.tex - float2(pixel.x, 0)).a;
    float avg = (alphaUp + alphaDown + alphaRight + alphaLeft) / 4;

    if (avg > 0 && avg < 1)
        return lerp(albedo * color, outline, avg);
    else
        return albedo * color;
}