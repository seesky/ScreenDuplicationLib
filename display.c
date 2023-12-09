/*
 * Author: XueLong Ba
 * Date: 2023-09-05
 * Description: 
 */

#include "displayl-priv.h"
#include "dll/d3d_l.h"
#include "dll/dxgi_l.h"
#include "dll/user32.h"
#include "dll/shcore.h"
#include "json-glib/json-glib.h"
#include "capture-output-priv.h"

G_DEFINE_TYPE(DisplayL, display_l, G_TYPE_OBJECT);

static void display_l_init(DisplayL *self)
{
    /*
    self->priv->name = "Unknown";
    self->priv->adapter_name = "Unknown Adapter";
    self->priv->resolutionX = 0;
    self->priv->resolutionY = 0;
    self->priv->positionLeft = 0;
    self->priv->positionTop = 0;
    self->priv->positionRight = 0;
    self->priv->positionButtom = 0;
    self->priv->rotation = 1.0;
    self->priv->is_primary = TRUE;
    self->priv->hmonitor = NULL;
    self->priv->dxgi_adapter = NULL;
    self->priv->dxgi_output = NULL;
    self->priv->d3d_device = NULL;
    self->priv->d3d_device_context = NULL;
    self->priv->dxgi_output_deplication = NULL;
    */
}

gchar* display_l_repr(DisplayL *self)
{
    
    return g_strconcat("<Display name=", self->priv->name, 
        " adapter=", self->priv->adapter_name, 
        " resolution=", self->priv->resolutionX, "x", self->priv->resolutionY, 
        " rotation=", self->priv->rotation, 
        " scale_factor=", self->priv->scale_factor, 
        " primary=", self->priv->is_primary, ">");
    
}

static void display_l_class_init(DisplayLClass *klass)
{

}

int *display_l_get_clean_region(DisplayL *self, int *region)
{
    //int returnValue[4];
    int *returnValue = (int *)g_malloc0(4 * sizeof(int));
    if(region == NULL)
    {
        returnValue[0] = 0;
        returnValue[1] = 0;
        returnValue[2] = self->priv->resolutionX;
        returnValue[3] = self->priv->resolutionY;
        return returnValue;
    }

    if(region[0] < 0 || region[0] > self->priv->resolutionX){
        returnValue[0] = 0;
    }else{
        returnValue[0] = region[0];
    }

    if(region[1] < 0 || region[1] > self->priv->resolutionY){
        returnValue[1] = 0;
    }else{
        returnValue[1] = region[1];
    }

    if(region[2] < 0 || region[2] > self->priv->resolutionX){
        returnValue[2] = self->priv->resolutionX;
    }else{
        returnValue[2] = region[2];
    }

    if(region[3] < 0 || region[3] > self->priv->resolutionY){
        returnValue[3] = self->priv->resolutionY;
    }else{
        returnValue[3] = region[3];
    }

    return returnValue;
}

