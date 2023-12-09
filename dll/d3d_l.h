/*
 * Author: XueLong Ba
 * Date: 2023-09-05
 * Description: 
 */

#pragma once

#include "com_l.h"
#include <mmdeviceapi.h>
#include <d3d11.h>

// "1841e5c8-16b0-489b-bcc8-44cfb0d5deae"
PA_DEFINE_IID(ID3D11DeviceChild,      1841e5c8, 16b0, 489b, bc, c8, 44, cf, b0, d5, de, ae);
// "dc8e63f3-d12b-4952-b47b-5e45026a862d"
PA_DEFINE_IID(ID3D11Resource, dc8e63f3, d12b, 4952, b4, 7b, 5e, 45, 02, 6a, 86, 2d);
// "6f15aaf2-d208-4e89-9ab4-489535d34f9c"
PA_DEFINE_IID(ID3D11Texture2D, 6f15aaf2, d208, 4e89, 9a, b4, 48, 95, 35, d3, 4f, 9c);
// "c0bfa96c-e089-44fb-8eaf-26f8796190da"
PA_DEFINE_IID(ID3D11DeviceContext, c0bfa96c, e089, 44fb, 8e, af, 26, f8, 79, 61, 90, da);
// "db6f6ddb-ac77-4e88-8253-819df9bbf140"
PA_DEFINE_IID(ID3D11Device, db6f6ddb, ac77, 4e88, 82, 53, 81, 9d, f9, bb, f1, 40);




typedef struct {
    ID3D11Device *d3d_device;
    ID3D11DeviceContext *d3d_device_context;
} D3DDeviceReturnValue;

D3DDeviceReturnValue initialize_d3d_device(IDXGIAdapter *dxgi_adapter);
D3D11_TEXTURE2D_DESC  describe_d3d11_texture_2d(ID3D11Texture2D *d3d11_texture_2d);
ID3D11Texture2D *  prepare_d3d11_texture_2d_for_cpu(ID3D11Texture2D *d3d11_texture_2d, ID3D11Device *d3d_device);