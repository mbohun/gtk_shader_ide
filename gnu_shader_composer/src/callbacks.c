#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>

#include <gtk/gtk.h>

//#include "gl_ext.h"

/* #include <gdk/gdkglglext.h> */

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

extern GtkTextView* vp_txt_view;
extern GtkTextView* fp_txt_view;

#ifndef GL_FRAGMENT_PROGRAM_ARB /* crappy condition */
extern GdkGL_GL_ARB_vertex_program* gdk_glext_vp;
static void compile_execute_vpfp(char* test_arb_vp, char* test_arb_fp )
{
    GLint error_pos;

    GLuint vp;
    GLuint fp;

    if( (NULL==test_arb_vp) || (NULL==test_arb_fp) ) {
	/* reject invalid vertex/fragment programs */
	return;
    }

    //Enable ARB vertex program.
    glEnable(GL_VERTEX_PROGRAM_ARB );

    //Generate a program.
    gdk_glext_vp->glGenProgramsARB(1, &vp );

    //Bind the program.
    gdk_glext_vp->glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vp );	

    //Load the program.
    gdk_glext_vp->glProgramStringARB(GL_VERTEX_PROGRAM_ARB, 
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
    gdk_glext_vp->glGenProgramsARB(1, &fp );

    //Bind the program.
    gdk_glext_vp->glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fp );	

    //Load the program.
    gdk_glext_vp->glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, 
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
#else

static void compile_execute_vpfp(char* test_arb_vp, char* test_arb_fp )
{
    GLint error_pos;

    GLuint vp;
    GLuint fp;

    if( (NULL==test_arb_vp) || (NULL==test_arb_fp) ) {
	/* reject invalid vertex/fragment programs */
	return;
    }

    //Enable ARB vertex program.
    glEnable(GL_VERTEX_PROGRAM_ARB );

    //Generate a program.
    glGenProgramsARB(1, &vp );

    //Bind the program.
    glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vp );	

    //Load the program.
    glProgramStringARB(GL_VERTEX_PROGRAM_ARB, 
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
    glGenProgramsARB(1, &fp );

    //Bind the program.
    glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fp );	

    //Load the program.
    glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, 
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
#endif


//debug msg counter
static unsigned int counter =0;

static GtkWidget* gl_win;

//extern GtkTextTag *tag;


/* this is only a test, the real stuff will read this type of info from an external XML file */
/* KEYWORDS_ARB_VP10_HEADER is not used yet, i have to come with something a bit more elegant */
static gchar* KEYWORDS_ARB_VP10_HEADER[] = 
{
    "!!ARBvp1.0", 
    NULL 
};

static gchar* KEYWORDS_ARB_VP10_DEFS[] =
{
  "ADDRESS", "ALIAS", "ATTRIB",
  "OPTION", "OUTPUT", 
  "PARAM",
  "TEMP",
  NULL
};

static gchar* KEYWORDS_ARB_VP10_INSTRUCTIONS[] =
{

    "ABS", "ADD", "ARL",
    "DP3", "DP4", "DPH", "DST",
    "EX2", "EXP", 
    "FLR", "FRC", 
    "LG2", "LIT", "LOG", 
    "MAD", "MAX", "MIN", "MOV", "MUL", 
    "POW", 
    "RCP", "RSQ", 
    "SGE", "SLT", "SUB", "SWZ", 
    "XPD",
    NULL
};

static gchar* KEYWORDS_ARB_VP10_BUILTINS[] =
{
    "program", 
    "result",
    "state", 
    "vertex",
    NULL
};

static gchar* KEYWORDS_ARB_VP10_SEPARATORS[] =
{
    "#", " ", "\t", ".", ",", ";", "\r", "\n", "", 
    NULL
};


static gchar* KEYWORDS_ARB_FP10_DEFS[] =
{
    "ALIAS", "ATTRIB",
    "OPTION", "OUTPUT",
    "PARAM",
    "TEMP",
    NULL
    
};

