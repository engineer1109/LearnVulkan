//
// Created by wjl on 21-2-23.
//

#ifndef LEARNVULKAN_BASE_TEMPLATE_H
#define LEARNVULKAN_BASE_TEMPLATE_H

#include "render_common.h"

BEGIN_NAMESPACE(VulkanEngine)

template<class T>
void delete_ptr(T* &ptr){
    if(ptr){
        delete ptr;
        ptr = nullptr;
    }
}

template<class T>
void delete_array(T* &ptr){
    if(ptr){
        delete[] ptr;
        ptr = nullptr;
    }
}

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_BASE_TEMPLATE_H
