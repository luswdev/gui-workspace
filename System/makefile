all: 
	@echo $(YELLOW)"Build STM32F4 standard peripheral driver..."$(RST)
	$(MAKE) all -C STM32
	@echo $(GREEN)"Finish building STM32F4 standard peripheral driver."$(RST)
	@echo ""
	@echo $(YELLOW)"Build CMSIS..."$(RST)
	$(MAKE) all -C CMSIS
	@echo $(GREEN)"Finish building CMSIS."$(RST)
	@echo ""
	@echo $(YELLOW)"Build STM32F429 driver..."$(RST)
	$(MAKE) all -C STM32F429
	@echo $(GREEN)"Finish building STM32F429 driver."$(RST)
	@echo ""
	@echo $(YELLOW)"Build USB driver..."$(RST)
	$(MAKE) all -C USB_HOST_HID
	@echo $(GREEN)"Finish building USB driver."$(RST)
	@echo ""

clean:
	$(MAKE) clean -e -C CMSIS
	$(MAKE) clean -e -C STM32
	$(MAKE) clean -e -C STM32F429
	$(MAKE) clean -e -C USB_HOST_HID