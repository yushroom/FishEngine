

## Engine

- [x] 数学库换成左手系

- [x] 最小化窗口是aspect是变成nan，glm::perspective会abort掉

- [x] 改成左手系之后cubemap反了

- [x] 如何减少transform::update的调用次数？简单实用脏属性来标记是不正确的，因为不知道父节点上的信息是不是更新了（那么父节点变化时通知所有子节点？或者脏属性像GameObject::active一样拿（这样也有个问题，就是父节点可能变脏了，但是在子节点check脏属性之前父节点update了）？）。

      ​	暂时的解决方案：节点变化（第一次变脏）时将所有子节点标记为脏（递归过程，会把子孙所有节点标记为脏）。
      
- [ ] SceneView按F选中的物体会自动居中，并且按boundingbox调整相机距离

- [ ] 动态添加/删除Component（删除搞定，script和component分开；添加要用到反射？）

- [ ] 将资源loader和Shader、Mesh、Texture之类的具体类分离（Model已经分离，动画导入WIP）

- [ ] depth \in (-1, 1)，下次换成D3D的时候小心

- [ ] glm四元数的-0问题，eulerangle的数字不太合适

- [ ] GameObject构造函数改成非public的(需要解决make_shared调用非public ctor，貌似没有特别优雅的解决方案，http://stackoverflow.com/questions/8147027/how-do-i-call-stdmake-shared-on-a-class-with-only-protected-or-private-const)

- [ ] 将Scene里面的Script和Component合并到一起去（需要在运行时知道是不是某个类的子类）

- [ ] 不依赖代码生成和外部工具的（自动）反射可行吗？

- [ ] 把不该开放的API隐藏掉（尤其是FishEngine）

- [ ] transform父子关系改变时的操作，应该维持子节点在world space下的T、R、S都不变

- [ ] assimp导入的fbx动画有冗余的（骨骼？）结点

- [ ] eulerangles数值不自然

- [ ] check Matrix->Quaternion的代码，RTR3 p76, DX11 24.3.3

- [ ] quaternion*vector3

- [ ] angleAxis

- [ ] Transform::rotate

- [ ] const的位置

- [ ] class Graphics

- [ ] 我需要一个动态的反射操作

      1: "MeshFilter" ==> make_shared<MeshFilter>() 而且要运行期动态生成或者变成dll动态加载

      2：classname="MeshFilter" mesh="..." -==> 

      auto mf = make_shared<MeshFilter>();

      mf->setMesh(...)

## Editor

- [x] GameObject inactive时在Hierarchy中颜色变灰色

- [x] skybox遮挡了（写depth buffer的问题？）

- [x] 相机旋转时的万向锁的问题

- [x] SceneGizmo：遮挡问题（clear depth buffer解决），鼠标hover事件，点击后旋转相机

- [x] 鼠标等的事件被Editor处理后不再被分发到场景中（wantCapture...或者说判断鼠标在不在scene window中）

- [x] Inspector窗口中的Headers的缩进（OK，用imgui::indent()），一行显示不全的问题（暂时解决，Unity也没有解决label多长的问题）

- [x] Editor的帧率改到30

- [x] 相机旋转时transform反应不正常，会越退越远

- [ ] 场景中物体的平移（完成，相机需要在任何scale下面都有一样的translation，selection改变时清空selected axis暂时搞定，用球代替Bounds求交？）、旋转、缩放等操作

- [ ] GUI样式美化

- [ ] 资源管理 WIP

- [ ] 场景中的网格平面

- [ ] 场景的序列化和反序列化（要依赖反射）

- [ ] Docking GUI

- [ ] hierarchy中的方向键的移动

      ​



## 渲染

- [ ] PBR

- [ ] Shadow（简单的shadow map，pcf）=>VSM

- [ ] expose更多的shader uniform给Editor

- [ ] 渲染优化：渲染前按父子关系将transform更新完，渲染时按批次重组draw call batching

- [ ] uniform buffer

- [ ] Gbuffer结构和deferred shading

- [ ] tone mapping

- [ ] 复杂场景的渲染，如sponza

      ​

## 物理

- [ ] PhysX