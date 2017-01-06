Shadow mapping [Williams 78]

Shadow mapping with percentage closer filtering(PCF) [Reeves et al. 87]

Extended PCF: Screen-space blurring [Shastry 05]

Percentage-closer soft shadows (PCSS) [Fernando 05]

Screen-space soft shadows(SSSS) [Gumbau et al. 10] GPU Pro 1, 2010

Practical Screen-Space Soft Shadows, GPU Pro 6, 2015

Mipmapped screen-space soft shadows (MSSSS) [Aguado and Montiel 11]





SSM

生成：

```c
return d;
```

使用：

```c
return z < d;
```

 



VSM

http://www.cnblogs.com/lookof/archive/2010/03/21/1690769.html



ESM 

http://www.olhovsky.com/2011/07/exponential-shadow-map-filtering-in-hlsl/

生成：

```c
return exp( ExpC * d);
```

 生成过程需要设备支持需要浮点数的render target，移动设备就不可以了。

使用：

```c
float occluder = texture(ShadowMap, uv).r;
return saturate( occluder * exp( -ExpC * z ) );
```





改进



生成：

```c
return d;
```

使用：

```c
return saturate( exp( occluder - c*z ) );
```



http://www.nvidia.com/object/doc_shadows.html

