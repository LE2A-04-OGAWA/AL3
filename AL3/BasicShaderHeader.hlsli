cbuffer cbuff0:register(b0)
{
	//float4 color;
	matrix mat;   //3D�ϊ��s��
};

struct VSOutput
{
	float4 svpos:SV_POSITION;
	//float3 normal:NORMAL;
	//float2 uv:TEXCOORD;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    //float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};