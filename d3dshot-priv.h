/*
 * @Description: 
 * @Version: 1.0
 * @Autor: Xuelong Ba
 * @Date: 2023-09-09 22:59:13
 * @LastEditors: Xuelong Ba
 * @LastEditTime: 2023-11-26 13:53:17
 */
/*
 * Author: XueLong Ba
 * Date: 2023-09-06
 * Description: 
 */

#pragma once

#include <glib.h>
#include "d3dshot.h"
#include "displayl-priv.h"
#include "capture-output-priv.h"

struct _D3dshotLPrivate
{
    gboolean gstreamer_is_available;
    GList *displays;
    DisplayL *display;
    CaptureOutputL *capture_output;
    gint frame_buffer_size;
    GQueue *frame_buffer;
    void *previous_screenshot;
    gint *region;
    gboolean _gstreamer_is_available;

    GThread *_capture_thread;
    gboolean _is_capturing;
    gboolean _capture_thread_finished;
};

struct _D3dshotLClassPrivate
{

};