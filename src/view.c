#include <glib/gerror.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdlib.h>
#include <stdio.h>

/** HIERARCHIE **/
/*
window
box
    boxConnect
        editAddress
        editSocket
        buttonConnect
    paned
        -scrolledWindowConsole
            textViewConsole
        -panedTreeView
            -boxLocal
                labelLocal
                scrolledWindowLocal
                    treeviewLocal
            -boxServer
                labelServer
                scrolledWindowServer
                    treeviewServer
*/

/** VARIABLES GLOBALES **/
    //Elements
    GtkWidget *win = NULL;
    GtkWidget *box = NULL;
    GtkWidget *paned = NULL;

    //Connexion
    GtkWidget *boxConnect = NULL;
    GtkWidget *editAddress = NULL, *editSocket = NULL;
    GtkWidget *buttonConnect = NULL;

    //Console
    GtkWidget *scrolledWindowConsole;
    GtkWidget *textViewConsole = NULL;
    GtkTextBuffer *textBufferConsole = NULL;

    //Dossiers
    GtkWidget *boxLocal, *boxServer;
    GtkWidget *labelLocal, *labelServer;
    GtkWidget *scrolledWindowLocal, *scrolledWindowServer;
    GtkWidget *panedTreeView = NULL;
    GtkWidget *treeviewLocal, *treeviewServer;

    //Images
    const char *pathFolder = "./../asset/directory.png";
    const char *pathFile = "./../asset/file.png";


//Clique bouton Connexion
static void connexion (GtkWidget *wid, GtkWidget *win){
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Connexion");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

//Liste colonne
enum {
    COL_TYPE = 0,
    COL_NAME,
    COL_SIZE,
    COL_LAST_UPDATE,
    NUM_COLS
};

//Insère les données
static GtkTreeModel *create_and_fill_model (void){
    GtkListStore  *store;
    GtkTreeIter    iter;
    GdkPixbuf *iconFolder = NULL, *iconFile = NULL;
    GError *error = NULL;

    iconFolder = gdk_pixbuf_new_from_file (pathFolder, &error);
    iconFile = gdk_pixbuf_new_from_file (pathFile, &error);
    if (error){
        g_warning ("L\'icone ne peut être chargé : %s\n", error->message);
        g_error_free (error);
        error = NULL;
    }

    store = gtk_list_store_new (NUM_COLS, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_STRING);

    /* Insertion de données dans le TreeView */
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter, COL_TYPE, iconFolder, COL_NAME, "Dossier 1", COL_SIZE, 0, COL_LAST_UPDATE, "31/01/2013 23:09:00", -1);
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter, COL_TYPE, iconFile, COL_NAME, "Fichier 1", COL_SIZE, 560, COL_LAST_UPDATE, "31/01/2013 23:59:00", -1);

    return GTK_TREE_MODEL (store);
}

//Créé TreeView
static GtkWidget *create_view_and_model (void){
    GtkCellRenderer     *renderer;
    GtkTreeModel        *model;
    GtkWidget           *treeview;

    treeview = gtk_tree_view_new ();

    /* --- Column #1 --- */
    renderer = gtk_cell_renderer_pixbuf_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview), -1,
                                                 "Type", renderer, "pixbuf", COL_TYPE, NULL);
    /* --- Column #2 --- */
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview), -1,
                                                 "Nom", renderer, "text", COL_NAME, NULL);
    /* --- Column #3 --- */
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview), -1,
                                                 "Taille", renderer, "text", COL_SIZE, NULL);
    /* --- Column #4 --- */
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview), -1,
                                                 "Dernière modif", renderer, "text", COL_LAST_UPDATE, NULL);

    model = create_and_fill_model ();
    gtk_tree_view_set_model (GTK_TREE_VIEW (treeview), model);
    g_object_unref (model);

    return treeview;
}



/** MAIN **/
int main (int argc, char *argv[]) {

/** GTK+ **/
    /* Initialise GTK+ */
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
    gtk_init (&argc, &argv);
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

/** WINDOW **/
    /* Création de la fenêtre principale */
    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (win), 8);
    gtk_window_set_title (GTK_WINDOW (win), "Projet FTP");
    gtk_window_set_default_size (GTK_WINDOW (win), 650, 400);
    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (win);
    g_signal_connect (win, "destroy", gtk_main_quit, NULL);


/** CONNECT **/
    /* Boite Connexion */
    boxConnect = gtk_box_new (FALSE, 6);
