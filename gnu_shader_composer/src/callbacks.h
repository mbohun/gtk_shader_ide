#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <GL/gl.h>
#include <GL/glu.h>

struct shader_txt_buffers_t {
  GtkTextBuffer *vp_buffer;
  GtkTextBuffer *fp_buffer;
};


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

void on_main_menu_item_file_quit(GtkMenuItem* menuitem, gpointer user_data );

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
//void on_drawing_area_configure_event(GtkWidget* widget, gpointer user_data );

void on_drawing_area_expose_event(GtkWidget* widget, gpointer user_data );

gboolean exit_dlg_button_quit_clicked(GtkWidget *widget, GdkEventExpose *event, gpointer user_data );

//gboolean exit_dlg_button_no_clicked(GtkWidget *widget, GdkEventExpose *event, gpointer user_data );
void exit_dlg_button_no_clicked(GtkWidget *widget, gpointer user_data );

gboolean button_press_event(GtkWidget* widget, GdkEventButton* event, gpointer data );

gboolean button_release_event(GtkWidget* widget, GdkEventButton* event, gpointer data );

gboolean motion_notify_event(GtkWidget* widget, GdkEventMotion* event, gpointer data );

/* hpaned1 */
void on_hpaned1_check_resize(GtkWidget* widget, gpointer data );
gboolean on_hpaned1_accept_position(GtkWidget* widget, gpointer data );
void on_hpaned1_state_changed(GtkWidget* widget, gpointer data );
void on_hpaned1_move_handle(GtkWidget* widget, gpointer data );

/* toolbar, toolbutton compile and execute shareds */
void on_toolbutton_compile_execute_shader_clicked(GtkWidget* widget, gpointer data );

/* toolbar, toolbutton remove shareds */
void on_toolbutton_remove_shaders_clicked(GtkWidget* widget, gpointer data );

/* vp text buffer handler */
void vp_txt_buffer_changed_handler(GtkTextBuffer *textbuffer, gpointer user_dat );

/* fp text buffer handler */
void fp_txt_buffer_changed_handler(GtkTextBuffer *textbuffer, gpointer user_dat );