GList *display_l_discover_displays()
{
    GHashTable *display_device_name_mapping = get_display_device_name_mapping();
    IDXGIFactory1 *dxgi_factory =  initialize_dxgi_factory();
    GList *dxgi_adapters = discover_dxgi_adapters(dxgi_factory);
    release_dxgi_factory(dxgi_factory); //释放IDXGIFactory
    
    GList *displays = NULL;

    GList *adapterIter = dxgi_adapters;
    while(adapterIter != NULL){
        IDXGIAdapter1 *dxgi_adapter = (IDXGIAdapter1 *)adapterIter->data;

        gchar* dxgi_adapter_description = describe_dxgi_adapter(dxgi_adapter);

        //
        GList *dxgi_outputs =  discover_dxgi_outputs(dxgi_adapter);
        GList *outputsIter = dxgi_outputs;
        while(outputsIter != NULL){
            IDXGIOutput1 *dxgi_output = (IDXGIOutput1 *)outputsIter->data;
            //description是json结构的数据
            gchar *dxgi_output_description = describe_dxgi_output(dxgi_output);
            //
            GError *error = NULL;
            JsonParser *parser = json_parser_new();
            if (json_parser_load_from_data(parser, dxgi_output_description, -1, &error)) {
                JsonObject *root = json_node_get_object(json_parser_get_root(parser));
                const gchar *name = json_object_get_string_member(root, "name");
                if(json_object_get_boolean_member(root, "is_attached_to_desktop")){
                    const DISPLAY_DEVICE_NAME_MAP* display_device = (DISPLAY_DEVICE_NAME_MAP*)g_hash_table_lookup(display_device_name_mapping, name);
 
                    if(display_device == NULL){
                        //display_device = ;
                    }
                    //get resolution
                    JsonObject *resolution = json_object_get_object_member(root, "resolution");
                    int width = json_object_get_int_member(resolution, "width");
                    int height = json_object_get_int_member(resolution, "height");
                    //get position
                    JsonObject *position = json_object_get_object_member(root, "position");
                    int positionLeft = json_object_get_int_member(position, "left");
                    int positionRight = json_object_get_int_member(position, "right");
                    int positionTop = json_object_get_int_member(position, "top");
                    int positionBottom = json_object_get_int_member(position, "bottom");
                    //get rotation
                    int rotation = json_object_get_int_member(root, "rotation");

                    HMONITOR hmonitor = get_hmonitor_by_point(positionLeft, positionTop);

                    float scale_factor = get_scale_factor_for_monitor(hmonitor);


                    DisplayL *display = display_l_new(display_device->DeviceName, 
                                                    dxgi_adapter_description,
                                                    width, height,
                                                    positionLeft, positionTop, positionRight, positionBottom,
                                                    rotation, scale_factor, display_device->is_primary_display_device, hmonitor,
                                                    dxgi_output, dxgi_adapter);

                    displays = g_list_append(displays, display);
                }

                
            }else{
                //TODO:json解析错误，需要处理
                g_warning("error:%s", error);
            }
            g_usleep(1000000);
            outputsIter = outputsIter->next;
            
        }
        g_list_free(dxgi_outputs);
        adapterIter = adapterIter->next;
    }

    g_list_free(dxgi_adapters);

    return displays;
}

IDXGIOutputDuplication *display_l_initialize_dxgi_output_duplication(DisplayL *self)
{
    //TODO:明确调用关系
    D3DDeviceReturnValue returnValue;
    returnValue = initialize_d3d_device(self->priv->dxgi_adapter);
    self->priv->d3d_device_context = returnValue.d3d_device_context;
    self->priv->d3d_device = returnValue.d3d_device;
    return initialize_dxgi_output_duplication(self->priv->dxgi_output, returnValue.d3d_device);
}

DisplayL *display_l_new(gchar *name, gchar *adapter_name, 
    gint resolutionX, gint resolutionY, gint positionLeft,
    gint positionTop, gint positionRight, gint positionButtom,
    gint rotation, gfloat scale_factor, gboolean is_primary,
    HMONITOR hmonitor, IDXGIOutput1 *dxgi_output, IDXGIAdapter1 *dxgi_adapter)
{
    DisplayL *displayl = g_new(DisplayL, 1);
    displayl = DISPLAYL(g_object_new(DISPLAYL_TYPE, NULL));
    DisplayLPrivate *priv = g_new(DisplayLPrivate, 1);
    displayl->priv = priv;
    priv->name = name;
    priv->adapter_name = adapter_name;
    priv->resolutionX = resolutionX;
    priv->resolutionY = resolutionY;
    priv->positionTop = positionTop;
    priv->positionLeft = positionLeft;
    priv->positionRight = positionRight;
    priv->positionButtom = positionButtom;
    priv->rotation = rotation;
    priv->scale_factor = scale_factor;
    priv->is_primary = is_primary;
    priv->hmonitor = hmonitor;
    priv->dxgi_output = dxgi_output;
    priv->dxgi_adapter = dxgi_adapter;
    
    priv->dxgi_output_deplication = display_l_initialize_dxgi_output_duplication(displayl);

    return displayl;
}

void *display_l_capture(DisplayL *self, CaptureOutputL *captureOutput, GetFrameL process_func, int *region, D3dshotL *d3dshot)
{
    int *regionl = display_l_get_clean_region(self, region);
    
    //TODO:需要接收并返回frame
    void *frame = get_dxgi_output_duplication_frame(captureOutput, self->priv->dxgi_output_deplication, 
                                    self->priv->d3d_device, process_func, self->priv->resolutionX, self->priv->resolutionY, regionl, self->priv->rotation, self, d3dshot);
    
    return frame; //return frame
}