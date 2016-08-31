
all: makefile.os lib examples

.PHONY: all lib examples install clean veryclean uninstall

makefile.os:
	@echo Run one of the fix* scripts to set up the makefile to your OS.

include LibPaths.mak
include makefile.os

lib: $(LIB_STATIC)

examples: $(EXAMPLES_DIR) lib $(EXAMPLE_EXE)

$(LIB_STATIC): $(LIB_O)
	$(AR) rs $@ $^

$(LIB_CPP_O): %.o: %.cpp
	$(CPP) -c -o $@ -MMD $(CPPFLAGS) $<

$(LIB_C_O): %.o: %.c
	$(CC) -c -o $@ -MMD $(CFLAGS) $<

$(EXAMPLE_EXE): $(EXE_MASK): $(EXAMPLE_DIR) %.o $(LIB_STATIC)
	$(CPP) -o $@ $(LDFLAGS) $< $(EXAMPLE_LIBS)

$(EXAMPLE_O): %.o: %.cpp
	$(CPP) -c -o $@ $(CPPFLAGS) $<

-include $(DEPENDENCIES)

ifdef EXAMPLES_DIR

$(EXAMPLES_DIR):
	$(MKDIR) $@

endif

clean:
	$(RM) $(EXAMPLE_O) $(LIB_O) $(DEPENDENCIES)

veryclean: clean
	$(RM) makefile.os $(EXAMPLE_EXE) $(LIB_STATIC)

install: $(LIB_STATIC)
	$(CP) $< $(LIBDIR)
	$(CP) include/gnelib.h $(INCDIR)
	-$(MKDIR) $(INCDIR)gnelib
	$(CP) include/gnelib/*.h $(INCDIR)gnelib

uninstall:
	$(RM) $(LIBDIR)$(LIB_STATIC_NAME)
	$(RM) $(INCDIR)gnelib.h
	$(RM) $(INCDIR)gnelib/*
	$(RMDIR) $(INCDIR)gnelib
