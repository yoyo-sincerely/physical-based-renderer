# AwesomeRenderer

run cmd commond 

```
git clone https://github.com/yoyo-sincerely/AwesomeRenderer.git
cd AwesomeRenderer
git submodule init
git submodule update

mkdir build
cd build
```
in win32, run:
```
cmake ..
```

in win64, run:
```
cmake -G "Visual Studio 15 2017 Win64" ..
```

- G-Buffer structure

<!--![](https://github.com/yoyo-sincerely/MyPic/blob/master/PBR/GBuffer_data_structure.png?raw=true)-->

| RT && Channel | Red | Green | Blue | Alpha |
| - | - | - | - | - |
| RT0 | ViewPosition.X | ViewPosition.Y | ViewPosition.Z | Depth |
| RT1 | Albedo.R | Albedo.G | Albedo.B | Roughness |
| RT2 | Normal.R | Normal.G | Normal.B | Matelness | 
| RT3 | AO | Velocity.R | Velocity.G | 
 

## How To Use



## Features

- Camera
	- Movements(移动)
	- Zoom in/out(放大/缩小)
	- Exposure(曝光) :
		- Aperture(光圈)
		- Shutter speed(快门速度)
		- ISO(感光度)
- Texture
	- Init/loading/binding by stb library
	- Anisotropic Filtering(各项异性扩散滤波)
	- HDR
	- Cubemap
- Material
	- PBR material pipeline
		- Albedo(反射率)
		- Normal(法线)
		- Roughness(粗糙度)
		- Matelness(金属度)
		- AO(环境光遮蔽)
- Model
	- Load model by Assimp
- PBR Pipline
	- BRDF
		- Cook-Torrance model
		- Diffuse : Lambertian/Disney
- Utility
	- GUI using ImGui
	- GPU profiling
	- G-Buffer visualization for debugging purpose

## Dependencies

in 3rdpart file:

- Window & Input system : GLFW
- OpenGL Function Loader : GLAD
- OpenGL Mathematic Functions : GLM
- Image Loading : stb
- Mesh Loading : Assimp

