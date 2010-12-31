
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>
#include <gio/gio.h>

#include <gtk/gtk.h>

#include "database.h"
#include "drawing.h"

static gboolean changed=FALSE;

static void quit_app(GtkWidget *item, GtkWidget * mainwindow);
static void Themanager_show_about_menu_callback(GtkWidget * item , gpointer callback_data);
static void themanager_savedata_cb(GtkWidget * item , GtkWidget* mainwindow);
static void themanager_search_cb(GtkWidget * item , GtkWidget* mainwindow);

static GtkDialog * gtk_input_box(GtkWidget * parent , gchar * title , const gchar ** out)
{
	GtkDialog * dialog = (GtkDialog*)gtk_dialog_new_with_buttons(title,NULL,GTK_DIALOG_MODAL,"OK","Cancel",NULL);
	GtkWidget * vbox = gtk_dialog_get_content_area(dialog);
	GtkWidget * entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(vbox),entry,1,1,1);
	gtk_widget_show(entry);

	if(*out)
		gtk_entry_set_text(GTK_ENTRY(entry),*out);

	*out = NULL;

	if (gtk_dialog_run(dialog) != GTK_RESPONSE_DELETE_EVENT)
	{
		GtkEntryBuffer * buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
		*out = gtk_entry_buffer_get_text(buffer);
	}
	return dialog;
}

static GtkDialog * gtk_inputs_box(GtkWindow * parent, gchar * title,...)
{
	GtkDialog * dialog = (GtkDialog*)gtk_dialog_new_with_buttons(title,parent,GTK_DIALOG_MODAL,"OK","Cancel",NULL);
	GtkWidget * vbox = gtk_dialog_get_content_area(dialog);

	struct Entrys{
		GtkWidget * entry;
		const char ** txt;
	};

	gchar*	label;
	const gchar **txt;
	GList *	entrys=NULL;

	va_list v;

	va_start(v,title);

	while (label = va_arg(v,gchar*))
	{
		txt = va_arg(v,const char**);
		GtkWidget * tlabel =gtk_label_new(label);

		GtkWidget * entry = gtk_text_view_new();


		if(*txt)
		{
			GtkTextBuffer * buffer = gtk_text_view_get_buffer((GtkTextView*)entry);
			gtk_text_buffer_set_text(buffer,*txt,-1);
			 *txt = NULL;
		}

		g_object_set(entry,"user-data",txt,NULL);

		GtkWidget * hbox = gtk_hbox_new(0,1);
		gtk_box_pack_start(GTK_BOX(hbox),tlabel,0,0,0);
		gtk_box_pack_end(GTK_BOX(hbox),entry,1,1,1);

		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);

		struct Entrys * pair = g_new0(struct Entrys,1);

		pair->entry = entry;
		pair->txt = txt;

		entrys = g_list_append(entrys,pair);
	}

	gtk_widget_show_all(GTK_WIDGET(dialog));

	if (gtk_dialog_run(dialog) != GTK_RESPONSE_DELETE_EVENT)
	{
		GList * p;

		for ( p = g_list_first(entrys); p; p = g_list_next(p))
		{
			struct Entrys * pair = p->data;

			GtkTextIter start[1],end[1];

			GtkTextBuffer * buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pair->entry));

			gtk_text_buffer_get_bounds(buffer,start,end);

			*(pair->txt) = gtk_text_buffer_get_text(buffer,start,end,FALSE);
		}

		g_list_foreach(entrys,(GFunc)g_free,NULL);
		g_list_free(entrys);
	}

	return dialog;
}

static void save_xml(GtkWidget *widget, DataBase * db)
{
	gchar * catalog;
	GtkTreeIter	iter[1];
	GtkTreeStore * treestore = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(widget)));

	if(changed)
	{
		GtkDialog * dialog = (GtkDialog*)gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"需要保存么?");

		if(gtk_dialog_run(dialog)==GTK_RESPONSE_YES);
			database_close(db);
		gtk_widget_destroy((GtkWidget*)dialog);
	}
}

