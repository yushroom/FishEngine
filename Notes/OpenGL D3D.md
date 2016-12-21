# 术语

NDCs: Normalized Device Coordinates

![CoordinateSpaces](D:\program\FishEngine\Notes\CoordinateSpaces.png)

出自OpenGL Shading Language(3rd Edition).

Perspective Divide又叫homogeneous divide



# OpenGL和D3D的比较

|                   | OpenGL                         | D3D                |
| ----------------- | ------------------------------ | ------------------ |
| 坐标系               | 右手系                            | 左手系                |
| 向量                | 列向量                            | 行向量                |
| 矩阵(2)             | 列主序(3)                         | 行主序                |
| 矩阵乘法              | Matrix * Vector                | Vector * Matrix    |
| NDC               | xyz都是[-1,1]                    | xy是[-1,1]，z是[0,1]  |
| Depth buffer      | (z/w+1)/2                      | z/w                |
| Depth buffer范围(1) | [0,1]，0为near plane，1为far plane | 相同                 |
| 窗口原点              | 左下角                            | 左上角                |
| 像素原点              | 像素中心                           | D3D9为像素左上角，其余为像素中心 |
|                   |                                |                    |

(1) 设备z指向屏幕里面，z值越大表示距离屏幕越远。

(2) 行主序还是列主序只是一种记号方法。
OpenGL和D3D的矩阵互为转置，但是物理存储的顺序一样
即数组A[4*4]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 16}会被理解为：$M_{D3D}=\begin{pmatrix} 1 & 2 & 3 & 4 \\ 5 & 6 & 7 & 8 \\ 9 & 10 & 11 & 12 \\ 13 & 14 & 15 & 16 \end{pmatrix}$ 和$M_{OpenGL}=\begin{pmatrix} 1 & 5 & 9 & 13 \\ 2 & 6 & 10 & 14 \\ 3 & 7 & 11 & 15 \\ 4 & 8 & 12 & 16 \end{pmatrix}$,为同一个变换矩阵。

(3) OpenGL没有规定矩阵是否为列主序，只规定了mat4为16个float的array，第13，14，15个元素表示translation（http://stackoverflow.com/questions/17717600/confusion-between-c-and-opengl-matrix-order-row-major-vs-column-major）。但一般的手册都是用列主序。



# 矩阵推导

以下矩阵使用行主序，向量为列向量，矩阵乘法使用Matrix * Vector。

## Perspective Projection

需要四个参数fovy，apect，zn，zf

tan2 = tan(fovy/2)

### OpenGL

$$
P_{OpenGL, RH} \cdot v =
\begin{pmatrix} 1 \over {tan2 \cdot aspect} & 0&0&0\\0& 1 \over tan2&0&0\\0&0& -{{f+n}\over{f-n}} &-{{2nf}\over{f-n}}\\0&0& -1 & 0 \end{pmatrix} 
\begin{pmatrix} x \\ y \\ z \\ 1\end{pmatrix} =
\begin{pmatrix} x\over{tan2\cdot aspect}\\ y\over{tan2} \\ -{n(z+f)+f(z+n)\over f-n} \\ -z \end{pmatrix}
$$

Perspective divide之后就是${z\over w} ={{n(z+f)+f(z+n)\over z(f-n)}}$，注意因为是右手系，能进入clip space的z一定是负的，取z=-n有		depth=-1，取z=-f有depth=1。视口变换之后就是[0,1]。



### D3D

$$
P_{D3D,LH} \cdot v =
\begin{pmatrix} 1 \over {tan2 \cdot aspect} & 0&0&0\\0& 1 \over tan2&0&0\\0&0& {f \over{f-n}} &-{{nf}\over{f-n}}\\0&0& 1 & 0\end{pmatrix} 
\begin{pmatrix} x \\ y \\ z \\ 1\end{pmatrix} =
\begin{pmatrix} x\over{tan2\cdot aspect}\\ y\over{tan2} \\ {(z-n)f\over f-n} \\ z \end{pmatrix}
$$

如果要在OpenGL中使用左手系的矩阵的话，由于NDC中z的值不一样，需要在这个矩阵的基础上加一项：
$$
P_{OpenGL,LH}=
\begin{pmatrix} 1&0&0&0\\0&1&0&0\\0&0&2&-1\\0&0&0&1 \end{pmatrix} \cdot P_{D3D}=
\begin{pmatrix} 1 \over {tan2 \cdot aspect} & 0&0&0\\0& 1 \over tan2&0&0\\0&0& {f+n\over{f-n}} &-{{2nf}\over{f-n}}\\0&0& 1 & 0\end{pmatrix}
$$

$$
P_{OpenGL,LH} \cdot v =
\begin{pmatrix} 1 \over {tan2 \cdot aspect} & 0&0&0\\0& 1 \over tan2&0&0\\0&0& {f+n\over{f-n}} &-{{2nf}\over{f-n}}\\0&0& 1 & 0\end{pmatrix} \cdot
\begin{pmatrix} x \\ y \\ z \\ 1 \end{pmatrix} =
\begin{pmatrix} x\over{tan2\cdot aspect}\\ y\over{tan2} \\ {n(z-f)+f(z-n)\over f-n} \\ z \end{pmatrix}
$$
等于是做了z*2-1这个操作。