//    gtk_box_set_homogeneous(GTK_BOX(boxConnect),TRUE);

    /* Champ Adresse */
    editAddress=gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(editAddress),"Adresse");
    gtk_box_pack_start(GTK_BOX(boxConnect),editAddress,TRUE,TRUE,0);
    gtk_widget_grab_focus(editAddress);

    /* Champ Port */
    editSocket=gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(editSocket),"Port");
    gtk_box_pack_start(GTK_BOX(boxConnect),editSocket,TRUE,TRUE,0);

    /* Bouton Connexion */
    buttonConnect = gtk_button_new_from_stock (GTK_STOCK_CONNECT);
    g_signal_connect (G_OBJECT (buttonConnect), "clicked", G_CALLBACK (connexion), (gpointer) win);
    gtk_box_pack_start (GTK_BOX (boxConnect), buttonConnect, FALSE, FALSE, 0);


/** CONSOLE **/
    /* Affichage de la Console */
    textViewConsole = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textViewConsole),FALSE);
    textBufferConsole=gtk_text_view_get_buffer(GTK_TEXT_VIEW(textViewConsole));
    gtk_text_buffer_set_text(textBufferConsole,"Console :\n\n\n\n\n\n\ntest\n\nscrolling",-1);
//    gtk_widget_set_size_request(textViewConsole,150,-1);

    /* Scroll local */
    scrolledWindowConsole = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_set_size_request(scrolledWindowConsole,150,-1);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindowConsole), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_container_add (GTK_CONTAINER(scrolledWindowConsole), textViewConsole);



/** TREEVIEW LOCAL **/
    /* TreeView local */
    treeviewLocal = create_view_and_model ();
//    gtk_widget_set_size_request(treeviewLocal,320,200);

    /* Scroll local */
    scrolledWindowLocal = gtk_scrolled_window_new (NULL, NULL);       /*init de la scrollbar*/
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindowLocal), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add (GTK_CONTAINER(scrolledWindowLocal), treeviewLocal); /* attache le treeview a la scrollbar*/

    /* Label local */
    labelLocal = gtk_label_new("Site local :");

    /* boite local */
    boxLocal = gtk_box_new (TRUE, 6);
    gtk_box_pack_start (GTK_BOX (boxLocal), labelLocal, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (boxLocal), scrolledWindowLocal, TRUE, TRUE, 0);


/** TREEVIEW SERVEUR **/
    /* TreeView serveur */
    treeviewServer = create_view_and_model ();
//    gtk_widget_set_size_request(treeviewServer,0,200);

    /* Scroll serveur */
    scrolledWindowServer = gtk_scrolled_window_new (NULL, NULL);       /*init de la scrollbar*/
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindowServer), GTK_POLICY_ALWAYS, GTK_POLICY_AUTOMATIC);
    gtk_container_add (GTK_CONTAINER(scrolledWindowServer), treeviewServer); /* attache le treeview a la scrollbar*/

    /* Label serveur */
    labelServer = gtk_label_new("Site distant :");

    /* boite serveur */
    boxServer = gtk_box_new (TRUE, 6);
    gtk_box_pack_start (GTK_BOX (boxServer), labelServer, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (boxServer), scrolledWindowServer, TRUE, TRUE, 0);


/** PANED (TREEVIEW LOCAL/SERVEUR) **/
    /* Paned avec les TreeView Local et serveur */
    panedTreeView=gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
//    gtk_widget_set_size_request(panedTreeView,200,-1);
    gtk_paned_set_position(GTK_PANED(panedTreeView),315);
    gtk_paned_pack1(GTK_PANED(panedTreeView),boxLocal,TRUE,TRUE);
    gtk_paned_pack2(GTK_PANED(panedTreeView),boxServer,TRUE,TRUE);


/** PANED (CONSOLE/TREEVIEWS) **/
    /* Paned avec Console et TreeView */
    paned = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
//    gtk_widget_set_size_request(paned,200,-1);
    gtk_paned_set_position(GTK_PANED(paned),100);
    gtk_paned_pack1(GTK_PANED(paned),scrolledWindowConsole,TRUE,FALSE);
    gtk_paned_pack2(GTK_PANED(paned),panedTreeView,TRUE,FALSE);


/** CONTAINER **/
    /* Boite Conteneur */
    box = gtk_box_new (TRUE, 6);

    //Ajoute les éléments à la boite conteneur
    gtk_box_pack_start(GTK_BOX(box),boxConnect,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),paned,TRUE,TRUE,0);
    gtk_container_add (GTK_CONTAINER (win), box);

    gtk_widget_show_all (win);
    gtk_main ();

    return 0;
}
