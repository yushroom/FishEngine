# PBR从原理到实现

​	好几次想整理一下PBR的实现过程，但是每次都是从原理到放弃，现在终于下定决心写这篇博客，希望对大家有帮助。主要参考了UE4的实现，shader代码虽然大家都可以看到，但是不结合各种材料的话还是很难理解的。

​	非常推荐这篇知乎专栏文章[基于物理着色：BRDF](https://zhuanlan.zhihu.com/p/21376124)，有名词解释，对入门者比较友好。

​	限于篇幅，本文写作的目标是写出完整physically based shader，对于很多知识点都是抱着“不求甚解”的态度——即不追求对知识点的完全掌握，不“推倒”公式，不介绍完整的历史和出处，会运用即可。



菲涅尔方程 Fresnel Equations



## UE4的标准材质

详细版本见UE[官方文档](https://docs.unrealengine.com/latest/INT/Engine/Rendering/Materials/PhysicallyBased/index.html#karis)。

UE4的标准材质中的几个参数：

### BaseColor

​	float3(RGB)，每个分量都是[0,1]，

​	【本段存疑】材质的整体颜色，要注意金属和非金属（本文中的非金属一般指绝缘体）是不一样的，对于非金属，只有强度（三个分量相等），如Charcoal是0.02，Ocean Ice是0.56；对于金属，三个分量都起作用，比如Gold是(1.000, 0.766, 0.336)，Iron是(0.560, 0.570, 0.580)。

### Metallic

​	float，[0,1]，缺省为0

​	控制材质的金属性，即有多么像金属（metal-like），说简单点就是一个插值系数0，表示非金属，1表示金属。对于单一材质，直接设为0或者1就好了，对于混合材质（分层）材质才需要设成0-1之间的值。

### Roughness:

​	float，[0,1]，缺省为0.5

​	控制材质的粗糙程度，0表示完全光滑（mirror reflection），1表示完全粗糙（completely matte）。

​	相当于Unity里面的Smoothness，Roughness = 1-Smoothness

### Specular

​	float，[0,1]，缺省为0.5

​	Specular决定物体镜面反射强度的大小，其实就是Fresnel项中的$F_0$(垂直入射时镜面反射的强度)的一个线性映射。Specular的范围为[0,1]，对应的$F_0$的范围为[0,0.08]，而$F_0$又对应于ior（index of refraction，折射率）,相应的ior范围为[1.0,1.8]。Specular的值一般为0.5，对应的$f_0$为0.04，对应的Ior为1.5。
$$
F_0 = 0.08*Specular \\
F_0=(\frac {ior-1} {ior+1})^2
$$
​	即使是完全的diffuse材质，也不要将Specular设为0，，因为[Everthing is Shiny](http://filmicgames.com/archives/547)。对于大多数情况，Specular使用缺省值0.5即可。Unreal自己不太喜欢这个参数，首先Specular这个名字具有误导性，往往应该去调整Roughness；其次是这个参数很少需要调整，因为ior对于非金属意义不大，可以认为非金属的$F_0$为常数0.04。在13年的SIGGRAPH Presentation上，主讲人将Specular用别的参数（Cavity）替换掉，但是后来还是使用Specular这一参数，具体见[这篇blog](https://www.unrealengine.com/blog/physically-based-shading-on-mobile)。



### 阅读材料

​	[龚敏敏的这篇文章](https://zhuanlan.zhihu.com/p/21961722?refer=highwaytographics)引用了一些比较新的材料，总结一下就是：

​	1.金属的diffuse为0，我们看到的金属颜色是SpecularColor，即金属的BaseColor=$F_0$；

​	2.绝缘体的$F_0=0.04$就够了；

​	2.半导体用metalness（类似UE4中的Metallic）参数来插值

​	总结成如下公式:

diffuse = albedo * (1 - metalness)
specular = lerp(0.04, albedo, metalness)

这样，寸土寸金的GBuffer中只要存3通道的albedo和一个通道的metalness就可以了，视觉上可以摆脱以前常见的塑料感。








## BRDF

​	

​	两个理论基石：辐射度学和微表面理论。辐射度学几个名词中我们最关心的是辐射率（Radiance），用符号L表示，定义为$$L={\rm d\Phi \over \rm d\omega A}$$。

​	BRDF定义为辐射率（radiance）与入射辐照度（irradiance）的比值：
$$
f(\mathbf l, \mathbf v)= {\rm d L_o( \mathbf{v}) \over \rm d E(\mathbf l)}
$$
$\mathbf{l}$表示入射方向，$\mathbf{o}$表示出射方向，$\mathbf{v}$是观察方向，也就是我们关心的反射光方向。



### Diffuse BRDF

​	UE4中使用了四种Diffuse BRDF，分别是Lambert，Burley，Oren Nayar，Gotanda

默认的是最简单的Lambert：
$$
f(\mathbf l, \mathbf v)={\mathbf {c_{diff}} \over \pi}
$$
其中$\mathbf{c_{diff}}$是材质的diffuse albedo。Lambert虽然不是物理真实的，但胜在足够简单。要除以PI是因为要在半球上积分的结果等于一（能量守恒）。



### Specular BRDF

​	基于Cook-Torrance微表面模型，公式如下：
$$
f(\mathbf l, \mathbf v) =
\frac{D(\mathbf h) F(\mathbf v, \mathbf h) G(\mathbf l, \mathbf v, \mathbf h)}
{4 (\mathbf n \cdot \mathbf l) (\mathbf n \cdot \mathbf v)}
$$
其中$\mathbf h$是半向量，定义为$\mathbf h = \text{normalize}(\mathbf v +\mathbf l)$，$\mathbf n$是法向。不过一般都会把分母和G合并掉，因为G中一般都可以约掉这两个点积。



#### Specular D

​	很多地方把D称为NDF（normal distribution function）,一般用GGX，定义为（选用的参数不一样，公式形式也不一样，本文主要使用Unreal4的术语）：
$$
D(\mathbf h)=\frac{\alpha^2}
{\pi ((\mathbf n \cdot \mathbf h)^2(\alpha^2-1)+1)^2}
$$
其中$\alpha=Roughness^2$,Roughness就是上面提到的标准材质参数。

​	Unreal4中的shader代码：

```c++
// GGX / Trowbridge-Reitz
// [Walter et al. 2007, "Microfacet models for refraction through rough surfaces"]
float D_GGX( float Roughness, float NoH )
{
    float a = Roughness * Roughness;
    float a2 = a * a;
    float d = ( NoH * a2 - NoH ) * NoH + 1; // 2 mad
    return a2 / ( PI*d*d );                 // 4 mul, 1 rcp
}
```



#### Specular G

​	目前UE4默认用的是一种称为joint Smith的近似形式
$$
G_1(\mathbf v) = (\mathbf n \cdot \mathbf v)
$$

```c++
// Appoximation of joint Smith term for GGX
// [Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"]
float Vis_SmithJointApprox( float Roughness, float NoV, float NoL )
{
    float a = Square( Roughness );
    float Vis_SmithV = NoL * ( NoV * ( 1 - a ) + a );
    float Vis_SmithL = NoV * ( NoL * ( 1 - a ) + a );
    return 0.5 * rcp( Vis_SmithV + Vis_SmithL );
}
```



#### Specular F

​	Fresnel项，一般都是用Schlick近似形式：
$$
F(\mathbf v, \mathbf h) =F_0 + (1-F_0)^5
$$
$F_0$是垂直入射时的反射率

```c++
// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
float3 F_Schlick( float3 SpecularColor, float VoH )
{
    float Fc = Pow5( 1 - VoH );                 // 1 sub, 3 mul
    //return Fc + (1 - Fc) * SpecularColor;     // 1 add, 3 mad
    
    // Anything less than 2% is physically impossible and is instead considered to be shadowing
    return saturate( 50.0 * SpecularColor.g ) * Fc + (1 - Fc) * SpecularColor;
}
```



有了BRDF之后，



全部结合起来就是这样的：

```c++
// @param DiffSpecMask .r: diffuse, .g:specular e.g. float2(1,1) for both, float2(1,0) for diffuse only
float3 StandardShading( float3 DiffuseColor, float3 SpecularColor, float3 LobeRoughness, float3 LobeEnergy, float3 L, float3 V, half3 N, float2 DiffSpecMask )
{
	float3 H = normalize(V + L);
	float NoL = saturate( dot(N, L) );
	float NoV = saturate( abs( dot(N, V) ) + 1e-5 );
	float NoH = saturate( dot(N, H) );
	float VoH = saturate( dot(V, H) );
	
	// Generalized microfacet specular
	float D = D_GGX( LobeRoughness[1], NoH ) * LobeEnergy[1];
	float Vis = Vis_SmithJointApprox( LobeRoughness[1], NoV, NoL );
	float3 F = F_Schlick( SpecularColor, VoH );

	float3 Diffuse = Diffuse_Lambert( DiffuseColor );
	//float3 Diffuse = Diffuse_Burley( DiffuseColor, LobeRoughness[1], NoV, NoL, VoH );
	//float3 Diffuse = Diffuse_OrenNayar( DiffuseColor, LobeRoughness[1], NoV, NoL, VoH );
	
	return Diffuse * (LobeEnergy[2] * DiffSpecMask.r) + (D * Vis * DiffSpecMask.g) * F;
}
```



## Mobile上的优化



## 参考资料

几篇总结博客

http://www.cnblogs.com/TracePlus/p/4141833.html

http://www.luluathena.com/?p=1781

https://zhuanlan.zhihu.com/p/21376124



历年SIGGRAPH PBR讲座

http://blog.selfshadow.com/publications/



GDCC2015 Epic的演讲，ppt和相关笔记：

http://twvideo01.ubm-us.net/o1/vault/gdcchina15/slides/Li_Wenlei_PBR%20Implications_CN.pdf

https://www.unrealengine.com/zh-CN/blog/gdcc2015-pbr



PBR on Mobile

https://www.unrealengine.com/blog/physically-based-shading-on-mobile

https://cdn2.unrealengine.com/Resources/files/GDC2014_Next_Generation_Mobile_Rendering-2033767592.pdf
