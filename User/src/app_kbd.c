#include "cogui.h"
#include "app_kbd.h"

char      text[100] = "type something...";                       
static widget_t *event_wgt = Co_NULL;

void app_kbd(void *par)
{
	app_t *app = gui_app_create("KBD");
	if (app == Co_NULL) {
		return;
	}
	
	app->optional_handler = app_kbd_handler; 
	gui_app_run(app);
	gui_app_delete(app);
	
	CoExitTask();
}

StatusType app_kbd_handler(event_t *event)
{
	ASSERT(event != Co_NULL);

	switch (event->type)
    {
	case EVENT_PAINT:
	    app_kbd_paint();
		break;
    
    case EVENT_KBD: {
        if (event->kbd_type == KBD_KEYDOWN) {
            char keys[2];
            if (event->ascii_code) {
                keys[0] = event->ascii_code;
                keys[1] = '\0';

                gui_widget_append_text(event_wgt, keys);
            } else if (event->key == KBD_KEY_BACKSPACE) {
                char *tmp = gui_strdup(event_wgt->text);
                uint64_t len = gui_strlen(tmp);
                if (len)
                    tmp[len-1] = '\0';
                gui_widget_clear_text(event_wgt);
                gui_widget_set_text(event_wgt, tmp);
            }
            gui_window_update(event_wgt->top ,event_wgt);
        }
        break;
    }
				
	default:
		return Co_FALSE;
				
	}
		
	return Co_TRUE;
}

void app_kbd_paint(void)
{
    window_t *win = gui_window_create_with_title();
	
	gui_window_show(win);

    widget_t *kbd_ex = gui_widget_create(win);
	gui_widget_set_rectangle(kbd_ex, 5, 45, 230, 270);
	kbd_ex->gc.background = green;
	kbd_ex->gc.foreground = green;
	kbd_ex->flag |= GUI_WIDGET_FLAG_RECT;
    kbd_ex->gc.padding = GUI_PADDING_SIMPLE(5);
    gui_widget_set_text_align(kbd_ex, GUI_TEXT_ALIGN_LEFT|GUI_TEXT_ALIGN_TOP);
    gui_widget_set_text(kbd_ex, text);
	print_result(gui_widget_show(kbd_ex), "create input box");
    event_wgt = kbd_ex;

	gui_printf("[%s] Paint ok.\r\n", gui_app_self()->name);
}