static void on_realize(GtkWidget *widget, DataBase * db)
{
	xmlNodePtr categorys;
	GtkTreeIter		itr;
	GtkTreeStore * treestore = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(widget)));

	//读取数据库
	categorys = database_getrootnode(db)->children;

	while (categorys)
	{
		if (!strcasecmp(categorys->name, "catalogs"))
		{
			gtk_tree_store_append(treestore, &itr, NULL);
			gtk_tree_store_set(treestore, &itr, 0,xmlGetProp(categorys, "name"), 1, categorys, -1);

			if (xmlChildElementCount(categorys))
			{
				GtkTreeIter entryitr[1];
				//子节点
				xmlNodePtr entry = categorys->children;
				while (entry)
				{
					if (!strcasecmp(entry->name, "entry"))
					{
						gtk_tree_store_append(treestore, entryitr, &itr);
						gtk_tree_store_set(treestore, entryitr, 0, xmlGetProp(entry, "name"), 1 , entry , -1);
					}
					entry = entry->next;
				}
			}
		}
		categorys = categorys->next;
	}
}

static void GtkTreeSelectionForeachFunc_del_each(GtkTreeModel *model, GtkTreePath *path,GtkTreeIter *iter, gpointer data)
{
	xmlNodePtr xml;
	gtk_tree_model_get(model,iter,1,&xml,-1);
	xmlUnlinkNode(xml);
	xmlFreeNode(xml);
	gtk_tree_store_remove(GTK_TREE_STORE(model),iter);

	changed = 1;
}

static void themanager_new_item_cb(GtkWidget * item , GtkWidget* rarea)
{
	drawing_data*	data;

	xmlNodePtr	xmlnode;

	g_object_get(rarea,"user-data",&data,NULL);

	xmlnode = data->xml;

	if(xmlnode) //有xmlnode的才对
	{
		gchar * type=NULL,*content=NULL;
		GtkDialog * input = gtk_inputs_box(NULL,"新信息录入","类型",&type,"内容",&content,NULL);

		if(type && content)
		{
			xmlNewChild(xmlnode,NULL,type,content);

			g_free(type);
			g_free(content);

			changed = 1;
		}

		gtk_widget_destroy((GtkWidget*)input);

		gtk_widget_queue_draw(rarea);

		changed = 1;
	}
}

static void themanager_edit_item_cb(GtkWidget * item , GtkWidget* rarea)
{
	drawing_data*	data;

	xmlNodePtr	xmlnode;

	const gchar * type=NULL,*content=NULL;

	g_object_get(rarea,"user-data",&data,NULL);

	xmlnode = data->xml;

	if(xmlnode && data->selected ) //有xmlnode并且被选择的才对
	{
		if(data->selected_title)
		{
			content = xmlGetProp(data->selected,"name");

			GtkDialog * input = gtk_input_box(NULL,"修改标题",&content);

			if (content && strlen(content))
			{
				xmlSetProp(data->selected,"name",content);
				changed = 1;
			}

			gtk_widget_destroy((GtkWidget*)input);

		}else
		{
			type = data->selected->name;

			content = xmlNodeGetContent(data->selected);

			GtkDialog * input = gtk_inputs_box(NULL,"信息修改","类型",&type,"内容",&content,NULL);

			if(type && content)
			{
				xmlNodeSetName(data->selected,type);
				xmlNodeSetContent(data->selected,content);
				changed = 1;

				g_free((void*)type);
				g_free((void*)content);

			}
			gtk_widget_destroy((GtkWidget*)input);
		}

		gtk_widget_queue_draw(rarea);
	}
}

static void themanager_delete_item_cb(GtkWidget * item , GtkWidget* rarea)
{
	drawing_data *data;

	xmlNodePtr	xmlnode;

	g_object_get(rarea,"user-data",&data,NULL);

	xmlnode = data->xml;

	if(data->selected && ! data->selected_title)
	{
		xmlUnlinkNode(data->selected);
		xmlFreeNode(data->selected);
		data->selected = NULL;
		changed = 1;
	}
	gtk_widget_queue_draw(rarea);
}

