/*****************************************************************************
 *   Copyright (C) 2005 by HECHT Franck                                      *
 *   franck.hecht@planete-c.com                                              *
 *                                                                           *
 *   self program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   self program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with self program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 *****************************************************************************/

/* --------------------------------------------------------------------------

                     Gestion de la classe: t_Tree_view
                                 Version 1.0

   --------------------------------------------------------------------------
   Module   :  hf_treeview
   Fichier  :  hf_treeview.c

   Creation :  06/07/2005 - 17:16

   But      :  Met en place un widget GTK 'GtkTreeView' au complet pour
               obtenir une zone de vue hierarchique avec ou sans icones.
   --------------------------------------------------------------------------
   Dernières modifications:
   -------------------------------------------------------------------------- */


#include "hf_treeview.h"


/* macros =================================================================== */
/* constants ================================================================ */
/* types ==================================================================== */
/* structures =============================================================== */
/* private variables ======================================================== */
/* private functions ======================================================== */

/* --------------------------------------------------------------------------
   Destroy_tw
   --------------------------------------------------------------------------
   Suppression du widget complet avec liberation de la memoire.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.
   S:
   -------------------------------------------------------------------------- */
static void Destroy_tw (struct t_Tree_view **self)
{
   g_free (*self);
   *self = (t_Tree_view *) NULL;
}


/* --------------------------------------------------------------------------
   Is_item_selected
   --------------------------------------------------------------------------
   Determine si un item est selectionne dans le GtkTreeView.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.
   S: TRUE si un item est selectionne sinon FALSE.
   -------------------------------------------------------------------------- */
static gboolean Is_item_selected (struct t_Tree_view *self)
{
   gboolean ret = gtk_tree_selection_get_selected (
                     self->Selection_type,
                     NULL,
                     NULL);

   return ret;
}


/* --------------------------------------------------------------------------
   Get_tw_selected_item_iter
   --------------------------------------------------------------------------
   Determine si un item est selectionne dans le GtkTreeView.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.
   S: Retourne un GtkTreeIter.
   -------------------------------------------------------------------------- */
static GtkTreeIter Get_tw_selected_item_iter (struct t_Tree_view *self)
{
   GtkTreeModel *p_Model = NULL;
   GtkTreeIter Iter;


   if ((Is_item_selected (self)) == TRUE)
   {
      /* Recherche de l'item selectionne et recuperation de son GtkTreeIter. */
      gtk_tree_selection_get_selected (self->Selection_type,
                                       &p_Model,
                                       &Iter);
   }


   return Iter;
}


/* --------------------------------------------------------------------------
   Get_tw_selected_item_type
   --------------------------------------------------------------------------
   Recupere le type de l'element selectionne.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.

   S: Constante de type 'e_TW_item_type' correspondant au style de
      l'element selectionne.

      ATTENTION:
      Cette fonction ne retourne pas un resultat d'erreur, il faut donc
      apres utilisation verifier la valeur de la propriete 'Treeview_error'.
      L'erreur la plus probable peut venir du fait qu'aucun element soit
      selectionne.
   -------------------------------------------------------------------------- */
e_TW_item_type Get_tw_selected_item_type (struct t_Tree_view *self)
{
   GtkTreeModel *p_Model = NULL;
   GtkTreeIter Iter;
   GdkPixbuf *p_Icon = NULL;
   gchar *p_Title = NULL;
   gint i_Item_type;
   e_TW_item_type Item_type;


   if ((Is_item_selected (self)) == TRUE)
   {
      /* Recherche de l'item selectionne et remplissage du model
         et du iter correspondant. */
      gtk_tree_selection_get_selected (self->Selection_type,
                                       &p_Model,
                                       &Iter);


      /* On recupere les donnees de l'element. */
      if (self->b_Icons == TRUE)
      {
         gtk_tree_model_get (
            GTK_TREE_MODEL (p_Model),
            &Iter,
            self->i_Icon_column_index, &p_Icon,
            self->i_Text_column_index, &p_Title,
            self->i_Type_column_index, &i_Item_type,
            -1);

         g_object_unref (p_Icon);
      }
      else
      {
         gtk_tree_model_get (
            GTK_TREE_MODEL (p_Model),
            &Iter,
            self->i_Text_column_index, &p_Title,
            self->i_Type_column_index, &i_Item_type,
            -1);
      }

      /* Affectation de la valeur du type. */
      Item_type = i_Item_type;


      /* Liberation de memoire. */
      g_free (p_Title);
   }
   else
   {
      /* Aucun element de selectionne. */
      self->Treeview_error = HF_TW_NO_ITEM_SELECTED;
   }


   return Item_type;
}


