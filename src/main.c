#include <glib/gerror.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdlib.h>
#include <stdio.h>

#include "hf_treeview.h"

//Variables for TreeView
gint Show_marks = 0;
gint Title = 0;
gint Show_header = 1;
gint Cell_editable = 0;

t_Tree_view *tw = NULL;

//Click Button Connexion
static void connexion (GtkWidget *wid, GtkWidget *win){
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Connexion");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

//Click on TreeView
void on_click (GtkWidget *p_widget, gpointer p_data){
   gint index = (gint)p_data;

   switch (index) {
      /* Get_selected_item_type */
      case 10: {
         switch (tw->Get_selected_item_type (tw)) {
            case HF_TW_DIRECTORY_ITEM :
               printf ("L'item est du type: HF_TW_DIRECTORY_ITEM: %d\n",
                       HF_TW_DIRECTORY_ITEM);
               break;
            case HF_TW_FILE_ITEM :
               printf ("L'item est du type: HF_TW_FILE_ITEM: %d\n",
                       HF_TW_FILE_ITEM);
               break;
            default:
                (void)NULL;
         }
      }
      break;

      /* Add_item : HF_TW_FILE_ITEM */
      case 0:
         tw->Add_item_by_selection (tw, "file.png", "Fichier...", HF_TW_FILE_ITEM);
      break;

      /* Add_item : HF_TW_DIRECTORY_ITEM */
      case 11:
         tw->Add_item_by_selection (tw, "directory.png", "Dossier...", HF_TW_DIRECTORY_ITEM);
      break;

      /* Add_item_as_root */
      case 9:
         tw->Add_item_as_root (tw, "directory.png", "Item sur root", HF_TW_DIRECTORY_ITEM);
      break;

      /* Remove_item */
      case 1:
         tw->Remove_item (tw);
      break;

      /* Clear_all */
      case 2:
         tw->Clear_all (tw);
      break;

      /* Show_column_header */
      case 3:
      {
         if (Show_header == 0) {
            tw->Show_column_header (tw, TRUE);
            Show_header = 1;
         } else {
            tw->Show_column_header (tw, FALSE);
            Show_header = 0;
         }
      }
      break;

      /* Change_column_title */
      case 4: {
         if (Title == 0) {
            tw->Change_column_title (tw, "Titre changé...");
            Title = 1;
         } else {
            tw->Change_column_title (tw, "Test...");
            Title = 0;
         }
      }
      break;

      /* Expand_all_items */
      case 5:
         tw->Expand_all_items (tw);
      break;

      /* Collapse_all_items */
      case 6:
         tw->Collapse_all_items (tw);
      break;

      /* Show_lines_marks */
      case 7:
      {
         if (Show_marks == 0) {
            tw->Show_lines_marks (tw, TRUE);
            Show_marks = 1;
         } else {
            tw->Show_lines_marks (tw, FALSE);
            Show_marks = 0;
         }
      }
      break;

      /* Set_cell_editable */
      case 8:
      {
         if (Cell_editable == 0) {
            tw->Set_cell_editable (tw, TRUE);
            Cell_editable = 1;
         } else {
            tw->Set_cell_editable (tw, FALSE);
            Cell_editable = 0;
         }
      }

      default:
        (void)NULL;
   }
}

///* Deuxieme   test files   */
//void store_filename (GtkWidget *widget, gpointer user_data) {
//   GtkWidget *file_selector = GTK_WIDGET (user_data);
//   const gchar *selected_filename;
//
//   selected_filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (file_selector));
//   g_print ("Selected filename: %s\n", selected_filename);
//}
//
//void create_file_selection (void) {
//
//   GtkWidget *file_selector;
//
//   /* Create the selector */
//
//   file_selector = gtk_file_selection_new ("Please select a file for editing.");
//
//   g_signal_connect (GTK_FILE_SELECTION (file_selector)->ok_button,
//                     "clicked",
//                     G_CALLBACK (store_filename),
//                     file_selector);
//
//   /* Ensure that the dialog box is destroyed when the user clicks a button. */
//
//   g_signal_connect_swapped (GTK_FILE_SELECTION (file_selector)->ok_button,
//                             "clicked",
//                             G_CALLBACK (gtk_widget_destroy),
//                             file_selector);
//
//   g_signal_connect_swapped (GTK_FILE_SELECTION (file_selector)->cancel_button,
//                             "clicked",
//                             G_CALLBACK (gtk_widget_destroy),
//                             file_selector);
//
//   /* Display that dialog */
//
//   gtk_widget_show (file_selector);
//}


/** MAIN **/
int main (int argc, char *argv[])
{
    //Elements
    GtkWidget *win = NULL;
    GtkWidget *box = NULL;
    GtkWidget *paned = NULL;

    //Connect
    GtkWidget *boxConnect = NULL;
    GtkWidget *editAddress = NULL, *editSocket = NULL;
    GtkWidget *buttonConnect = NULL;

    //Console
    GtkWidget *textViewConsole = NULL;
    GtkTextBuffer *textBufferConsole = NULL;

    //Treeview
    GtkWidget *boxTreeView;
    GtkWidget *boxButtons;
    GtkWidget *buttons[12];


/** GTK+ **/
    /* Initialize GTK+ */
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
    gtk_init (&argc, &argv);
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

/** WINDOW **/
    /* Create the main window */
    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (win), 8);
    gtk_window_set_title (GTK_WINDOW (win), "Projet FTP");
    gtk_window_set_default_size (GTK_WINDOW (win), 640, 480);
    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (win);
    g_signal_connect (win, "destroy", gtk_main_quit, NULL);


/** CONNECT **/
    /* Box Connect */
    boxConnect = gtk_box_new (FALSE, 6);
    gtk_box_set_homogeneous(GTK_BOX(boxConnect),TRUE);

    /* Entry Address */
    editAddress=gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(editAddress),"Adresse");
    gtk_box_pack_start(GTK_BOX(boxConnect),editAddress,TRUE,TRUE,0);
    gtk_widget_grab_focus(editAddress);

    /* Entry Socket */
    editSocket=gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(editSocket),"Port");
    gtk_box_pack_start(GTK_BOX(boxConnect),editSocket,TRUE,TRUE,0);

    /* Button Connect */
    buttonConnect = gtk_button_new_from_stock (GTK_STOCK_CONNECT);
    g_signal_connect (G_OBJECT (buttonConnect), "clicked", G_CALLBACK (connexion), (gpointer) win);
    gtk_box_pack_start (GTK_BOX (boxConnect), buttonConnect, TRUE, TRUE, 0);


