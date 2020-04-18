#ifndef __APP_KBD_H__
#define __APP_KBD_H__

#ifdef __cplusplus
extern "C" {
#endif

void app_kbd(void *par);
StatusType app_kbd_handler(struct event *event);
void app_kbd_paint(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_KBD_H__ */
