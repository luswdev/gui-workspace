INCFLAG =\
-I. \
-Iinc \
-I../CMSIS/inc \
-I../STM32/inc \
-I../STM32F429/inc \
-I../../GUI/inc \
-I../../CoOS/inc \
-I../../User/inc

CFLAGS += $(INCFLAG)

OBJS = \
$(OBJDIR)/usb_bsp.o \
$(OBJDIR)/usb_core.o \
$(OBJDIR)/usb_hcd.o \
$(OBJDIR)/usb_hcd_int.o \
$(OBJDIR)/usbh_core.o \
$(OBJDIR)/usbh_hcs.o \
$(OBJDIR)/usbh_ioreq.o \
$(OBJDIR)/usbh_stdreq.o \
$(OBJDIR)/usbh_hid_core.o \
$(OBJDIR)/usbh_hid_mouse.o \
$(OBJDIR)/usbh_hid_keybd.o


all: $(OBJS)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	@echo "bulid file: $<"
	$(CC) $(CFLAGS) -MMD -MF$(@:%.o=%.d) -o $@ $<  \
        && echo $(NOW) INFO Objecting System/USB_HOST/HID/$< success. >> ../../build.log \
        || echo $(NOW) ERROR Objecting System/USB_HOST/HID/$< failed, stop building. >> ../../build.log | exit 1

$(OBJDIR):
	echo $(NOW) INFO Make new folder System/USB_HOST/HID/$(OBJDIR). >> ../../build.log
	mkdir -p $(OBJDIR)

clean:
	-rm -rf $(OBJDIR)/*.o
	-rm -rf $(OBJDIR)/*.d