/*
 * Author: XueLong Ba
 * Date: 2023-09-04
 * Description: 
 */

#include "../dll/shcore.h"
#include <windows.h>
#include <shellscalingapi.h>


float get_scale_factor_for_monitor(HMONITOR hmonitor)
{
    UINT scale_factor;
    UINT dpiX, dpiY;
    //GetScaleFactorForMonitor(hmonitor, scale_factor);
    GetDpiForMonitor(hmonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
    return dpiY / 96.0;
}
