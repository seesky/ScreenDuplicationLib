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

static GstFlowReturn gstreamer_output_l_new_sample (GstElement *sink, void *data) {
  GstSample *sample;

  /* Retrieve the buffer */
  g_signal_emit_by_name (sink, "pull-sample", &sample);
  if (sample) {
    /* The only thing we do in this example is print a * to indicate a received buffer */
    g_print ("*");
    gst_sample_unref (sample);
    return GST_FLOW_OK;
  }

  return GST_FLOW_ERROR;
}

static void gstreamer_output_l_cb_message (GstBus *bus, GstMessage *msg, GstreamerOutputLPrivate *data) {

  switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_ERROR: {
      GError *err;
      gchar *debug;

      gst_message_parse_error (msg, &err, &debug);
      g_print ("Error: %s\n", err->message);
      g_error_free (err);
      g_free (debug);

      gst_element_set_state (data->pipeline, GST_STATE_READY);
      //TODO:退出循环
      break;
    }
    case GST_MESSAGE_EOS:
      /* end-of-stream */
      gst_element_set_state (data->pipeline, GST_STATE_READY);
      //TODO:退出循环
      break;
    case GST_MESSAGE_BUFFERING: {
      gint percent = 0;
      g_warning("GST_MESSAGE_BUFFERING..............");

      /* If the stream is live, we do not care about buffering. */
      //if (data->is_live) break;


      //gst_message_parse_buffering (msg, &percent);
      //g_print ("Buffering (%3d%%)\r", percent);
      /* Wait until buffering is complete before start/resume playing */
      /*
      if (percent < 100)
        gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
      else
        gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
      break;
      */
    }
    case GST_MESSAGE_CLOCK_LOST:
      /* Get a new clock */
      g_warning("GST_MESSAGE_CLOCK_LOST..............");
      gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
      gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
      break;
    default:

      //g_warning("UNAHNDLED MESSAGE TYPE:%d", GST_MESSAGE_TYPE (msg));
      /* Unhandled message */
      break;
    }
}