static void themanager_delete_catlog_cb(GtkWidget * item , GtkWidget* ltree)
{
//	GtkTreeStore * tree = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(ltree)));

	GtkTreeSelection * selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltree));

	gtk_tree_selection_selected_foreach(selection,GtkTreeSelectionForeachFunc_del_each,NULL);
}

static void themanager_new_entry_cb(GtkWidget * item , GtkWidget* ltree)
{
	GtkTreeIter	iter,itr;
	GtkTreeModel	* model;
	xmlNodePtr		xmlnode;
	//找到选中的条目
	GtkTreeSelection * selection = gtk_tree_view_get_selection((GtkTreeView*)ltree);

	gtk_tree_selection_get_selected(selection,&model,&iter);
	gtk_tree_model_get(model,&iter,1,&xmlnode,-1);

	if(!strcasecmp(xmlnode->name,"entry"))
	{
		xmlnode = xmlnode->parent;
		gtk_tree_model_iter_parent(model,&itr,&iter);

		iter = itr;
	}

	if(!strcasecmp(xmlnode->name,"catalogs"))
	{
		const gchar * entryname=NULL;
		GtkDialog * dialog = gtk_input_box(NULL,"新建",&entryname);

		if(entryname && strlen(entryname))
		{
			gtk_tree_store_append(GTK_TREE_STORE(model), &itr, &iter);
			//加入新类别吧
			xmlNodePtr n = xmlNewChild(xmlnode, NULL, "entry", NULL);

			xmlNewProp(n, "name", entryname);

			gtk_tree_store_set(GTK_TREE_STORE(model), &itr, 0, entryname,1,n, -1);

			changed = 1;
		}
		gtk_widget_destroy((GtkWidget*) dialog);
	}
}

static void themanager_new_catlog_cb(GtkWidget * item , GtkWidget* ltree)
{
	DataBase 	* db;
	GtkTreeIter	itr;
	const gchar * catelog = NULL;

	GtkTreeStore * tree = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(ltree)));

	g_object_get(ltree,"user-data",&db,NULL);

	// ask 类型
	GtkDialog * dialog = gtk_input_box(NULL,"新建类别",&catelog);

	if (catelog && strlen(catelog) > 0)
	{
		gtk_tree_store_append(tree, &itr, NULL);
		//加入新类别吧
		xmlNodePtr n = xmlNewChild(database_getrootnode(db), NULL, "catalogs",NULL);
		xmlNewProp(n, "name", catelog);

		gtk_tree_store_set(tree, &itr, 0, catelog, 1, n , -1);

		changed = 1;
	}

	gtk_widget_destroy((GtkWidget*) dialog);
}

static void cursor_changed(GtkTreeView *tree_view, gpointer user_data)
{
	GtkTreeIter	iter;
	GtkTreeModel	* model;
	xmlNodePtr		xmlnode;
	GtkTreePath * path;
	GtkTreeViewColumn * column;
	//找到选中的条目
//	GtkTreeSelection * selection = gtk_tree_view_get_selection((GtkTreeView*)tree_view);

//	gtk_tree_selection_get_selected(selection,&model,&iter);

	model = gtk_tree_view_get_model(tree_view);

	gtk_tree_view_get_cursor(tree_view,&path,&column);

	///gtk_tree_path_

	if (path)
	{
		gtk_tree_model_get_iter(model,&iter, path);

		//开始绘制吧 ^_^
		gtk_tree_model_get(model, &iter, 1, &xmlnode, -1);

		if (!strcasecmp(xmlnode->name, "entry"))
		{
			drawing_data *data;
			g_object_get(user_data, "user-data", &data, NULL);
			data->xml = xmlnode;

			gtk_widget_queue_draw(user_data);
		}
	}
}

static gboolean ltree_mouse_down(GtkWidget *widget, GdkEventButton *event,gpointer user_data)
{
	if(event->button == 3)
	{
		//判断所在位置
		gtk_menu_popup(user_data,NULL,NULL,NULL,NULL,event->button,event->time);
	}
	return FALSE;
}

