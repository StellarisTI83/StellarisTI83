# ----------------------------
# Makefile Options
# ----------------------------

NAME = STELLARI
ICON = icon.png
DESCRIPTION = "Stellaris for TI83PCE"
COMPRESSED = YES
ARCHIVED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk

# This is just so the main makefile in $CEDEV/examples makes this one be like "make debug"
.DEFAULT_GOAL := debug

FONTDIR ?= $(SRCDIR)/fonts
#VERSIONDIR ?= $(SRCDIR)/version


# This is a roundabout way to tell make that fonts.c depends on testfont.inc.
# It does it by saying the compiled object code depends on the .inc file.
$(OBJDIR)/fonts/fonts.src: $(FONTDIR)/testfont.inc
#$(OBJDIR)/version/version.src: $(VERSIONDIR)/version.h


$(FONTDIR)/testfont.inc: $(FONTDIR)/testfont.fnt
	$(call MKDIR,$(@D))
	convfont -o carray -f $^ -a 1 -b 1 -w bold -c 2 -x 9 -l 0x0B -Z $@
