define compileVertFrag
	glslangValidator -V $(1)/$(2).glsl.frag -o $(1)/$(2).so.frag
	glslangValidator -V $(1)/$(2).glsl.vert -o $(1)/$(2).so.vert
endef

all:
	$(call compileVertFrag,StaticTriangle,statictriangle)
	$(call compileVertFrag,StaticCube,staticcube)
	$(call compileVertFrag,Texture2dCube,texture2d)
	$(call compileVertFrag,Skybox,skybox)
	$(call compileVertFrag,PhongLighting,phonglighting)
	$(call compileVertFrag,InstanceDraw,instancedraw)
	$(call compileVertFrag,ShadowMapping,plane)
	$(call compileVertFrag,ShadowMapping,quad)
	$(call compileVertFrag,ShadowMapping,scene)
	$(call compileVertFrag,ShadowMapping,shadow)
	$(call compileVertFrag,ShadowMappingOnmi,cubemapdisplay)
	$(call compileVertFrag,ShadowMappingOnmi,offscreen)
	$(call compileVertFrag,ShadowMappingOnmi,room)
	$(call compileVertFrag,ShadowMappingOnmi,scene)