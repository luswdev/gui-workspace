#ifndef __APP_WGT_H__
#define __APP_WGT_H__

#ifdef __cplusplus
extern "C" {
#endif

void app_wgt(void *par);
StatusType app_wgt_handler(struct event *event);
void app_wgt_paint(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_WGT_H__ */