/* --------------------------------------------------------------------------
   Add_tw_item
   --------------------------------------------------------------------------
   Ajout d'un elements dans le vue hierarchique. C'est l'utilisateur lui meme
   qui doit definir si l'element en cours d'ajout doit posseder un lien de
   parente avec un autre element.
   Cette fonction est surtout utilisee par le programme lui meme pour l'ajout
   de nouveaux elements au chargement d'un projet pour construire la
   hierarchie telles qu'elle est definie par les dossiers sur le disque dur.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.
   E: Icone du nouvel element, peut etre aussi NULL si aucune icone
      doit etre inseree.
   E: Texte du nouvel element.
   E: Type de l'element a inserer.
   E: TRUE  =  Ajoute un element enfant,
      FALSE =  Ajoute un element au sommet de la hierarchie.
   E: GtkTreeIter contenant la position de l'elememnt parant pour l'insertion
      d'un element enfant. Si l'argument Is_child vaut FALSE alors l'argument
      peut etre NULL, il sera de toutes manieres pas pris en compte.
      Si Is_child vaut TRUE et l'argument est a NULL alors la variable d'erreur
      de t_Tree_view contiendra une valeur d'erreur.

   S: GtkTreeIter du dernier parent ajoute.

      ATTENTION:
      Cette fonction ne retourne pas un resultat d'erreur, il faut donc
      apres utilisation verifier la valeur de la propriete 'Treeview_error'.
   -------------------------------------------------------------------------- */
static GtkTreeIter Add_tw_item (struct t_Tree_view *self,
                                const gchar *s_Item_icon,
                                const gchar *s_Item_text,
                                e_TW_item_type Item_type,
                                gboolean Is_child,
                                GtkTreeIter *p_Parent_iter)
{
   GtkTreeIter Iter;
   GdkPixbuf *Icon = NULL;
   GError *Error = NULL;
   gint ret_val = HF_TW_NO_ERROR;
   gint cont_val = 0;


   /* Chargement de l'icone si la propriete 'b_Icons'
      de la classe a sur TRUE. */
   if (self->b_Icons == TRUE)
   {
      Icon = gdk_pixbuf_new_from_file (s_Item_icon, &Error);

      if (Error)
      {
         g_error_free (Error);
         Error = NULL;

         self->Treeview_error = HF_TW_ICON_NOT_LOADED;
      }

      cont_val = 1;
   }


   /* On verifie la validite du GtkTreeIter parent et
      s'il doit etre utilise. */
   if (cont_val == 1)
   {
      if (Is_child == TRUE && p_Parent_iter != NULL)
      {
         /* On verifie la validite de p_Parent_iter. */


         /* Aucune erreur alors on continu. */
         cont_val = 2;
      }
      else if (Is_child == TRUE && p_Parent_iter == NULL)
      {
         ret_val = HF_TW_INVALID_ITER;
         cont_val = 0;
      }
   }


   /* On determine si l'element a ajouter est un element enfant
      ou bien s'il faut le placer au sommet de la hierarchie. */
   if (cont_val == 1 || cont_val == 2)
   {
      if (Is_child == FALSE)
      {
         /* Creation de l'element en tant que root. */
         gtk_tree_store_append (
            self->Store,
            &Iter,
            NULL);
      }
      else
      {
         /* Creation de l'element en tant qu'enfant. */
         gtk_tree_store_append (
            self->Store,
            &Iter,
            p_Parent_iter);
      }


      /* Insertion du nouvel element dans le GtkTreeView. */
      if (self->b_Icons == TRUE)
      {
         gtk_tree_store_set (
            self->Store,
            &Iter,
            self->i_Icon_column_index, Icon,
            self->i_Text_column_index, s_Item_text,
            self->i_Type_column_index, Item_type,
            -1);

         /* On efface la reference de l'objet Icon provisoire. */
         g_object_unref (Icon);
      }
      else
      {
         gtk_tree_store_set (
            self->Store,
            &Iter,
            self->i_Text_column_index, s_Item_text,
            self->i_Type_column_index, Item_type,
            -1);
      }
   }


   return Iter;
}