static gchar* KEYWORDS_ARB_FP10_INSTRUCTIONS[] =
{
    "ABS", "ABS_SAT", "ADD", "ADD_SAT",
    "CMP", "CMP_SAT", "COS", "COS_SAT",
    "DP3", "DP3_SAT", "DP4", "DP4_SAT", "DPH", "DPH_SAT", "DST", "DST_SAT", 
    "EX2", "EX2_SAT", 
    "FLR", "FLR_SAT", "FRC", "FRC_SAT", 
    "KIL", 
    "LG2", "LG2_SAT", "LIT", "LIT_SAT", "LRP", "LRP_SAT",
    "MAD", "MAD_SAT", "MAX", "MAX_SAT", "MIN", "MIN_SAT", "MOV", "MOV_SAT", "MUL", "MUL_SAT", 
    "POW", "POW_SAT", 
    "RCP", "RCP_SAT", "RSQ", "RSQ_SAT", 
    "SIN", "SIN_SAT", "SCS", "SCS_SAT", "SGE", "SGE_SAT", "SLT", "SLT_SAT", "SUB", "SUB_SAT", "SWZ", "SWZ_SAT", 
    "TEX", "TEX_SAT", "TXB", "TXB_SAT", "TXP", "TXP_SAT", 
    "XPD", "XPD_SAT",
    NULL
};

static gchar* KEYWORDS_ARB_FP10_BUILTINS[] =
{
    "fragment", 
    "program", 
    "result", 
    "state", 
    "texture", "texture[",
    NULL
};

static gchar* KEYWORDS_ARB_FP10_SEPARATORS[] =
{
    "#", " ", "\t", ".", ",", ";", "\r", "\n", "", 
    NULL
};

gboolean is_separator(const gchar* c )
{
  

  if( (0 == g_ascii_strncasecmp(c, " ", 1 ))
      ||(0 == g_ascii_strncasecmp(c, "", 1 ))
      ||(0 == g_ascii_strncasecmp(c, ".", 1 ))
      ||(0 == g_ascii_strncasecmp(c, ",", 1 ))
      ||(0 == g_ascii_strncasecmp(c, ";", 1 ))
      ||(0 == g_ascii_strncasecmp(c, "\t", 1 ))
      ||(0 == g_ascii_strncasecmp(c, "\n", 1 ))
      ||(0 == g_ascii_strncasecmp(c, "\r", 1 ))
      ||(0 == g_ascii_strncasecmp(c, "#", 1 )) )

    {
      //g_print("is_separator(): %s\n", c );
      return TRUE;
    }
  
  return FALSE;
}

static gboolean handle(GtkTextBuffer* buf, GtkTextIter* start, GtkTextIter* end, const char** keywords, const char* tag_name )
{
    gchar* txt =gtk_text_buffer_get_text(buf, start, end, TRUE );
    guint i =0;

    while(keywords[i]!=NULL ) {
	if(0 == strcmp(keywords[i], txt ) ) {
	    gtk_text_buffer_apply_tag_by_name(buf, tag_name, start, end );
	    return TRUE;
	}
	
	i++;
    }
    
    return FALSE;
}

static unsigned int header_handler_flag =-1;

static void handle_text(GtkTextBuffer* buf, guint so, guint eo ) 
{
    GtkTextIter start;
    GtkTextIter end;
    gchar* txt;


    gtk_text_buffer_get_iter_at_offset(buf, &start, so );
    gtk_text_buffer_get_iter_at_offset(buf, &end, eo );

    /* cleanup/fix later - remove all posible tags */
    gtk_text_buffer_remove_tag_by_name(buf, "keyword-vp-header", &start, &end );
    gtk_text_buffer_remove_tag_by_name(buf, "keyword-vp-defs", &start, &end );
    gtk_text_buffer_remove_tag_by_name(buf, "keyword-vp-instructions", &start, &end );
    gtk_text_buffer_remove_tag_by_name(buf, "keyword-vp-builtins", &start, &end );

    gtk_text_buffer_remove_tag_by_name(buf, "comment", &start, &end );

    /* get the actual text , and check if it is a keyword - this could be a optimized later */
    //txt =gtk_text_buffer_get_text(buf, &start, &end, TRUE );
    if(0==header_handler_flag) {
	if(handle(buf, &start, &end, KEYWORDS_ARB_VP10_DEFS, "keyword-vp-defs" ) 
	   || handle(buf, &start, &end, KEYWORDS_ARB_VP10_INSTRUCTIONS, "keyword-vp-instructions" ) 
	   || handle(buf, &start, &end, KEYWORDS_ARB_VP10_BUILTINS, "keyword-vp-builtins" ) 
	   
	    ) {
	
	    return;
	}

    }
    else if(1==header_handler_flag) {
	if(handle(buf, &start, &end, KEYWORDS_ARB_FP10_DEFS, "keyword-vp-defs" ) 
	   || handle(buf, &start, &end, KEYWORDS_ARB_FP10_INSTRUCTIONS, "keyword-vp-instructions" ) 
	   || handle(buf, &start, &end, KEYWORDS_ARB_FP10_BUILTINS, "keyword-vp-builtins" ) 
	   
	    ) {
	
	    return;
	}
    }
    else {
	//the program has no header
    }


}

