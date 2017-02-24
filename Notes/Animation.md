Mesh

​	Matrix4x4[] **bindposes**	The bind poses. The bind pose at each index refers to the bone with the same index.

​	BoneWeight[] **boneWeights** The bone weights of each vertex.



SkinnedMesh

​	Transform **rootBone** Use this to define the bone that is the “root” of the animation (that is, the bone relative to which all the others move).

​	Transform[] **bones** The bones used to skin the mesh.



术语

bind pose，也叫reference pose，rest pose或者T-pose



bindposes = bone->worldToLocalMatrix()



FBX SDK中拿到的bindposeMatrix实际上是骨骼节点的localToWorldMatrix，而Unity中，Mesh.bindposes是WorldToLocalMatrix，这里的World指的是模型所在的空间（对于bone来说就是world space），而local是bone所在的空间，所以更确切的说法应该是boneToObject和objectToBone。

生产Matrix palette的代码如下：

```c++
// bindposes[i]： objectToBone
// bone->localToWorldMatrix(): boneToObject
mat = worldToLocal * bone->localToWorldMatrix() * bindposes[i];
```

worldToLocal是该SkinnedMeshRenderer所在的transform的worldToLocalMatrix，先乘上这个是因为后面两个矩阵把mesh变换到object space下， 而在shader里面做坐标变换时默认mesh是在local space下，这个worldToLocalMatrix是为了抵消WVP中的W（也就是localToWorld）。