/*!
 * Unlit model shader for the cloud floor. Draws the texture color as it is,
 * and fades the far end out so that it blends into the sky behind it.
 */

////////////////////////////////////////////////
// Structs
////////////////////////////////////////////////

// Input of the pixel shader
struct SPSIn
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 biNormal : BINORMAL;
	float2 uv : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
};

///////////////////////////////////////
// Common vertex shader code
///////////////////////////////////////
#include "ModelVSCommon.h"

///////////////////////////////////////
// Constants (NUM_DIRECTIONAL_LIGHT etc. are needed by the resource header)
///////////////////////////////////////
#include "PBRLighting_const.h"

///////////////////////////////////////
// Sampler and shader resources
///////////////////////////////////////
#include "Sampler.h"
#include "model_srv_uav_register.h"

////////////////////////////////////////////////
// Vertex shader (same as model.fx)
////////////////////////////////////////////////
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal, uniform bool isUsePreComputedVertexBuffer)
{
	SPSIn psIn;

	psIn.pos = CalcVertexPositionInWorldSpace(vsIn.pos, mWorldLocal, isUsePreComputedVertexBuffer);
	psIn.worldPos = psIn.pos;

	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

	CalcVertexNormalTangentBiNormalInWorldSpace(
		psIn.normal,
		psIn.tangent,
		psIn.biNormal,
		mWorldLocal,
		vsIn.normal,
		vsIn.tangent,
		vsIn.biNormal,
		isUsePreComputedVertexBuffer
	);

	psIn.uv = vsIn.uv;

	return psIn;
}

///////////////////////////////////////
// Fade range (world Z). Must match the sky position in BackGround.cpp
///////////////////////////////////////
static const float FADE_START_Z = 1800.0f;
static const float FADE_END_Z = 3700.0f;

////////////////////////////////////////////////
// Pixel shader: the texture color as it is (with gamma handling)
////////////////////////////////////////////////
float4 PSMain(SPSIn In) : SV_Target0
{
	float4 color = albedoTexture.Sample(Sampler, In.uv);

	// The post effect applies gamma correction (pow 1/2.2) at the end,
	// so convert to linear first. Then the result looks like the original picture.
	color.xyz = pow(color.xyz, 2.2f);

	// The farther the pixel is, the more transparent it becomes
	color.a = 1.0f - smoothstep(FADE_START_Z, FADE_END_Z, In.worldPos.z);
	return color;
}
