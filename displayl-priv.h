/*
 * Author: XueLong Ba
 * Date: 2023-09-05
 * Description: 
 */



#pragma once

#include <glib.h>
#include "dxgi1_2.h"
#include "d3d11.h"
#include "display.h"

struct _DisplayLPrivate
{
    gchar *name;
    gchar *adapter_name;
    gint resolutionX;
    gint resolutionY;
    gint positionLeft;
    gint positionTop;
    gint positionRight;
    gint positionButtom;
    gint rotation;
    gfloat scale_factor;
    gboolean is_primary;
    HMONITOR hmonitor;
    IDXGIOutput1 *dxgi_output;
    IDXGIAdapter1 *dxgi_adapter;
    ID3D11Device *d3d_device;
    ID3D11DeviceContext *d3d_device_context;
    IDXGIOutputDuplication *dxgi_output_deplication;
};

struct _DisplayLClassPrivate
{

};