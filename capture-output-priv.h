/*
 * Author: XueLong Ba
 * Date: 2023-09-06
 * Description: 
 */

#pragma once

#include "capture-output.h"

enum CaptureOutputs {
    PIL = 0,
    NUMPY = 1,
    NUMPY_FLOAT = 2,
    PYTORCH = 3,
    PYTORCH_FLOAT = 4,
    PYTORCH_GPU = 5,
    PYTORCH_FLOAT_GPU = 6,
    GSTREAMER_BUFFER = 7
};

struct _CaptureOutputLPrivate
{
    CaptureOutputL *backend;
    GstElement *appsrc;
};

struct _CaptureOutputLClassPrivate
{

};