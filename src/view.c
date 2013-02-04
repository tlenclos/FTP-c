#include "view.h"


/** VARIABLES GLOBALES **/
    //Elements
    GtkWidget *win = NULL;
    GtkWidget *box = NULL;
    GtkWidget *paned = NULL;
    GtkWidget *statusbar;
    GtkWidget *spinner;

    //Connexion
    GtkWidget *boxConnect = NULL;
    GtkWidget *editAddress = NULL, *editSocket = NULL;
    GtkWidget *buttonConnect = NULL;

    //Console
    GtkWidget *scrolledWindowConsole;
    GtkWidget *textViewConsole = NULL;
    GtkTextBuffer *textBufferConsole = NULL;
    GtkTextIter iter;

    //Dossiers
    GtkWidget *boxFile, *boxLocal, *boxServer;
    GtkWidget *labelLocal, *labelServer;
    GtkWidget *scrolledWindowLocal, *scrolledWindowServer;
    GtkWidget *panedTreeView = NULL;
    GtkWidget *treeviewLocal, *treeviewServer;
    GtkListStore *storeLocal, *storeServer;
    GtkTreeSelection *selection;

    //Fonctionnalités
    GtkWidget *boxAction;
    GtkWidget *buttonRename, *buttonCopy, *buttonPaste, *buttonAddFolder, *buttonRemoveFolder;
    GtkWidget *buttonDownload, *buttonLoad, *buttonCancel;

    //Chemins
    char *dir_nameLocal;
    char *dir_nameServer;



//Ajoute le texte dans la console
void insertConsole(char *newText){
    gtk_text_buffer_get_end_iter(textBufferConsole,&iter);
//    snprintf(newText, sizeof newText,"%s", "\n");
    gtk_text_buffer_insert(textBufferConsole,&iter,"\n",-1);
    gtk_text_buffer_get_end_iter(textBufferConsole,&iter);
    gtk_text_buffer_insert(textBufferConsole,&iter,newText,-1);
    gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(textViewConsole),&iter,0.0,FALSE,0,0);
}

//Clique bouton Connexion
static void connexion (GtkWidget *wid, GtkWidget *win){
    setStatusBar("Connexion en cours...");

	const char *address = gtk_entry_get_text(GTK_ENTRY(editAddress));
	const char *socket = gtk_entry_get_text(GTK_ENTRY(editSocket));

//	init_client (address,socket);
//	read_cmd("LIST");
    setStatusBar("Connexion réussie");
	//insertConsole(g_object_get (G_OBJECT (editAddress), "editAddress", &editAddress, NULL));

/*
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Connexion");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
  */
}

// lorsque l'on quitte l'appli
void cmd_QUIT (void)
{
	read_cmd("QUIT");
}

// lorsque l'on veut charger un fichier depuis le client
void cmd_STOR (void)
{
	//TODO: verifier que l'on a un fichier client de selectionné
	//TODO: ajouter le fichier selectionné
	char* cmd = "STOR ";
	char* file_name;


	strcat (cmd, file_name);
	read_cmd(cmd);
}

// lorsque l'on veut télécharger un fichier depuis le serveur
void cmd_RETR (void)
{
	//TODO: verifier que l'on a un fichier serveur de selectionné
	//TODO: ajouter le fichier selectionné
	char* cmd = "RETR ";
	char* file_name;


	strcat (cmd, file_name);
	read_cmd(cmd);
}

// lorsque l'on ouvre un dossier serveur doit fournir la liste du contenu
void cmd_LIST (void)
{
	//TODO: recupérer la fameuse liste ...
	read_cmd("LIST");
}

// lorsque l'on veut changer de port
void cmd_PORT (void)
{
	//TODO: ouvrir une fenetre pour saisir le nouveau port
	char* cmd = "PORT ";
	char* nouveau_port;


	strcat (cmd, nouveau_port);
	read_cmd(cmd);
}

// lorsque l'on selectionne de dossier sur le serveur (comme $cd <dir>)
void cmd_CWD (void)
{
	//TODO: trouver un moyen d'avoir le chemin complet du fichier selectionné
	read_cmd("CWD");
}