/* --------------------------------------------------------------------------
   Add_tw_item_as_root
   --------------------------------------------------------------------------
   Methode d'ajout d'item dans la plus haute partie de la
   hierarchie des items.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.
   E: Icone du nouvel element, peut etre aussi NULL si aucune icone
      doit etre inseree.
   E: Texte du nouvel element.
   E: Type de l'element a inserer.

   S: GtkTreeIter du dernier parent ajoute.

      ATTENTION:
      Cette fonction ne retourne pas un resultat d'erreur, il faut donc
      apres utilisation verifier la valeur de la propriete 'Treeview_error'.
   -------------------------------------------------------------------------- */
GtkTreeIter Add_tw_item_as_root (struct t_Tree_view *self,
                                 const gchar *s_Item_icon,
                                 const gchar *s_Item_text,
                                 e_TW_item_type Item_type)
{
   GtkTreeIter Iter;


   /* Appel de la fonction standard d'ajout d'item. */
   Iter = Add_tw_item (self,
                       s_Item_icon,
                       s_Item_text,
                       Item_type,
                       FALSE,
                       NULL);


   return Iter;
}


/* --------------------------------------------------------------------------
   Add_tw_item_by_selection
   --------------------------------------------------------------------------
   Ajout d'un elements dans le vue hierarchique. C'est l'utilisateur lui meme
   qui doit definir si l'element en cours d'ajout doit posseder un lien de
   parente avec un autre element.
   Cette fonction sera celle le plus utilisee (par rapport a sa soeur jumelle
   'Add_tw_item') par l'utilisateur car elle permet l'ajout d'element suivant
   celui qui a ete selectionne.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.
   E: Icone du nouvel element, peut etre aussi NULL si aucune icone
      doit etre inseree.
   E: Texte du nouvel element.
   E: Type de l'element a inserer.

   S: Constantes d'erreur de type 'e_Treeview_Error'.
   -------------------------------------------------------------------------- */
static gint Add_tw_item_by_selection (struct t_Tree_view *self,
                                      const gchar *s_Item_icon,
                                      const gchar *s_Item_text,
                                      e_TW_item_type Item_type)
{
   GtkTreeIter Iter;
   GtkTreeIter Parent_iter;
   GdkPixbuf *Icon = NULL;
   GError *Error = NULL;
   gint ret_val = HF_TW_NO_ERROR;


   /* Chargement de l'icone si la propriete 'b_Icons'
      de la classe a sur TRUE. */
   if (self->b_Icons == TRUE)
   {
      Icon = gdk_pixbuf_new_from_file (s_Item_icon, &Error);

      if (Error)
      {
         g_error_free (Error);
         Error = NULL;

         self->Treeview_error = HF_TW_ICON_NOT_LOADED;

         return HF_TW_ICON_NOT_LOADED;
      }
   }


   /* On recupere le iter parent et le iter courant,
      si un item est selectionne. */
   if ((Is_item_selected (self)) == TRUE)
   {
      if ((self->Get_selected_item_type (self)) == HF_TW_DIRECTORY_ITEM ||
          (self->Get_selected_item_type (self)) == HF_TW_CATEGORY_ITEM)
      {
         /* Recuperation du GtkTreeIter de l'element selectionne. */
         Parent_iter = Get_tw_selected_item_iter (self);

         /* Creation d'un nouvel item dans le magasin. */
         gtk_tree_store_insert_after (GTK_TREE_STORE (self->Store),
                                       &Iter,
                                       &Parent_iter,
                                       NULL);


         /* Definition du nouvel item. */
         if (self->b_Icons == TRUE)
         {
            gtk_tree_store_set (
               self->Store,
               &Iter,
               self->i_Icon_column_index, Icon,
               self->i_Text_column_index, s_Item_text,
               self->i_Type_column_index, Item_type,
               -1);


            /* On efface la reference de l'objet Icon provisoire. */
            g_object_unref (Icon);
         }
         else
         {
            gtk_tree_store_set (
               self->Store,
               &Iter,
               self->i_Text_column_index, s_Item_text,
               self->i_Type_column_index, Item_type,
               -1);
         }
      }
      else
      {
         ret_val = HF_TW_INVALID_ITEM;
      }
   }
   else
   {
      ret_val = HF_TW_NO_ITEM_SELECTED;
   }


   return ret_val;
}


