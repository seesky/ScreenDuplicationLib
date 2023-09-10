/*
 * Author: XueLong Ba
 * Date: 2023-09-04
 * Description: 
 */

#pragma once

#include <glib.h>
#include <Windows.h>

typedef struct DISPLAY_DEVICE_NAME_MAP{
    gchar *DeviceName;
    gchar *DeviceString;
    int is_primary_display_device;
}DISPLAY_DEVICE_NAME_MAP;

GHashTable* get_display_device_name_mapping();
HMONITOR get_hmonitor_by_point(LONG x, LONG y);

