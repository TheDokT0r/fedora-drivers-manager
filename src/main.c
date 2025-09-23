#include <gtk/gtk.h>
#include "drivers-manager.h"

static GtkWidget *create_main_area_box()
{
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

  return box;
}

static GtkWidget *drivers_found_message()
{
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  GtkWidget *label = gtk_label_new("Required drivers found, would you like to install them now?");

  GtkWidget *button = gtk_button_new_with_label("Install now");
  gtk_box_append(GTK_BOX(box), label);
  gtk_box_append(GTK_BOX(box), button);

  return box;
}

static void on_activate(GtkApplication *app)
{
  // Create a new window
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 200);

  GtkWidget *mainAreaBox = create_main_area_box();
  enum GPU gpu = detect_gpu();
  g_print(gpu);
  if (gpu != NONE)
  {
    GtkWidget *driversFoundBox = drivers_found_message();
    gtk_box_append(GTK_BOX(mainAreaBox), driversFoundBox);
  }

  gtk_window_set_child(GTK_WINDOW(window), mainAreaBox);
  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[])
{
  // Create a new application
  GtkApplication *app = gtk_application_new("com.example.GtkApplication",
                                            G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  return g_application_run(G_APPLICATION(app), argc, argv);
}
