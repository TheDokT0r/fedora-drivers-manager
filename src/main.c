#include <gtk/gtk.h>
#include "drivers-manager.h"

static void on_install_button_clicked(GtkButton *button, gpointer user_data)
{
    enum GPU gpu = GPOINTER_TO_INT(user_data);
    int result = install_drivers(gpu);

    if (result == 0)
        g_print("Drivers installed successfully.\n");
    else
        g_print("Driver installation failed.\n");
}


static GtkWidget *create_main_area_box()
{
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

  return box;
}

static GtkWidget *drivers_found_message(enum GPU gpu)
{
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  char *driversFoundText;
  asprintf(&driversFoundText, "Required drivers found for device: %s\nWould you like to install the appropriate drivers?", get_drivers_name(gpu));
  GtkWidget *label = gtk_label_new(driversFoundText);

  GtkWidget *button = gtk_button_new_with_label("Install now");

  g_signal_connect_swapped(button, "clicked", G_CALLBACK(on_install_button_clicked), GINT_TO_POINTER(gpu));

  gtk_box_append(GTK_BOX(box), label);
  gtk_box_append(GTK_BOX(box), button);

  return box;
}

static void on_activate(GtkApplication *app)
{
  // Create a new window
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 200);
  gtk_window_set_title(GTK_WINDOW(window), "Fedora Drivers Manager");

  GtkWidget *mainAreaBox = create_main_area_box();
  enum GPU gpu = detect_gpu();
  // g_print(gpu);
  if (gpu != NONE)
  {
    GtkWidget *driversFoundBox = drivers_found_message(gpu);
    gtk_box_append(GTK_BOX(mainAreaBox), driversFoundBox);
  }

  gtk_window_set_child(GTK_WINDOW(window), mainAreaBox);
  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[])
{
  // Create a new application
  GtkApplication *app = gtk_application_new("com.orche.fdm",
                                            G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  return g_application_run(G_APPLICATION(app), argc, argv);
}
