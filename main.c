/*
 * Author: XueLong Ba
 * Date: 2023-09-04
 * Description: 
 */

#include "dll/user32.h"
#include "dll/shcore.h"
#include "dll/dxgi_l.h"
#include "glib.h"
#include "dxgi.h"
#include <stdio.h>
#include "screen-duplication-types.h"
#include "display.h"
//#include "d3dshot.h"
#include "d3dshot-priv.h"
#include "capture-output.h"
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include "capture_outputs/gstreamer-capture-output-priv.h"

static GstElement *appsrc;

void Test_user32()
{
    GHashTable* display_device_name_mapping = g_hash_table_new(g_str_hash, g_int_equal);
    get_display_device_name_mapping(display_device_name_mapping);

    const gchar* key_to_find = ".\\DISPLAY1";
    const DISPLAY_DEVICE_NAME_MAP* found_value = (DISPLAY_DEVICE_NAME_MAP*)g_hash_table_lookup(display_device_name_mapping, key_to_find);
    
    for(int i = 0; i < 128; i++)
    {
        printf_s("%c", found_value->DeviceString[i]);
    }
}

void Test_shcore(int monitor)
{

    POINT pt; // 定义一个点坐标
    pt.x = 100; // 设置点的 x 坐标
    pt.y = 100; // 设置点的 y 坐标
    HMONITOR hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
    if (hMonitor != NULL) {
        float scaleFactor = get_scale_factor_for_monitor(hMonitor);
        printf("Scale Factor:%f", scaleFactor);
    }
    
}

void Test_dxgi_l()
{
    
    IDXGIFactory1 *iDXGIFactory;
    iDXGIFactory = initialize_dxgi_factory();

    GList *dxgi_adapters; //IDXGIAdapter1
    dxgi_adapters = discover_dxgi_adapters(iDXGIFactory);

    GList *iter = dxgi_adapters;
    while(iter){
        IDXGIAdapter1 *adapter = iter->data;
        
        GList * outputs = discover_dxgi_outputs(adapter);

        GList *outputIter = outputs;
        while(outputIter != NULL){
            if(outputs->data != NULL){
                gchar * s = describe_dxgi_output(outputs->data);
                outputIter = outputIter->next;
                continue;
            }
        }

        
        iter = iter->next;
        adapter->lpVtbl->Release(adapter);
    }
    
    g_free(iter);
    g_free(dxgi_adapters);
    iDXGIFactory->lpVtbl->Release(iDXGIFactory);
    
    
    /*
    for(iter = dxgi_adapters; iter != NULL; iter = iter->next)
    {
        IDXGIAdapter1 *adapter = iter->data;
        WCHAR* desc = describe_dxgi_adapter(adapter);
        continue;
    }
    */
    //使用g_list_foreach遍历链表并调用回调函数
    //g_list_foreach(dxgi_adapters, print_item, NULL);

}

void Test_DisplayL()
{
    //DisplayL *display = display_l_new();
}


int main(int argc, int argv[])
{

    // Initialize GStreamer
    gst_init(&argc, &argv);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

    D3dshotL *d = d3dshot_l_new(GSTREAMER_BUFFER, 60, TRUE);


    g_usleep(5000000); 
    d3dshot_l_capture(d, 60, NULL, NULL);



    g_main_loop_run(loop);

    g_main_loop_unref(loop);

    return 0;
}