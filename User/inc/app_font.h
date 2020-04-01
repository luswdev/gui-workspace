#ifndef __APP_FONT_H__
#define __APP_FONT_H__

#ifdef __cplusplus
extern "C" {
#endif

void app_font(void *par);
StatusType app_font_handler(struct event *event);
void app_font_paint(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_FONT_H__ */
