texture的mipmap关掉之后性能有明显下降（255fps->185fps）

glTexImage2D和glTexStorage2D

glTexImage2D是老的版本，可变

glTexStorage2D是4.2引入的，不可变，glTextureStorage2D是4.5加入的DSA版本。

glTexStorage2D中的mipmap level需要自己指定，不像glTexImage2D可以设为0从而自动计算。

各种资料上写着不可变的可变的效率高，但实测没有发现

### How to calculate max mipmap level count in glTexStorage2D:

https://www.opengl.org/sdk/docs/man/html/glTexStorage2D.xhtml

**GL_INVALID_OPERATION** is generated if target is GL_TEXTURE_1D_ARRAY or GL_PROXY_TEXTURE_1D_ARRAY and levels is greater than ⌊log2(width)⌋+1log2width+1.

**GL_INVALID_OPERATION** is generated if target is not GL_TEXTURE_1D_ARRAY or GL_PROXY_TEXTURE_1D_ARRAY and levels is greater than ⌊log2(max(width, height))⌋+1log2maxwidth, height+1.

$\lfloor \log_2(\max(width, height)) \rfloor+1$

->floor( log2(max(width, height) ) )



https://en.wikipedia.org/wiki/S3_Texture_Compression

S3 Texture Compression (S3TC) (sometimes also called DXTn or DXTC)

**DXT1** (also known as Block Compression 1 or **BC1**)

**DXT2** and **DXT3** (collectively also known as Block Compression 2 or **BC2**)

**DXT4** and **DXT5** (collectively also known as Block Compression 3 or **BC3**)

| FOURCC | DX 10 Name | Description          | Alpha premultiplied? | Compression ratio            | Texture Type     |
| ------ | ---------- | -------------------- | -------------------- | ---------------------------- | ---------------- |
| DXT1   | BC1        | 1-bit Alpha / Opaque | Yes                  | 6:1(for 24 bit source image) | Simple non-alpha |
| DXT2   | BC2        | Explicit alpha       | Yes                  | 4:1                          | Sharp alpha      |
| DXT3   | BC2        | Explicit alpha       | No                   | 4:1                          | Sharp alpha      |
| DXT4   | BC3        | Interpolated alpha   | Yes                  | 4:1                          | Gradient alpha   |
| DXT5   | BC3        | Interpolated alpha   | No                   | 4:1                          | Gradient alpha   |



~~使用bc1的压缩纹理会慢一点（257fps->247fps）~~发现是没开mipmap的缘故

使用bc1的压缩纹理会略快一点（257fps->262fps）

使用mipmap确实会快很多，尤其对非压缩格式