/*
 * Author: XueLong Ba
 * Date: 2023-09-05
 * Description: 
 */

#pragma once

#include "glib.h"
#include "glib-object.h"
#include "screen-duplication-types.h"
#include "capture-output-priv.h"
#include "utils/dxgi_utils.h"

#define DISPLAYL_TYPE (display_l_get_type())
#define DISPLAYL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), DISPLAYL_TYPE, DisplayL))
#define DISPLAYL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), DISPLAYL_TYPE, DisplayLClass))
#define IS_DISPLAYL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DISPLAYL_TYPE))
#define IS_DISPLAYL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), DISPLAYL_TYPE))
#define DISPLAYL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), DISPLAYL_TYPE, DisplayLClass))

struct _DisplayL {
    GObject parent;
    DisplayLPrivate *priv;
    /* Do not add fields to this struct */
};


struct _DisplayLClass
{
    GObjectClass parent_class;
    DisplayLClassPrivate *priv;
    /* Do not add fields to this struct */
};

GType display_l_get_type(void);



DisplayL *display_l_new(gchar *name, gchar *adapter_name, 
    gint resolutionX, gint resolutionY, gint positionLeft,
    gint positionTop, gint positionRight, gint positionButtom,
    gint rotation, gfloat scale_factor, gboolean is_primary,
    HMONITOR hmonitor, IDXGIOutput1 *dxgi_output, IDXGIAdapter1 *dxgi_adapter);

IDXGIOutputDuplication *display_l_initialize_dxgi_output_duplication();

GList *display_l_discover_displays();

int *display_l_get_clean_region(DisplayL *self, int *region);

void *display_l_capture(DisplayL *self, CaptureOutputL *captureOutput, GetFrameL process_func, int *region, D3dshotL *d3dshot);
