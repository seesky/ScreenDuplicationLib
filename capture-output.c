/*
 * Author: XueLong Ba
 * Date: 2023-09-06
 * Description: 
 */

#include "capture-output-priv.h"
#include "capture_outputs/gstreamer-capture-output-priv.h"
#include "displayl-priv.h"

G_DEFINE_TYPE(CaptureOutputL, capture_output_l, G_TYPE_OBJECT);

static void capture_output_l_init(CaptureOutputL *self)
{
    
}


static void capture_output_l_class_init()
{

}

CaptureOutputL *capture_output_l_new(int captureOutput, DisplayL *display)
{
    if(captureOutput == GSTREAMER_BUFFER)
    {
        CaptureOutputL *output = gstreamer_output_l_new(display->priv->resolutionX, display->priv->resolutionY);
        return output; //TODO:需要返回正确的后端对象，
    }
    return NULL;
}
