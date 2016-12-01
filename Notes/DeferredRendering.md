Linear Depth:

​	Unity中的方法：http://www.humus.name/temp/Linearize%20depth.txt

​	Depth buffer中存的depth的范围是[0, 1]，称之为device z，0对应near plane， 1对应far plane。OpenGL的clip space中的z的范围为[-1, 1]，所以有：z_clip = device_depth * 2 - 1。

​	现在我们需要view space下的depth，因为MATRIX_P * Pos_view = Pos_clip，可以得到：
$$
depth =\frac{2nf}{f + n - z_{clip} * (f -n)} = \frac{nf}{f-depth_{device} *(f-n)}
$$
这个式子形式和D3D是一样的。

​	从这个公式再来看Unity中的Shader写法，只需要一条mad和一条rcp:

```c
// Values used to linearize the Z buffer (http://www.humus.name/temp/Linearize%20depth.txt)
// x = 1-far/near
// y = far/near
// z = x/far
// w = y/far
vec4 ZBufferParams;

// Z buffer to linear 0..1 depth (0 at eye, 1 at far plane)
inline float Linear01Depth( float z )
{
	return 1.0 / (_ZBufferParams.x * z + _ZBufferParams.y);
}
// Z buffer to linear depth
inline float LinearEyeDepth( float z )
{
	return 1.0 / (_ZBufferParams.z * z + _ZBufferParams.w);
}
```

