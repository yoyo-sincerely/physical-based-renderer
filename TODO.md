# Awesome Renderer

- Assimp 支持 .fbx .obj .3ds .dae .ase 等常见的3D模型格式，支持 .vox .vol 两种体素文件格式。
- 离线渲染： 目前支持环境遮挡渲染、光线追踪渲染、路径追踪渲染，支持法线贴图、景深、面光源/软阴影、KD-Tree加速以及体积散射区域渲染。
- 实时渲染：使用openGL 3 + 延迟渲染 + 基于物理的shader + ShadowMap + 环境光照明/基于图像的照明(IBL) + SSR + SSAO + HDR + FXAA/TXAA
- 内置C++反射/序列化/反序列化支持。
- 支持蒙皮骨骼动画。