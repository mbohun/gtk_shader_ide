#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <GL/gl.h>
#include <GL/glu.h>

void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_cut1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_copy1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_paste1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_delete1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void on_about1_activate(GtkMenuItem* menuitem, gpointer user_data );

void on_main_window_delete_event(GtkMenuItem* menuitem, gpointer user_data );

void on_drawing_area_realize(GtkWidget* widget, gpointer user_data );

gboolean on_drawing_area_configure_event(GtkWidget* widget, GdkEventConfigure* event, gpointer user_data );

gboolean on_drawing_area_expose_event(GtkWidget* widget, GdkEventExpose* event, gpointer user_data );

gboolean exit_dlg_button_quit_clicked(GtkWidget *widget, GdkEventExpose *event, gpointer user_data );

gboolean exit_dlg_button_no_clicked(GtkWidget *widget, GdkEventExpose *event, gpointer user_data );

gboolean button_press_event (GtkWidget* widget, GdkEventButton* event, gpointer data );

gboolean button_release_event (GtkWidget* widget, GdkEventButton* event, gpointer data );

gboolean motion_notify_event (GtkWidget* widget, GdkEventMotion* event, gpointer data );
