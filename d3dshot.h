/*
 * Author: XueLong Ba
 * Date: 2023-09-06
 * Description: 
 */

#pragma once

#include "glib.h"
#include "glib-object.h"
#include "screen-duplication-types.h"

#define D3DSHOTL_TYPE (d3dshot_l_get_type())
#define D3DSHOTL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), D3DSHOTL_TYPE, D3dshotL))
#define D3DSHOTL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), D3DSHOTL_TYPE, D3dshotLClass))
#define IS_D3DSHOTL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), D3DSHOTL_TYPE))
#define IS_D3DSHOTL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), D3DSHOTL_TYPE))
#define D3DSHOTL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), D3DSHOTL_TYPE, D3dshotLClass))

struct _D3dshotL {
    GObject parent;
    D3dshotLPrivate *priv;
    /* Do not add fields to this struct */
};


struct _D3dshotLClass
{
    GObjectClass parent_class;
    D3dshotLClassPrivate *priv;
    /* Do not add fields to this struct */
};

typedef struct {
    D3dshotL *self;
    gint target_fps;
    int *region;
} CaptureThreadData;

typedef struct {
    D3dshotL *self;
    gfloat interval;
    int *region;
} ScreenshotEveryData;

GType d3dshot_l_get_type(void);

gboolean d3dshot_l_is_capturing(D3dshotL *self);

void* d3dshot_l_get_frame(D3dshotL *self, gint frame_index);
void* d3dshot_l_get_latest_frame(D3dshotL *self);
GList *d3dshot_l_get_frames(D3dshotL *self, GArray *frame_indices);
int *d3dshot_l_validate_region(D3dshotL *self, int *region);
void *d3dshot_l_screenshot(D3dshotL *self, int *region);
D3dshotL *d3dshot_l_new(gint capture_output, gint frame_buffer_size, gboolean gstreamer_is_available);
gboolean d3dshot_l_capture(D3dshotL *self, gint target_fps, int *region, void* streamChannel);
gboolean d3dshot_l_rebuild(D3dshotL *self);
gboolean d3dshot_l_stop(D3dshotL *self);
