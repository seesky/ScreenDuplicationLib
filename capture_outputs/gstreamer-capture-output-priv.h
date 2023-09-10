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
    GstElement *d3d11videosink;
};

struct _GstreamerOutputLClassPrivate
{

};