#include "cogui.h"
#include "app_font.h"

void app_font(void *par)
{
	app_t *app = gui_app_create("Fonts");
	if (app == Co_NULL) {
		return;
	}
	
	app->optional_handler = app_font_handler; 
	gui_app_run(app);
	gui_app_delete(app);
	
	CoExitTask();
}

StatusType app_font_handler(event_t *event)
{
	ASSERT(event != Co_NULL);

	switch (event->type)
    {
	case EVENT_PAINT:
		 app_font_paint();
		break;
				
	default:
		return Co_FALSE;
				
	}
		
	return Co_TRUE;
}

void app_font_paint(void)
{
	window_t *win = gui_window_create_with_title();
	
	gui_window_show(win);

    /* widget 1: top-left */
    widget_t *font_ex = gui_widget_create(win);
	gui_widget_set_rectangle(font_ex, 5, 45, 230, 50);
	font_ex->gc.background = blue;
	font_ex->flag |= GUI_WIDGET_FLAG_RECT | GUI_WIDGET_FLAG_FILLED;
    font_ex->gc.padding = GUI_PADDING_SIMPLE(5);
    gui_widget_set_text_align(font_ex, GUI_TEXT_ALIGN_LEFT|GUI_TEXT_ALIGN_TOP);
    gui_widget_set_text(font_ex, "Text align left-top");
	print_result(gui_widget_show(font_ex), "create left-top example");

    /* widget 2: middle-center */
    font_ex = gui_widget_create(win);
	gui_widget_set_rectangle(font_ex, 5, 100, 230, 50);
	font_ex->gc.background = red;
	font_ex->flag |= GUI_WIDGET_FLAG_RECT | GUI_WIDGET_FLAG_FILLED;
    font_ex->gc.padding = GUI_PADDING_SIMPLE(5);
    gui_widget_set_text_align(font_ex, GUI_TEXT_ALIGN_CENTER|GUI_TEXT_ALIGN_MIDDLE);
    gui_widget_set_text(font_ex, "Text align middle-center");
	print_result(gui_widget_show(font_ex), "create middle-center example");

    /* widget 3: bottom-right */
    font_ex = gui_widget_create(win);
	gui_widget_set_rectangle(font_ex, 5, 155, 230, 50);
	font_ex->gc.background = green;
	font_ex->gc.foreground = dark_grey;
	font_ex->flag |= GUI_WIDGET_FLAG_RECT | GUI_WIDGET_FLAG_FILLED;
    font_ex->gc.padding = GUI_PADDING_SIMPLE(5);
    gui_widget_set_text_align(font_ex, GUI_TEXT_ALIGN_BOTTOM|GUI_TEXT_ALIGN_RIGHT);
    gui_widget_set_text(font_ex, "Text align bottom-right");
	print_result(gui_widget_show(font_ex), "create bottom-right example");

    /* widget 4: font size 11x18 */
    font_ex = gui_widget_create(win);
	gui_widget_set_rectangle(font_ex, 5, 210, 230, 50);
	font_ex->gc.background = yellow;
	font_ex->gc.foreground = dark_grey;
	font_ex->flag |= GUI_WIDGET_FLAG_RECT | GUI_WIDGET_FLAG_FILLED;
    font_ex->gc.padding = GUI_PADDING_SIMPLE(15);
    gui_widget_set_font(font_ex, &tm_font_11x18);
    gui_widget_set_text_align(font_ex, GUI_TEXT_ALIGN_CENTER|GUI_TEXT_ALIGN_MIDDLE);
    gui_widget_set_text(font_ex, "11x18");
	print_result(gui_widget_show(font_ex), "create 11x18 example");

    /* widget 5: font size 16x26 */
    font_ex = gui_widget_create(win);
	gui_widget_set_rectangle(font_ex, 5, 265, 230, 50);
	font_ex->gc.background = purple;
	font_ex->flag |= GUI_WIDGET_FLAG_RECT | GUI_WIDGET_FLAG_FILLED;
    font_ex->gc.padding = GUI_PADDING_SIMPLE(10);
    gui_widget_set_font(font_ex, &tm_font_16x26);
    gui_widget_set_text_align(font_ex, GUI_TEXT_ALIGN_CENTER|GUI_TEXT_ALIGN_MIDDLE);
    gui_widget_set_text(font_ex, "16x26");
	print_result(gui_widget_show(font_ex), "create 16x26 example");

	gui_printf("[%s] Paint ok.\r\n", gui_app_self()->name);
}
