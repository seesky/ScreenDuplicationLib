/*
 * Author: XueLong Ba
 * Date: 2023-09-06
 * Description: 
 */

#pragma once

#include "glib.h"
#include "glib-object.h"
#include "screen-duplication-types.h"
#include "windows.h"
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>

#define CAPTURE_OUTPUTL_TYPE (capture_output_l_get_type())
#define CAPTURE_OUTPUTL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), CAPTURE_OUTPUTL_TYPE, CaptureOutputL))
#define CAPTURE_OUTPUTL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAPTURE_OUTPUTL_TYPE, CaptureOutputLClass))
#define IS_CAPTURE_OUTPUTL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAPTURE_OUTPUTL_TYPE))
#define IS_CAPTURE_OUTPUTL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAPTURE_OUTPUTL_TYPE))
#define CAPTURE_OUTPUTL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAPTURE_OUTPUTL_TYPE, CaptureOutputLClass))

struct _CaptureOutputL {
    GObject parent;
    CaptureOutputLPrivate *priv;
    /* Do not add fields to this struct */
};

struct _CaptureOutputLClass {
    GObjectClass parent_class;
    CaptureOutputLClassPrivate *priv;
    /* Do not add fields to this struct */
};

GType capture_output_l_get_type();

CaptureOutputL *capture_output_l_new(int captureOutput, DisplayL *display);

void *capture_output_l_process(CaptureOutputL *self, BYTE pointer, int pitch, int size, int width, int height, int *region , int rotation);