/* --------------------------------------------------------------------------
   Remove_tw_item
   --------------------------------------------------------------------------
   Supprime un element selectionne dans le GtkTreeView. Si l'element en
   cours de suppression possede des noeuds enfants, ils seront eux aussi
   supprimes.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.

   S: Constantes d'erreur de type 'e_Treeview_Error'.
      (en generale l'erreur provient du fait qu'aucun item soit selectionne.).
   -------------------------------------------------------------------------- */
static gint Remove_tw_item (struct t_Tree_view *self)
{
   GtkTreeModel *p_Model = NULL;
   GtkTreeIter Iter;
   gint Ret = HF_TW_NO_ERROR;


   if ((Is_item_selected (self)) == TRUE)
   {
      /* Recherche de l'item selectionne et remplissage du model
         et du iter correspondant. */
      gtk_tree_selection_get_selected (self->Selection_type,
                                       &p_Model,
                                       &Iter);

      /* Suppression de l'item selectionne, les fils
         sont automatiquement supprimes. */
      gtk_tree_store_remove (self->Store, &Iter);
   }
   else
   {
      /* Aucun item de selectionne. */
      Ret = HF_TW_NO_ITEM_SELECTED;
   }


   return Ret;
}


/* --------------------------------------------------------------------------
   Clear_tw
   --------------------------------------------------------------------------
   Vide completement le GtkTreeView.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.

   S:
   -------------------------------------------------------------------------- */
static void Clear_tw (struct t_Tree_view *self)
{
   /* Vidage complet de la colonne ainsi que du renderer associe. */
   gtk_tree_store_clear (GTK_TREE_STORE (self->Store));
}


/* --------------------------------------------------------------------------
   Read_tw_error
   --------------------------------------------------------------------------
   Methode de lecture de l'erreur en cours.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.

   S: Erreur de type 'e_Treeview_error'
   -------------------------------------------------------------------------- */
static e_Treeview_error Read_tw_error (struct t_Tree_view *self)
{
   return self->Treeview_error;
}


/* --------------------------------------------------------------------------
   Show_tw_column_header
   --------------------------------------------------------------------------
   Rend l'en-tete de la colonne visibie ou non.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.
   E: TRUE  =  En-tete de colonne visible,
      FALSE =  En-tete de colonne cachee.

   S:
   -------------------------------------------------------------------------- */
static void Show_tw_column_header (struct t_Tree_view *self,
                                   gboolean b_Visible)
{
   if ((gtk_tree_view_get_headers_visible (
         GTK_TREE_VIEW (self->Tree_view))) != b_Visible)
   {
      gtk_tree_view_set_headers_visible (
         GTK_TREE_VIEW (self->Tree_view),
         b_Visible);
   }
}


/* --------------------------------------------------------------------------
   Change_tw_column_title
   --------------------------------------------------------------------------
   Change le texte dans l'en-tete de la colonne.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.

   S:
   -------------------------------------------------------------------------- */
static void Change_tw_column_title (struct t_Tree_view *self,
                                    const gchar *s_Title)
{
   gtk_tree_view_column_set_title (
      GTK_TREE_VIEW_COLUMN (self->Column),
      s_Title);
}


/* --------------------------------------------------------------------------
   Expand_tw_all_items
   --------------------------------------------------------------------------
   Affiche tous les items contenus dans le GtkTreeView.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.

   S:
   -------------------------------------------------------------------------- */
