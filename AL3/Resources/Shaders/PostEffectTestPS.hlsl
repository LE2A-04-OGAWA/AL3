#include "PostEffectTest.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 PSmain(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    return texcolor;
    
    //�P�x���o
    //�T���v�����O�����J���[�̖��邳���v�Z
    //float t = dot(texcolor.xyz, float3(0.2125f, 0.7154f, 0.0721f));
    
    //clip(t - 0.9f);
    //return texcolor;
    
    //���σu���[
    //2.5�e�N�Z�����炷���߂�UV�l�����߂�
    //float offsetU = 1.5f / 1280.0f;
    //float offsetV = 1.5f / 720.0f;
    
    ////��e�N�Z������E�̃e�N�Z���̃J���[���T���v�����O
    //texcolor += tex.Sample(smp, input.uv + float2(offsetU, 0.0f));
    ////��e�N�Z�����獶�̃e�N�Z���̃J���[���T���v�����O
    //texcolor += tex.Sample(smp, input.uv + float2(-offsetU, 0.0f));
    ////��e�N�Z�����牺�̃e�N�Z���̃J���[���T���v�����O
    //texcolor += tex.Sample(smp, input.uv + float2(0.0f, offsetV));
    ////��e�N�Z�������̃e�N�Z���̃J���[���T���v�����O
    //texcolor += tex.Sample(smp, input.uv + float2(0.0f, -offsetV));
    ////��e�N�Z������E���̃e�N�Z���̃J���[���T���v�����O
    //texcolor += tex.Sample(smp, input.uv + float2(offsetU, offsetV));
    ////��e�N�Z������E��̃e�N�Z���̃J���[���T���v�����O
    //texcolor += tex.Sample(smp, input.uv + float2(offsetU, -offsetV));
    ////��e�N�Z�����獶���̃e�N�Z���̃J���[���T���v�����O
    //texcolor += tex.Sample(smp, input.uv + float2(-offsetU, offsetV));
    ////��e�N�Z�����獶��̃e�N�Z���̃J���[���T���v�����O
    //texcolor += tex.Sample(smp, input.uv + float2(-offsetU, -offsetV));
    
    ////��e�N�Z���ƋߖT8�e�N�Z���̕��ς��Z�o
    //texcolor /= 9.0f;
    
    //return texcolor;
    
    //�ʏ�
    //return float4(texcolor.rgb, 1);
    //return float4(texcolor.r, texcolor.g, 0,          1);
    //return float4(texcolor.r, 0,          texcolor.b, 1);
    //return float4(0,          texcolor.g, texcolor.b, 1);
    
    //���U�C�N
    //float blockSize = 16;
    //float uvX = input.uv.x * 1280;
    //float uvY = input.uv.y * 720;
    
    //uvX /= blockSize;
    //uvY /= blockSize;
    
    //uvX = floor(uvX) * blockSize;
    //uvY = floor(uvY) * blockSize;
    
    //uvX /= 1280;
    //uvY /= 720;
    
    //float2 uv = { uvX, uvY };
  
    //return tex.Sample(smp, uv);
    
    //�Z�s�A��
    //float sepiaRatio = (texcolor.r + texcolor.g + texcolor.b) / 765.0f;
    //return float4(
    //240.0f * sepiaRatio,
    //200.0f * sepiaRatio,
    //145.0f * sepiaRatio,
    //1
    //);
    
    //�O���[�X�P�[��
    //float grayAverage = (texcolor.r + texcolor.g + texcolor.b) / 3.0f;
    //return float4(
    //grayAverage,
    //grayAverage,
    //grayAverage,
    //1
    //);
}

float4 PSXmain(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    
    //���σu���[
    //1.5�e�N�Z�����炷���߂�UV�l�����߂�
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

float4 PSYmain(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    
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