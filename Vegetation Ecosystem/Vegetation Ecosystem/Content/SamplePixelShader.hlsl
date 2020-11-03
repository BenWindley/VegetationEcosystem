Texture2D _texture : register(t0);
SamplerState _sampler : register(s0);

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
	float2 uv : TEXCOORD;
};

// A pass-through function for the (interpolated) color data.
float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 col = _texture.Sample(_sampler, input.uv);

	if (col.a < 0.5f) discard;

	return float4(input.color.rgb * _texture.Sample(_sampler, input.uv).rgb, col.a);
}
