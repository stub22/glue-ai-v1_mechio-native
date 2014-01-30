/* 
 * File:   logger.h
 * Author: matt
 *
 * Created on November 12, 2011, 10:49 AM
 */

#ifndef MECHIO_COMMON_LOGGER_H
#define	MECHIO_COMMON_LOGGER_H

#include <string>
#include <exception>

namespace mechio{
    namespace common{
        
        void info(const std::string &message);
        void info(const std::string &message, const std::exception &ex);
        
        void warn(const std::string &message);
        void warn(const std::string &message, const std::exception &ex);
        
        void error(const std::string &message);
        void error(const std::string &message, const std::exception &ex);
    }
}
#endif	/* LOGGER_H */

