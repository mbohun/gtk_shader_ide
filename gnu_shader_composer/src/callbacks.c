#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

//#include "gl_ext.h"

//#include <gdk/gdkglglext.h>

#include "callbacks.h"
#include "support.h"
#include "trackball.h"

#define DIG_2_RAD (G_PI / 180.0)
#define RAD_2_DIG (180.0 / G_PI)

#define ANIMATE_THRESHOLD 25.0

#define VIEW_SCALE_MAX 2.0
#define VIEW_SCALE_MIN 0.5


extern GtkWidget* main_window;
extern GtkWidget* gsc_quit_dialog;
extern GtkWidget* console_txt_view;


//extern GdkGL_GL_ARB_vertex_program* gdk_glext_vp;

static GtkWidget* gl_win;

extern GtkTextTag *tag;

static void print_error_to_console(const char* err_msg ) {

    GtkTextBuffer* console_txt_buffer;
    GtkTextIter start, end;
    GdkColor color;
    GtkTextMark* mark;

    console_txt_buffer =gtk_text_view_get_buffer(console_txt_view);

    /* start is the end, because we are going to append at the end */
    gtk_text_buffer_get_end_iter(console_txt_buffer, &start );

    /* save a mark for later */
    mark =gtk_text_buffer_create_mark(console_txt_buffer, NULL, &start, TRUE );

    /* insert the error msg */
    gtk_text_buffer_insert(console_txt_buffer, &start, err_msg, strlen(err_msg ) );

    /* use the mark to set the start iter */
    gtk_text_buffer_get_iter_at_mark(console_txt_buffer, &start, mark );

    /* set the end iter */
    gtk_text_buffer_get_end_iter(console_txt_buffer, &end );

    /* apply a tag */
    gtk_text_buffer_apply_tag(console_txt_buffer, tag, &start, &end );
    //gtk_text_buffer_apply_tag_by_name(console_txt_buffer, "martin", &start, &end );
    
    /* free the mark */
    /* !!! change thi to gtk_text_buffer_move_mark() later */
    gtk_text_buffer_delete_mark(console_txt_buffer, mark );

    gtk_text_view_scroll_to_iter(console_txt_view,
				 &end,
				 0.0,
				 FALSE,
				 0.0,
				 0.0 );

    on_drawing_area_expose_event(gl_win, NULL );
}

/* typedef void (*ProgramStringARB_t)(GLenum target, GLenum format, GLuint len, const GLbyte *string ); */
/* ProgramStringARB_t glProgramStringARB =NULL; */

/* static void compile_execute_vpfp_dummy(char* test_arb_vp, char* test_arb_fp ) {} */
/* static void (*init_shaders)(char*, char* ) =compile_execute_vpfp_dummy; */

static void compile_execute_vpfp(char* test_arb_vp, char* test_arb_fp )
{
    GLint error_pos;

    GLuint vp;
    GLuint fp;

/*   glProgramStringARB =NULL; */
/*   g_print("glProgramStringARB: %u\n", glProgramStringARB ); */


/*   glProgramStringARB =(ProgramStringARB_t)gdk_gl_get_proc_address("glProgramStringARB"); */
/*   g_print("glProgramStringARB: %u\n", glProgramStringARB ); */

    if( (NULL==test_arb_vp) || (NULL==test_arb_fp) ) {
	/* reject invalid vertex/fragment programs */
	return;
    }

    //Enable ARB vertex program.
    glEnable(GL_VERTEX_PROGRAM_ARB );

    //Generate a program.
    /* gdk_glext_vp-> */glGenProgramsARB(1, &vp );

    //Bind the program.
    /* gdk_glext_vp-> */glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vp );	

    //Load the program.
    /* gdk_glext_vp-> */glProgramStringARB(GL_VERTEX_PROGRAM_ARB, 
				     GL_PROGRAM_FORMAT_ASCII_ARB, 
				     strlen((const char*)test_arb_vp ), 
				     test_arb_vp );


    glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &error_pos );
    //g_print("GL_PROGRAM_ERROR_POSITION_ARB: %d\n", error_pos );

    if( -1 != error_pos ) {
	//g_print("Vertex Program ERROR: %s\n", glGetString(GL_PROGRAM_ERROR_STRING_ARB) );
	print_error_to_console(glGetString(GL_PROGRAM_ERROR_STRING_ARB) );
	glDisable(GL_VERTEX_PROGRAM_ARB );
	return;
    }

    //Enable ARB vertex program.
    glEnable(GL_FRAGMENT_PROGRAM_ARB );

    //Generate a program.
   /*  gdk_glext_vp-> */glGenProgramsARB(1, &fp );

    //Bind the program.
    /* gdk_glext_vp-> */glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fp );	

    //Load the program.
    /* gdk_glext_vp-> */glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, 
				     GL_PROGRAM_FORMAT_ASCII_ARB, 
				     strlen((const char*)test_arb_fp ), 
				     test_arb_fp );

    glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &error_pos );
    //g_print("GL_PROGRAM_ERROR_POSITION_ARB: %d\n", error_pos );

    if( -1 != error_pos ) {
	//g_print("Fragment Program - ERROR: %s\n", glGetString(GL_PROGRAM_ERROR_STRING_ARB) );
	print_error_to_console(glGetString(GL_PROGRAM_ERROR_STRING_ARB) );
	glDisable(GL_VERTEX_PROGRAM_ARB );
	glDisable(GL_FRAGMENT_PROGRAM_ARB );
	return;
    }

}



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

