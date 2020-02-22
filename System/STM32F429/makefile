INCFLAG =\
-I. \
-Iinc \
-I../CMSIS/inc \
-I../STM32/inc

CFLAGS += $(INCFLAG)

OBJS = \
$(OBJDIR)/Common/fonts.o \
$(OBJDIR)/stm32f429i_discovery.o \
$(OBJDIR)/stm32f429i_discovery_i2c_ee.o \
$(OBJDIR)/stm32f429i_discovery_ioe.o \
$(OBJDIR)/stm32f429i_discovery_l3gd20.o \
$(OBJDIR)/stm32f429i_discovery_lcd.o \
$(OBJDIR)/stm32f429i_discovery_sdram.o \

all: $(OBJS)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	@echo "bulid file: $<"
	$(CC) $(CFLAGS) -MMD -MF$(@:%.o=%.d) -o $@ $<  \
        && echo $(NOW) INFO Objecting System/STM32F429/$< success. >> ../../build.log \
        || echo $(NOW) ERROR Objecting System/STM32F429/$< failed, stop building. >> ../../build.log | exit 1

$(OBJDIR)/Common/%.o: Common/%.c | $(OBJDIR)
	@echo "bulid file: $<"
	$(CC) $(CFLAGS) -MMD -MF$(@:%.o=%.d) -o $@ $<  \
        && echo $(NOW) INFO Objecting System/STM32F429/Common/$< success. >> ../../build.log \
        || echo $(NOW) ERROR Objecting System/STM32F429/Common/$< failed, stop building. >> ../../build.log | exit 1

$(OBJDIR):
	echo $(NOW) INFO Make new folder System/STM32F429/$(OBJDIR). >> ../../build.log
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/Common

clean:
	-rm -rf $(OBJDIR)/*.o
	-rm -rf $(OBJDIR)/*.d
	-rm -rf $(OBJDIR)/Common/*.o
	-rm -rf $(OBJDIR)/Common/*.d