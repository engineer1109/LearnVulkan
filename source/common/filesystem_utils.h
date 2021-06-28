//
// Created by wjl on 2021/4/10.
//

#ifndef LEARNVULKAN_FILESYSTEM_UTILS_H
#define LEARNVULKAN_FILESYSTEM_UTILS_H

#include "render_common.h"

#include <string>
#include <iostream>

#if !defined(USE_STD_FILESYSTEM) && !defined(USE_STD_EXPERIMENTAL_FILESYSTEM)
#define USE_STD_FILESYSTEM
#endif

#if defined(USE_STD_FILESYSTEM)
#include <filesystem>
using namespace std::filesystem;
#elif defined(USE_STD_EXPERIMENTAL_FILESYSTEM)
#include <experimental/filesystem>
using namespace std::experimental::filesystem;
#else
#endif

BEGIN_NAMESPACE(VulkanEngine)

class FS{
public:
    static std::string getPath(std::string p){
#if defined(USE_STD_FILESYSTEM) || defined(USE_STD_EXPERIMENTAL_FILESYSTEM)
        path pa=path(std::string("../data/"+p).c_str());
        path pb=path(std::string(PROJECT_ABSOLUTE_PATH+("/data/"+p)).c_str());
        if(exists(pa)){
            return std::string("../data/"+p);
        }
        else if(exists(pb)){
            return std::string(PROJECT_ABSOLUTE_PATH+("/data/"+p));
        }
        else {
            std::cout<<"Error!Could not find this file:"<<p<<std::endl;
            exit(1);
        }
#else
        return p;
#endif
    }
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_FILESYSTEM_UTILS_H
