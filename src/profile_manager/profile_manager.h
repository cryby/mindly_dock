//
// Created by Tobias Kocúr on 03/12/2025.
//

#pragma once
#include "../device_manager/device_manager.h"
#include <libimobiledevice/property_list_service.h>
#ifndef MINDLY_PROFILE_MANAGER_H
#define MINDLY_PROFILE_MANAGER_H


class profile_manager {
    private:
        device_manager& device;

    public:
        bool is_installed{false};

    public:
        explicit profile_manager(device_manager& d) : device(d) {};
        bool install_profile(const char* path);
        bool remove_profile(const char* profile_identifier);

    private:
        static plist_t create_webclip_profile();
        static plist_t create_restriction_profile();
};


#endif //MINDLY_PROFILE_MANAGER_H