static guint handle_separator(GtkTextBuffer* buf, guint so, guint eo )
{
    GtkTextIter start;
    GtkTextIter end;
    gchar* txt;

    guint offset =0;

    gtk_text_buffer_get_iter_at_offset(buf, &start, so );
    gtk_text_buffer_get_iter_at_offset(buf, &end, eo );
  
    txt =gtk_text_buffer_get_text(buf, &start, &end, TRUE );

    gtk_text_buffer_remove_tag_by_name(buf, "comment", &start, &end );

    //is it a comment ?
    if((0 == g_ascii_strncasecmp(txt, "#", 1 ) ) ) {

	/* BUG ? */
	/* gtk_text_iter_forward_to_line_end(&end); */

	/* I HAD to do it this way ! */
	while(!gtk_text_iter_ends_line(&end) ) {
	    gtk_text_iter_forward_char(&end);
	}
	
	/* g_print("comment: %s\n", gtk_text_buffer_get_text(buf, &start, &end, TRUE ) ); */


	gtk_text_buffer_apply_tag_by_name(buf, "comment", &start, &end );

	offset =  gtk_text_iter_get_offset(&end ) - so;
    }

    return offset;

}


static void handle_header(GtkTextBuffer* buf ) 
{
    GtkTextIter start;
    GtkTextIter end;
    gchar* txt;

    gtk_text_buffer_get_iter_at_offset(buf, &start, 0 );
    gtk_text_buffer_get_iter_at_offset(buf, &end, 10 );

    gtk_text_buffer_remove_tag_by_name(buf, "keyword-vp-header", &start, &end );

    txt =gtk_text_buffer_get_text(buf, &start, &end, TRUE );

    if(0==strcmp(txt, "!!ARBvp1.0") ) {
	gtk_text_buffer_apply_tag_by_name(buf, "keyword-vp-header", &start, &end );
	header_handler_flag =0;
    }

    if(0==strcmp(txt, "!!ARBfp1.0") ) {
	gtk_text_buffer_apply_tag_by_name(buf, "keyword-vp-header", &start, &end );
	header_handler_flag =1;
    }

}

static void handle_end(GtkTextBuffer* buf ) 
{
    GtkTextIter start;
    GtkTextIter end;
    gchar* txt;
    gint e0 =0;

    gtk_text_buffer_get_end_iter(buf, &end );
    start =end;

    //eo =gtk_text_iter_get_offset(&end);
    if(!gtk_text_iter_backward_chars(&start, 3 ))
	return;

    //gtk_text_buffer_remove_tag_by_name(buf, "keyword-vp-header", &start, &end );

    txt =gtk_text_buffer_get_text(buf, &start, &end, TRUE );

    //g_print("end tag: %s\n", txt );

    if(0==strcmp(txt, "END") ) {
	gtk_text_buffer_apply_tag_by_name(buf, "keyword-vp-header", &start, &end );
    }

}

static void syntax_highlight_buffer(GtkTextBuffer* buf)
{

  GString* tmp =g_string_new("");
  //GtkTextBuffer* buf =gtk_text_view_get_buffer(vp_txt_view );

  const gint max =gtk_text_buffer_get_char_count(buf) + 1; /* +1 means to position */

  gint i =1;
  gint j =0;

  while(i < max ) {

      GtkTextIter start;
      GtkTextIter end;

      gchar* txt;

      gtk_text_buffer_get_iter_at_offset(buf, &start, i-1 );
      gtk_text_buffer_get_iter_at_offset(buf, &end, i );

      txt =gtk_text_buffer_get_text(buf, &start, &end, TRUE );

      handle_text(buf, j, i-1 );

      if(is_separator(txt ) ) {

	  guint e =0;

	  /* do we have something in the before BEFORE the separator ? */
	  if(0 < tmp->str ) {
	      //handle_text(buf, j, i-1 );
	      tmp =g_string_erase(tmp, 0, -1 );
	  }

	  e =handle_separator(buf, i-1, i );
	  
	  j =i+e;
	  i =j+1;
	  

      }
      else {
	  tmp =g_string_append(tmp, txt );
	  i++;
      }

      /* handle specials, later...  */
      handle_header(buf);
      handle_end(buf);
  }

  

}


