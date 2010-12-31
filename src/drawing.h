/*
 * drawing.h
 *
 *  Created on: 2010-9-24
 *      Author: cai
 */

#ifndef DRAWING_H_
#define DRAWING_H_

typedef struct _drawing_data{
	xmlNodePtr	xml,selected;
	gboolean	selected_title;
	gdouble		x,y;
	gint		button;

}drawing_data;

gboolean drawing_card(GtkWidget *widget, GdkEventExpose *event, gpointer user_data);

#endif /* DRAWING_H_ */
