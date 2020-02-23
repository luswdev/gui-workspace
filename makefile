TCPREFIX := arm-none-eabi-
CC       := $(TCPREFIX)gcc
LD       := $(TCPREFIX)g++
CP       := $(TCPREFIX)objcopy
OD       := $(TCPREFIX)objdump
GD  	 := $(TCPREFIX)gdb

CFLAGS 	:= -c -Wall -fno-common -O0 -g -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=nosys.specs
LFLAGS  = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os -T$(LDFILE) --specs=nosys.specs
CPFLAGS = -Obinary
ODFLAGS = -S

OBJDIR 	:= obj

BLACK 	:= "\033[30;1m"
RED  	:= "\033[31;1m"
GREEN 	:= "\033[32;1m"
YELLOW 	:= "\033[33;1m"
BLUE  	:= "\033[34;1m"
PURPLE 	:= "\033[35;1m"
CYAN  	:= "\033[361m"
RST		:= "\033[0m"

NOW := $(shell date +"%Y-%m-%d %H:%M:%S")

export

STM32FLASH 	= ./User/Startup/stm32.pl
LDFILE = ./User/Startup/STM32F429ZI_FLASH.ld

OCDCFG 		= ./openocd.cfg
TAIL		= multitail
LOGFILE		= ./build.log

OBJS =\
$(wildcard ./User/obj/*.o) \
$(wildcard ./System/*/obj/*.o) \
$(wildcard ./CoOS/obj/*.o) \
$(wildcard ./GUI/obj/*.o) \
./User/Startup/startup.o

all: 
	@echo "usage: make <target> <option>"
	@echo ""
	@echo "\tall \t\t\t\t print this information"
	@echo "\trun \t\t\t\t build and flash binary into board"
	@echo "\tbuild \t\t\t\t build binary file"
	@echo "\tocd \t\t\t\t connect to board with openocd"
	@echo "\tdebug \t\t\t\t debug using gdb tool"
	@echo "\tgit  <commit log> \t\t upload GUI to github"
	@echo "\tclean \t\t\t\t clean object files"
	@echo "\t\tclean_dir \t\t clean object directory"
	@echo "\t\tclean_log \t\t clean build log"
	@echo ""

run: build
	@echo $(YELLOW)"Flashing main.bin."$(RST)
	$(STM32FLASH) main.bin > log
	@echo $(GREEN)"Finish flashing main.bin."$(RST)
	@if grep "error" log; then 												\
		echo $(NOW) ERROR Flash binary into STM32 failed. >> build.log; 	\
		echo $(NOW) WARNING Trying flash binary again. >> build.log; 		\
		make run;															\
	else																	\
		echo $(NOW) INFO Build finished without error. >> build.log;		\
	fi
	@-rm -rf log

build: main.bin

obj:
	$(MAKE) all -C System
	@echo $(YELLOW)"Build CoOS..."$(RST)
	$(MAKE) all -C CoOS
	@echo $(GREEN)"Finish building CoOS."$(RST)
	@echo ""
	@echo $(YELLOW)"Build GUI..."$(RST)
	$(MAKE) all -C GUI
	@echo $(GREEN)"Finish building GUI."$(RST)
	@echo ""
	@echo $(YELLOW)"Build user files..."$(RST)
	$(MAKE) all -C User
	@echo $(GREEN)"Finish building user files."$(RST)
	@echo ""

main.bin: obj main.elf
	@echo "copy file main.elf"
	$(CP) $(CPFLAGS) main.elf $@ \
		&& echo $(NOW) INFO Copying main.elf success. >> build.log \
        || echo $(NOW) ERROR Copying main.elf failed, stop building. >> build.log | exit 1
	$(OD) $(ODFLAGS) main.elf > main.lst

main.elf: $(OBJS) $(LDFILE)
	@echo "link file: $@"
	$(LD) $(LFLAGS) -o $@ $(OBJS) \
        && echo $(NOW) INFO Linking $@ success. >> build.log \
        || echo $(NOW) ERROR Linking $@ failed, stop building. >> build.log | exit 1

ocd:
	openocd -f $(OCDCFG)

log:
	$(TAIL) -cS build_log $(LOGFILE)

debug:
	$(GD) -ex "target remote :3333" \
	-ex "c" main.elf

git: 
	$(MAKE) git com_msg="$(filter-out $@,$(MAKECMDGOALS))" -C  GUI

# handle argument error
%:
	@:

clean:
	$(MAKE) clean -e -C System
	$(MAKE) clean -e -C CoOS
	$(MAKE) clean -e -C GUI
	$(MAKE) clean -e -C User
	-rm -rf main.bin main.elf
	@echo $(NOW) INFO Clean everythings up. >> build.log
ifdef clean_dir
	-rm -rf */obj */*/obj
	@echo $(NOW) INFO Clean every dir:$(OBJDIR) up. >> build.log
endif
ifdef clean_log
	@cat /dev/null > build.log
	@echo $(NOW) INFO Clean up building log. >> build.log
endif

.PHONY: all run build clean debug git log ocd obj
