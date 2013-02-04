#include <glib/gerror.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <string.h>
#include "client.c"


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

//Images
const char *pathFolder = "./../asset/directory.png";
const char *pathFile = "./../asset/file.png";

//Liste colonne
enum GColumns{
    COL_TYPE = 0,
    COL_NAME,
    COL_SIZE,
    //COL_LAST_UPDATE,
    NUM_COLS
};

static void connexion (GtkWidget *wid, GtkWidget *win);

void setStatusBar(const char *newText);
void on_changed(GtkWidget *widget, gpointer statusbar);
void insertConsole(char *newText);

GtkWidget *create_view (int isLocal);
void create_model (GtkListStore *store, char *dir_name);
void cb_select (GtkTreeView *tree_view, GtkTreePath *arg1, GtkTreeViewColumn *arg2, gpointer user_data);











