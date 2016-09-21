## Engine

- [ ] 动态添加/删除Component（删除搞定，script和component分开；添加要用到反射？）

- [ ] 将资源loader和Shader、Mesh、Texture之类的具体类分离

- [ ] 数学库换成左手系（暂时将glm变成左手系，自己实现的WIP）

- [ ] glm四元数的-0问题

- [ ] GameObject构造函数改成非public的(需要解决make_shared调用非public ctor，貌似没有特别优雅的解决方案，http://stackoverflow.com/questions/8147027/how-do-i-call-stdmake-shared-on-a-class-with-only-protected-or-private-const)

- [ ] 将Scene里面的Script和Component合并到一起去

- [ ] 不依赖代码生成和外部工具的（自动）反射可行吗？

- [ ] 把不该开放的API隐藏掉（尤其是FishEngine）

- [ ] transform父子关系改变时的操作，应该维持子节点在world space下的T、R、S都不变

- [x] 如何减少transform::update的调用次数？简单实用脏属性来标记是不正确的，因为不知道父节点上的信息是不是更新了（那么父节点变化时通知所有子节点？或者脏属性想GameObject::active一样拿（这样也有个问题，就是父节点可能变脏了，但是在子节点check脏属性之前父节点update了）？）。

      ​	暂时的解决方案：节点变化（第一次变脏）时将所有子节点标记为脏。

- [x] 最小化窗口是aspect是变成nan，glm::perspective会abort掉

- [ ] 改成左手系之后cubemap反了

## Editor

- [x] GameObject inactive时在Hierarchy中颜色变灰色

- [x] skybox遮挡了（写depth buffer的问题？）

- [x] 相机旋转时的万向锁的问题

- [ ] SceneGizmo：遮挡问题（clear depth buffer解决），鼠标hover事件，点击后旋转相机

- [ ] GUI样式美化

- [ ] 资源管理 WIP

- [ ] 场景中的网格平面

- [ ] 场景的序列化和反序列化

- [ ] 鼠标等的事件被Editor处理后不再被分发到场景中（wantCapture...或者说判断鼠标在不在scene window中）

- [ ] Inspector窗口中的Headers的缩进（OK，用imgui::indent()），一行显示不全的问题

- [x] Editor的帧率改到30

- [x] 相机旋转时transform反应不正常，会越退越远

- [ ] Docking GUI

      ​



## 渲染

- [ ] PBR

- [ ] Shadow

- [ ] expose更多的shader uniform给Editor

- [ ] 渲染优化：渲染前按父子关系将transform更新完，渲染时按批次重组draw call batching

- [ ] uniform buffer

- [ ] Gbuffer结构和deferred shading

- [ ] tone mapping

- [ ] 复杂场景的渲染，如sponza

      ​

## 物理

- [ ] PhysX