#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
//#include "interface.h"
#include "support.h"

#include "trackball.h"

#define DIG_2_RAD (G_PI / 180.0)
#define RAD_2_DIG (180.0 / G_PI)

#define ANIMATE_THRESHOLD 25.0

#define VIEW_SCALE_MAX 2.0
#define VIEW_SCALE_MIN 0.5


extern GtkWidget* main_window;
extern GtkWidget* gsc_quit_dialog;


/* private util functions */

//debug msg counter
static unsigned int counter =0;

static void gsc_refresh_visible_areas() {

}

static int create_dialog_confirmation_exit() {

}

static float view_quat_diff[4] = { 0.0, 0.0, 0.0, 1.0 };
static float view_quat[4] = { 0.0, 0.0, 0.0, 1.0 };
static float view_scale = 1.0;

static gboolean animate = FALSE;

static void toggle_animation (GtkWidget *widget) {}

static void
init_view (void)
{
  view_quat[0] = view_quat_diff[0] = 0.0;
  view_quat[1] = view_quat_diff[1] = 0.0;
  view_quat[2] = view_quat_diff[2] = 0.0;
  view_quat[3] = view_quat_diff[3] = 1.0;
  view_scale = 1.0;
}
static float begin_x = 0.0;
static float begin_y = 0.0;

static float dx = 0.0;
static float dy = 0.0;

//ok_button_pressed()

void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_cut1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_copy1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_paste1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    gtk_widget_show(gsc_quit_dialog );
}


void
on_delete1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    
}


void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    
}


void on_main_window_delete_event (GtkMenuItem* menuitem, gpointer user_data )
{
/*     gtk_widget_set_parent_window(gsc_quit_dialog, //widget  */
/* 				 main_window );   //parent */

    gtk_widget_show(gsc_quit_dialog );

}

void on_drawing_area_realize (GtkWidget* widget, gpointer   user_data )
{

    g_print("%u. on_drawing_area_realize()\n", counter++ );  

  GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

  GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat position[] = {0.0, 3.0, 3.0, 0.0};

  GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat local_view[] = {0.0};

  /*** OpenGL BEGIN ***/
  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
    return;

  glClearColor (0.5, 0.5, 0.8, 1.0);
  glClearDepth (1.0);

  glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv (GL_LIGHT0, GL_POSITION, position);
  glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModelfv (GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

  glFrontFace (GL_CW);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glEnable (GL_AUTO_NORMAL);
  glEnable (GL_NORMALIZE);
  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LESS);

  gdk_gl_drawable_gl_end (gldrawable);
  /*** OpenGL END ***/

  return;

}


gboolean
on_drawing_area_configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer user_data )
{

    g_print("%u. on_drawing_area_configure_event()\n", counter++ );  

  GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

  GLfloat w = widget->allocation.width;
  GLfloat h = widget->allocation.height;
  GLfloat aspect;

  /*** OpenGL BEGIN ***/
  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext)) {
      g_print("%u. on_drawing_area_configure_event(), gdk_gl_drawable_gl_begin()==FALSE\n", counter++ );
      return FALSE;
  }

  glViewport (0, 0, w, h);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  if (w > h)
    {
      aspect = w / h;
      glFrustum (-aspect, aspect, -1.0, 1.0, 5.0, 60.0);
    }
  else
    {
      aspect = h / w;
      glFrustum (-1.0, 1.0, -aspect, aspect, 5.0, 60.0);
    }

  glMatrixMode (GL_MODELVIEW);

  gdk_gl_drawable_gl_end (gldrawable);
  /*** OpenGL END ***/

  return TRUE;

}


