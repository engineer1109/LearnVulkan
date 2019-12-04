#include "vulkan_basicengine_vertexobject.h"
VertexObject::~VertexObject(){
    if(m_pipeline){
        vkDestroyPipeline(m_device, m_pipeline, nullptr);
    }
    m_vertexBuffer.destroy();
    m_indexBuffer.destroy();
    m_uniformBuffers.destroy();
}

void VertexObject::create(){
    generateVertex();
}

void VertexObject::update(){

}

void VertexObject::generateVertex(){

}
