## Engine

- [ ] 动态添加/删除Component
- [ ] 将资源loader和Shader、Mesh、Texture之类的具体类分离
- [ ] 数学库换成左手系（要不要自己实现？）
- [ ] glm四元数的-0问题

## Editor

- [x] GameObject inactive时在Hierarchy中颜色变灰色

- [ ] skybox遮挡了（写depth buffer的问题？）

- [ ] 相机旋转时的万向锁的问题

- [ ] AxisIndicator：遮挡问题（clear depth buffer解决），鼠标hover事件，点击后旋转相机

- [ ] GUI样式美化

- [ ] 资源管理 WIP

- [ ] 场景中的网格平面

- [ ] 场景的序列化和反序列化

- [ ] 鼠标等的事件被Editor处理后不再被分发到场景中（wantCapture...或者说判断鼠标在不在scene window中）

- [ ] Inspector窗口中的Headers的缩进，一行显示不全的问题

- [ ] Editor的帧率改到30

      ​



## 渲染

- [ ] PBR

- [ ] Shadow

- [ ] expose更多的shader uniform给Editor

- [ ] 渲染优化：渲染前按父子关系将transform更新完，渲染时按批次重组draw call batching

- [ ] uniform buffer

- [ ] Gbuffer结构和deferred shading

- [ ] tone mapping

- [ ] 复制场景的渲染，如sponza

      ​

## 物理

- [ ] PhysX