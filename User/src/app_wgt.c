#include "cogui.h"
#include "app_wgt.h"

bool_t wgt_toggle = Co_FALSE;
uint8_t move_pos = 1;
uint8_t colors = 1;
uint16_t pos[8] = {80, 133, 5, 173, 80, 223, 155, 173};
cogui_widget_t *event_wgt = Co_NULL;

void app_wgt(void *par) 
{    
	cogui_app_t *app = cogui_app_create("Widgets");
	if (app == Co_NULL) {
		return;
	}
	
	app->optional_handler = app_wgt_handler; 

	cogui_app_run(app);
	cogui_app_delete(app);
	
	CoExitTask();
}

StatusType app_wgt_handler(struct cogui_event *event)
{
	COGUI_ASSERT(event != Co_NULL);

	switch (event->type)
    {
	case COGUI_EVENT_PAINT:
		app_wgt_paint();
        return GUI_E_OK;
		break;

    case COGUI_EVENT_MOUSE_CLICK: {
        cogui_color_t rainbow[7] = {red, orange, yellow, green, blue, indigo, purple};

        if (!cogui_strcmp(event->widget->text, "move")) {
            if (wgt_toggle) {
                cogui_widget_show(event_wgt);
                wgt_toggle = ~wgt_toggle;
            }
            cogui_widget_move_to_phy(event_wgt, pos[move_pos*2], pos[move_pos*2+1]);
            ++move_pos;
            if (move_pos >= 4) {
                move_pos = 0;
            }
        } else if (!cogui_strcmp(event->widget->text, "hide")) {
            wgt_toggle ?  cogui_widget_show(event_wgt) : cogui_widget_hide(event_wgt);
            wgt_toggle = ~wgt_toggle;
        } else if (!cogui_strcmp(event->widget->text, "event")) {
            event_wgt->gc.background = rainbow[colors++];
            if (colors >= 7) {
                colors = 0;
            }
            cogui_window_update(event_wgt->top ,event_wgt);
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
	cogui_window_t *win = cogui_window_create_with_title();
	
	cogui_window_show(win);
	
	cogui_widget_t *wgt_ex = cogui_widget_create(win);                      /* widget ex-1: move button */
	cogui_widget_set_rectangle(wgt_ex, 20, 60, 90, 36);
	wgt_ex->gc.background = light_grey;
	wgt_ex->gc.foreground = dark_grey;
	wgt_ex->flag |= COGUI_WIDGET_FLAG_RECT | COGUI_WIDGET_FLAG_FILLED;
    cogui_widget_set_text_align(wgt_ex, COGUI_TEXT_ALIGN_CENTER|COGUI_TEXT_ALIGN_MIDDLE);
    cogui_widget_set_text(wgt_ex, "move");
	print_result(cogui_widget_show(wgt_ex), "create move button");

    wgt_ex = cogui_widget_create(win);                                      /* widget ex-2: hide button */
	cogui_widget_set_rectangle(wgt_ex, 130, 60, 90, 36);
	wgt_ex->gc.background = light_grey;
	wgt_ex->gc.foreground = dark_grey;
	wgt_ex->flag |= COGUI_WIDGET_FLAG_RECT | COGUI_WIDGET_FLAG_FILLED;
    cogui_widget_set_text_align(wgt_ex, COGUI_TEXT_ALIGN_CENTER|COGUI_TEXT_ALIGN_MIDDLE);
    cogui_widget_set_text(wgt_ex, "hide");
	print_result(cogui_widget_show(wgt_ex), "create hide button");

    wgt_ex = cogui_widget_create(win);                                      /* widget ex-3: event widget */
	cogui_widget_set_rectangle(wgt_ex, 80, 133, 80, 80);
	wgt_ex->gc.background = red;
	wgt_ex->gc.foreground = light_grey;
	wgt_ex->flag |= COGUI_WIDGET_FLAG_RECT | COGUI_WIDGET_FLAG_FILLED;
    cogui_widget_set_text_align(wgt_ex, COGUI_TEXT_ALIGN_CENTER|COGUI_TEXT_ALIGN_MIDDLE);
    cogui_widget_set_text(wgt_ex, "event");
	print_result(cogui_widget_show(wgt_ex), "create event widget");
    event_wgt = wgt_ex;
	
	cogui_printf("[%s] Paint ok.\r\n", cogui_app_self()->name);
}
