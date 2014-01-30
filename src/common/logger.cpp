#include "mechio/common/logger.h"
#include <iostream>

void mechio::common::info(const std::string &message){
    std::cout << "[INFO] " << message << std::endl;
}

void mechio::common::info(const std::string &message, const std::exception &ex){
    std::cout << "[INFO] " << message << std::endl << ex.what() << std::endl;
}

void mechio::common::warn(const std::string &message){
    std::cout << "[WARN] " << message << std::endl;
}

void mechio::common::warn(const std::string &message, const std::exception &ex){
    std::cout << "[WARN] " << message << std::endl << ex.what() << std::endl;
}

void mechio::common::error(const std::string &message){
    std::cout << "[ERROR] " << message << std::endl;
}

void mechio::common::error(const std::string &message, const std::exception &ex){
    std::cout << "[ERROR] " << message << std::endl << ex.what() << std::endl;
}