/** CONSOLE **/
    /* TextView Console */
    textViewConsole = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textViewConsole),FALSE);
    textBufferConsole=gtk_text_view_get_buffer(GTK_TEXT_VIEW(textViewConsole));
    gtk_text_buffer_set_text(textBufferConsole,"Console :",-1);
    gtk_widget_set_size_request(textViewConsole,100,-1);



/** TREEVIEW **/
    boxTreeView = gtk_box_new (FALSE, 0);

    /* Container pour les boutons */
    boxButtons = gtk_box_new (TRUE, 0);
    gtk_box_pack_end (GTK_BOX (boxTreeView), boxButtons, TRUE, TRUE, 0);

    /* Mise en place des boutons pour les tests */
    buttons[10] = gtk_button_new_with_label ("Fonction : Get_selected_item_type ()");
    g_signal_connect (G_OBJECT (buttons[10]), "clicked", G_CALLBACK (on_click), (gpointer)10);
    gtk_box_pack_start (GTK_BOX (boxButtons), buttons[10], TRUE, TRUE, 1);
    buttons[0] = gtk_button_new_with_label ("Fonction : Add_item () : HF_TW_FILE_ITEM");
    g_signal_connect (G_OBJECT (buttons[0]), "clicked", G_CALLBACK (on_click), (gpointer)0);
    gtk_box_pack_start (GTK_BOX (boxButtons), buttons[0], TRUE, TRUE, 1);
    buttons[11] = gtk_button_new_with_label ("Fonction : Add_item () : HF_TW_DIRECTORY_ITEM");
    g_signal_connect (G_OBJECT (buttons[11]), "clicked", G_CALLBACK (on_click), (gpointer)11);
    gtk_box_pack_start (GTK_BOX (boxButtons), buttons[11], TRUE, TRUE, 1);
    buttons[9] = gtk_button_new_with_label ("Fonction : Add_item_as_root ()");
    g_signal_connect (G_OBJECT (buttons[9]), "clicked", G_CALLBACK (on_click), (gpointer)9);
    gtk_box_pack_start (GTK_BOX (boxButtons), buttons[9], TRUE, TRUE, 1);
    buttons[1] = gtk_button_new_with_label ("Fonction : Remove_item ()");
    g_signal_connect (G_OBJECT (buttons[1]), "clicked", G_CALLBACK (on_click), (gpointer)1);
    gtk_box_pack_start (GTK_BOX (boxButtons), buttons[1], TRUE, TRUE, 1);
    buttons[2] = gtk_button_new_with_label ("Fonction : Clear_all ()");
    g_signal_connect (G_OBJECT (buttons[2]), "clicked", G_CALLBACK (on_click), (gpointer)2);
    gtk_box_pack_start (GTK_BOX (boxButtons), buttons[2], TRUE, TRUE, 1);
    buttons[3] = gtk_button_new_with_label ("Fonction : Show_column_header ()");
    g_signal_connect (G_OBJECT (buttons[3]), "clicked", G_CALLBACK (on_click), (gpointer)3);
    gtk_box_pack_start (GTK_BOX (boxButtons), buttons[3], TRUE, TRUE, 1);
    buttons[4] = gtk_button_new_with_label ("Fonction : Change_column_title ()");
    g_signal_connect (G_OBJECT (buttons[4]), "clicked", G_CALLBACK (on_click), (gpointer)4);
    gtk_box_pack_start (GTK_BOX (boxButtons), buttons[4], TRUE, TRUE, 1);
    buttons[5] = gtk_button_new_with_label ("Fonction : Expand_all_items ()");
    g_signal_connect (G_OBJECT (buttons[5]), "clicked", G_CALLBACK (on_click), (gpointer)5);
    gtk_box_pack_start (GTK_BOX (boxButtons), buttons[5], TRUE, TRUE, 1);
    buttons[6] = gtk_button_new_with_label ("Fonction : Collapse_all_items ()");
    g_signal_connect (G_OBJECT (buttons[6]), "clicked", G_CALLBACK (on_click), (gpointer)6);
    gtk_box_pack_start (GTK_BOX (boxButtons), buttons[6], TRUE, TRUE, 1);
    buttons[7] = gtk_button_new_with_label ("Fonction : Show_lines_marks ()");
    g_signal_connect (G_OBJECT (buttons[7]), "clicked", G_CALLBACK (on_click), (gpointer)7);
    gtk_box_pack_start (GTK_BOX (boxButtons), buttons[7], TRUE, TRUE, 1);
    buttons[8] = gtk_button_new_with_label ("Fonction : Set_cell_editable ()");
    g_signal_connect (G_OBJECT (buttons[8]), "clicked", G_CALLBACK (on_click), (gpointer)8);
    gtk_box_pack_start (GTK_BOX (boxButtons), buttons[8], TRUE, TRUE, 1);


    /*  TEST TREEVIEW   */
    /* Creation du treeview. */