static void Expand_tw_all_items (struct t_Tree_view *self)
{
   gtk_tree_view_expand_all (GTK_TREE_VIEW (self->Tree_view));
}


/* --------------------------------------------------------------------------
   Change_tw_column_title
   --------------------------------------------------------------------------
   Cache tous les items contenus dans le GtkTreeView sauf
   ceux du plus haut niveau.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.

   S:
   -------------------------------------------------------------------------- */
static void Collapse_tw_all_items (struct t_Tree_view *self)
{
   gtk_tree_view_collapse_all (GTK_TREE_VIEW (self->Tree_view));
}


/* --------------------------------------------------------------------------
   Show_tw_lines_marks
   --------------------------------------------------------------------------
   Affiche les marques suivi des lignes.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.
   E: TRUE  =  Marques visibles,
      FALSE =  Marques cachees.

   S:
   -------------------------------------------------------------------------- */
static void Show_tw_lines_marks (struct t_Tree_view *self,
                                 gboolean b_Visible)
{
   if ((gtk_tree_view_get_rules_hint (
         GTK_TREE_VIEW (self->Tree_view))) != b_Visible)
   {
      gtk_tree_view_set_rules_hint (
         GTK_TREE_VIEW (self->Tree_view),
         b_Visible);
   }
}


/* --------------------------------------------------------------------------
   Set_tw_cell_editable
   --------------------------------------------------------------------------
   Rend les cellule editables depuis l'afficheur par l'utilisateur.
   --------------------------------------------------------------------------
   E: Pointeur vers l'instance de la classe en cours d'utilisation.
   E: TRUE  =  Cellules editables,
      FALSE =  Cellules non-editables.

   S:
   -------------------------------------------------------------------------- */
void Set_tw_cell_editable (struct t_Tree_view *self, gboolean b_Editable)
{
   g_object_set (self->Renderer, "editable", b_Editable, NULL);
}


/* internal public functions ================================================ */

/* --------------------------------------------------------------------------
   New_treeview
   --------------------------------------------------------------------------
   Creation d'une instance de la classe 't_Tree_view' ce qui engendre en
   meme temps la creation d'un nouveau widget GtkTreeView au complet.
   --------------------------------------------------------------------------
   E: TRUE  = Prevoit une colonne pour afficher une icone avant le texte,
      FALSE = Sans colonne pour les icones, texte seulement.
   E: TRUE  =  En-tete de la colonne visible (pour mettre un titre),
      FALSE =  En-tete de la colonne non visible.
   E: Chaine de caracteres pour le titre de l'en-tete de la colonne.
      Si aucun titre ne doit etre mis, mettre l'argument à NULL.

   S: Renvoie un pointeur vers une nouvelle instance de la classe 't_Tree_view'
      sinon NULL en cas d'erreurs.
   -------------------------------------------------------------------------- */
