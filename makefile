# ----------------------------
# Set NAME to the program name
# Set ICON to the png icon file name
# Set DESCRIPTION to display within a compatible shell
# Set COMPRESSED to "YES" to create a compressed program
# ----------------------------

NAME        ?= STELLARI
COMPRESSED  ?= YES
ICON        ?= icon.png
DESCRIPTION ?= "Stellaris for TI83PCE and "

# ----------------------------
# Other Options (Advanced)
# ----------------------------

#EXTRA_CFLAGS        ?=
#USE_FLASH_FUNCTIONS ?= YES|NO
#OUTPUT_MAP          ?= YES|NO
#ARCHIVED            ?= YES|NO
#OPT_MODE            ?= -optsize|-optspeed
#SRCDIR              ?= src
#OBJDIR              ?= obj
#BINDIR              ?= bin
#GFXDIR              ?= src/gfx
#V                   ?= 1

FONTDIR ?= $(SRCDIR)/fonts
#VERSIONDIR ?= $(SRCDIR)/version

include $(CEDEV)/include/.makefile

# This is a roundabout way to tell make that fonts.c depends on testfont.inc.
# It does it by saying the compiled object code depends on the .inc file.
$(OBJDIR)/fonts/fonts.src: $(FONTDIR)/testfont.inc
#$(OBJDIR)/version/version.src: $(VERSIONDIR)/version.h

#$(VERSIONDIR)/version.h:
#	echo \#define SHA > $(VERSIONDIR)/version.h
#	git rev-parse HEAD >> $(VERSIONDIR)/version.h

$(FONTDIR)/testfont.inc: $(FONTDIR)/testfont.fnt
	$(call MKDIR,$(@D))
	convfont -o carray -f $^ -a 1 -b 1 -w bold -c 2 -x 9 -l 0x0B -Z $@