static void toggle_animation(GtkWidget *widget) 
{

}

static void init_view (void)
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


void on_main_menu_item_file_quit(GtkMenuItem* menuitem, gpointer user_data )
{
  gtk_widget_show(gsc_quit_dialog );
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

  GLfloat ambient[] = {0.0, 0.0, 0.2, 1.0};
  GLfloat diffuse[] = {0.2, 0.3, 0.6, 1.0};
  GLfloat position[] = {1.0, 3.0, 1.0, 0.0};

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

  gl_win =widget;

  //init_shaders();

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

void on_toolbutton_compile_execute_shader_clicked(GtkWidget* widget, gpointer data )
{
  GtkTextIter start, end;
  gchar* vp_txt;
  gchar* fp_txt;  

  g_printf("%u: on_toolbutton_compile_execute_shader_clicked()\n", counter++ );  

  struct shader_txt_buffers_t* shaders =(struct shader_txt_buffers_t*)data;

  gtk_text_buffer_get_start_iter(shaders->vp_buffer, &start );
  gtk_text_buffer_get_end_iter(shaders->vp_buffer, &end );
  vp_txt =gtk_text_buffer_get_text(shaders->vp_buffer, &start, &end, FALSE );
  g_printf("%u: Vertex Shader string:\n%s\n", counter++, vp_txt );

  gtk_text_buffer_get_start_iter(shaders->fp_buffer, &start );
  gtk_text_buffer_get_end_iter(shaders->fp_buffer, &end );
  fp_txt =gtk_text_buffer_get_text(shaders->fp_buffer, &start, &end, FALSE );    
  g_printf("%u: Fragment Shader string:\n%s\n", counter++, fp_txt );

  compile_execute_vpfp(vp_txt, fp_txt);
  
  //g_printf("what the hell ???\n");

  on_drawing_area_expose_event(gl_win, NULL ); //call the signal handler directly
  //g_signal_emit_by_name(gl_win, "expose_event" ); //emit signal
  
}

void on_toolbutton_remove_shaders_clicked(GtkWidget* widget, gpointer data )
{
    GtkTextBuffer* console_txt_buffer;
    GtkTextIter start, end;
    GdkColor color;
    GtkTextTag *tag;

    static const gchar* msg ="Shaders removed !\n";

    g_print("on_toolbutton_remove_shaders_clicked()\n");

    glDisable(GL_VERTEX_PROGRAM_ARB );
    glDisable(GL_FRAGMENT_PROGRAM_ARB );
    
    //print_error_to_console(msg);

    console_txt_buffer =gtk_text_view_get_buffer(console_txt_view);

    gtk_text_buffer_get_end_iter(console_txt_buffer, &end );


    //gdk_color_parse ("green", &color);
    //gtk_widget_modify_text (console_txt_view, GTK_STATE_NORMAL, &color);

    gtk_text_buffer_insert(console_txt_buffer, &end, msg, strlen(msg) );

    gtk_text_buffer_get_end_iter(console_txt_buffer, &end );
    gtk_text_view_scroll_to_iter(console_txt_view,
				 &end,
				 0.0,
				 FALSE,
				 0.0,
				 0.0 );

    //g_signal_emit_by_name(gl_win, "expose_event" ); //emit signal - SEG FAULT !
    on_drawing_area_expose_event(gl_win, NULL );

}
