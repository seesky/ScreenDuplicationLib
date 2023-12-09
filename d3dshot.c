/*
 * Author: XueLong Ba
 * Date: 2023-09-06
 * Description: 
 */

#include "d3dshot-priv.h"
#include "capture_outputs/gstreamer-capture-output-priv.h"

G_DEFINE_TYPE(D3dshotL, d3dshot_l, G_TYPE_OBJECT);

static void d3dshot_l_init(D3dshotL *self)
{
    
}

static void d3dshot_l_class_init(D3dshotLClass *klass)
{

}

gboolean d3dshot_l_is_capturing(D3dshotL *self)
{
    return self->priv->_is_capturing;
}

void* d3dshot_l_get_frame(D3dshotL *self, gint frame_index)
{
    if(frame_index < 0 || (frame_index + 1) > self->priv->frame_buffer_size){
        return NULL;
    }
    return g_queue_peek_nth(self->priv->frame_buffer, frame_index);
}

void* d3dshot_l_get_latest_frame(D3dshotL *self)
{
    return g_queue_pop_tail(self->priv->frame_buffer);
    //return d3dshot_l_get_frame(self, 0);
}

GList *d3dshot_l_get_frames(D3dshotL *self, GArray *frame_indices)
{
    GList *frames = NULL;
    for (guint i = 0; i < frame_indices->len; i++) {
        void *frame = d3dshot_l_get_frame(self, g_array_index(frame_indices, gint, i));
        if(frame != NULL)
        {
            frames = g_list_append(frames, frame);
        }
    }
    return frames;
}

int *d3dshot_l_validate_region(D3dshotL *self, int *region)
{
    int *regionl;
    if(region == NULL && self->priv->region == NULL)
    {
    return NULL;
    }else if(region == NULL && self->priv->region != NULL){
    regionl = self->priv->region;
    }else if(region != NULL){
    regionl = region;
    }

    //TODO:需要判断region数组的格式是否正确，应该为4元素int数组

    gboolean valid = TRUE;

    if(region[2] <= region[0] || region[3] <= region[1])
    {
        valid = FALSE;
    }

    if(!valid)
    {
        //TODO:region的取值范围错误，处理
    }

    return regionl;

}


void *d3dshot_l_screenshot(D3dshotL *self, int *region)
{
    int *regionl = d3dshot_l_validate_region(self, region);

    if(self->priv->previous_screenshot == NULL)
    {
        void *frame = NULL;
        while(frame == NULL)
        {
            if(IS_GSTREAMER_OUTPUTL(self->priv->capture_output))
            {
                frame = display_l_capture(self->priv->display, self->priv->capture_output, gstreamer_output_l_process, regionl, self);
            }
        }

        self->priv->previous_screenshot = frame;
        return frame;
    }else{
        void *frame = NULL;
        for(int i = 0; i < 300; i++)
        {
            if(IS_GSTREAMER_OUTPUTL(self->priv->capture_output))
            {
                frame = display_l_capture(self->priv->display, self->priv->capture_output, gstreamer_output_l_process, regionl, self);
            }
            if(frame != NULL)
            {
                self->priv->previous_screenshot = frame;
                return frame;
            }
        }
        return self->priv->previous_screenshot;
    }
}

gchar* d3dshot_l_validate_directory(D3dshotL *self, gchar* directory)
{
    if(directory == NULL)
    {
        directory = ".";
    }

    //TODO:需要判断directory是否是合法的目录路径

    return directory;
}

gchar* d3dshot_l_validate_file_name(D3dshotL *self, gchar* file_name)
{
    if(file_name == NULL)
    {
        GTime current_time = g_get_real_time();
        file_name = g_strconcat(g_time_val_to_iso8601(&current_time), ".png", NULL);
    }

    gchar *file_extension = g_strsplit(file_name, ".", -1);

    file_extension = file_extension++;

    if(!g_strcmp0(file_extension, "png") && !g_strcmp0(file_extension, "jpg") && !g_strcmp0(file_extension, "jpeg"))
    {
        //TODO:此处需要进行错误处理，不支持其他扩展名
        return NULL;
    }

    return file_name;
}


gchar* d3dshot_l_screenshot_to_disk(D3dshotL *self, gchar *directory, gchar* file_name, int*region)
{
    file_name = d3dshot_l_validate_file_name(self, file_name);
    directory = d3dshot_l_validate_directory(self, directory);

    gchar* file_path = g_strconcat(directory, "/", file_name);

    void *frame = d3dshot_l_screenshot(self, region);

    //TODO:此处需要把获取到的图像帧保存到磁盘
    // frame_pil = self.capture_output.to_pil(frame)
    // frame_pil.save(file_path)
    return file_path;
}

void d3dshot_l_frame_buffer_to_disk(D3dshotL *self, gchar* directory)
{
    directory = d3dshot_l_validate_directory(self, directory);
    for(int i = 0; i < self->priv->frame_buffer_size; i++)
    {
        void *frame = g_queue_peek_nth(self->priv->frame_buffer, i);
        //TODO:此处需要把帧写入磁盘
        //frame_pil = self.capture_output.to_pil(frame)
        //frame_pil.save(f"{directory}/{i + 1}.png")
    }
}

