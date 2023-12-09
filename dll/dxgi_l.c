/*
 * Author: XueLong Ba
 * Date: 2023-09-04
 * Description: 
 */

#include "../dll/dxgi_l.h"
#include "../utils/dxgi_utils.h"
#include "d3d_l.h"
#include "dxgi.h"
#include "dxgi1_2.h"
#include "d3d11.h"
#include <glib.h>
#include <json-glib/json-glib.h>
#include <windows.h>
#include "../screen-duplication-types.h"
#include "../displayl-priv.h"
#include "../capture_outputs/gstreamer-capture-output-priv.h"
#include <glib.h>
#include "../d3dshot.h"


const IID * const pa_IID_IDXGIFactory1_c = &pa_IID_IDXGIFactory1;
const IID * const pa_IID_ID3D11Texture2D_c = &pa_IID_ID3D11Texture2D;
const IID * const pa_IID_IDXGISurface_c = &pa_IID_IDXGISurface;

IDXGIFactory1 * initialize_dxgi_factory()
{
    IDXGIFactory1 *idxgi_factory;
    CreateDXGIFactory1(pa_IID_IDXGIFactory1_c, (void**)&idxgi_factory);
    return idxgi_factory;
}

void release_dxgi_factory(IDXGIFactory1 *idxgi_factory)
{
    idxgi_factory->lpVtbl->Release(idxgi_factory);
}

GList* discover_dxgi_adapters(IDXGIFactory1 *dxgi_factory)
{
    GList *dxgi_adapters = NULL;

    for(int i = 0; i < 10; i++)
    {
        HRESULT hr;
        IDXGIAdapter1 *dxgi_adapter = (struct IDXGIAdapter1*)malloc(sizeof(struct IDXGIAdapter1));
        hr = dxgi_factory->lpVtbl->EnumAdapters1(dxgi_factory, i, &dxgi_adapter);
        if(hr == S_OK)
        {
            dxgi_adapters = g_list_append(dxgi_adapters, dxgi_adapter);
        }
    }

    return dxgi_adapters;
} 

//TODO:函数有问题，可能是因为没有分配内存
gchar *describe_dxgi_adapter(IDXGIAdapter1 *dxgi_adapter)
{
    DXGI_ADAPTER_DESC1 *dxgi_adapter_description = g_new(DXGI_ADAPTER_DESC1, 1);
    dxgi_adapter->lpVtbl->GetDesc1(dxgi_adapter, dxgi_adapter_description);
    WCHAR description[128];
    WCHAR_ARRAY_COPY(dxgi_adapter_description->Description, description, 128);
    gchar *returnValue = NULL;
    returnValue = g_utf16_to_utf8((gunichar2*)description, -1, NULL, NULL, NULL);
    return returnValue;
}

GList * discover_dxgi_outputs(IDXGIAdapter1 *dxgi_adapter)
{
    GList *dxgi_outputs = NULL;
    for(int i = 0; i < 10; i++)
    {
        IDXGIOutput1 *dxgi_output;
        HRESULT hr = dxgi_adapter->lpVtbl->EnumOutputs(dxgi_adapter, i, &dxgi_output);
        if(hr == S_OK)
        {
            dxgi_outputs = g_list_append(dxgi_outputs, dxgi_output);
        }
    }
    return dxgi_outputs;
}

