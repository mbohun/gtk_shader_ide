#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
//#include "interface.h"
#include "support.h"


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

void
on_drawing_area_realize (GtkWidget *widget,
                         gpointer   user_data)
{
    GdkGLContext *glcontext =gtk_widget_get_gl_context(widget );
    GdkGLDrawable *gldrawable =gtk_widget_get_gl_drawable(widget );

    GLUquadricObj *qobj;

    static GLfloat light_diffuse[] = {0.6f, 0.6f, 0.6f, 1.0};
    static GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

    /*** OpenGL BEGIN ***/
    if (!gdk_gl_drawable_gl_begin(gldrawable, glcontext) )
	return;

/*     qobj = gluNewQuadric(); */
/*     gluQuadricDrawStyle(qobj, GLU_FILL); */
/*     glNewList(1, GL_COMPILE); */
/*     gluSphere(qobj, 1.0, 20, 20 ); */
/*     glEndList(); */

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse );
    glLightfv(GL_LIGHT0, GL_POSITION, light_position );

    glEnable(GL_LIGHTING );
    glEnable(GL_LIGHT0 );
    glEnable(GL_DEPTH_TEST );

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f );
    glClearDepth(1.0 );

    glViewport(0, 0, widget->allocation.width, widget->allocation.height );

    glMatrixMode(GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(40.0, 1.0, 1.0, 10.0 );

    glMatrixMode(GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 3.0,
	       0.0, 0.0, 0.0,
	       0.0, 1.0, 0.0);
    glTranslatef (0.0, 0.0, -3.0 );

    gdk_gl_drawable_gl_end(gldrawable );
    /*** OpenGL END ***/
}


gboolean
on_drawing_area_configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer user_data )
{

    GdkGLContext *glcontext =gtk_widget_get_gl_context(widget );
    GdkGLDrawable *gldrawable =gtk_widget_get_gl_drawable(widget );

    /*** OpenGL BEGIN ***/
    if (!gdk_gl_drawable_gl_begin(gldrawable, glcontext ) )
	return FALSE;
    
    glViewport (0, 0, widget->allocation.width, widget->allocation.height );
    
    gdk_gl_drawable_gl_end (gldrawable);
    /*** OpenGL END ***/
    
    return FALSE;
}


gboolean
on_drawing_area_expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer user_data )
{
    GdkGLContext *glcontext =gtk_widget_get_gl_context (widget);
    GdkGLDrawable *gldrawable =gtk_widget_get_gl_drawable (widget);

    /*** OpenGL BEGIN ***/
    if(!gdk_gl_drawable_gl_begin(gldrawable, glcontext) )
	return FALSE;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //glCallList(1);
    gdk_gl_draw_teapot(TRUE, 1.0 );

    if(gdk_gl_drawable_is_double_buffered(gldrawable ) )
	gdk_gl_drawable_swap_buffers(gldrawable );
    else
	glFlush();

    gdk_gl_drawable_gl_end(gldrawable );
    /*** OpenGL END ***/

    return FALSE;
}