gint d3dshot_l_validate_target_fps(D3dshotL *self, gint target_fps)
{
    if(target_fps < 1)
    {
        //TODO:错误处理
    }
    return target_fps;
}

void d3dshot_l_reset_frame_buffer(D3dshotL *self)
{
    if(self->priv->frame_buffer != NULL)
    {
        g_queue_clear(self->priv->frame_buffer);
    }
    self->priv->frame_buffer = g_queue_new();
}

void d3dshot_l_capturepre(gpointer data)
{
    CaptureThreadData *d = (CaptureThreadData *)data;
    D3dshotL *self = d->self;
    gint target_fps = d->target_fps;
    int *region = d->region;
    d3dshot_l_reset_frame_buffer(self);

    gfloat frame_time = 1.0 / target_fps;

    self->priv->_capture_thread_finished = FALSE;



    while(self->priv->_is_capturing && !self->priv->_capture_thread_finished)
    {
        GTime cycle_start =  g_get_real_time();

        
        //void *frame = NULL;
        if(IS_GSTREAMER_OUTPUTL(self->priv->capture_output))
        {
            
            FrameData *frame = display_l_capture(self->priv->display, self->priv->capture_output, gstreamer_output_l_process, d3dshot_l_validate_region(self, region), self);
            GstreamerOutputL *output = (GstreamerOutputL *)self->priv->capture_output;
            //gst_app_src_push_buffer(output->priv->appsrc, frame->buffer);
        }



        /*
        if(frame != NULL)
        {
            g_queue_push_head(self->priv->frame_buffer, frame);
            
            if(g_queue_get_length(self->priv->frame_buffer) > self->priv->frame_buffer_size){
                if(IS_GSTREAMER_OUTPUTL(self->priv->capture_output))
                {
                    FrameData *f = (FrameData *)g_queue_pop_tail(self->priv->frame_buffer);
                    if(GST_IS_BUFFER(f->buffer) && f->buffer != NULL){
                        gst_buffer_unref(f->buffer);
                    }
                    g_free(f->region);
                    g_free(f);
                    continue;
                }
            }
            
        }else{
            if(g_queue_get_length(self->priv->frame_buffer)){
                gpointer tail_node_data = g_queue_peek_head(self->priv->frame_buffer);
                if(tail_node_data != NULL){
                    g_queue_push_head(self->priv->frame_buffer, tail_node_data);
                }
                
                if(g_queue_get_length(self->priv->frame_buffer) > self->priv->frame_buffer_size){
                    if(IS_GSTREAMER_OUTPUTL(self->priv->capture_output))
                    {
                        FrameData *f = (FrameData *)g_queue_pop_tail(self->priv->frame_buffer);
                        if(GST_IS_BUFFER(f->buffer) && f->buffer != NULL){
                            gst_buffer_unref(f->buffer);
                        }
                        g_free(f->region);
                        g_free(f);
                        continue;
                    }
                }
                
            }
        }

        */

        GTime cycle_end = g_get_real_time();
        GTimeSpan frame_time_left = frame_time * 1000000 - (cycle_end - cycle_start);
        if(frame_time_left > 0){
            g_usleep(frame_time_left);
        }

    }
    self->priv->_is_capturing = FALSE;
}

gboolean d3dshot_l_capture(D3dshotL *self, gint target_fps, int *region, void* streamChannel)
{
    target_fps = d3dshot_l_validate_target_fps(self, target_fps);

    if(self->priv->_is_capturing)
    {
        return FALSE;
    }

    self->priv->_is_capturing = TRUE;
    
    CaptureThreadData *captureData = g_new(CaptureThreadData, 1);
    captureData->self = self;
    captureData->target_fps = target_fps;
    captureData->region = region;

    self->priv->_capture_thread = g_thread_new(NULL, d3dshot_l_capturepre, captureData);

    //g_thread_join(self->priv->_capture_thread);

    

    return TRUE;
}

void d3dshot_l_reset_displays(D3dshotL *self)
{
    self->priv->displays = NULL;
}

void d3dshot_l_detect_desplays(D3dshotL *self)
{
    self->priv->displays = display_l_discover_displays();
}

gfloat d3dshot_l_validate_interval(D3dshotL *self, gfloat interval)
{
    if(interval < 1.0)
    {
        //TODO:需要进行错误处理
        //raise AttributeError("'interval' should be one of (int, float) and be >= 1.0")
    }
    return interval;
}

void d3dshot_l_screenshot_every_pre(gpointer data)
{
    ScreenshotEveryData *d = (ScreenshotEveryData *)data;
    D3dshotL *self = d->self;
    gfloat interval = d->interval;
    int region = d->region;

    d3dshot_l_reset_frame_buffer(self);

    while(self->priv->_is_capturing)
    {
        GTime cycle_start = g_get_real_time();
        void *frame = d3dshot_l_screenshot(self, d3dshot_l_validate_region(self, region));
        g_queue_push_head(self->priv->frame_buffer, frame);

        GTime cycle_end = g_get_real_time();
        GTimeSpan time_left = cycle_end - cycle_start;
        if(time_left > 0){
            g_usleep(time_left);
        }
    }
    self->priv->_is_capturing = FALSE;
}

