/*
 * Author: XueLong Ba
 * Date: 2023-09-06
 * Description: 
 */

#include "../capture-output-priv.h"
#include "gstreamer-capture-output-priv.h"
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

G_DEFINE_TYPE(GstreamerOutputL, gstreamer_output_l, CAPTURE_OUTPUTL_TYPE);

static void gstreamer_output_l_init()
{

}

static void gstreamer_output_l_class_init()
{

}

GstreamerOutputL *gstreamer_output_l_new(int width, int height)
{
    GstreamerOutputL *gstreamerOutput = g_new(GstreamerOutputL, 1);
    gstreamerOutput = GSTREAMER_OUTPUTL(g_object_new(GSTREAMER_OUTPUTL_TYPE,  NULL));
    GstreamerOutputLPrivate *priv = g_new(GstreamerOutputLPrivate, 1);
    gstreamerOutput->priv = priv;



    priv->pipeline = gst_pipeline_new(NULL);
    priv->appsrc = gst_element_factory_make("appsrc", NULL);
    priv->d3d11videosink = gst_element_factory_make("d3d11videosink", NULL);

    // Set properties for appsrc
    g_object_set(GST_APP_SRC(priv->appsrc),
                 "caps", gst_caps_new_simple("video/x-raw",
                                             "format", G_TYPE_STRING, "BGRx",
                                             "width", G_TYPE_INT, width,
                                             "height", G_TYPE_INT, height,
                                             "framerate", GST_TYPE_FRACTION, 30, 1,
                                             NULL),
                 "is-live", TRUE,
                 "block", TRUE,
                 NULL);
    
    // Build the pipeline
    gst_bin_add_many(GST_PIPELINE(priv->pipeline), priv->appsrc, priv->d3d11videosink, NULL);
    gst_element_link(priv->appsrc, priv->d3d11videosink);
    
    // Set the pipeline to playing state
    gst_element_set_state(priv->pipeline, GST_STATE_PLAYING);

    return gstreamerOutput;
}

void *gstreamer_output_l_process(GstreamerOutputL *captureOutput, BYTE* pointer, int pitch, int size, int width, int height, int* region, int rotation)
{
    
    if(pointer == NULL){
        return NULL;
    }
    //TODO:在这里进行编解码操作
    FrameData *frameData = g_new(FrameData, 1);
    frameData->pitch = pitch;
    frameData->size = size;
    frameData->width = width;
    frameData->height = height;
    frameData->region = region;
    frameData->rotation = rotation;

    frameData->buffer = gst_buffer_new();
    frameData->buffer = gst_buffer_new_allocate(NULL, size, NULL);

    GstMapInfo map;
    if(gst_buffer_map(frameData->buffer, &map, GST_MAP_WRITE))
    {
        memcpy(map.data, pointer, size);
        gst_buffer_unmap(frameData->buffer, &map);
    }


    gst_app_src_push_buffer(captureOutput->priv->appsrc, frameData->buffer);
    g_warning("process a frame...");

    return NULL;
}