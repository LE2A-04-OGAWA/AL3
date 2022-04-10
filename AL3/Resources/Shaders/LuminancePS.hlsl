#include "PostEffectTest.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 PSmain(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    
    //�P�x���o
    //�T���v�����O�����J���[�̖��邳���v�Z
    float t = dot(texcolor.xyz, float3(0.2125f, 0.7154f, 0.0721f));
    
    clip(t - 0.9f);
    return texcolor;
    
    //���σu���[
    //2.5�e�N�Z�����炷���߂�UV�l�����߂�
    float offsetU = 1.5f / 1280.0f;
    float offsetV = 1.5f / 720.0f;
    
    //��e�N�Z������E�̃e�N�Z���̃J���[���T���v�����O
    texcolor += tex.Sample(smp, input.uv + float2(offsetU, 0.0f));
    //��e�N�Z�����獶�̃e�N�Z���̃J���[���T���v�����O
    texcolor += tex.Sample(smp, input.uv + float2(-offsetU, 0.0f));
    //��e�N�Z�����牺�̃e�N�Z���̃J���[���T���v�����O
    texcolor += tex.Sample(smp, input.uv + float2(0.0f, offsetV));
    //��e�N�Z�������̃e�N�Z���̃J���[���T���v�����O
    texcolor += tex.Sample(smp, input.uv + float2(0.0f, -offsetV));
    //��e�N�Z������E���̃e�N�Z���̃J���[���T���v�����O
    texcolor += tex.Sample(smp, input.uv + float2(offsetU, offsetV));
    //��e�N�Z������E��̃e�N�Z���̃J���[���T���v�����O
    texcolor += tex.Sample(smp, input.uv + float2(offsetU, -offsetV));
    //��e�N�Z�����獶���̃e�N�Z���̃J���[���T���v�����O
    texcolor += tex.Sample(smp, input.uv + float2(-offsetU, offsetV));
    //��e�N�Z�����獶��̃e�N�Z���̃J���[���T���v�����O
    texcolor += tex.Sample(smp, input.uv + float2(-offsetU, -offsetV));
    
    //��e�N�Z���ƋߖT8�e�N�Z���̕��ς��Z�o
    texcolor /= 9.0f;
    
    return texcolor;
}