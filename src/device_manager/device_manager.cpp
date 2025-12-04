//
// Created by Tobias Kocúr on 01/12/2025.
//

#include "device_manager.h"

#include <iostream>
#include <ostream>

device_manager::~device_manager() {
    this->disconnect();
}

bool device_manager::connect()
{
    if (this->connected) return true;

    char** device_list{nullptr}; int device_count{};
    if (idevice_get_device_list(&device_list, &device_count) != IDEVICE_E_SUCCESS)
    {
        std::cerr << "Failed to get device list" << std::endl;
        idevice_device_list_free(device_list);
        return false;
    }

    if (device_list == nullptr) {
        std::cerr << "Failed to get device list" << std::endl;
        idevice_device_list_free(device_list);
        return false;
    }

    if (idevice_new(&this->device, device_list[0]) != IDEVICE_E_SUCCESS)
    {
        std::cerr << "Failed to create device" << std::endl;
        idevice_device_list_free(device_list);
        return false;
    }

    if (lockdownd_client_new_with_handshake(this->device, &this->lockdown_client, "com.mindly") != LOCKDOWN_E_SUCCESS)
    {
        std::cerr << "failed to create lockdown client" << std::endl;
        idevice_free(this->device);
        return false;
    }

    this->connected = true;
    return true;
}

void device_manager::disconnect()
{
    if (this->lockdown_client)
    {
        lockdownd_client_free(this->lockdown_client);
        this->lockdown_client = nullptr;
    }

    if (this->device)
    {
        idevice_free(this->device);
        this->device = nullptr;
    }

    this->connected = false;
}

std::string device_manager::get_device_name() const
{
    char* device_name{nullptr};
    get_lockdown_value("DeviceName", &device_name);
    std::string result = device_name ? device_name : "";
    if (device_name) free(device_name);
    return result;
}

void device_manager::get_lockdown_value(const char *key, char **output) const {
    plist_t plist;
    if (lockdownd_get_value(this->lockdown_client, nullptr, key, &plist) != LOCKDOWN_E_SUCCESS) {
        std::cerr << "failed to get lockdown value" << std::endl;
        *output = nullptr;
        return;
    };

    char* temp = nullptr;
    plist_get_string_val(plist, &temp);
    *output = temp;
    plist_free(plist);
}

lockdownd_service_descriptor_t device_manager::start_service(const char *id) const
{
    lockdownd_service_descriptor_t service{nullptr};
    if (lockdownd_start_service(this->lockdown_client, id, &service) != LOCKDOWN_E_SUCCESS)
    {
        std::cerr << "failed to start service" << std::endl;
        return nullptr;
    }
    return service;
}

