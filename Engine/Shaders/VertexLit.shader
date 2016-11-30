struct VS_OUT
{
	vec3 color;
};

@vertex
{
	#include <AppData.inc>

	uniform vec3 _Color = vec3(1, 1, 1);

	const vec3 dir = normalize(vec3(0, 0, 1));

	out VS_OUT vs_out;

	void vs_main(AppData appdata)
	{
	    gl_Position = MATRIX_MVP * appdata.position;
	    float c = dot(normalize(mat3(MATRIX_IT_MV) * appdata.normal), normalize(LightPosition));
	    c = clamp(c, 0, 1);
	    vs_out.color = _Color * c;
	}
}

@fragment
{
	in VS_OUT vs_out;

	out vec4 color;

	void main()
	{
	    color = vec4(vs_out.color, 1);
	}
}