GstreamerOutputL *gstreamer_output_l_new(int width, int height)
{
    GstreamerOutputL *gstreamerOutput = g_new(GstreamerOutputL, 1);
    gstreamerOutput = GSTREAMER_OUTPUTL(g_object_new(GSTREAMER_OUTPUTL_TYPE,  NULL));
    GstreamerOutputLPrivate *priv = g_new(GstreamerOutputLPrivate, 1);
    gstreamerOutput->priv = priv;

    GError *error = NULL;
    gchar *video_caps = 
        g_strdup_printf("video/x-raw,format=\"BGRA\",width=%d,height=%d,framerate=%d/1", width, height, 30);
    g_warning("video_caps:%s", video_caps);
    //!!!>读取h264码流：gst-launch-1.0.exe filesrc location="d:/output.h264" ! queue ! h264parse ! nvh264dec ! d3d11videosink
    //!!!桌面图像吸入h264码流文件：gchar *pipeline = g_strdup_printf("appsrc is-live=1 block=1 caps=\"%s\" name=\"appsrc\"  ! queue ! videoconvert ! nvh264enc ! h264parse ! filesink sync=0 location=\"d:/output.h264\"", video_caps);
    //!!!图像数据编解码后写入BGRA流文件：gchar *pipeline = g_strdup_printf("appsrc is-live=1 block=1 caps=\"%s\" name=\"appsrc\" ! queue ! videoconvert ! nvh264enc ! video/x-h264, stream-format=byte-stream ! h264parse ! nvh264dec ! queue ! videoconvert ! %s ! filesink location=\"d:/output_video_bgra_raw.raw\"", video_caps, video_caps);
    //!!!读取BGRA流文件：gst-launch-1.0 filesrc location="d:/output_video_bgra_raw.raw" ! videoparse format=GST_VIDEO_FORMAT_RGBA width=2560 height=1440 framerate=30/1 ! videoconvert ! autovideosink
    //!!!读取BGRA流文件直接压缩再解压再显示：gst-launch-1.0 filesrc location="d:/output_video_bgra_raw.raw" ! videoparse format=GST_VIDEO_FORMAT_BGRA width=2560 height=1440 framerate=30/1 ! queue ! nvh264enc ! h264parse ! nvh264dec ! videoconvert ! d3d11videosink
    //!!!

    //gchar *pipeline = g_strdup_printf("appsrc is-live=1 block=1 caps=\"%s\" name=\"appsrc\"  ! queue ! videoconvert ! nvh264enc bitrate=2000 preset=5 ! h264parse ! queue ! rtph264pay config-interval=1 pt=96 ! udpsink host=127.0.0.1 port=1234", video_caps);
   
    //gchar *pipeline = g_strdup_printf("appsrc is-live=1 block=1 caps=\"%s\" name=\"appsrc\" ! queue ! videoconvert ! %s ! nvh264enc ! video/x-h264, stream-format=byte-stream ! h264parse ! nvh264dec ! queue ! videoconvert ! %s ! filesink location=\"d:/output_video_bgra_raw.raw\"", video_caps, video_caps, video_caps);
    //gchar *pipeline = g_strdup_printf("d3d11screencapturesrc show-cursor=1 ! videoconvert !  d3d11videosink");
   
   
   
   // gchar *pipeline = g_strdup_printf("appsrc is-live=1 block=1 caps=\"%s\" name=\"appsrc\"  ! videoconvert ! queue ! nvh264enc ! h264parse ! fakesink", video_caps);
    
    //gchar *pipeline = g_strdup_printf("appsrc is-live=1 block=1 caps=\"%s\" name=\"appsrc\" ! queue ! videoconvert ! nvh264enc ! h264parse ! flvmux ! filesink sync=true location=\"%s\"", video_caps, "d:/xyz.flv");
    //gchar *pipeline = g_strdup_printf("appsrc is-live=1 block=1 caps=\"%s\" name=\"appsrc\" ! queue ! nvh264enc !  filesink sync=true location=\"%s\"", video_caps, "d:/h264.h264");

    //gchar *pipeline = g_strdup_printf("gst-launch-1.0 filesrc location=\"d:/xyz.flv\" ! h264parse ! nvh264dec ! videoconvert ! %s ! d3d11videosink", video_caps);
    //gchar *pipeline = g_strdup_printf("appsrc is-live=1 do-timestamp=0 format=time caps=\"%s\" name=\"appsrc\" ! d3d11videosink", video_caps);
    //gchar *pipeline = g_strdup_printf("appsrc is-live=1 block=1 caps=\"%s\" name=\"appsrc\" ! d3d11videosink", video_caps);

    gchar *pipeline = g_strdup_printf("appsrc is-live=1 block=1 caps=\"%s\" name=\"appsrc\"  ! queue ! videoconvert ! d3d11videosink", video_caps);
    g_warning("pipline:%s", pipeline);

    priv->pipeline = gst_parse_launch(pipeline, &error);

    

    if (error != NULL) {
            g_warning("Failed to create pipeline: %s", error->message);
            goto cleanup;
    }

    priv->bus = gst_element_get_bus (priv->pipeline);
    gst_bus_add_signal_watch (priv->bus);
    g_signal_connect (priv->bus, "message", G_CALLBACK (gstreamer_output_l_cb_message), &priv);

    gst_element_set_state(priv->pipeline, GST_STATE_PLAYING);



    priv->appsrc = gst_bin_get_by_name(GST_BIN(priv->pipeline), "appsrc");
    //priv->appsink = gst_bin_get_by_name(GST_BIN(priv->pipeline), "appsink");
    //g_signal_connect(priv->appsink, "new-sample", G_CALLBACK (gstreamer_output_l_new_sample), NULL);

cleanup:
    if (error != NULL)
        g_clear_pointer(&priv->pipeline, gst_object_unref);
        g_clear_error(&error);
        g_free(video_caps);
        g_free(pipeline);


    /*


    priv->pipeline = gst_pipeline_new(NULL);
    priv->appsrc = gst_element_factory_make("appsrc", NULL);
    //priv->d3d11upload = gst_element_factory_make("d3d11upload", NULL);
    priv->videoconvert = gst_element_factory_make("videoconvert", NULL);
    //priv->d3d11convert = gst_element_factory_make("d3d11convert", NULL);
    //priv->video_queue_from = gst_element_factory_make("queue", NULL);
    priv->flvmux = gst_element_factory_make("flvmux", NULL);
    priv->nvh264enc = gst_element_factory_make("nvh264enc", NULL);
    //priv->video_queue_to = gst_element_factory_make("queue", NULL);
    //priv->nvh264dec = gst_element_factory_make("nvh264dec", NULL);
    priv->d3d11videosink = gst_element_factory_make("d3d11videosink", NULL);
    //priv->appsink = gst_element_factory_make("appsink", "appsink");
    priv->filesink = gst_element_factory_make("filesink", "");
    priv->h264parse = gst_element_factory_make("h264parse", "");
    

    // Set properties for appsrc
    g_object_set(GST_APP_SRC(priv->appsrc),
                 "caps", gst_caps_new_simple("video/x-raw",
                                             "format", G_TYPE_STRING, "BGRA",
                                             "width", G_TYPE_INT, width,
                                             "height", G_TYPE_INT, height,
                                             "framerate", GST_TYPE_FRACTION, 30, 1,
                                             NULL),
                 "is-live", TRUE,
                 "block", TRUE,
                 NULL);
    
    g_object_set(GST_ELEMENT(priv->filesink),
                "location", "xyz.flv",
                NULL);


    // Build the pipeline
    gst_bin_add_many(GST_PIPELINE(priv->pipeline), priv->appsrc, priv->h264parse, priv->videoconvert, priv->nvh264enc, priv->flvmux, priv->filesink, NULL);
    gst_element_link(priv->appsrc, priv->videoconvert);
    gst_element_link(priv->videoconvert, priv->nvh264enc);
    gst_element_link(priv->nvh264enc, priv->h264parse);
    gst_element_link(priv->h264parse, priv->flvmux);
    gst_element_link(priv->flvmux, priv->filesink);
    //gst_element_link(priv->video_queue_to, priv->nvh264dec);
    //gst_element_link(priv->videoconvert, priv->d3d11videosink);

    
    // Set the pipeline to playing state
    gst_element_set_state(priv->pipeline, GST_STATE_PLAYING);
    
    */
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

    gst_app_src_push_buffer(GST_APP_SRC(captureOutput->priv->appsrc), frameData->buffer);
    //g_warning("process a frame...");

    return frameData;
}