static gboolean rarea_mouse_down(GtkWidget *widget, GdkEventButton *event,gpointer user_data)
{
	drawing_data *data;

	xmlNodePtr xml;

	g_object_get(widget,"user-data",&data,NULL);

	xml = data->xml;

	data->x = event->x;
	data->y = event->y;

	data->button = event->button;

	gtk_widget_queue_draw(widget);

	if(!xml)
		return FALSE;

	//判断所在位置进行选中

	if(event->button == 3)
	{
//		drawing_card(widget,NULL,NULL);
//		if(data->selected)
		gtk_menu_popup(user_data,NULL,NULL,NULL,NULL,event->button,event->time);
	}

	return FALSE;
}

static GtkUIManager *
uimgr_build_menu_and_toolbar(GtkWidget * mainwindow,
		GtkWidget ** menubar , GtkWidget * *toolbar,GtkWidget *ltree,GtkWidget ** ltreepopup,GtkWidget * rarea,GtkWidget ** rareapopup)
{
	static const gchar menubar_str [] = {
			"<ui>"
			  "<menubar name=\"MenuBar\" >"
				"<menuitem action = \"Search\" />"
				"<menuitem action = \"SaveData\" />"
				"<menu name=\"HelpMenu\" action = \"HelpMenu\" >"
					"<menuitem action = \"HelpAbout\" />"
				"</menu>"
			  "</menubar>"
			  "<toolbar name=\"TOOLBAR\">"
				  "<toolitem action=\"Search\" />"
				  "<toolitem action=\"SaveData\"/>"
				  "<toolitem action=\"FileQuit\"/>"
			  "</toolbar>"
			  "<popup name=\"catgorypopup\">"
				"<menuitem action = \"CreateNewCatelog\"/>"
				"<menuitem action = \"CreateNewEntry\"/>"
				"<menuitem action = \"Delete\"/>"
			  "</popup>"
			  "<popup name=\"cardpopup\">"
				"<menuitem action = \"NewItem\"/>"
				"<menuitem action = \"ChangeItem\"/>"
				"<menuitem action = \"DeleteItem\"/>"
			  "</popup>"
			  "<popup name=\"entrypopup\">"
				"<menuitem action = \"HelpAbout\" />"
			  "</popup>"
			"</ui>"
	};

	const GtkActionEntry actions1[] = {
			{ "CreateNewCatelog", GTK_STOCK_NEW, _("新建类别(_C)") , "<control>M" , "新建一个信息的类型" , G_CALLBACK(themanager_new_catlog_cb)},
			{ "CreateNewEntry", GTK_STOCK_NEW, _("新建信息(_E)") , "<control>N" , "新建一个信息" , G_CALLBACK(themanager_new_entry_cb)},
			{ "Delete", GTK_STOCK_DELETE, _("删除信息(_D)") , "<control>D" , "删除所选项目" , G_CALLBACK(themanager_delete_catlog_cb)},
			{ "Search", GTK_STOCK_FIND, _("Search") , "" , "搜索" , G_CALLBACK(themanager_search_cb)},
		};

	const GtkActionEntry actions2[] = {
			{ "NewItem", GTK_STOCK_NEW, _("新建描述(_N)") , NULL , NULL, G_CALLBACK(themanager_new_item_cb)},
			{ "ChangeItem", GTK_STOCK_EDIT, _("修改描述(_N)") , NULL , NULL, G_CALLBACK(themanager_edit_item_cb)},
			{ "DeleteItem", GTK_STOCK_DELETE, _("删除描述(_D)") , NULL , NULL, G_CALLBACK(themanager_delete_item_cb)},
		};

	const GtkActionEntry actions3[] = {
			{ "SaveData", GTK_STOCK_SAVE, _("_Save") , "<control>S" , "保存" , G_CALLBACK(themanager_savedata_cb)},
			{ "FileQuit", GTK_STOCK_QUIT, _("_Quit") , "<control>Q" , "退出" , G_CALLBACK(quit_app)},
			{ "HelpMenu", GTK_STOCK_HELP, _("_Help") },
			{ "HelpAbout", GTK_STOCK_ABOUT, _("_About") , "<control>H" , _("_About") , G_CALLBACK(Themanager_show_about_menu_callback)},
		};

	GtkUIManager * uimgr = gtk_ui_manager_new();

	gtk_ui_manager_add_ui_from_string(uimgr, menubar_str, -1, NULL);

	GtkActionGroup* actionGroup = gtk_action_group_new("Actions");

	gtk_action_group_add_actions(actionGroup, actions1, G_N_ELEMENTS(actions1), ltree);
	gtk_action_group_add_actions(actionGroup, actions2, G_N_ELEMENTS(actions2), rarea);
	gtk_action_group_add_actions(actionGroup, actions3, G_N_ELEMENTS(actions3), mainwindow);

	gtk_ui_manager_insert_action_group(uimgr,actionGroup,0);

	gtk_window_add_accel_group(GTK_WINDOW(mainwindow),gtk_ui_manager_get_accel_group(uimgr));

	*menubar = gtk_ui_manager_get_widget(uimgr,"/MenuBar");
	*toolbar = gtk_ui_manager_get_widget(uimgr,"/TOOLBAR");
	*ltreepopup = gtk_ui_manager_get_widget(uimgr,"/catgorypopup");
	*rareapopup = gtk_ui_manager_get_widget(uimgr,"/cardpopup");
	return uimgr;
}

