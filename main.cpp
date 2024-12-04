#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <unordered_set>
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
    std::vector<Service> services; 
    std::ifstream file(fileName);  

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
            
            interval = interval.substr(1); 
            services.push_back({name, code, tariff, interval});
        }
    }

    file.close();
    return services;
}

std::time_t convertToTimeT(const std::string& dateTimeStr) {
    std::tm tm = {};
    std::istringstream ss(dateTimeStr);
    ss >> std::get_time(&tm, "%d.%m.%Y %H:%M:%S");
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
        std::string fullName, phoneNumber, dateTimeStr, duration;
        int serviceCode;
        char delimiter;

        if (getline(ss, fullName, ',') &&
            getline(ss, phoneNumber, ',') &&
            ss >> serviceCode >> delimiter &&
            getline(ss, dateTimeStr, ',') &&
            getline(ss, duration)) {

            dateTimeStr = dateTimeStr.substr(1);

            std::time_t dateTime = convertToTimeT(dateTimeStr);

            const Service* service = findServiceByCode(services, serviceCode);
            if (service) {
                usages.push_back({phoneNumber, fullName, service, dateTime, duration});
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
              << "ФИО: " << usage.fullName << ", "
              << "Код услуги: " << usage.service->code << ", "
              << "Дата и время: " << std::put_time(&tm, "%d.%m.%Y %H:%M:%S") << ", "
              << "Время использования: " << usage.duration << std::endl;
}

int getYearFromDate(std::time_t dateTime) {
    struct tm * timeInfo;
    timeInfo = localtime(&dateTime);
    return 1900 + timeInfo->tm_year;  
}

std::vector<ServiceUsage> GetYear(
    const std::vector<ServiceUsage>& usages,
    const std::string& client1,
    const std::string& client2)
{
    std::vector<ServiceUsage> filteredUsages;

    std::time_t currentTime = std::time(nullptr);
    std::tm* currentTm = std::localtime(&currentTime);
    
    int currentYear = 1900 + currentTm->tm_year;  
    int previousYear = currentYear - 1;

    std::unordered_set<int> client1Services;
    std::unordered_set<int> client2Services;

    for (const auto& usage : usages) {
        int year = getYearFromDate(usage.dateTime);
        
        if (year == previousYear) {
            if (usage.fullName == client1) {
                client1Services.insert(usage.service->code); 
            }
            if (usage.fullName == client2) {
                client2Services.insert(usage.service->code);
            }
        }
    }
    for (const auto& usage : usages) {
        int year = getYearFromDate(usage.dateTime);
        
        if (year == previousYear && 
            (usage.fullName == client1 || usage.fullName == client2) &&
            client1Services.count(usage.service->code) > 0 && 
            client2Services.count(usage.service->code) > 0) 
        {
            filteredUsages.push_back(usage);
        }
    }

    return filteredUsages;
}



  

int main() {
    vector<Service> services = readServicesFromFile(ServicesPatch);
    vector<ServiceUsage> serviceUsage = readServiceUsageFromFile(ServiceUsagePatch, services);

    string client1, client2;
    cout << "Введите ФИО первого клиента: ";
    getline(cin, client1);
    cout << "Введите ФИО второго клиента: ";
    getline(cin, client2);

    vector<ServiceUsage> filteredUsages = GetYear(serviceUsage,client1,client2 ;

    if (filteredUsages.empty()) {
        cout << "Нет услуг, которые использовали оба клиента в прошедшем году." << endl;
    } else {
        for (const auto& usage : filteredUsages) {
            printServiceUsage(usage);
        }
    }

    return 0;
}