t_Tree_view* New_treeview (gboolean b_With_icons,
                            gboolean b_Show_column_header,
                            const gchar *s_Column_title)
{
   /* Allocation dynamique d'un pointeur vers la structure du widget. */
   t_Tree_view *self = (t_Tree_view *) g_malloc (sizeof (t_Tree_view));


   if (self == (t_Tree_view *) NULL)
   {
      return ((t_Tree_view *) NULL);
   }


   /* On definit les index de colonnes. */
   if (b_With_icons == TRUE)
   {
      self->i_Icon_column_index = 0;
      self->i_Text_column_index = 1;
      self->i_Type_column_index = 2;
   }
   else
   {
      self->i_Icon_column_index = -1; /* -1 car non utilise dans ce cas. */
      self->i_Text_column_index = 0;
      self->i_Type_column_index = 1;
   }


   /* Creation du magasin qui contiendra les donnees du widget. */
   if (b_With_icons == TRUE)
   {
      self->Store = gtk_tree_store_new (3,
                                        GDK_TYPE_PIXBUF,
                                        G_TYPE_STRING,
                                        G_TYPE_INT);
   }
   else
   {
      self->Store = gtk_tree_store_new (2, G_TYPE_STRING, G_TYPE_INT);
   }


   /* ----- Creation du widget GtkTreeView. */

   self->Tree_view = gtk_tree_view_new_with_model (
      GTK_TREE_MODEL (self->Store));

   /* Creation d'une nouvelle colonne. */
   self->Column = gtk_tree_view_column_new ();

   /* ----- */


   /* ----- Mise en place de la colonne en mode image+texte. */

   /* Colonne d'image. */
   if (b_With_icons == TRUE)
   {
      self->Renderer = gtk_cell_renderer_pixbuf_new ();
      gtk_tree_view_column_pack_start (
         self->Column,
         self->Renderer,
         FALSE);

      gtk_tree_view_column_set_attributes (
         self->Column,
         self->Renderer,
         "pixbuf",
         self->i_Icon_column_index,
         NULL);
   }

   /* Colonne de texte. */
   self->Renderer = gtk_cell_renderer_text_new ();
   gtk_tree_view_column_pack_start (
      self->Column,
      self->Renderer,
      TRUE);

   gtk_tree_view_column_set_attributes (
      self->Column,
      self->Renderer,
      "text",
      self->i_Text_column_index,
      NULL);

   /* ----- */


   /* Ajout de la colonne dans le widget. */
   gtk_tree_view_append_column (
      GTK_TREE_VIEW (self->Tree_view),
      self->Column);


   /* On cache l'en-tete de la colonne si l'utilisateur a mis l'argument
      'Show_column_header' à FALSE. */
   if (b_Show_column_header == FALSE)
   {
      gtk_tree_view_set_headers_visible (
         GTK_TREE_VIEW (self->Tree_view),
         FALSE);
   }

   /* Mise en place du titre de la colonne si une chaine de
      caracteres a ete fournie en argument. */
   if (s_Column_title != NULL)
   {
      gtk_tree_view_column_set_title (
         GTK_TREE_VIEW_COLUMN (self->Column),
         s_Column_title);
   }


   /* ----- Creation de barre de defilement. */

   self->Main_container = gtk_scrolled_window_new (NULL, NULL);

   /* Ajout du widget dans le widget GtkScrolledWindow. */
   gtk_scrolled_window_add_with_viewport (
      GTK_SCROLLED_WINDOW (self->Main_container),
      self->Tree_view);

   /* Reglages des barres de defilement. */
   gtk_scrolled_window_set_policy (
      GTK_SCROLLED_WINDOW (self->Main_container),
      GTK_POLICY_AUTOMATIC,
      GTK_POLICY_AUTOMATIC);

   /* ----- */


   /* ----- Affectation du style de selection pour le widget. */

   /* Recuperation de l'objet de selection du widget. */
   self->Selection_type = gtk_tree_view_get_selection (
      GTK_TREE_VIEW (self->Tree_view));

   /* Attributs de la selection: Seulement 1 item peut etre selectionne. */
   gtk_tree_selection_set_mode (self->Selection_type, GTK_SELECTION_BROWSE);

   /* ----- */


   /* ----- Affectation des methodes de la classe. */

   self->Destroy = Destroy_tw;
   self->Get_selected_item_type = Get_tw_selected_item_type;
   self->Add_item = Add_tw_item;
   self->Add_item_as_root = Add_tw_item_as_root;
   self->Add_item_by_selection = Add_tw_item_by_selection;
   self->Remove_item = Remove_tw_item;
   self->Clear_all = Clear_tw;
   self->Read_error = Read_tw_error;

   /* ----- */


   /* ----- Affectation des proprietes privees. */

   self->b_Icons = b_With_icons;
   self->Treeview_error = HF_TW_NO_ERROR;

   /* ----- */

   /* ----- */


   /* ----- Affectation des methodes pour les proprietes publiques. */

   self->Show_column_header = Show_tw_column_header;
   self->Change_column_title = Change_tw_column_title;
   self->Expand_all_items = Expand_tw_all_items;
   self->Collapse_all_items = Collapse_tw_all_items;
   self->Show_lines_marks = Show_tw_lines_marks;
   self->Set_cell_editable = Set_tw_cell_editable;

   /* ----- */


   return self;
}