static drawing_data drawingdata={0};

int main(int argc,char * argv[])
{
	gchar	*	database_file;
	GtkWidget* window,* menubar , * toolbar ,*ltreepopup,*rareapopup;
	DataBase * db;

	setlocale(LC_ALL,"");
	g_thread_init(NULL);
	gdk_threads_init();

	if((database_file = database_checking_existence(argv[0])) || (database_file = database_making_new(argv[0])) )
	{
		//打开数据库
		db = database_open(database_file);
		g_free(database_file);
	}else
		g_error("db not found or cann't create new one");

	gdk_threads_enter();
	gtk_init(&argc,&argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_object_set(window,"user-data",db,NULL);

	GtkWidget * vbox = gtk_vbox_new(0,0);
	GtkWidget * paned = gtk_hpaned_new();

	gtk_container_add(GTK_CONTAINER(window),vbox);

	GtkWidget * statusbar = gtk_statusbar_new();

	GtkWidget * ltree, *rarea;

	rarea = gtk_drawing_area_new();

	ltree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(gtk_tree_store_new(2,G_TYPE_STRING,G_TYPE_POINTER)));

	gtk_widget_set_size_request(ltree,100,200);
	gtk_widget_set_size_request(rarea,300,200);

	gtk_window_set_default_size(GTK_WINDOW(window),500,200);

	g_object_set(ltree,"user-data",db,NULL);


	g_object_set(rarea,"user-data",&drawingdata,NULL);

	gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(ltree),GTK_TREE_VIEW_GRID_LINES_BOTH);

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(ltree),FALSE);

	gtk_tree_view_append_column(GTK_TREE_VIEW(ltree),gtk_tree_view_column_new_with_attributes("Catelog",gtk_cell_renderer_text_new(), "text", 0, NULL));

	GtkUIManager *uimgr = uimgr_build_menu_and_toolbar(window,&menubar,&toolbar,ltree,&ltreepopup,rarea,&rareapopup);

	gtk_box_pack_start(GTK_BOX(vbox),menubar,FALSE,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(vbox),toolbar,FALSE,FALSE,FALSE);
	gtk_box_pack_end(GTK_BOX(vbox),statusbar,FALSE,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(vbox),paned,TRUE,TRUE,TRUE);


	GtkWidget * lscroll =	gtk_scrolled_window_new(NULL,NULL);
	GtkWidget * rscroll =	gtk_scrolled_window_new(NULL,NULL);

	gtk_container_add(GTK_CONTAINER(lscroll),ltree);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(rscroll),rarea);

	gtk_paned_add1(GTK_PANED(paned),lscroll);
	gtk_paned_add2(GTK_PANED(paned),rscroll);

	g_signal_connect(ltree,"realize",G_CALLBACK(on_realize),db);
	g_signal_connect(ltree,"destroy",G_CALLBACK(save_xml),db);
	g_signal_connect(ltree,"cursor-changed",G_CALLBACK(cursor_changed),rarea);

	g_signal_connect(rarea,"expose-event",G_CALLBACK(drawing_card),db);

	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(ltree,"button-press-event",G_CALLBACK(ltree_mouse_down),ltreepopup);

	gtk_widget_set_events(rarea,GDK_EXPOSURE_MASK|GDK_BUTTON_PRESS_MASK);
	g_signal_connect(rarea,"button-press-event",G_CALLBACK(rarea_mouse_down),rareapopup);


	gtk_widget_show_all(window);
	gtk_main();
	g_object_unref(uimgr);
	gdk_threads_leave();
	return EXIT_SUCCESS;
}

