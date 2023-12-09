/*
 * Author: XueLong Ba
 * Date: 2023-09-04
 * Description: 
 */

#pragma once

#include "com_l.h"
#include <mmdeviceapi.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <glib.h>
#include "../utils/dxgi_utils.h"



// "aec22fb8-76f3-4639-9be0-28eb43a67a2e"
PA_DEFINE_IID(IDXGIObject,      aec22fb8, 76f3, 4639, 9b, e0, 28, eb, 43, a6, 7a, 2e);
// "3d3e0379-f9de-4d58-bb6c-18d62992f1a6"
PA_DEFINE_IID(IDXGIDeviceSubObject, 3d3e0379, f9de, 4d58, bb, 6c,18, d6, 29, 92, f1, a6);
// "035f3ab4-482e-4e50-b41f-8a7f8bd8960b"
PA_DEFINE_IID(IDXGIResource, 035f3ab4, 482e, 4e50, b4, 1f, 8a, 7f, 8b, d8, 96, 0b);
// "cafcb56c-6ac3-4889-bf47-9e23bbd260ec"
PA_DEFINE_IID(IDXGISurface, cafcb56c, 6ac3, 4889, bf, 47, 9e, 23, bb, d2, 60, ec);
// "191cfac3-a341-470d-b26e-a864f428319c"
PA_DEFINE_IID(IDXGIOutputDuplication, 191cfac3, a341, 470d, b2, 6e, a8, 64, f4, 28, 31, 9c);
// "ae02eedb-c735-4690-8d52-5a8dc20213aa"
PA_DEFINE_IID(IDXGIOutput, ae02eedb, c735, 4690, 8d, 52, 5a, 8d, c2, 02, 13, aa);
// "00cddea8-939b-4b83-a340-a685226666cc"
PA_DEFINE_IID(IDXGIOutput1, 00cddea8, 939b, 4b83 , a3, 40, a6, 85, 22, 66, 66, cc);
// "2411e7e1-12ac-4ccf-bd14-9798e8534dc0"
PA_DEFINE_IID(IDXGIAdapter, 2411e7e1, 12ac, 4ccf, bd, 14, 97, 98, e8, 53, 4d, c0);
// "29038f61-3839-4626-91fd-086879011a05"
PA_DEFINE_IID(IDXGIAdapter1, 29038f61, 3839, 4626, 91, fd, 08, 68, 79, 01, 1a, 05);
// "7b7166ec-21c7-44ae-b21a-c9ae321ae369"
PA_DEFINE_IID(IDXGIFactory, 7b7166ec, 21c7, 44ae, b2, 1a, c9, ae, 32, 1a, e3, 69);
// "770aae78-f26f-4dba-a829-253c83d1b387"
PA_DEFINE_IID(IDXGIFactory1, 770aae78, f26f, 4dba, a8, 29, 25, 3c, 83, d1, b3, 87);




IDXGIFactory1 * initialize_dxgi_factory();
void release_dxgi_factory(IDXGIFactory1 *idxgi_factory);
GList* discover_dxgi_adapters(IDXGIFactory1 *dxgi_factory);
gchar* describe_dxgi_adapter(IDXGIAdapter1 *dxgi_adapter);
GList * discover_dxgi_outputs(IDXGIAdapter1 *dxgi_adapter);
gchar * describe_dxgi_output(IDXGIOutput1 *dxgi_output);
IDXGIOutputDuplication * initialize_dxgi_output_duplication(IDXGIOutput1 *dxgi_output, ID3D11Device *d3d_device);
void *get_dxgi_output_duplication_frame(CaptureOutputL *captureOutput, IDXGIOutputDuplication *dxgi_output_duplication, ID3D11Device *d3d_device, GetFrameL process_func, int width, int height, int *region, int rotation, DisplayL *display, D3dshotL *dedshot);

