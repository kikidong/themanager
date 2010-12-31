/*
 * drawing.c
 *
 *  Created on: 2010-9-24
 *      Author: cai
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>

#include "database.h"
#include "drawing.h"

gboolean drawing_card(GtkWidget *widget, GdkEventExpose *event, gpointer user_data)
{
	drawing_data *data;
	gint	width,height,pw,ph;
	xmlNodePtr	xmlnode;

	PangoContext * pc;
	PangoFontDescription * ftdes;
	PangoLayout * pl;

	//就等着绘图吧，o(∩∩)o...哈哈
	g_object_get(widget, "user-data", &data, NULL);

	xmlnode = data->xml;

	if (!xmlnode)
		return FALSE;

	gdk_drawable_get_size(widget->window, &width, &height);
	//那个啥，咱先算标题位置

	pc = gtk_widget_get_pango_context(widget);

	ftdes = pango_font_description_copy(pango_context_get_font_description(pc));

	pango_font_description_set_family(ftdes,"Sans");

	pango_font_description_set_size(ftdes,72000);

	pango_context_set_font_description(pc,ftdes);

	pl = pango_layout_new(pc);

	pango_layout_set_text(pl, xmlGetProp(xmlnode, "name"), -1);

	pango_layout_get_pixel_size(pl,&pw,&ph);

	if(data->button >0 )
	{
		data->selected = NULL;
		//看是不是选中了标题
		GdkRectangle rect={(width - pw)/2, 5, pw, ph};

		GdkRegion * region = gdk_region_rectangle(&rect);

		if(data->selected_title = gdk_region_point_in(region,data->x,data->y))
		{
			data->selected = xmlnode;
			data->button = 0;
		}
		gdk_region_destroy(region);
	}

	if(data->selected == xmlnode)
		gtk_paint_shadow(widget->style,widget->window,GTK_STATE_SELECTED,GTK_SHADOW_OUT,NULL,NULL,NULL,(width - pw)/2, 5, pw, ph);

	gdk_draw_layout(widget->window,widget->style->text_gc[0],(width - pw)/2, 5 , pl);

	//接下来该绘制别的了
	gulong childcount = xmlChildElementCount(xmlnode);

	xmlnode = xmlnode->children;

	pango_font_description_set_size(ftdes,18000);

	pango_context_set_font_description(pc,ftdes);

	pango_layout_context_changed(pl);

	int i=0;

	while(xmlnode)
	{
		if(xmlnode->name && strcmp(xmlnode->name,"text"))
		{
			//看是不是选中了这些
			GdkRectangle rect;

			gchar * txt = g_strdup_printf("%s:\t",xmlnode->name);

			pango_layout_set_text(pl, txt, -1);

			g_free(txt);

			pango_layout_get_pixel_size(pl,&pw,&rect.height);

			rect.x = width/2 - pw;
			rect.y = i * 30 + ph + 10;
			rect.width = pw;

			gdk_draw_layout(widget->window, widget->style->text_gc[0], width/2 - pw,i * 30 + ph + 10, pl);

			pango_layout_set_text(pl,xmlNodeGetContent(xmlnode), -1);

			pango_layout_get_pixel_size(pl,&pw,NULL);

			rect.width += pw;

			gdk_draw_layout(widget->window, widget->style->text_gc[0], width /2 ,i * 30 + ph + 10, pl);

			i++;

			if(data->button > 0)
			{
				GdkRegion * region = gdk_region_rectangle(&rect);

				if (gdk_region_point_in(region, data->x, data->y))
				{
					data->selected = xmlnode;
					data->button = 0;
				}
				gdk_region_destroy(region);
			}

			if(data->selected == xmlnode)
			{
				gtk_paint_shadow(widget->style,widget->window,GTK_STATE_SELECTED,GTK_SHADOW_OUT,0,widget,0,rect.x,rect.y,rect.width,rect.height);
			}
		}
		xmlnode = xmlnode->next;
	}

	data->button = 0;
	pango_font_description_free(ftdes);
	g_object_unref(pl);
	return TRUE;
}
