//
// Created by Tobias Kocúr on 03/12/2025.
//

#include "profile_manager.h"

#include <iostream>
#include <ostream>

plist_t profile_manager::create_webclip_profile()
{
    plist_t profile = plist_new_dict();
    plist_dict_set_item(profile, "PayloadVersion", plist_new_uint(1));
    plist_dict_set_item(profile, "PayloadType", plist_new_string("Configuration"));
    plist_dict_set_item(profile, "PayloadIdentifier", plist_new_string("com.mindly.testy"));
    plist_dict_set_item(profile, "PayloadUUID", plist_new_string("1234-5678"));
    plist_dict_set_item(profile, "PayloadDisplayName", plist_new_string("Mindly Tobias"));

    plist_t content = plist_new_array();
    plist_t webclip = plist_new_dict();

    plist_dict_set_item(webclip, "PayloadType", plist_new_string("com.apple.webClip.managed"));
    plist_dict_set_item(webclip, "URL", plist_new_string("https://google.com"));
    plist_dict_set_item(webclip, "Label", plist_new_string("Mindly App"));
    plist_dict_set_item(webclip, "PayloadIdentifier", plist_new_string("com.mindly.testo"));
    plist_dict_set_item(webclip, "PayloadUUID", plist_new_string("5678-1234"));
    plist_dict_set_item(webclip, "PayloadDisplayName", plist_new_string("Mindly App"));
    plist_dict_set_item(webclip, "PayloadVersion", plist_new_uint(1));

    plist_array_append_item(content, webclip);
    plist_dict_set_item(profile, "PayloadContent", content);

    return profile;
}

plist_t profile_manager::create_restriction_profile()
{
    plist_t profile = plist_new_dict();
    plist_dict_set_item(profile, "PayloadVersion", plist_new_uint(1));
    plist_dict_set_item(profile, "PayloadType", plist_new_string("Configuration"));
    plist_dict_set_item(profile, "PayloadIdentifier", plist_new_string("com.mindly.restriction"));
    plist_dict_set_item(profile, "PayloadUUID", plist_new_string("1234-5678"));
    plist_dict_set_item(profile, "PayloadDisplayName", plist_new_string("Mindly App Restrict"));
    plist_dict_set_item(profile, "PayloadRemovalPassword", plist_new_string("1234"));

    plist_t content = plist_new_array();

    plist_t restriction = plist_new_dict();

    plist_dict_set_item(restriction, "PayloadVersion", plist_new_uint(1));
    plist_dict_set_item(restriction, "PayloadType", plist_new_string("com.apple.applicationaccess"));
    plist_dict_set_item(restriction, "PayloadIdentifier", plist_new_string("com.mindly.restrictions"));
    plist_dict_set_item(restriction, "PayloadUUID", plist_new_string("5678-1234"));
    plist_dict_set_item(restriction, "PayloadDisplayName", plist_new_string("Mindly App"));
    plist_dict_set_item(restriction, "ratingApps", plist_new_uint(0));
    plist_dict_set_item(restriction, "allowUIConfigurationProfileInstallation", plist_new_bool(0));

    plist_dict_set_item(restriction, "ratingRegion", plist_new_string("us"));


    plist_t blacklist = plist_new_array();

    plist_array_append_item(blacklist, plist_new_string("com.alltrails.alltrails"));

    plist_dict_set_item(restriction, "blacklistedAppBundleIDs", blacklist);
    plist_array_append_item(content, restriction);
    plist_dict_set_item(profile, "PayloadContent", content);

    return profile;
}

bool profile_manager::install_profile(const char *path) {
    lockdownd_service_descriptor_t service = this->device.start_service("com.apple.mobile.MCInstall");
    property_list_service_client_t client{nullptr};
    if (property_list_service_client_new(this->device.get_device(), service, &client) != PROPERTY_LIST_SERVICE_E_SUCCESS)
    {
        std::cerr << "failed to get property list service" << std::endl;
        return false;
    }

    plist_t profile = create_restriction_profile();
    char* xml{nullptr}; uint32_t length{0};
    plist_to_xml(profile, &xml, &length);

    plist_t command = plist_new_dict();
    plist_dict_set_item(command, "RequestType", plist_new_string("InstallProfile"));
    plist_dict_set_item(command, "Payload", plist_new_data(xml, length));

    property_list_service_send_xml_plist(client, command);
    free(xml);
    plist_free(profile);
    plist_free(command);
    property_list_service_client_free(client);

    this->is_installed = true;

    return true;
}

bool profile_manager::remove_profile(const char* profile_identifier) {
    lockdownd_service_descriptor_t service = this->device.start_service("com.apple.mobile.MCInstall");
    property_list_service_client_t client{nullptr};

    if (property_list_service_client_new(this->device.get_device(), service, &client) != PROPERTY_LIST_SERVICE_E_SUCCESS)
    {
        std::cerr << "failed to get property list service" << std::endl;
        return false;
    }

    plist_t command = plist_new_dict();
    plist_dict_set_item(command, "RequestType", plist_new_string("RemoveProfile"));
    plist_dict_set_item(command, "ProfileIdentifier", plist_new_string(profile_identifier));

    property_list_service_send_xml_plist(client, command);
    plist_free(command);
    property_list_service_client_free(client);

    this->is_installed = false;
    return true;
}