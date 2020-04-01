/**
 *******************************************************************************
 * @file       user_config.h
 * @version    V0.1.1
 * @date       2020.01.19
 * @brief      User define file.
 *******************************************************************************
 */ 
#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

/* ANSI define */
#define RED_BOLD "\x1b[;31;1m"
#define YEL_BOLD "\x1b[;33;1m"
#define BLU_BOLD "\x1b[;34;1m"
#define MAN_BOLD "\x1b[;35;1m"
#define GRN_BOLD "\x1b[;32;1m"
#define WHITE_BG "\x1b[;47;1m"
#define CYAN_BOLD_ITALIC "\x1b[;36;1;3m"
#define ARESET "\x1b[0;m"

#define UP_CUR "\033[A"

/* hardware initial function */
void UART_Init(void);
void LED_Init(void);
void LCD_Config(void);
void LCD_AF_GPIOConfig(void);
void Delay_Init(void);

/* printf */
void stm_print_char(const char ch);
void stm_print_string(const char *str);
void wait_user_btn(char *s);
void print_result(StatusType r, char *str);
void print_task_list(void);

#endif /* __USER_CONFIG_H__ */
