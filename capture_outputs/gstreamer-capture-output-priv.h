/*
 * Author: XueLong Ba
 * Date: 2023-09-05
 * Description: 
 */

#pragma once

#include <glib.h>
#include "gstreamer-capture-output.h"
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>

struct _GstreamerOutputLPrivate
{
    GstElement *pipeline;
    GstElement *appsrc;
    GstElement *d3d11upload;
    GstElement *videoconvert;
    GstElement *d3d11convert;
    GstElement *video_queue_from;
    GstElement *nvh264enc;
    GstElement *video_queue_to;
    GstElement *nvh264dec;
    GstElement *appsink;
    GstElement *d3d11videosink;
    GstElement *flvmux;
    GstElement *filesink;
    GstElement *h264parse;
    GstBus *bus;
    GstStateChangeReturn ret;
};

struct _GstreamerOutputLClassPrivate
{

};