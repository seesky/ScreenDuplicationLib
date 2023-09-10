/*
 * Author: XueLong Ba
 * Date: 2023-09-04
 * Description: 
 */


#include <Windows.h>
#include <glib.h>
#include <wchar.h>
#include "../dll/user32.h"
#include "../utils/dxgi_utils.h"



GHashTable* get_display_device_name_mapping()
{
    //std::vector<std::tuple<std::wstring, bool>> display_names;
    GHashTable* display_device_name_mapping = g_hash_table_new(g_str_hash, g_str_equal);

    for (DWORD deviceIndex = 0; ; ++deviceIndex) {
        DISPLAY_DEVICEW dd;
        dd.cb = sizeof(DISPLAY_DEVICEW);

        if (EnumDisplayDevicesW(NULL, deviceIndex, &dd, 0)) {
            if (dd.StateFlags > 0) {
                //struct DISPLAY_DEVICE_NAME_MAP *ddnm = {0};
                struct DISPLAY_DEVICE_NAME_MAP* ddnm = (struct DISPLAY_DEVICE_NAME_MAP*)malloc(sizeof(struct DISPLAY_DEVICE_NAME_MAP));
                BOOL is_primary_display_device = (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) != 0;
                WCHAR device_name[32];
                WCHAR *device_string = dd.DeviceString;
                WCHAR_SPLIT(dd.DeviceName, device_name);
                //WCHAR_ARRAY_COPY(dd.DeviceString, ddnm.DeviceString, 128);
                ddnm->is_primary_display_device = is_primary_display_device;
                //gchar* utf8_final_device_name = g_utf16_to_utf8((gunichar2*)ddnm.DeviceName, -1, NULL, NULL, NULL);
                ddnm->DeviceName = g_utf16_to_utf8((gunichar2*)device_name, -1, NULL, NULL, NULL);
                ddnm->DeviceString = g_utf16_to_utf8((gunichar2*)device_string, -1, NULL, NULL, NULL);
                g_hash_table_insert(display_device_name_mapping,  ddnm->DeviceName, ddnm);
            }
        } else {
            break; // 没有更多的显示设备可用
        }
    }

    return display_device_name_mapping;
}

HMONITOR get_hmonitor_by_point(LONG x, LONG y)
{
    POINT point;
    point.x = x;
    point.y = y;
    return MonitorFromPoint(point, 0);
}

