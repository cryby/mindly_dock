//
// Created by Tobias Kocúr on 01/12/2025.
//
#pragma once
#include <string>
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>


#ifndef MINDLY_DEVICE_MANAGER_H
#define MINDLY_DEVICE_MANAGER_H


class device_manager {
    private:
        idevice_t device{nullptr};
        lockdownd_client_t lockdown_client{nullptr};
        bool connected{false};

    public:
        device_manager() = default;
        ~device_manager();
        bool connect();
        void disconnect();

        [[nodiscard]] std::string get_device_name() const;

        [[nodiscard]] lockdownd_client_t get_lockdown_client() const { return lockdown_client; };
        [[nodiscard]] idevice_t get_device() const { return device; };
        lockdownd_service_descriptor_t start_service(const char* id) const;
        [[nodiscard]] bool get_connected() const { return connected; };
    private:
        void get_lockdown_value(const char* key, char** output) const;
};


#endif //MINDLY_DEVICE_MANAGER_H