// lorsque l'on appuie sur le bouton supprimer
void cmd_DELE (void)
{
	//TODO: verifier que l'on a un fichier de selectionné
	//TODO: ajouter le fichier selectionné à la commande
	char* cmd = "DELE ";
	char* file_name;


	strcat (cmd, file_name);
	read_cmd(cmd);
}

void cmd_RMD (void)
{
	//TODO: verifier que l'on a un dossier de selectionné
	//TODO: ajouter le dossier selectionné à la commande
	char* cmd = "RMD ";
	char* folder_name;


	strcat (cmd, folder_name);
	read_cmd(cmd);
}

void cmd_MKD (void)
{
	//TODO: ouvrir une fenetre pour saisir le nom du dossier
	char* cmd = "MKD ";
	char* folder_name;


	strcat (cmd, folder_name);
	read_cmd(cmd);
}

/*
void cmd_RNFR (void)
{
	read_cmd("QUIT");
}

void cmd_RNTO (void)
{
	read_cmd("QUIT");
}
*/


//Insère les données
//static GtkTreeModel *create_and_fill_model (void){
//    GtkTreeIter    iter;
//    GdkPixbuf *iconFolder = NULL, *iconFile = NULL;
//    GError *error = NULL;

//    iconFolder = gdk_pixbuf_new_from_file (pathFolder, &error);
//    iconFile = gdk_pixbuf_new_from_file (pathFile, &error);
//    if (error){
//        g_warning ("L\'icone ne peut être chargé : %s\n", error->message);
//        g_error_free (error);
//        error = NULL;
//    }

//    store = gtk_list_store_new (NUM_COLS, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_STRING);
//
//    gtk_list_store_clear(store);
//
//    //Affiche liste dossier
//    dir_list();

    /* Exemple d'insertion de données dans le TreeView */
//    gtk_list_store_append (store, &iter);
//    gtk_list_store_set (store, &iter, COL_TYPE, iconFolder, COL_NAME, "Dossier 1", COL_SIZE, 0, COL_LAST_UPDATE, "31/01/2013 23:09:00", -1);
//    gtk_list_store_append (store, &iter);
//    gtk_list_store_set (store, &iter, COL_TYPE, iconFile, COL_NAME, "Fichier 1", COL_SIZE, 560, COL_LAST_UPDATE, "31/01/2013 23:59:00", -1);

//    return GTK_TREE_MODEL (store);
//}


//Créé la vue du TreeView
GtkWidget *create_view (int isLocal){
    GtkCellRenderer     *renderer;
    GtkTreeModel        *model;
    GtkWidget           *treeview;

    //Création treeview
    treeview = gtk_tree_view_new ();

    //Création des Colonnes
    renderer = gtk_cell_renderer_pixbuf_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview), -1, "Type", renderer, "pixbuf", COL_TYPE, NULL);
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview), -1, "Nom", renderer, "text", COL_NAME, NULL);
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview), -1, "Taille", renderer, "text", COL_SIZE, NULL);
//    renderer = gtk_cell_renderer_text_new ();
//    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview), -1, "Dernière modif", renderer, "text", COL_LAST_UPDATE, NULL);

    //Type de données
    if(isLocal){
        storeLocal = gtk_list_store_new (NUM_COLS, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_UINT);
        model = GTK_TREE_MODEL(storeLocal);
    }else{
        storeServer = gtk_list_store_new (NUM_COLS, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_UINT);
        model = GTK_TREE_MODEL(storeServer);
    }
    //Chargement des Données
//    dir_list();

    //Insertion des données dans treeview
    gtk_tree_view_set_model (GTK_TREE_VIEW (treeview), model);

    //Ajoute sélection item
    g_signal_connect (G_OBJECT (treeview), "row-activated", G_CALLBACK (cb_select), NULL);

    g_object_unref (model);

    return treeview;
}

