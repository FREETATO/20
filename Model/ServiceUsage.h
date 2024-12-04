#ifndef SERVICE_USAGE_H
#define SERVICE_USAGE_H

#include "Service.h"
#include <string>
#include <ctime>

struct ServiceUsage {
    std::string phoneNumber;
    std::string fullName; 
    const Service* service;
    std::time_t dateTime;
    std::string duration;
};

#endif // SERVICE_USAGE_H
