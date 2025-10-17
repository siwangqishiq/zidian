// compile :
//    slangc draw_triangle.slang -target spirv -profile spirv_1_4 
//          -emit-spirv-directly -fvk-use-entrypoint-name 
//          -entry vertMain -entry fragMain -o draw_triangle.spirv

static float2 positions[3] = float2[](
    float2(0.0f, -0.5f),
    float2(0.5f, 0.5f),
    float2(-0.5f, 0.5f)
);

static float3 colors[3] = float3[](
    float3(1.0f, 0.0f, 0.0f),
    float3(0.0f, 1.0f, 0.0f),
    float3(0.0f, 0.0f, 1.0f)
);

struct VertexOutput {
    float3 color;
    float4 sv_postion : SV_Position;
};

[shader("vertex")]
VertexOutput vertMain(uint vid: SV_VertexID) {
    VertexOutput output;
    output.color = colors[vid];
    output.sv_postion = float4(positions[vid], 0.0, 1.0f);
    return output;
}

[shader("fragment")]
float4 fragMain(VertexOutput inVert) : SV_Target {
    return float4(inVert.color, 1.0);
}