static void print_error_to_console(const char* err_msg ) {

    GtkTextBuffer* console_txt_buffer;
    GtkTextIter start, end;
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
    gtk_text_buffer_apply_tag_by_name(console_txt_buffer, "italic", &start, &end );
    
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




/* private util functions */

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
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  gdk_gl_draw_teapot(TRUE, 1.0);

  glDisable( GL_BLEND );


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

  GtkTextBuffer* vpb;

  g_printf("%u: on_toolbutton_compile_execute_shader_clicked()\n", counter++ );  

  struct shader_txt_buffers_t* shaders =(struct shader_txt_buffers_t*)data;

  vpb =shaders->vp_buffer;

  syntax_highlight_buffer(vpb);

  gtk_text_buffer_get_start_iter(vpb, &start );

  gtk_text_buffer_get_end_iter(vpb, &end );
  vp_txt =gtk_text_buffer_get_text(vpb, &start, &end, FALSE );
  g_printf("%u: Vertex Shader string:\n%s\n", counter++, vp_txt );

  gtk_text_buffer_get_start_iter(shaders->fp_buffer, &start );
  gtk_text_buffer_get_end_iter(shaders->fp_buffer, &end );
  fp_txt =gtk_text_buffer_get_text(shaders->fp_buffer, &start, &end, FALSE );    
  g_printf("%u: Fragment Shader string:\n%s\n", counter++, fp_txt );

  compile_execute_vpfp(vp_txt, fp_txt);

  //g_signal_emit_by_name(gl_win, "expose_event" ); //emit signal - SEG FAULT !  
  on_drawing_area_expose_event(gl_win, NULL ); //call the signal handler directly

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
    
    console_txt_buffer =gtk_text_view_get_buffer(console_txt_view);

    gtk_text_buffer_get_end_iter(console_txt_buffer, &end );

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


void test_vp_txt_view_handler(GtkTextView *textview, gchar *arg1, gpointer user_data )
{
    g_print("!!! test_vp_txt_view_handler()\n");
}

void test_vp_txt_view_handler_two(GtkTextView *textview, gpointer user_data )
{
    gchar* name =(gchar*)user_data;

    if(NULL==name)
	name="NULL";


    g_print("!!! test_vp_txt_view_handler_two(): %s\n", name );
}

void 
vp_txt_buffer_insert_text_handler(GtkTextBuffer* vp_txt_buffer,
				  GtkTextIter *arg1,
				  gchar *arg2,
				  gint arg3,
				  gpointer user_data ) {

    GdkWindow* gdk_win;
    GdkRegion* gdk_reg;

    //g_print("vp_txt_buffer_insert_text_handler(), ", arg2 );

    //syntax_highlight_buffer_dummy(vp_txt_buffer);

    gdk_win =gtk_widget_get_root_window(vp_txt_view);
    gdk_reg =gdk_window_get_update_area(gdk_win);

    gdk_window_invalidate_region(gdk_win, gdk_reg, TRUE );
    gdk_window_process_updates(gdk_win, TRUE );

}

void        
vp_txt_buffer_delete_range_handler(GtkTextBuffer* vp_txt_buffer,
				   GtkTextIter* start_iter,
				   GtkTextIter* end_iter,
				   gpointer user_data ) {

    GdkWindow* gdk_win;
    GdkRegion* gdk_reg;

    gchar* del_txt =gtk_text_buffer_get_text(vp_txt_buffer, start_iter, end_iter, TRUE );

    //g_print("vp_txt_buffer_delete_range_handler(), ", del_txt );
    
    //syntax_highlight_buffer_dummy(vp_txt_buffer);

    gdk_win =gtk_widget_get_root_window(vp_txt_view);
    gdk_reg =gdk_window_get_update_area(gdk_win);

    gdk_window_invalidate_region(gdk_win, gdk_reg, TRUE );
    gdk_window_process_updates(gdk_win, TRUE );

}

void vp_txt_buffer_changed_handler(GtkTextBuffer *textbuffer, gpointer user_dat )
{ 
    header_handler_flag =-1;
    syntax_highlight_buffer(textbuffer);
}

void fp_txt_buffer_changed_handler(GtkTextBuffer *textbuffer, gpointer user_dat )
{
    header_handler_flag =-1;
    syntax_highlight_buffer(textbuffer);
}




void vp_txt_buffer_modified_changed_handler(GtkTextBuffer *textbuffer, gpointer user_data ) 
{
    //g_print("vp_txt_buffer_modified_changed_handler(), " );
    //syntax_highlight_buffer_dummy(textbuffer );
}
