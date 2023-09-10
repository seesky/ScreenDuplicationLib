/*
 * Author: XueLong Ba
 * Date: 2023-09-06
 * Description: 
 */

#pragma once

#include "glib.h"
#include "glib-object.h"
#include "../capture-output-priv.h"

#define GSTREAMER_OUTPUTL_TYPE (gstreamer_output_l_get_type())
#define GSTREAMER_OUTPUTL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSTREAMER_OUTPUTL_TYPE, GstreamerOutputL))
#define GSTREAMER_OUTPUTL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GSTREAMER_OUTPUTL_TYPE, GstreamerOutputLClass))
#define IS_GSTREAMER_OUTPUTL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSTREAMER_OUTPUTL_TYPE))
#define IS_GSTREAMER_OUTPUTL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GSTREAMER_OUTPUTL_TYPE))
#define GSTREAMER_OUTPUTL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GSTREAMER_OUTPUTL_TYPE, GstreamerOutputLClass))

struct _GstreamerOutputL
{
    CaptureOutputL parent;
    GstreamerOutputLPrivate *priv;
    /* Do not add fields to this struct */
};

struct _GstreamerOutputLClass
{
    CaptureOutputLClass parent_class;
    GstreamerOutputLClassPrivate *priv;
};

GType gstreamer_output_l_get_type(void);

typedef struct {
    int pitch;
    int size;
    int width;
    int height;
    int* region;
    int rotation;
    GstBuffer *buffer;
}FrameData ;

GstreamerOutputL *gstreamer_output_l_new(int width, int height);
void *gstreamer_output_l_process(GstreamerOutputL *captureOutput, BYTE* pointer, int pitch, int size, int width, int height, int* region, int rotation);