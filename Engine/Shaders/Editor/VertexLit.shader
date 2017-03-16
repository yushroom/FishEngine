// for Gizmos, like scene gizmo, translation gizmo.
// assume that camera is the light sourse
uniform mat4 MATRIX_MVP;
uniform mat4 MATRIX_IT_MV;
uniform vec3 _Color = vec3(1, 0, 0);

struct V2F
{
	vec3 color;
};

@vertex
{
	layout(location = 0)	in vec3 InputPositon;
	layout(location = 1)	in vec3 InputNormal;

	out V2F v2f;

	void main()
	{
		gl_Position = MATRIX_MVP * vec4(InputPositon, 1);
		vec3 N_view = normalize(mat3(MATRIX_IT_MV) * InputNormal);
		float NoL = clamp(-N_view.z, 0, 1);
		v2f.color = _Color * NoL;
	}
}

@fragment
{
	in V2F v2f;
	out vec4 fragColor;

	void main()
	{
		fragColor = vec4(v2f.color, 1);
	}
}
