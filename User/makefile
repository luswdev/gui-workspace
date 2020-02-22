INCFLAG =\
-I. \
-Iinc \
-I../System/STM32/inc \
-I../System/STM32F429/inc \
-I../System/CMSIS/inc \
-I../System/USB_HOST_HID/inc \
-I../CoOS/inc \
-I../GUI/inc

CFLAGS  += $(INCFLAG)

OBJS =\
$(OBJDIR)/user_config.o \
$(OBJDIR)/LCD.o \
$(OBJDIR)/main.o \
$(OBJDIR)/stm32f4xx_it.o \
$(OBJDIR)/usbh_usr.o \
Startup/startup.o

all: $(OBJS)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	@echo "bulid file: $<"
	$(CC) $(CFLAGS) -MMD -MF$(@:%.o=%.d) -o $@ $<\
        && echo $(NOW) INFO Objecting User/$< success. >> ../build.log \
        || echo $(NOW) ERROR Objecting User/$< failed, stop building. >> ../build.log | exit 1
	
Startup/startup.o: ./Startup/startup.s | $(OBJDIR)
	@echo "bulid file: $<"
	$(CC) $(CFLAGS) -MMD -MF$(@:%.o=%.d) -o $@ $< \
        && echo $(NOW) INFO Objecting User/$< success. >> ../build.log \
        || echo $(NOW) ERROR Objecting User/$< failed, stop building. >> ../build.log | exit 1

$(OBJDIR):
	@echo $(NOW) INFO Make new folder User/$(OBJDIR). >> ../build.log
	mkdir -p $(OBJDIR)

clean:
	-rm -rf $(OBJDIR)/*.o 
	-rm -rf $(OBJDIR)/*.lst
	-rm -rf $(OBJDIR)/*.d
	-rm -rf Startup/*.o