gboolean d3dshot_l_screenshot_every(D3dshotL *self, gfloat interval, int *region)
{
    if(self->priv->_is_capturing){
        return FALSE;
    }

    interval = d3dshot_l_validate_interval(self, interval);

    self->priv->_is_capturing = TRUE;
    self->priv->_capture_thread = g_thread_new(NULL, NULL, NULL); //TODO
    g_thread_join(self->priv->_capture_thread);
    return TRUE;
}

gboolean d3dshot_l_stop(D3dshotL *self)
{
    /*
    if(self->priv->_is_capturing)
    {
        return FALSE;
    }
    */

    self->priv->_is_capturing = FALSE;

    if(self->priv->_capture_thread != NULL)
    {
        self->priv->_capture_thread_finished = TRUE;
        self->priv->_capture_thread = NULL;
    }

    return TRUE;
}

void d3dshot_l_benchmark(D3dshotL *self)
{
    int frame_count = 0;
    GTime start_time = g_get_real_time();
    GTime end_time = start_time + 60;

    while(g_get_real_time() < end_time)
    {
        int region[4];
        d3dshot_l_screenshot(self, region); //TODO
        frame_count += 1;
    }

}

D3dshotL *d3dshot_l_new(gint capture_output, gint frame_buffer_size, gboolean gstreamer_is_available)
{
    D3dshotL *d3dshot;
    d3dshot = D3DSHOTL(g_object_new(D3DSHOTL_TYPE, NULL)); 

    D3dshotLPrivate *priv = g_new(D3dshotLPrivate, 1);
    d3dshot->priv = priv;

    d3dshot_l_detect_desplays(d3dshot);
    if(g_list_length(d3dshot->priv->displays) > 0)
    {
        d3dshot->priv->display = (DisplayL *)g_list_nth_data(d3dshot->priv->displays, 0);
    }else{
        d3dshot->priv->display = NULL;
    }
    
    GList* iter;
    for (iter = d3dshot->priv->displays; iter != NULL; iter = iter->next) {
        DisplayL *d = (DisplayL *)iter->data;
        if(d->priv->is_primary){
            d3dshot->priv->display = d;
        }
    }

    d3dshot->priv->capture_output = capture_output_l_new(capture_output, d3dshot->priv->display);

    d3dshot->priv->frame_buffer_size = frame_buffer_size;
    d3dshot->priv->frame_buffer = NULL;

    d3dshot->priv->previous_screenshot = NULL;

    d3dshot->priv->region = NULL;

    d3dshot->priv->_gstreamer_is_available = gstreamer_is_available;

    d3dshot->priv->_capture_thread = NULL;
    d3dshot->priv->_is_capturing = FALSE;

    

    return d3dshot;
}


gboolean d3dshot_l_rebuild(D3dshotL *self)
{
    //gboolean stoped = d3dshot_l_stop(self);

    //g_object_unref(self);

    D3dshotL *d3dshot = self;
    //d3dshot = D3DSHOTL(g_object_new(D3DSHOTL_TYPE, NULL)); 

    //D3dshotLPrivate *priv = g_new(D3dshotLPrivate, 1);
    //d3dshot->priv = priv;

    g_object_unref(d3dshot->priv->display);
    g_list_free(d3dshot->priv->displays);

    
    d3dshot_l_detect_desplays(d3dshot);
    if(g_list_length(d3dshot->priv->displays) > 0)
    {
        d3dshot->priv->display = (DisplayL *)g_list_nth_data(d3dshot->priv->displays, 0);
    }else{
        d3dshot->priv->display = NULL;
    }
    
    GList* iter;
    for (iter = d3dshot->priv->displays; iter != NULL; iter = iter->next) {
        DisplayL *d = (DisplayL *)iter->data;
        if(d->priv->is_primary){
            d3dshot->priv->display = d;
        }
    }

    GstreamerOutputL *out = GSTREAMER_OUTPUTL(d3dshot->priv->capture_output);
    gst_element_set_state(out->priv->pipeline, GST_STATE_NULL);
    gst_object_unref(out->priv->pipeline);

    d3dshot->priv->capture_output = capture_output_l_new(GSTREAMER_BUFFER, d3dshot->priv->display);

    //d3dshot->priv->frame_buffer_size = 60;
    //d3dshot->priv->frame_buffer = NULL;

    //d3dshot->priv->previous_screenshot = NULL;

    //d3dshot->priv->region = NULL;

    //d3dshot->priv->_gstreamer_is_available = TRUE;

    //d3dshot->priv->_capture_thread = NULL;
    //d3dshot->priv->_is_capturing = FALSE;

    

    //self =  d3dshot;
}