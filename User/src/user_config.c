/**
 *******************************************************************************
 * @file       user_config.c
 * @version    V0.1.1
 * @date       2020.01.19
 * @brief      Some initialization functions.
 *******************************************************************************
 */ 

#include <cogui.h>
#include <stm32f429i_discovery_lcd.h>
#include <user_config.h>

/**
 *******************************************************************************
 * @brief      Initial USART 1 for printing output.
 * @param[in]  None
 * @param[out] None
 * @retval     None
 *******************************************************************************
 */
void UART_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate            = 9600;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
			
	USART_Cmd(USART1, ENABLE);
}		

/**
 *******************************************************************************
 * @brief      Initial LED pins.
 * @param[in]  None
 * @param[out] None
 * @retval     None
 *******************************************************************************
 */
void LED_Init(void)
{
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);
}

void Delay_Init(void)
{
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);    /* get system clock */
    /* while loop takes 4 cycles */
    /* Enable SysTick for Delay function */
    if (SysTick_Config(SystemCoreClock / 1000)) //1ms
    { 
        /* Capture error */ 
        while (1);
    }
}

/**
 *******************************************************************************
 * @brief      Initial LTDC and layer (from STM library).
 * @param[in]  None
 * @param[out] None
 * @retval     None
 *******************************************************************************
 */
void LCD_Config(void)
{
    LTDC_InitTypeDef               LTDC_InitStruct;
    LTDC_Layer_InitTypeDef         LTDC_Layer_InitStruct;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    /* Configure NCS in Output Push-Pull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* Configure the LCD Control pins ------------------------------------------*/
    LCD_CtrlLinesConfig();
    LCD_ChipSelect(DISABLE);
    LCD_ChipSelect(ENABLE);

    /* Configure the LCD_SPI interface -----------------------------------------*/
    LCD_SPIConfig(); 
    
    /* Power on the LCD --------------------------------------------------------*/
    LCD_PowerOn();
        
    /* Enable the LTDC Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);
    
    /* Enable the DMA2D Clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE); 
    
    /* Configure the LCD Control pins */
    LCD_AF_GPIOConfig();  
    
    /* Configure the FMC Parallel interface : SDRAM is used as Frame Buffer for LCD */
    SDRAM_Init();
    
/* Enable Pixel Clock --------------------------------------------------------*/

    /* Configure PLLSAI prescalers for LCD */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/3 = 64 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 64/8 = 8 Mhz */
    RCC_PLLSAIConfig(192, 7, 3);
    RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);
    
    /* Enable PLLSAI Clock */
    RCC_PLLSAICmd(ENABLE);
    /* Wait for PLLSAI activation */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
    {
    }

/* LTDC Initialization -------------------------------------------------------*/

    /* Initialize the horizontal synchronization polarity as active low*/
    LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;     
    /* Initialize the vertical synchronization polarity as active low */  
    LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;     
    /* Initialize the data enable polarity as active low */ 
    LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;     
    /* Initialize the pixel clock polarity as input pixel clock */ 
    LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;
    
    /* Timing configuration */
        /* Configure horizontal synchronization width */     
    LTDC_InitStruct.LTDC_HorizontalSync = 9;
    /* Configure vertical synchronization height */
    LTDC_InitStruct.LTDC_VerticalSync = 1;
    /* Configure accumulated horizontal back porch */
    LTDC_InitStruct.LTDC_AccumulatedHBP = 29; 
    /* Configure accumulated vertical back porch */
    LTDC_InitStruct.LTDC_AccumulatedVBP = 3;  
    /* Configure accumulated active width */  
    LTDC_InitStruct.LTDC_AccumulatedActiveW = 269;
    /* Configure accumulated active height */
    LTDC_InitStruct.LTDC_AccumulatedActiveH = 323;
    /* Configure total width */
    LTDC_InitStruct.LTDC_TotalWidth = 279; 
    /* Configure total height */
    LTDC_InitStruct.LTDC_TotalHeigh = 327;
    
    LTDC_Init(&LTDC_InitStruct);
    
    /* Configure R,G,B component values for LCD background color */                   
    LTDC_InitStruct.LTDC_BackgroundRedValue = 0;            
    LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;          
    LTDC_InitStruct.LTDC_BackgroundBlueValue = 0; 
                
    LTDC_Init(&LTDC_InitStruct);
    