//{"name":".\\DISPLAY1","position":{"left":0,"top":0,"right":2560,"bottom":1440},"resolution":{"width":2560,"height":1440},"rotation":1,"is_attached_to_destop":true}
gchar * describe_dxgi_output(IDXGIOutput1 *dxgi_output)
{
    DXGI_OUTPUT_DESC dxgi_output_description;
    dxgi_output->lpVtbl->GetDesc(dxgi_output, &dxgi_output_description);

    GHashTable *rotation_mapping = g_hash_table_new(g_int_hash, g_int_equal);
    g_hash_table_insert(rotation_mapping, "0", "0");
    g_hash_table_insert(rotation_mapping, "1", "0");
    g_hash_table_insert(rotation_mapping, "2", "90");
    g_hash_table_insert(rotation_mapping, "3", "180");
    g_hash_table_insert(rotation_mapping, "4", "270");
    

    JsonObject *root = json_object_new();

    //添加设备名称
    WCHAR deviceNameT[32];
    WCHAR*deviceNameF = dxgi_output_description.DeviceName;
    WCHAR_SPLIT(deviceNameF, deviceNameT);
    gchar* utf8_final_device_name = g_utf16_to_utf8((gunichar2*)deviceNameT, -1, NULL, NULL, NULL);
    json_object_set_string_member(root, "name", utf8_final_device_name);

    //添加坐标信息
    JsonObject *position = json_object_new();
    int left =  dxgi_output_description.DesktopCoordinates.left;
    int top = dxgi_output_description.DesktopCoordinates.top;
    int right = dxgi_output_description.DesktopCoordinates.right;
    int bottom = dxgi_output_description.DesktopCoordinates.bottom;
    json_object_set_int_member(position, "left", left);
    json_object_set_int_member(position, "top", top);
    json_object_set_int_member(position, "right", right);
    json_object_set_int_member(position, "bottom", bottom);
    json_object_set_object_member(root, "position", position);

    //添加分辨率
    JsonObject *resolution_obj = json_object_new();
    int width = dxgi_output_description.DesktopCoordinates.right - dxgi_output_description.DesktopCoordinates.left;
    int height = dxgi_output_description.DesktopCoordinates.bottom - dxgi_output_description.DesktopCoordinates.top;
    json_object_set_int_member(resolution_obj, "width", width);
    json_object_set_int_member(resolution_obj, "height", height);
    json_object_set_object_member(root, "resolution", resolution_obj);

    //添加屏幕旋转角度
    int rotationF = 0;
    switch (dxgi_output_description.Rotation) {
        case DXGI_MODE_ROTATION_UNSPECIFIED:
            rotationF = 0;
            break;
        case DXGI_MODE_ROTATION_IDENTITY:
            rotationF = 0;
            break;
        case DXGI_MODE_ROTATION_ROTATE90:
            rotationF = 90;
            break;
        case DXGI_MODE_ROTATION_ROTATE180:
            rotationF = 180;
            break;
        case DXGI_MODE_ROTATION_ROTATE270:
            rotationF = 270;
            break;
        default:
            rotationF = 0;
    }
    //int rotationT = g_hash_table_lookup(rotation_mapping, rotationF);
    json_object_set_int_member(root, "rotation", rotationF);

    //添加是否连接了桌面
    gboolean is_attached_to_desktop = dxgi_output_description.AttachedToDesktop ? 1 : 0;
    json_object_set_boolean_member(root, "is_attached_to_desktop", is_attached_to_desktop);


    //生成JSON数据
    JsonGenerator *generator = json_generator_new();
    JsonNode *json_node = json_node_new(JSON_NODE_OBJECT);
    json_node_set_object(json_node, root);
    json_generator_set_root(generator, json_node);  
    gchar *json_string = json_generator_to_data(generator, NULL);
    json_object_unref(root);
    //json_object_unref(generator);
    g_print("Generated JSON: %s\n", json_string);
    return json_string;
    
}

IDXGIOutputDuplication * initialize_dxgi_output_duplication(IDXGIOutput1 *dxgi_output, ID3D11Device *d3d_device)
{
    IDXGIOutputDuplication *dxgi_output_duplication = g_new(IDXGIOutputDuplication, 1);
    dxgi_output->lpVtbl->DuplicateOutput(dxgi_output, d3d_device, &dxgi_output_duplication);
    return dxgi_output_duplication;
}