//Affiche la liste des fichiers dans le dossier courant
void create_model (GtkListStore *store, char *dir_name){
    GDir *dir = NULL;

    //Type d'un fichier
    typedef enum {
      G_FILE_TEST_IS_REGULAR    = 1 << 0, /* TRUE si le fichier est un fichier standard (ni un lien symbolique ni un dossier) */
      G_FILE_TEST_IS_SYMLINK    = 1 << 1, /* TRUE si le fichier est un lien symbolique */
      G_FILE_TEST_IS_DIR        = 1 << 2, /* TRUE si le fichier est un dossier */
      G_FILE_TEST_IS_EXECUTABLE = 1 << 3, /* TRUE si le fichier est un executable */
      G_FILE_TEST_EXISTS        = 1 << 4  /* TRUE si le fichier existe */
    } GFileTest;

    dir = g_dir_open (dir_name, 0, NULL);
    if (dir) {
        const gchar *read_name = NULL;

        GtkTreeIter iter;
        GdkPixbuf *file_image = NULL;

        gtk_list_store_clear (store);
        file_image = gdk_pixbuf_new_from_file (pathFolder, NULL);
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store, &iter, 0, file_image, 1, "..", 2, NULL, -1);

        while ((read_name = g_dir_read_name (dir))) {
            GFile *file=NULL;
            GFileInfo *info =NULL;
            gchar *file_name = NULL;
            GdkPixbuf *file_image = NULL;
//            gchar *file_size = NULL;
            GTimeVal file_lastUpdate;

            //Nom du fichier
            file_name = g_build_path (G_DIR_SEPARATOR_S, dir_name, read_name, NULL);

            //Info du fichier
            file = g_file_new_for_path(file_name);
            info = g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_SIZE","G_FILE_ATTRIBUTE_TIME_MODIFIED, G_FILE_QUERY_INFO_NONE, NULL, NULL);

            //Taille du fichier
            int size = g_file_info_get_size(info);

            //Dernière modification
            g_file_info_get_modification_time(info, &file_lastUpdate);
//            g_memdup(&file_lastUpdate, sizeof(file_lastUpdate));

            //Icone du fichier
            if (g_file_test (file_name, G_FILE_TEST_IS_DIR)){       //Si dossier
                file_image = gdk_pixbuf_new_from_file (pathFolder, NULL);
                size = 0;
            } else {                                                //Sinon fichier
                file_image = gdk_pixbuf_new_from_file (pathFile, NULL);
//                sprintf(file_size, "%d octets", size);
//                itoa(size, file_size, 10);
            }

            /* Ajout d'une nouvelle entree au magasin */
            gtk_list_store_append (store, &iter);
            gtk_list_store_set (store, &iter, 0, file_image, 1, read_name, 2, size, -1);
        }

        g_dir_close (dir), dir = NULL;
    }
}

//Sélectionne un item
void cb_select (GtkTreeView *tree_view, GtkTreePath *arg1, GtkTreeViewColumn *arg2, gpointer user_data){
  gchar *str = NULL;
  GtkTreeIter iter;
  GtkTreeModel *tree_model = NULL;

  tree_model = gtk_tree_view_get_model (tree_view);
  gtk_tree_model_get_iter (tree_model, &iter, arg1);
  gtk_tree_model_get (tree_model, &iter, 1, &str, -1);
  {
    gchar *file_name = NULL;

    file_name = g_build_path (G_DIR_SEPARATOR_S, dir_nameLocal, str, NULL);
    g_free (str), str = NULL;

    if (g_file_test (file_name, G_FILE_TEST_IS_DIR)){
        //Dossier
        g_free (dir_nameLocal), dir_nameLocal = NULL;
        dir_nameLocal = file_name;
        create_model (storeLocal, dir_nameLocal);
    } else {
        //Fichier

    }
  }
  /* parametres inutilises */
  (void)arg2;
  (void)user_data;
}

//Remplace le texte dans la barre de status
void setStatusBar(const char *newText){
    gtk_statusbar_push(GTK_STATUSBAR(statusbar),
            gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), newText),
            newText);
}