/* LTDC initialization end ---------------------------------------------------*/

/* Layer1 Configuration ------------------------------------------------------*/  
    
    /* Windowing configuration */ 
    /* In this case all the active display area is used to display a picture then :
        Horizontal start = horizontal synchronization + Horizontal back porch = 30 
        Horizontal stop = Horizontal start + window width -1 = 30 + 240 -1
        Vertical start   = vertical synchronization + vertical back porch     = 4
        Vertical stop   = Vertical start + window height -1  = 4 + 320 -1      */ 
    LTDC_Layer_InitStruct.LTDC_HorizontalStart = 30;
    LTDC_Layer_InitStruct.LTDC_HorizontalStop = (240 + 30 - 1); 
    LTDC_Layer_InitStruct.LTDC_VerticalStart = 4;
    LTDC_Layer_InitStruct.LTDC_VerticalStop = (320 + 4 - 1);
    
    /* Pixel Format configuration*/           
    LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
    
    /* Alpha constant (255 totally opaque) */
    LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255; 
    
    /* Configure blending factors */       
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;  
    
    /* Default Color configuration (configure A,R,G,B component values) */          
    LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;        
    LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;       
    LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;         
    LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;   
    
    /* Input Address configuration */    
    LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER + BUFFER_OFFSET;
    
    /* the length of one line of pixels in bytes + 3 then :
        Line Lenth = Active high width x number of bytes per pixel + 3 
        Active high width         = 240 
        number of bytes per pixel = 2    (pixel_format : RGB565) 
    */
    LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((240 * 2) + 3);
    
    /*  the pitch is the increment from the start of one line of pixels to the 
        start of the next line in bytes, then :
        Pitch = Active high width x number of bytes per pixel     
    */
    LTDC_Layer_InitStruct.LTDC_CFBPitch = (240 * 2);  
    
    /* configure the number of lines */
    LTDC_Layer_InitStruct.LTDC_CFBLineNumber = 320;
    
    LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);
    
    LTDC_DitherCmd(ENABLE);
}

/**
 *******************************************************************************
 * @brief      GPIO config for LTDC (from STM library).
 * @param[in]  None
 * @param[out] None
 * @retval     None
 *******************************************************************************
 */
void LCD_AF_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* Enable GPIOI, GPIOJ, GPIOG, GPIOF, GPIOH AHB Clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
                            RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | \
                            RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

    /* GPIOs Configuration */
    /*
    +------------------------+-----------------------+----------------------------+
    +                       LCD pins assignment                                   +
    +------------------------+-----------------------+----------------------------+
    |  LCD_TFT R2 <-> PC.12  |  LCD_TFT G2 <-> PA.06 |  LCD_TFT B2 <-> PD.06      |
    |  LCD_TFT R3 <-> PB.00  |  LCD_TFT G3 <-> PG.10 |  LCD_TFT B3 <-> PG.11      |
    |  LCD_TFT R4 <-> PA.11  |  LCD_TFT G4 <-> PB.10 |  LCD_TFT B4 <-> PG.12      |
    |  LCD_TFT R5 <-> PA.12  |  LCD_TFT G5 <-> PB.11 |  LCD_TFT B5 <-> PA.03      |
    |  LCD_TFT R6 <-> PB.01  |  LCD_TFT G6 <-> PC.07 |  LCD_TFT B6 <-> PB.08      |
    |  LCD_TFT R7 <-> PG.06  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PB.09      |
    -------------------------------------------------------------------------------
            |  LCD_TFT HSYNC <-> PC.06  | LCDTFT VSYNC <->  PA.04 |
            |  LCD_TFT CLK   <-> PG.07  | LCD_TFT DE   <->  PF.10 |
            -----------------------------------------------------

    */

    /* GPIOA configuration */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | \
                                GPIO_Pin_11 | GPIO_Pin_12;
                                
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* GPIOB configuration */  
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | \
                                GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* GPIOC configuration */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LTDC);
    //  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_LTDC);
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;// | GPIO_Pin_10;
                                
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* GPIOD configuration */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_LTDC);
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;
                                
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    /* GPIOF configuration */
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
                                
    GPIO_Init(GPIOF, &GPIO_InitStruct);     

    /* GPIOG configuration */  
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | \
                                GPIO_Pin_11 | GPIO_Pin_12;
    
    GPIO_Init(GPIOG, &GPIO_InitStruct);
}

