INCFLAG =\
-I. \
-Iinc \
-I../CMSIS/inc

CFLAGS += $(INCFLAG)

OBJS = \
$(OBJDIR)/misc.o \
$(OBJDIR)/stm32f4xx_adc.o \
$(OBJDIR)/stm32f4xx_can.o \
$(OBJDIR)/stm32f4xx_dac.o \
$(OBJDIR)/stm32f4xx_dbgmcu.o \
$(OBJDIR)/stm32f4xx_dcmi.o \
$(OBJDIR)/stm32f4xx_dma.o \
$(OBJDIR)/stm32f4xx_dma2d.o \
$(OBJDIR)/stm32f4xx_exti.o \
$(OBJDIR)/stm32f4xx_flash.o \
$(OBJDIR)/stm32f4xx_fmc.o \
$(OBJDIR)/stm32f4xx_gpio.o \
$(OBJDIR)/stm32f4xx_i2c.o \
$(OBJDIR)/stm32f4xx_iwdg.o \
$(OBJDIR)/stm32f4xx_ltdc.o\
$(OBJDIR)/stm32f4xx_pwr.o \
$(OBJDIR)/stm32f4xx_rcc.o \
$(OBJDIR)/stm32f4xx_rng.o \
$(OBJDIR)/stm32f4xx_rtc.o \
$(OBJDIR)/stm32f4xx_sai.o \
$(OBJDIR)/stm32f4xx_sdio.o \
$(OBJDIR)/stm32f4xx_spi.o \
$(OBJDIR)/stm32f4xx_syscfg.o\
$(OBJDIR)/stm32f4xx_tim.o \
$(OBJDIR)/stm32f4xx_usart.o\
$(OBJDIR)/stm32f4xx_wwdg.o 

all: $(OBJS)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	@echo "bulid file: $<"
	$(CC) $(CFLAGS) -MMD -MF$(@:%.o=%.d) -o $@ $< \
        && echo $(NOW) INFO Objecting System/STM32/$< success. >> ../../build.log \
        || echo $(NOW) ERROR Objecting System/STM32/$$< failed, stop building. >> ../../build.log | exit 1

$(OBJDIR):
	echo $(NOW) INFO Make new folder System/STM32/$(OBJDIR). >> ../../build.log
	mkdir -p $(OBJDIR)

clean:
	-rm -rf $(OBJDIR)/*.o
	-rm -rf $(OBJDIR)/*.d
	