//Sélection d'un élément d'un treeview
void on_changed(GtkWidget *widget, gpointer statusbar) {
    GtkTreeIter iter;
    GtkTreeModel *model;
    char *value;

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) {
        gtk_tree_model_get(model, &iter, COL_NAME, &value,  -1);
        setStatusBar(value);
        g_free(value);
    }
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

/** SPINNER **/
    spinner = gtk_spinner_new();
    gtk_spinner_start(GTK_SPINNER(spinner));


/** STATUSBAR **/
    statusbar = gtk_statusbar_new();
    gtk_statusbar_push(GTK_STATUSBAR(statusbar), gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "Bienvenue"),"Bienvenue sur notre FTP !");


/** CONNECT **/
    /* Boite Connexion */
    boxConnect = gtk_box_new (FALSE, 6);
//    gtk_box_set_homogeneous(GTK_BOX(boxConnect),TRUE);

    /* Champ Adresse */
    editAddress=gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(editAddress),"Adresse");
    gtk_box_pack_start(GTK_BOX(boxConnect),editAddress,TRUE,TRUE,0);
    gtk_widget_grab_focus(editAddress);
    gtk_entry_set_text(GTK_ENTRY(editAddress), "localhost");

    /* Champ Port */
    editSocket=gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(editSocket),"Port");
    gtk_box_pack_start(GTK_BOX(boxConnect),editSocket,TRUE,TRUE,0);
    gtk_entry_set_text(GTK_ENTRY(editSocket), "8070");

    /* Bouton Connexion */
    buttonConnect = gtk_button_new_from_stock (GTK_STOCK_CONNECT);
    g_signal_connect (G_OBJECT (buttonConnect), "clicked", G_CALLBACK (connexion), (gpointer) win);
    gtk_box_pack_start (GTK_BOX (boxConnect), buttonConnect, FALSE, FALSE, 0);


/** CONSOLE **/
    /* Affichage de la Console */
    textViewConsole = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textViewConsole),FALSE);
    textBufferConsole=gtk_text_view_get_buffer(GTK_TEXT_VIEW(textViewConsole));
    gtk_text_buffer_set_text(textBufferConsole,"Console :",-1);
//    gtk_widget_set_size_request(textViewConsole,150,-1);

    /* Scroll local */
    scrolledWindowConsole = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_set_size_request(scrolledWindowConsole,150,-1);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindowConsole), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_container_add (GTK_CONTAINER(scrolledWindowConsole), textViewConsole);


/** TREEVIEW LOCAL **/
    dir_nameLocal = g_strdup(g_get_home_dir());

    /* TreeView local */
    treeviewLocal = create_view(TRUE);
    create_model(storeLocal, dir_nameLocal);
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

    /* sélectionne une ligne */
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeviewLocal));
    g_signal_connect(selection, "changed", G_CALLBACK(on_changed), statusbar);


/** TREEVIEW SERVEUR **/
//    dir_nameServer = g_strdup(g_get_home_dir());

    /* TreeView serveur */
    treeviewServer = create_view(FALSE);
//    create_model();
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

    /* sélectionne une ligne */
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeviewServer));
    g_signal_connect(selection, "changed", G_CALLBACK(on_changed), statusbar);


/** PANED (TREEVIEW LOCAL/SERVEUR) **/
    /* Paned avec les TreeView Local et serveur */
    panedTreeView=gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
//    gtk_widget_set_size_request(panedTreeView,200,-1);
    gtk_paned_set_position(GTK_PANED(panedTreeView),315);
    gtk_paned_pack1(GTK_PANED(panedTreeView),boxLocal,TRUE,TRUE);
    gtk_paned_pack2(GTK_PANED(panedTreeView),boxServer,TRUE,TRUE);


/** FONCTIONNALITES **/
    boxAction = gtk_box_new(FALSE,5);

    /* Bouton Renommer */
    buttonRename = gtk_button_new_with_label("Renommer");
