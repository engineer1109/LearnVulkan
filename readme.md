
# LearnVulkan
😀This project is mainly faced to GUI engineering applications and advanced backend computing, such as X11, Qt, ffmpeg, OpenCV, CUDA and so on. Combining Vulkan with engineering applications, not a pure demo.  <br>
## Planned Application Areas: 
   1. Imaging Proceessing of Vulkan😁  
   2. Medical Imaging of Vulkan😂  
   3. Vulkan on Qt (GAMES)🤣  
   4. Vulkan on XWindow (GAMES)😃  
   5. Vulkan with CUDA😅  
   6. Vulkan server rendering with ffmpeg😆  
    <br>
## What is more than SaschaWillems's Project
This project is partly based on [SaschaWillems's Project](https://github.com/SaschaWillems/Vulkan.git), add some functions of cross-platform window operation , deleted MFC code of the main function(WinMain), and provides a unified interface api for cross-platform.  <br>
Use sharelib (libvulkanbase.so, libvulkan_basicengine.so) instead of static lib.  <br>

* Sharelib : ✅  
* Supported System : Linux & Windows   
* PNG and JPEG Texture Load  : ✅ (Auto turn to RGBA8 format)  
* Volume Texture3d Load : ⭕ (Will support nii and raw data)  
* Auto Mipmap-Gen  : ✅  
* Qt-binding(vulkan widget)  : ✅  
* Relative path and absolute path management for IDE  : ✅  
* Basic object template(Ball & Square & Cube and so on)  : ⭕  
For examples of cross-platform api:  
```
int main(int argc,char** argv){
    bool debug=false;
    StaticTriangle* pVulkan=new StaticTriangle(debug);
    pVulkan->initVulkan();
    pVulkan->initWindow();
    pVulkan->prepare();
    pVulkan->renderLoop();
    delete pVulkan;
    return 1;
}
```
![image](https://github.com/engineer1109/LearnVulkan/blob/master/data/gif/output.gif) <br>
The libvulkan_basicengine.so also integrates 'stb_image' from [https://github.com/JoeyDeVries/LearnOpenGL](https://github.com/SaschaWillems/Vulkan.git), supporting jpg and png image loading, and you don't need to generate ktx images for Vulkan Texture now. See 'vulkan_basicengine_texture.h'.

Finally, welcome to clone or fork my project, remember to star if you consider my project is quite good.  

# Requirements
## Supported System:
   **Now:** Linux(Mainly)  Windows(Partly)
   **Planned supported:** Mac(I have successfully installed black Apple) Android IOS
## Libraries and Tools
   **CMake:** 3.14+ is recommended 
   **Vulkan:** 1.1.121+
   **Glm:** 0.9.9+
   **Xcb & Xlib**    (Linux needs)
   **GLFW3**         
   **OpenCV:** 4.0+  (Some projects need)
   **CUDA:** 10.0+  (Some projects need)
   **Qt:** 5.12+  (Some projects need)
##
# C/C++ Compiler
   GCC 7+   using -std=c++17
   MSVC 2017
# Download
```
git clone https://github.com/engineer1109/LearnVulkan.git --depth=1
```
   '--depth=1' will reduce the download size of repo.  
# Compile & BUILD
## For Linux/UNIX
   Compile  
```
    cmake ..
    make
    make install
```
The programs will be installed in the {PROJECT_DIR}/bin .  
### For Qt
```
    cmake .. -DBUILD_QtExamples=ON
```
## For Windows
   Compile
```
    cmake -G "Visual Studio 15 2017 Win64" -T host=x64 ..
    cmake --build . --config Release
    cmake --build . --config Release --target INSTALL
```
The programs will be installed in the {PROJECT_DIR}/Release .
# PLANS
1. Simple Examples (80%) (Only need Vulkan)
>01_StaticTriangle  
>02_StaticCube  
>03_Texture2dCube  
>04_MultiImageSampler  
>05_MultiObjects  
>06_Lighting  
>07_MipmapGen
>08_InstanceDraw
>09_VolumeCloud  
2. Advanced Examples (20%) (Need Assimp)
>01_Skybox
>02_ReflectSky
3. Qt Vulkan (20%) (Need Qt, some need assimp)
>01_QtTriangle  
>02_FramelessVulkan  
4. Cuda and Vulkan
5. X11 and Vulkan
6. Vulkan and deep learning
7. Vulkan Server Rendering
   