//gboolean on_drawing_area_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer user_data )
void on_drawing_area_expose_event(GtkWidget *widget, gpointer user_data )
{
/*     GdkGLContext *glcontext =gtk_widget_get_gl_context (widget); */
/*     GdkGLDrawable *gldrawable =gtk_widget_get_gl_drawable (widget); */

/*     g_printf("%u: on_drawing_area_expose_event()\n", counter++ ); */

/*     /*** OpenGL BEGIN ***/ 
/*     if(!gdk_gl_drawable_gl_begin(gldrawable, glcontext) ) */
/* 	return FALSE; */

/*     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); */

/*     glCallList(1); */
    

/*     if(gdk_gl_drawable_is_double_buffered(gldrawable ) ) */
/* 	gdk_gl_drawable_swap_buffers(gldrawable ); */
/*     else */
/* 	glFlush(); */

/*     gdk_gl_drawable_gl_end(gldrawable ); */
/*     /*** OpenGL END ***/ 

/*     return FALSE; */
    g_print("%u. on_drawing_area_expose_event\n", counter++ );  

  GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

  float m[4][4];

  /*** OpenGL BEGIN ***/
  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
      return;// FALSE;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity ();

  /* View transformation. */
  glTranslatef (0.0, 0.0, -10.0);
  glScalef (view_scale, view_scale, view_scale);
  add_quats (view_quat_diff, view_quat, view_quat);
  build_rotmatrix (m, view_quat);
  glMultMatrixf (&m[0][0]);

  /* Render shape */
/*   glMaterialfv (GL_FRONT, GL_AMBIENT, mat_current->ambient); */
/*   glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_current->diffuse); */
/*   glMaterialfv (GL_FRONT, GL_SPECULAR, mat_current->specular); */
/*   glMaterialf (GL_FRONT, GL_SHININESS, mat_current->shininess * 128.0); */
/*   glCallList (shape_list_base + shape_current); */

  //glCallList(1);
  gdk_gl_draw_teapot(TRUE, 1.0);

  /* Swap buffers */
  if (gdk_gl_drawable_is_double_buffered (gldrawable))
    gdk_gl_drawable_swap_buffers (gldrawable);
  else
    glFlush ();

  gdk_gl_drawable_gl_end (gldrawable);
  /*** OpenGL END ***/

  //return TRUE;


}

//exit/quit confirmation dialog button handlers
gboolean exit_dlg_button_quit_clicked(GtkWidget *widget, GdkEventExpose *event, gpointer user_data )
{
    g_printf("%u: exit_dlg_button_quit_clicked()\n", counter++ );
    gtk_main_quit();
    return FALSE;
}

/* !!!This prototype does not work with glade_xml_signal_connect_data() */
//gboolean exit_dlg_button_no_clicked(GtkWidget *widget, GdkEventExpose *event, gpointer user_data )

void exit_dlg_button_no_clicked(GtkWidget *widget, gpointer user_data )
{
    //void* crap =*user_data;
    //guint* vp =(guint*)user_data;
    //guint val = *vp;
    //g_print("%u: exit_dlg_button_no_clicked()\n", counter++ );
    //g_print("Secret: %u, VALUE: %s\n", user_data, (gchar*)user_data );
    g_print("Secret agent: %s\n",(gchar*)user_data );
    //g_print("val=%u\n", val );
    gtk_widget_hide(gsc_quit_dialog );
    return FALSE;
}



gboolean button_press_event(GtkWidget* widget, GdkEventButton* event, gpointer data)
{

  g_printf("%u: button_press_event()\n", counter++ );

  if (animate)
    {
      if (event->button == 1)
        toggle_animation (widget);
    }
  else
    {
      view_quat_diff[0] = 0.0;
      view_quat_diff[1] = 0.0;
      view_quat_diff[2] = 0.0;
      view_quat_diff[3] = 1.0;
    }

  begin_x = event->x;
  begin_y = event->y;

  return FALSE;
}

gboolean button_release_event(GtkWidget* widget, GdkEventButton* event, gpointer data )
{
  g_printf("%u: button_release_event()\n", counter++ );

  if (!animate)
    {
      if (event->button == 1 &&
          ((dx*dx + dy*dy) > ANIMATE_THRESHOLD))
        toggle_animation (widget);
    }

  dx = 0.0;
  dy = 0.0;

  return FALSE;
}

gboolean motion_notify_event(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
  g_printf("%u: motion_notify_event()\n", counter++ );

  float w = widget->allocation.width;
  float h = widget->allocation.height;
  float x = event->x;
  float y = event->y;
  gboolean redraw = FALSE;

  /* Rotation. */
  if (event->state & GDK_BUTTON1_MASK)
    {
      trackball (view_quat_diff,
		 (2.0 * begin_x - w) / w,
		 (h - 2.0 * begin_y) / h,
		 (2.0 * x - w) / w,
		 (h - 2.0 * y) / h);

      dx = x - begin_x;
      dy = y - begin_y;

      redraw = TRUE;
    }

  /* Scaling. */
  if (event->state & GDK_BUTTON2_MASK)
    {
      view_scale = view_scale * (1.0 + (y - begin_y) / h);
      if (view_scale > VIEW_SCALE_MAX)
	view_scale = VIEW_SCALE_MAX;
      else if (view_scale < VIEW_SCALE_MIN)
	view_scale = VIEW_SCALE_MIN;

      redraw = TRUE;
    }

  begin_x = x;
  begin_y = y;

  if (redraw && !animate)
    gdk_window_invalidate_rect (widget->window, &widget->allocation, FALSE);

  return TRUE;
}