//   tw = New_treeview (TRUE, TRUE, "Test");
//
//   /* Mise en place du treeview dans l'interface. */
//   gtk_box_pack_start (GTK_BOX (boxTreeView), tw->Main_container, TRUE, TRUE, 4);
//
//   /* Ajout d'items. Normalement il faudrais verifier la valeur de retour
//      de la fonction. A ne pas oublier donc pour une bonne gestion des
//      erreurs. */
//   GtkTreeIter Parent_iter;
//   GtkTreeIter Sub_parent_iter;
//   GtkTreeIter Iter;
//
//   Parent_iter = tw->Add_item_as_root (tw, "directory.png", "Dossier 1", HF_TW_DIRECTORY_ITEM);
//
//   Iter = tw->Add_item (tw, "file.png", "Fichier 1", HF_TW_FILE_ITEM, TRUE, &Parent_iter);
//   Iter = tw->Add_item (tw, "file.png", "Fichier 2", HF_TW_FILE_ITEM, TRUE, &Parent_iter);
//
//   Parent_iter = tw->Add_item_as_root (tw, "directory.png", "Dossier 2", HF_TW_DIRECTORY_ITEM);
//
//   Iter = tw->Add_item (tw, "file.png", "Fichier 3", HF_TW_FILE_ITEM, TRUE, &Parent_iter);
//   Iter = tw->Add_item (tw, "file.png", "Fichier 4", HF_TW_FILE_ITEM, TRUE, &Parent_iter);
//   Sub_parent_iter = tw->Add_item (tw, "directory.png", "Dossier 3", HF_TW_DIRECTORY_ITEM, TRUE, &Parent_iter);
//   Iter = tw->Add_item (tw, "file.png", "Fichier 5", HF_TW_FILE_ITEM, TRUE, &Sub_parent_iter);
//   Iter = tw->Add_item (tw, "file.png", "Fichier 6", HF_TW_FILE_ITEM, TRUE, &Sub_parent_iter);
//
//   Parent_iter = tw->Add_item_as_root (tw, "directory.png", "Dossier 4", HF_TW_DIRECTORY_ITEM);
//
//   Sub_parent_iter = tw->Add_item (tw, "directory.png", "Dossier 5", HF_TW_FILE_ITEM, TRUE, &Parent_iter);
//   Parent_iter = tw->Add_item (tw, "directory.png", "Dossier 6", HF_TW_FILE_ITEM, TRUE, &Sub_parent_iter);
//   Sub_parent_iter = tw->Add_item (tw, "directory.png", "Dossier 7", HF_TW_FILE_ITEM, TRUE, &Parent_iter);
//   Parent_iter = tw->Add_item (tw, "directory.png", "Dossier 8", HF_TW_FILE_ITEM, TRUE, &Sub_parent_iter);
//   Sub_parent_iter = tw->Add_item (tw, "directory.png", "Dossier 9", HF_TW_FILE_ITEM, TRUE, &Parent_iter);
//
//   /* On affiche tous les items. */
//   tw->Expand_all_items (tw);





/** PANED **/
    /* Paned with Console and TreeView */
    paned=gtk_paned_new(GTK_ORIENTATION_VERTICAL);
    gtk_widget_set_size_request(paned,200,-1);
    gtk_paned_set_position(GTK_PANED(paned),100);
    gtk_paned_pack1(GTK_PANED(paned),textViewConsole,TRUE,FALSE);
    gtk_paned_pack2(GTK_PANED(paned),boxTreeView,TRUE,FALSE);


/** CONTAINER **/
    /* Box Container */
    box = gtk_box_new (TRUE, 6);

    //Add elements to the box container
    gtk_box_pack_start(GTK_BOX(box),boxConnect,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),paned,TRUE,TRUE,0);
    gtk_container_add (GTK_CONTAINER (win), box);

    gtk_widget_show_all (win);
    gtk_main ();    //Enter the main loop

    /* Liberation des ressources. */
    tw->Destroy (&tw);

    return 0;
}
