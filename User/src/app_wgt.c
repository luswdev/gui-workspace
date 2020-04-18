#include "cogui.h"
#include "app_wgt.h"

bool_t wgt_toggle = Co_FALSE;
uint8_t move_pos = 1;
uint8_t colors = 1;
uint16_t pos[8] = {80, 133, 5, 173, 80, 223, 155, 173};
static widget_t *event_wgt = Co_NULL;

void app_wgt(void *par) 
{    
	app_t *app = gui_app_create("Widgets");
	if (app == Co_NULL) {
		return;
	}
	
	app->optional_handler = app_wgt_handler; 

	gui_app_run(app);
	gui_app_delete(app);
	
	CoExitTask();
}

StatusType app_wgt_handler(event_t *event)
{
	ASSERT(event != Co_NULL);

	switch (event->type)
    {
	case EVENT_PAINT:
		app_wgt_paint();
        return GUI_E_OK;
		break;

    case EVENT_MOUSE_CLICK: {
        color_t rainbow[7] = {red, orange, yellow, green, blue, indigo, purple};

        if (!gui_strcmp(event->widget->text, "move")) {
            if (wgt_toggle) {
                gui_widget_show(event_wgt);
                wgt_toggle = ~wgt_toggle;
            }
            gui_widget_move_to_phy(event_wgt, pos[move_pos*2], pos[move_pos*2+1]);
            ++move_pos;
            if (move_pos >= 4) {
                move_pos = 0;
            }
        } else if (!gui_strcmp(event->widget->text, "hide")) {
            wgt_toggle ?  gui_widget_show(event_wgt) : gui_widget_hide(event_wgt);
            wgt_toggle = ~wgt_toggle;
        } else if (!gui_strcmp(event->widget->text, "event")) {
            event_wgt->gc.background = rainbow[colors++];
            if (colors >= 7) {
                colors = 0;
            }
            gui_window_update(event_wgt->top ,event_wgt);
        } else {
            return GUI_E_ERROR;
        }
    }
				
	default:
		return GUI_E_ERROR;
				
	}
		
	return GUI_E_ERROR;
}

void app_wgt_paint(void)
{
	window_t *win = gui_window_create_with_title();
	
	gui_window_show(win);
	
	widget_t *wgt_ex = gui_widget_create(win);                      /* widget ex-1: move button */
	gui_widget_set_rectangle(wgt_ex, 20, 60, 90, 36);
	wgt_ex->gc.background = light_grey;
	wgt_ex->gc.foreground = dark_grey;
	wgt_ex->flag |= GUI_WIDGET_FLAG_RECT | GUI_WIDGET_FLAG_FILLED;
    gui_widget_set_text_align(wgt_ex, GUI_TEXT_ALIGN_CENTER|GUI_TEXT_ALIGN_MIDDLE);
    gui_widget_set_text(wgt_ex, "move");
	print_result(gui_widget_show(wgt_ex), "create move button");

    wgt_ex = gui_widget_create(win);                                      /* widget ex-2: hide button */
	gui_widget_set_rectangle(wgt_ex, 130, 60, 90, 36);
	wgt_ex->gc.background = light_grey;
	wgt_ex->gc.foreground = dark_grey;
	wgt_ex->flag |= GUI_WIDGET_FLAG_RECT | GUI_WIDGET_FLAG_FILLED;
    gui_widget_set_text_align(wgt_ex, GUI_TEXT_ALIGN_CENTER|GUI_TEXT_ALIGN_MIDDLE);
    gui_widget_set_text(wgt_ex, "hide");
	print_result(gui_widget_show(wgt_ex), "create hide button");

    wgt_ex = gui_widget_create(win);                                      /* widget ex-3: event widget */
	gui_widget_set_rectangle(wgt_ex, 80, 133, 80, 80);
	wgt_ex->gc.background = red;
	wgt_ex->gc.foreground = light_grey;
	wgt_ex->flag |= GUI_WIDGET_FLAG_RECT | GUI_WIDGET_FLAG_FILLED;
    gui_widget_set_text_align(wgt_ex, GUI_TEXT_ALIGN_CENTER|GUI_TEXT_ALIGN_MIDDLE);
    gui_widget_set_text(wgt_ex, "event");
	print_result(gui_widget_show(wgt_ex), "create event widget");
    event_wgt = wgt_ex;
	
	gui_printf("[%s] Paint ok.\r\n", gui_app_self()->name);
}