/**
 *******************************************************************************
 * @brief      Print character via USART1.
 * @param[in]  ch       Character to print.
 * @param[out] None
 * @retval     None
 *******************************************************************************
 */
void stm_print_char(const char ch)
{
    USART1->DR = (uint8_t) ch;

    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/**
 *******************************************************************************
 * @brief      Print string via USART1.
 * @param[in]  *str     String to print.
 * @param[out] None
 * @retval     None
 *******************************************************************************
 */
void stm_print_string(const char *str)
{
    while (*str) {
        if (*str == '\n') {
            stm_print_char('\n');
            str++;
        }
        else if (*str == '\r') {
            stm_print_char('\r');
            str++;
        }
        else {
            stm_print_char(*str++);
        }
    }
}

void wait_user_btn(char *s)
{
    uint16_t len;
	len = gui_printf(">>>>> Press the USER BUTTON to %s <<<<<\r", s);

	 /* Wait for User push-button is pressed */
    while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET) {
    }
    
    /* Wait for User push-button is released */
    while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET) {
    }

    /* clear ouput buffer with space */
    while (len --){
        gui_printf(" ");
    }
    gui_printf(" \r");
}

/**
 *******************************************************************************
 * @brief      Print result type by word.
 * @param[in]  r        Result type.
 * @param[in]  *str     Other message.
 * @param[out] None
 * @retval     None
 *******************************************************************************
 */
void print_result(StatusType r, char *str) {
    gui_printf("[User] %s => ", str);
	switch (r) {
		case GUI_E_ERROR:
			gui_printf(RED_BOLD"GUI_E_ERROR\r\n"ARESET);
			break;
		
		case GUI_E_OK:
			gui_printf(GRN_BOLD"GUI_E_OK\r\n"ARESET);
			break;
        
        case GUI_E_APP_NULL:
			gui_printf(RED_BOLD"GUI_E_APP_NULL\r\n"ARESET);
			break;

        default:
            gui_printf(MAN_BOLD"None\r\n"ARESET);
            break;
	}	
}

/**
 *******************************************************************************
 * @brief      Print task list from TCBTbl.
 * @param[in]  None
 * @param[out] None
 * @retval     None
 *******************************************************************************
 */
void print_task_list(void)
{
	extern OSTCB TCBTbl[CFG_MAX_USER_TASKS+SYS_TASK_NUM];
	P_OSTCB 	 ptcb;
	int i;
    
	ptcb = &TCBTbl[0];
	for ( i = 0; i < (CFG_MAX_USER_TASKS+SYS_TASK_NUM-1); i++ ) {
		gui_printf("Task %2d: state =>", ptcb->taskID);
		switch (ptcb->state) {
			case TASK_READY:
				gui_printf(GRN_BOLD" TASK_READY  "ARESET);
				break;
		
			case TASK_RUNNING:
				gui_printf(BLU_BOLD" TASK_RUNNING"ARESET);
				break;
			
			case TASK_WAITING:
				gui_printf(YEL_BOLD" TASK_WAITING"ARESET);
				break;
			
			case TASK_DORMANT:
				gui_printf(RED_BOLD" TASK_DORMANT"ARESET);
				break;
		}
		
		gui_printf(", priority => %d (0x%p)\r\n", ptcb->prio, ptcb);
		ptcb++;
	}
}