static void quit_app(GtkWidget *item, GtkWidget * mainwindow)
{
	gtk_widget_destroy(mainwindow);
}

static void Themanager_show_about_menu_callback(GtkWidget * item , gpointer callback_data)
{
	Themanager_show_about(callback_data);
}


static void themanager_savedata_cb(GtkWidget * item , GtkWidget* mainwindow)
{
	DataBase *db;

	if (changed)
	{
		g_object_get(mainwindow, "user-data", &db, NULL);
		database_save(db);
		changed = 0;
	}
}

static gboolean tree_model_find_str(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer user_data)
{
	struct find_data{
		GtkTreeView	* tree;
		const char * find_str;
	};

	struct find_data * data = user_data;

	char * str;
	xmlNodePtr	xmlnode;

	gtk_tree_model_get(model,iter,0,&str,1,&xmlnode,-1);

	if (gtk_tree_store_iter_depth((GtkTreeStore*)model,iter)==0 && strstr(str, data->find_str))
	{
		//找到
		gtk_tree_view_expand_to_path(data->tree,path);
		gtk_tree_view_set_cursor(data->tree,path,gtk_tree_view_get_column(data->tree,0),0);
		g_free(str);
		return TRUE;
	}
	g_free(str);

	//进入 XML 节点内部查
	if(gtk_tree_store_iter_depth((GtkTreeStore*)model,iter)==1)
	{
		if(strstr(xmlGetProp(xmlnode, "name"),data->find_str))
		{
			gtk_tree_view_expand_to_path(data->tree,path);
			gtk_tree_view_set_cursor(data->tree,path,gtk_tree_view_get_column(data->tree,0),0);
			drawingdata.selected_title = TRUE;
			drawingdata.selected = xmlnode;

			return TRUE;
		}

		if(xmlChildElementCount(xmlnode)>0)
		{
			xmlnode = xmlnode->children;

			while(xmlnode)
			{
				if(xmlnode->name && strcmp(xmlnode->name,"text"))
				{
					if(strstr(xmlnode->name,data->find_str) || strstr(xmlNodeGetContent(xmlnode),data->find_str))
					{
						gtk_tree_view_expand_to_path(data->tree,path);
						gtk_tree_view_set_cursor(data->tree,path,gtk_tree_view_get_column(data->tree,0),0);

						drawingdata.selected_title = FALSE;
						drawingdata.selected = xmlnode;
						return TRUE;
					}
				}
				xmlnode = xmlnode->next;
			}
		}
	}

	return FALSE;
}

static GtkDialog * mgr_search_dlg()
{
	GtkDialog * dlg = gtk_dialog_new();

	return dlg;
}

static void themanager_search_cb(GtkWidget * item , GtkWidget* ltree)
{
	gint	response;
	GtkTreeModel * model;
	const char* find_str = NULL;

	struct find_data{
		GtkTreeView	* tree;
		const char * find_str;
	}data;

	data.tree = (GtkTreeView*)ltree;

	model = gtk_tree_view_get_model((GtkTreeView*)ltree);

	GtkDialog * dialog = gtk_input_box(NULL,"查找",(const char **)&find_str);

	gtk_widget_hide((GtkWidget*)dialog);

	if(find_str && strlen(find_str))
	{
		data.find_str = find_str;
		gtk_tree_model_foreach(model,tree_model_find_str,&data);
	}
	gtk_widget_destroy((GtkWidget*)dialog);
}
