# ----------------------------
# agondev Makefile
# ----------------------------

NAME = tmstest
DESCRIPTION = "Agon TMS9918 board driver test"

FAE_HOME = ~/agon/fab/

# ----------------------------
include $(shell agondev-config --makefile)

install: bin/$(NAME).bin
	cp bin/$(NAME).bin $(FAE_HOME)/sdcard/$(NAME).bin
