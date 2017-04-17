#include <ShaderVariables.inc>
#include <CG.inc>
//#include <ShadowCommon.inc>
//#include <CascadedShadowMapCommon.inc>

struct VS_OUT {
	vec3 normal;	// in world space
};

@vertex
{
	layout (location = PositionIndex) 	in vec3 InputPositon;
	layout (location = NormalIndex)		in vec3 InputNormal;

	out VS_OUT vs_out;

	void main()
	{
		vec4 position = vec4(InputPositon, 1);
		vec3 normal = InputNormal;
		//gl_Position = ObjectToWorld * ObjectPosition;
		//position = UnityClipSpaceShadowCasterPos2(position, normal);
		//position = UnityApplyLinearShadowBias(position);
		gl_Position = position;
		vs_out.normal = normalize(normal);
	}
}

@geometry
{
	layout(triangles, invocations = 4) in;
	layout(triangle_strip, max_vertices = 3) out;

	in VS_OUT vs_out[];
	
	float4 ClipSpaceShadowCasterPos(float4 vertex, float3 normal, float biasScale)
	{
		float4 wPos = mul(MATRIX_M, vertex);

		if (unity_LightShadowBias.z != 0.0)
		{
			float3 wNormal = UnityObjectToWorldNormal(normal);
			float3 wLight = normalize(UnityWorldSpaceLightDir(wPos.xyz));

			// apply normal offset bias (inset position along the normal)
			// bias needs to be scaled by sine between normal and light direction
			// (http://the-witness.net/news/2013/09/shadow-mapping-summary-part-1/)
			//
			// unity_LightShadowBias.z contains user-specified normal offset amount
			// scaled by world space texel size.

			float shadowCos = dot(wNormal, wLight);
			float shadowSine = sqrt(1-shadowCos*shadowCos);
			float normalBias = unity_LightShadowBias.z * biasScale * shadowSine;

			wPos.xyz -= wNormal * normalBias;
		}

		//return mul(UNITY_MATRIX_VP, wPos);
		return wPos;
	}

	float4 ApplyLinearShadowBias(float4 clipPos, float biasScale)
	{
		float bias = unity_LightShadowBias.x * biasScale;
	#if defined(UNITY_REVERSED_Z)
		// We use max/min instead of clamp to ensure proper handling of the rare case
		// where both numerator and denominator are zero and the fraction becomes NaN.
		clipPos.z += max(-1, min(bias / clipPos.w, 0));
		float clamped = min(clipPos.z, clipPos.w*UNITY_NEAR_CLIP_VALUE);
	#else
		clipPos.z += saturate(bias/clipPos.w);
		float clamped = max(clipPos.z, clipPos.w*UNITY_NEAR_CLIP_VALUE);
	#endif
		clipPos.z = lerp(clipPos.z, clamped, unity_LightShadowBias.y);
		return clipPos;
	}

	void main()
	{
		for (int i = 0; i < gl_in.length(); ++i)
		{
		#ifdef SHOWMAP_NO_BIAS
			vec4 position = LightMatrix[gl_InvocationID] * MATRIX_M * gl_in[i].gl_Position;
			gl_Position = position;
		#else
			float4x4 matrixVP = LightMatrix[gl_InvocationID];
			//float biasScale = matrixVP[2][2];
			//biasScale = abs(biasScale);
			float4 oPosition = gl_in[i].gl_Position;
			float3 oNormal = normalize(vs_out[i].normal);
			float4 wPos = ClipSpaceShadowCasterPos(oPosition, oNormal, 0.01);
			float4 cPos = matrixVP * wPos;
			gl_Position = ApplyLinearShadowBias(cPos, 0.1);
		#endif
			gl_Layer = gl_InvocationID;
			EmitVertex();
		}
		EndPrimitive();
	}
}

@fragment
{
	void main()
	{
	}
}