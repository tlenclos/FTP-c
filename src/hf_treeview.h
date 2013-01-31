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


#ifndef H_HF_TREEVIEW_060720051716
#define H_HF_TREEVIEW_060720051716


#include <gtk/gtk.h>


/* macros =================================================================== */
/* constants ================================================================ */

/* Constantes d'erreurs de la classe. */
typedef enum
{
   /* Pas d'erreur. */
   HF_TW_NO_ERROR,

   /* L'icone specifiee n'a pas pu etre chargee. */
   HF_TW_ICON_NOT_LOADED,

   /* La suppression d'un item n'a pas pu se faire
      car aucun item n'a ete selectionne. */
   HF_TW_NO_ITEM_SELECTED,

   /* L'element selectionne ne permet pas l'ajout d'un nouvel element
      Il est de fait qu'il faut selectionner un dossier ou une categorie
      principale pour l'ajout. CELA PEUT ENCORE EVOLUER ! */
   HF_TW_INVALID_ITEM,


   /* GtkTreeIter invalide ou pas de GtkTreeIter fourni. */
   HF_TW_INVALID_ITER,


   HF_TW_NB_ERROR
}
e_Treeview_error;


/* Constantes de type des elements a inserer.
   Se sont essentiellement des constantes a but descriptif. */
typedef enum
{
   /*
      Types destines essentiellement pour des elements en relations
      directe avec les fichiers et dossiers du disque dur.
   */

   /* Item de type 'fichier'. */
   HF_TW_FILE_ITEM,

   /* Item de type 'dossier'. */
   HF_TW_DIRECTORY_ITEM,


   /*
      Types (completement abstraits) qui peuvent etre utilises pour definir
      des categorie et sous-categorie pour une liste d'options
      de preferences par exemple.
   */

   /* Item d'une categorie principale. */
   HF_TW_CATEGORY_ITEM,

   /* Item d'une sous-categorie. */
   HF_TW_SUB_CATEGORY_ITEM,


   /*
      Constantes utilisateur, dans le cas où celles
      du dessus ne conviendraient pas.
   */
   HF_TW_USER_TYPE_1,
   HF_TW_USER_TYPE_2,
   HF_TW_USER_TYPE_3,


   HF_TW_NB_ITEM_TYPE
}
e_TW_item_type;


/* types ==================================================================== */
/* structures =============================================================== */

/* Structure du widget complet avec barres de defilement incluses. */
typedef struct t_Tree_view
{
   /* ----- Composant du widget complet. */

   /* Barre de defilement pour le widget (GtkScrolledWindow).
      C'est ce widget qu'il faut ajouter dans un container
      car c'est lui qui contient le widget GtkTreeView,
      d'ou le choix du nom 'Main_container'. */
   GtkWidget *Main_container;

   /* Magasin renfermant les donnees affichees
      dans le widget GtkTreeView. */
   GtkTreeStore *Store;

   /* Formatteur de cellule. */
   GtkCellRenderer *Renderer;

   /* Colonne dans laquelle seront affichees les donnees. */
   GtkTreeViewColumn *Column;

   /* Objet de selection. */
   GtkTreeSelection *Selection_type;

   /* Le widget qui affichera les donnees finales
      grace aux objets ci-dessus. */
   GtkWidget *Tree_view;

   /* ----- */


   /* ----- Destructeur */

   void (*Destroy)(struct t_Tree_view **self);

   /* ----- */


   /* ----- Methodes publiques. */

   /* Methode de recuperation du type de l'item. */
   e_TW_item_type (*Get_selected_item_type)(struct t_Tree_view *self);

   /* Methode d'ajout d'item.
      PREVUE SURTOUT POUR ETRE UTILISEE PAR LE PROGRAMME ! */
   GtkTreeIter (*Add_item)(struct t_Tree_view *self,
                           const gchar *s_Item_icon,
                           const gchar *s_Item_text,
                           e_TW_item_type Item_type,
                           gboolean Is_child,
                           GtkTreeIter *p_Parent_iter);

   /* Methode d'ajout d'item dans la plus haute partie de la
      hierarchie des items. */
   GtkTreeIter (*Add_item_as_root)(struct t_Tree_view *self,
                                   const gchar *s_Item_icon,
                                   const gchar *s_Item_text,
                                   e_TW_item_type Item_type);

   /* Methode d'ajout d'item par rapport a l'element
      selectionne dans le GtkTreeView. */
   gint (*Add_item_by_selection)(struct t_Tree_view *self,
                                 const gchar *s_Item_icon,
                                 const gchar *s_Item_text,
                                 e_TW_item_type Item_type);

   /* Methode de suppression d'item. Ne supprime qu'un item selectionne. */
   gint (*Remove_item)(struct t_Tree_view *self);

   /* Methode de vidage complet du widget. */
   void (*Clear_all)(struct t_Tree_view *self);

   /* Methode de lecture de l'erreur en cours. */
   e_Treeview_error (*Read_error)(struct t_Tree_view *self);

   /* ----- */


   /* ----- Proprietes privees */

   /*
      ATTENTION !! NE JAMAIS CHANGER LA VALEUR DE CES PROPRIETES SINON LE
      COMPORTEMENT DES METHODES SERA INDETERMINE !!
   */

   /* Permet de determiner si une colonne pour icones est en place.
      Ceci est gere d'apres la valeur de l'argument 'b_With_icons' (de la
      fonction d'instanciation) fournie par l'utilisateur. */
   gboolean b_Icons;

   /* Variable contenant la derniere erreur qui s'est produite.
      La valeur est a lire avec la methode 'Read_error' de l'instance de la
      classe en cours d'utilisation. */
   e_Treeview_error Treeview_error;

   /* Index des colonnes. Ceci est gere d'apres la valeur de l'argument
      'b_With_icons' (de la fonction d'instanciation) fournie par
      l'utilisateur. Si la valeur de 'i_Icon_column_index' est de -1, cela
      indique alors que la colonne d'icone a ete desactivee lors de
      l'instanciation de la classe. */
   gint i_Icon_column_index;
   gint i_Text_column_index;
   gint i_Type_column_index;

   /* ----- */


   /* ----- Proprietes publiques */

   /* Rend l'en-tete de la colonne visible ou non. */
   void (*Show_column_header)(struct t_Tree_view *self, gboolean b_Visible);

   /* Change le texte dans l'en-tete de la colonne. */
   void (*Change_column_title)(struct t_Tree_view *self, const gchar *s_Title);

   /* Affiche tous les items contenus dans le GtkTreeView. */
   void (*Expand_all_items)(struct t_Tree_view *self);

   /* Cache tous les items contenus dans le GtkTreeView sauf
      ceux du plus haut niveau. */
   void (*Collapse_all_items)(struct t_Tree_view *self);

   /* Affiche les marques suivies des lignes. */
   void (*Show_lines_marks)(struct t_Tree_view *self, gboolean b_Visible);

   /* Rend les cellule editables depuis l'afficheur par l'utilisateur. */
   void (*Set_cell_editable)(struct t_Tree_view *self, gboolean b_Editable);

   /* ----- */
}
t_Tree_view;


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
      sinon NULL en cas d'erreur.
   -------------------------------------------------------------------------- */
t_Tree_view* New_treeview (gboolean b_With_icons,
                            gboolean b_Show_column_header,
                            const gchar *s_Column_title);


#endif /* H_HF_TREEVIEW_060720051716 */
