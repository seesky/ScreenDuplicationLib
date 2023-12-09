/*
 * Author: XueLong Ba
 * Date: 2023-09-05
 * Description: 
 */

#include "d3d_l.h"
#include "dxgi.h"
#include "dxgi1_2.h"
#include "d3d11.h"
#include <windows.h>



D3DDeviceReturnValue initialize_d3d_device(IDXGIAdapter *dxgi_adapter)
{
    D3DDeviceReturnValue returnValue;

    D3D_FEATURE_LEVEL FeatureLevels[] = //支持的特性
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_1
    };

    UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);
    D3D_FEATURE_LEVEL FeatureLevel;


    ID3D11Device *d3d_device;
    ID3D11DeviceContext *d3d_device_context;
    D3D11CreateDevice(dxgi_adapter, 0, NULL, 0, FeatureLevels, NumFeatureLevels, D3D11_SDK_VERSION, &d3d_device, NULL, &d3d_device_context);


    returnValue.d3d_device = d3d_device;
    returnValue.d3d_device_context = d3d_device_context;

    return returnValue;
}


D3D11_TEXTURE2D_DESC  describe_d3d11_texture_2d(ID3D11Texture2D *d3d11_texture_2d)
{
    D3D11_TEXTURE2D_DESC d3d11_texture_2d_description;
    d3d11_texture_2d->lpVtbl->GetDesc(d3d11_texture_2d, &d3d11_texture_2d_description);
    return d3d11_texture_2d_description;
}

ID3D11Texture2D *  prepare_d3d11_texture_2d_for_cpu(ID3D11Texture2D *d3d11_texture_2d, ID3D11Device *d3d_device)
{
    D3D11_TEXTURE2D_DESC d3d11_texture_2d_description;
    d3d11_texture_2d_description = describe_d3d11_texture_2d(d3d11_texture_2d);

    D3D11_TEXTURE2D_DESC d3d11_texture_2d_description_cpu;
    d3d11_texture_2d_description_cpu.Width = d3d11_texture_2d_description.Width;
    d3d11_texture_2d_description_cpu.Height = d3d11_texture_2d_description.Height;
    d3d11_texture_2d_description_cpu.MipLevels = 1;
    d3d11_texture_2d_description_cpu.ArraySize = 1;
    d3d11_texture_2d_description_cpu.SampleDesc.Count = 1;
    d3d11_texture_2d_description_cpu.SampleDesc.Quality = 0;
    d3d11_texture_2d_description_cpu.Usage = 3;
    d3d11_texture_2d_description_cpu.Format = d3d11_texture_2d_description.Format;
    d3d11_texture_2d_description_cpu.BindFlags = 0;
    d3d11_texture_2d_description_cpu.CPUAccessFlags = 131072;
    d3d11_texture_2d_description_cpu.MiscFlags = 0;

    ID3D11Texture2D * d3d11_texture_2d_cpu;
    d3d_device->lpVtbl->CreateTexture2D(d3d_device,  &d3d11_texture_2d_description_cpu, NULL, &d3d11_texture_2d_cpu);
    return d3d11_texture_2d_cpu;
}
