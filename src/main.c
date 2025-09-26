#include <gtk/gtk.h>
#include "drivers-manager.h"
#include "private/assist.h"

typedef struct InstallButtonClickData
{
  enum GPU gpu;
  GtkWidget *box;
} InstallButtonClickData;


static void on_install_button_clicked(gpointer user_data)
{
    InstallButtonClickData *data = (InstallButtonClickData *)user_data;

    GtkWidget *spinner = gtk_spinner_new();
    gtk_box_append(GTK_BOX(data->box), GTK_WIDGET(spinner));
    gtk_spinner_start(GTK_SPINNER(spinner));

    int result = install_drivers(data->gpu);

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
  free(driversFoundText);

  GtkWidget *button = gtk_button_new_with_label("Install now");

  // Allocate InstallButtonClickData on heap and pass pointer
  InstallButtonClickData *btnCbData = g_new(InstallButtonClickData, 1);
  btnCbData->gpu = gpu;
  btnCbData->box = box;

  // Connect signal with destroy notifier to free btnCbData automatically
  g_signal_connect_data(button, "clicked", G_CALLBACK(on_install_button_clicked), btnCbData, (GClosureNotify)g_free, 0);

  gtk_box_append(GTK_BOX(box), label);
  gtk_box_append(GTK_BOX(box), button);

  return box;
}

static void atomic_message(GtkWidget *window) {
  GtkWidget *label = gtk_label_new("The app is currently only available on Fedora non-atomic. Sorry :(");
  gtk_window_set_child(GTK_WINDOW(window), label);
  gtk_window_present(GTK_WINDOW(window));
}

static void on_activate(GtkApplication *app)
{
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 200);
  gtk_window_set_title(GTK_WINDOW(window), "Fedora Drivers Manager");

  if(is_running_fedora_or_atomic() == ATOMIC) {
    atomic_message(window);
    return;
  }

  GtkWidget *mainAreaBox = create_main_area_box();
  enum GPU gpu = detect_gpu();

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
  GtkApplication *app = gtk_application_new("com.orche.fdm", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