/* fp textview */
gboolean on_fp_textview_delete_from_cursor(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_fp_textview_delete_from_cursor()\n", counter++ );
    return FALSE;
}

gboolean on_fp_textview_insert_at_cursor(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_fp_textview_insert_at_cursor()\n", counter++ );
    return FALSE;
}

gboolean on_fp_textview_move_cursor(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_fp_textview_move_cursor()\n", counter++ );
    return FALSE;
}

gboolean on_fp_textview_move_focus(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_fp_textview_move_focus()\n", counter++ );
    return FALSE;
}

gboolean on_fp_textview_move_viewport(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_fp_textview_move_viewport()\n", counter++ );
    return FALSE;
}

gboolean on_fp_textview_select_all(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_fp_textview_select_all()\n", counter++ );
    return FALSE;
}

gboolean on_fp_textview_toggle_overwrite(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_fp_textview_toggle_overwrite()\n", counter++ );
    return FALSE;
}

gboolean on_fp_textview_key_press_event(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_fp_textview_key_press_event()\n", counter++ );
    return FALSE;
}

gboolean on_fp_textview_key_release_event(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_fp_textview_key_release_event()\n", counter++ );
    return FALSE;
}

gboolean on_fp_textview_state_changed(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_fp_textview_state_changed()\n", counter++ );
    return FALSE;
}

gboolean on_fp_textview_selection_received(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_fp_textview_selection_received()\n", counter++ );
    return FALSE;
}

/* vp textview */
gboolean on_vp_textview_delete_from_cursor(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_vp_textview_delete_from_cursor()\n", counter++ );
    return FALSE;
}

gboolean on_vp_textview_insert_at_cursor(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_vp_textview_insert_at_cursor()\n", counter++ );
    return FALSE;
}

gboolean on_vp_textview_move_cursor(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_vp_textview_move_cursor()\n", counter++ );
    return FALSE;
}

gboolean on_vp_textview_move_focus(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_vp_textview_move_focus()\n", counter++ );
    return FALSE;
}

gboolean on_vp_textview_move_viewport(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_vp_textview_move_viewport()\n", counter++ );
    return FALSE;
}

gboolean on_vp_textview_select_all(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_vp_textview_select_all()\n", counter++ );
    return FALSE;
}

gboolean on_vp_textview_toggle_overwrite(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_vp_textview_toggle_overwrite()\n", counter++ );
    return FALSE;
}

gboolean on_vp_textview_key_press_event(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_vp_textview_key_press_event()\n", counter++ );
    return FALSE;
}

gboolean on_vp_textview_key_release_event(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    //GtkTextIter iter;
    GtkTextIter start, end;
    GtkTextBuffer *buffer;
    gchar* txt;
    //int i;
  
    g_printf("%u: on_vp_textview_key_release_event()\n", counter++ );

    buffer = gtk_text_view_get_buffer(widget );

    gtk_text_buffer_get_start_iter(buffer, &start );
    gtk_text_buffer_get_end_iter(buffer, &end );
    txt =gtk_text_buffer_get_text(buffer, &start, &end, FALSE );
    
    g_printf("%u: on_vp_textview_key_release_event() text: %s \n", counter++, txt );

    return FALSE;
}

gboolean on_vp_textview_state_changed(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_vp_textview_state_changed()\n", counter++ );
    return FALSE;
}

gboolean on_vp_textview_selection_received(GtkWidget* widget, GdkEventMotion* event, gpointer data )
{
    g_printf("%u: on_vp_textview_selection_received()\n", counter++ );
    return FALSE;
}

void on_hpaned1_check_resize(GtkWidget* widget, gpointer data )
{
    g_printf("%u: on_hpaned1_check_resize()\n", counter++ );
}

gboolean on_hpaned1_accept_position(GtkWidget* widget, gpointer data )
{
    g_printf("%u: on_hpaned1_accept_position()\n", counter++ );
    return FALSE;
}

void on_hpaned1_state_changed(GtkWidget* widget, gpointer data )
{
    g_printf("%u: on_hpaned1_state_changed()\n", counter++ );
}

void on_hpaned1_move_handle(GtkWidget* widget, gpointer data )
{
    g_printf("%u: on_hpaned1_move_handle()\n", counter++ );
}
