INCFLAG =\
-I. \
-Iinc

CFLAGS += $(INCFLAG)

OBJS =\
$(OBJDIR)/arch.o \
$(OBJDIR)/core.o \
$(OBJDIR)/event.o \
$(OBJDIR)/flag.o \
$(OBJDIR)/hook.o \
$(OBJDIR)/kernelHeap.o \
$(OBJDIR)/mbox.o \
$(OBJDIR)/mutex.o \
$(OBJDIR)/queue.o \
$(OBJDIR)/sem.o \
$(OBJDIR)/serviceReq.o \
$(OBJDIR)/task.o \
$(OBJDIR)/time.o \
$(OBJDIR)/timer.o \
$(OBJDIR)/utility.o \
$(OBJDIR)/port.o \

all: $(OBJS)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	@echo "bulid file: $<"
	$(CC) $(CFLAGS) -MMD -MF$(@:%.o=%.d) -o $@ $< \
        && echo $(NOW) INFO Objecting CoOS/$< success. >> ../build.log \
        || echo $(NOW) ERROR Objecting CoOS/$< failed, stop building. >> ../build.log | exit 1

$(OBJDIR):
	@echo $(NOW) INFO Make new folder CoOS/$(OBJDIR). >> ../build.log
	mkdir -p $(OBJDIR)

clean:
	-rm -rf $(OBJDIR)/*.o
	-rm -rf $(OBJDIR)/*.d
