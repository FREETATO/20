#ifndef SERVICE_H
#define SERVICE_H

#include <string>

struct Service {
    std::string name;
    int code;
    double tariff;
    std::string interval;
};

#endif // SERVICE_H