#include <stdlib.h>
#include <gtk/gtk.h>

static void connexion (GtkWidget *wid, GtkWidget *win)
{
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Connexion");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}




int main (int argc, char *argv[])
{
  GtkWidget *button = NULL;
  GtkWidget *win = NULL;
  GtkWidget *box = NULL;
  GtkWidget *edit= NULL;

  /* Initialize GTK+ */
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (&argc, &argv);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  /* Create the main window */
  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (win), 8);
  gtk_window_set_title (GTK_WINDOW (win), "Projet FTP");
  gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
  //gtk_window_resize(GTK_WINDOW(win),600,300);
  gtk_widget_realize (win);
  g_signal_connect (win, "destroy", gtk_main_quit, NULL);

  /* Create a horizontal box with buttons */
  box = gtk_box_new (FALSE, 6);
  gtk_box_set_homogeneous(GTK_BOX(box),TRUE);
  gtk_container_add (GTK_CONTAINER (win), box);

    edit=gtk_entry_new();
    gtk_entry_set_placeholder_text(G_OBJECT(edit),"Adresse");
    gtk_box_pack_start(GTK_BOX(box),edit,TRUE,TRUE,0);
    gtk_widget_grab_focus(edit);

    edit=gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(edit),"Port");
    gtk_box_pack_start(GTK_BOX(box),edit,TRUE,TRUE,0);

  button = gtk_button_new_from_stock (GTK_STOCK_CONNECT);
  g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (connexion), (gpointer) win);
  gtk_box_pack_start (GTK_BOX (box), button, TRUE, TRUE, 0);

//  button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
//  g_signal_connect (button, "clicked", gtk_main_quit, NULL);
//  gtk_box_pack_start (GTK_BOX (box), button, TRUE, TRUE, 0);

  /* Enter the main loop */
  gtk_widget_show_all (win);
  gtk_main ();
  return 0;
}