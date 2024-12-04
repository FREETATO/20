#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Model/Service.h"
#include "Model/ServiceUsage.h"

#define ServicesPatch "Input/Services.txt"
#define ServiceUsagePatch "Input/ServiceUsage.txt"


using namespace std;

const Service* findServiceByCode(const std::vector<Service>& services, int code) {
    for (const auto& service : services) {
        if (service.code == code) {
            return &service;
        }
    }
    return nullptr; 
}
std::vector<Service> readServicesFromFile(const std::string& fileName) {
    std::vector<Service> services; // Вектор для хранения прочитанных услуг
    std::ifstream file(fileName);  // Открываем файл для чтения

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << fileName << std::endl;
        return services;
    }

    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string name, interval;
        int code;
        double tariff;
        char delimiter;

        if (getline(ss, name, ',') &&
            ss >> code >> delimiter &&
            ss >> tariff >> delimiter &&
            getline(ss, interval)) {
            
            interval = interval.substr(1); // Убираем пробел перед интервалом
            services.push_back({name, code, tariff, interval});
        }
    }

    file.close();
    return services;
}
std::time_t convertToTimeT(const std::string& dateTimeStr) {
    std::tm tm = {};
    std::istringstream ss(dateTimeStr);
    ss >> std::get_time(&tm, "%d.%m.%Y %H:%M:%S");  // ну что-то типо регулярного выражения
    if (ss.fail()) {
        std::cerr << "Failed to parse date-time string." << std::endl;
        return 0;
    }
    return std::mktime(&tm); 
}

std::vector<ServiceUsage> readServiceUsageFromFile(const std::string& fileName, const std::vector<Service>& services) {
    std::vector<ServiceUsage> usages;
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << fileName << std::endl;
        return usages;
    }

    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string phoneNumber, dateTimeStr, duration;
        int serviceCode;
        char delimiter;

        if (getline(ss, phoneNumber, ',') &&
            ss >> serviceCode >> delimiter &&
            getline(ss, dateTimeStr, ',') &&
            getline(ss, duration)) {

            dateTimeStr = dateTimeStr.substr(1); 

            std::time_t dateTime = convertToTimeT(dateTimeStr);

            const Service* service = findServiceByCode(services, serviceCode);
            if (service) {
                usages.push_back({phoneNumber, service, dateTime, duration});
            } else {
                std::cerr << "Услуга с кодом " << serviceCode << " не найдена." << std::endl;
            }
        }
    }

    file.close();
    return usages;
}

void printServiceUsage(const ServiceUsage& usage) {
    std::tm tm = *std::localtime(&usage.dateTime);

 std::cout << "Телефон: " << usage.phoneNumber << ", "
              << "Код услуги: " << usage.service->code << ", "
              << "Дата и время: " << std::put_time(&tm, "%d.%m.%Y %H:%M:%S") << ", "
              << "Время использования: " << usage.duration << std::endl;
}
int main(){

    vector<ServiceUsage> ServiceUsage = readServiceUsageFromFile(ServiceUsagePatch,readServicesFromFile(ServicesPatch));

 for (const auto& usage : ServiceUsage) {
        printServiceUsage(usage);
    }

    return 0;
 

}