void *get_dxgi_output_duplication_frame(CaptureOutputL *captureOutput, IDXGIOutputDuplication *dxgi_output_duplication, ID3D11Device *d3d_device, GetFrameL process_func, int width, int height, int *region, int rotation, DisplayL *display, D3dshotL *d3dshot)
{
    DXGI_OUTDUPL_FRAME_INFO *dxgi_output_duplication_frame_information = g_new(DXGI_OUTDUPL_FRAME_INFO, 1);
    IDXGIResource *dxgi_resource = g_new(IDXGIResource, 1);

    void *frame = NULL;
    
    HRESULT hr = dxgi_output_duplication->lpVtbl->AcquireNextFrame(dxgi_output_duplication, 0, dxgi_output_duplication_frame_information, &dxgi_resource);
    if (SUCCEEDED(hr)) {
        if(dxgi_output_duplication_frame_information->LastPresentTime.QuadPart > 0)
        {
            //
            ID3D11Texture2D *id3d11_texture_2d;
            
            if(dxgi_resource == NULL){
                g_free(dxgi_output_duplication_frame_information);
                return NULL;
            }else{
                //g_warning("dxgi_resource not null....");
                //g_warning("dxgi_resource value:%s", dxgi_resource);
            }
            
            dxgi_resource->lpVtbl->QueryInterface(dxgi_resource, pa_IID_ID3D11Texture2D_c, &id3d11_texture_2d);


            //
            ID3D11Texture2D *id3d11_texture_2d_cpu;
            id3d11_texture_2d_cpu = prepare_d3d11_texture_2d_for_cpu(id3d11_texture_2d, d3d_device);
            //
            ID3D11DeviceContext *d3d_device_context;
            d3d_device->lpVtbl->GetImmediateContext(d3d_device, &d3d_device_context);
            //
            d3d_device_context->lpVtbl->CopyResource(d3d_device_context, id3d11_texture_2d_cpu, id3d11_texture_2d);
            //
            IDXGISurface *id3d11_surface;
            id3d11_texture_2d_cpu->lpVtbl->QueryInterface(id3d11_texture_2d_cpu, pa_IID_IDXGISurface_c, &id3d11_surface);
            //
            DXGI_MAPPED_RECT dxgi_mapped_rect;
            id3d11_surface->lpVtbl->Map(id3d11_surface, &dxgi_mapped_rect, 1);

            BYTE *pointer = dxgi_mapped_rect.pBits; //像素数据指针
            int pitch = dxgi_mapped_rect.Pitch;

            int size;
            if(rotation == 0 || rotation == 180){
                size = pitch * height;
            }else{
                size = pitch * width;
            }

            //TODO:需要把返回值赋值给frame，目前需要确认frame类型
            frame = process_func(captureOutput, pointer, pitch, size, width, height, region, rotation);

            id3d11_surface->lpVtbl->Unmap(id3d11_surface);
            id3d11_surface->lpVtbl->Release(id3d11_surface);
            id3d11_texture_2d_cpu->lpVtbl->Release(id3d11_texture_2d_cpu);
            d3d_device_context->lpVtbl->Release(d3d_device_context);

            id3d11_texture_2d->lpVtbl->Release(id3d11_texture_2d);
            dxgi_resource->lpVtbl->Release(dxgi_resource);
        }

        dxgi_output_duplication->lpVtbl->ReleaseFrame(dxgi_output_duplication);
    } else if (hr == DXGI_ERROR_WAIT_TIMEOUT) {
        // 没有新帧可用，可以继续等待下一帧
        g_warning("no new frame,wait next...");
    } else if (hr == DXGI_ERROR_ACCESS_LOST) {


        d3dshot_l_rebuild(d3dshot);
        


        g_warning("shoud init dxgi_output_duplication");
        
    } else {


        d3dshot_l_rebuild(d3dshot);
        
        g_warning("get_dxgi_output_duplication_frame other error。");
    }

    //TODO: 需要返回正确的Frame
    return frame;

}