//    g_signal_connect (G_OBJECT (buttonRename), "clicked", G_CALLBACK (connexion), (gpointer) win);
    gtk_box_pack_start (GTK_BOX (boxAction), buttonRename, FALSE, FALSE, 0);

    /* Bouton Renommer */
    buttonCopy = gtk_button_new_with_label("Copier");
//    g_signal_connect (G_OBJECT (buttonCopy), "clicked", G_CALLBACK (connexion), (gpointer) win);
    gtk_box_pack_start (GTK_BOX (boxAction), buttonCopy, FALSE, FALSE, 0);

    /* Bouton Renommer */
    buttonPaste = gtk_button_new_with_label("Coller");
//    g_signal_connect (G_OBJECT (buttonPaste), "clicked", G_CALLBACK (connexion), (gpointer) win);
    gtk_box_pack_start (GTK_BOX (boxAction), buttonPaste, FALSE, FALSE, 0);

    /* Bouton Renommer */
    buttonAddFolder = gtk_button_new_with_label("Ajouter dossier");
//    g_signal_connect (G_OBJECT (buttonAddFolder), "clicked", G_CALLBACK (connexion), (gpointer) win);
    gtk_box_pack_start (GTK_BOX (boxAction), buttonAddFolder, FALSE, FALSE, 0);

    /* Bouton Renommer */
    buttonRemoveFolder = gtk_button_new_with_label("Supprimer dossier");
//    g_signal_connect (G_OBJECT (buttonRemoveFolder), "clicked", G_CALLBACK (connexion), (gpointer) win);
    gtk_box_pack_start (GTK_BOX (boxAction), buttonRemoveFolder, FALSE, FALSE, 0);

    /* Bouton Renommer */
    buttonLoad = gtk_button_new_with_label("Charger fichier");
//    g_signal_connect (G_OBJECT (buttonLoad), "clicked", G_CALLBACK (connexion), (gpointer) win);
    gtk_box_pack_start (GTK_BOX (boxAction), buttonLoad, FALSE, FALSE, 0);

    /* Bouton Telecharger */
    buttonDownload = gtk_button_new_with_label("Télécharger");
//    g_signal_connect (G_OBJECT (buttonDownload), "clicked", G_CALLBACK (connexion), (gpointer) win);
    gtk_box_pack_start (GTK_BOX (boxAction), buttonDownload, FALSE, FALSE, 0);

    /* Bouton Annuler */
    buttonCancel = gtk_button_new_with_label("Annuler");
//    g_signal_connect (G_OBJECT (buttonCancel), "clicked", G_CALLBACK (connexion), (gpointer) win);
    gtk_box_pack_start (GTK_BOX (boxAction), buttonCancel, FALSE, FALSE, 0);



/** BOITE TREEVIEWS+BUTTONS **/
    boxFile = gtk_box_new(TRUE,6);
    gtk_box_pack_start(GTK_BOX(boxFile),panedTreeView,TRUE,TRUE,0);
    gtk_box_pack_end(GTK_BOX(boxFile), boxAction, FALSE, TRUE, 1);


/** PANED (CONSOLE/TREEVIEWS) **/
    /* Paned avec Console et TreeView */
    paned = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
//    gtk_widget_set_size_request(paned,200,-1);
    gtk_paned_set_position(GTK_PANED(paned),100);
    gtk_paned_pack1(GTK_PANED(paned),scrolledWindowConsole,TRUE,FALSE);
    gtk_paned_pack2(GTK_PANED(paned),boxFile,TRUE,FALSE);


/** CONTAINER **/
    /* Boite Conteneur */
    box = gtk_box_new (TRUE, 6);

    //Ajoute les éléments à la boite conteneur
    gtk_box_pack_start(GTK_BOX(box),boxConnect,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),paned,TRUE,TRUE,0);
    gtk_box_pack_end(GTK_BOX(box), statusbar, FALSE, TRUE, 1);

    gtk_spinner_stop(GTK_SPINNER(spinner));

    gtk_container_add (GTK_CONTAINER (win), box);
    gtk_widget_show_all (win);
    gtk_main ();

    return 0;
}
