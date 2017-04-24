#**********************************************************#
#file     makefile
#author   Rajmund Szymanski
#date     23.04.2017
#brief    STM8 makefile.
#**********************************************************#

COSMIC     := c:/sys/cosmic/cxstm8/
GNUCC      := c:/sys/gcc/bin/arm-none-eabi-
STVP       := c:/sys/tools/stvp/stvp_cmdline -BoardName=ST-LINK -Port=USB -ProgMode=SWIM -Device=STM8S105x6 -verif -no_loop -no_log

#----------------------------------------------------------#

PROJECT    ?=
DEFS       ?=
LIBS       ?=
DIRS       ?=
KEYS       ?=
INCS       ?=
SCRIPT     ?=

#----------------------------------------------------------#

DEFS       += STM8S105
KEYS       += .csmc *
LIBS       += crtsx0 libisl0 libm0
MODEL      := modsl0

#----------------------------------------------------------#

AS         := $(COSMIC)cxstm8
CC         := $(COSMIC)cxstm8
LD         := $(COSMIC)clnk
AR         := $(COSMIC)clib
DUMP       := $(COSMIC)chex
COPY       := $(COSMIC)cvdwarf
LABS       := $(COSMIC)clabs
SIZE       := $(GNUCC)size

RM         ?= rm -f

#----------------------------------------------------------#

DTREE       = $(foreach d,$(foreach k,$(KEYS),$(wildcard $1$k)),$(dir $d) $(call DTREE,$d/))

VPATH      := $(sort $(call DTREE,) $(foreach d,$(DIRS),$(call DTREE,$d/)))

#----------------------------------------------------------#

INC_DIRS   := $(sort $(dir $(foreach d,$(VPATH),$(wildcard $d*.h))))
LIB_DIRS   := $(sort $(dir $(foreach d,$(VPATH),$(wildcard $d*.sm8))))
AS_SRCS    :=              $(foreach d,$(VPATH),$(wildcard $d*.s))
CC_SRCS    :=              $(foreach d,$(VPATH),$(wildcard $d*.c))
LIB_SRCS   :=     $(notdir $(foreach d,$(VPATH),$(wildcard $d*.sm8)))
ifeq ($(strip $(SCRIPT)),)
SCRIPT     :=  $(firstword $(foreach d,$(VPATH),$(wildcard $d*.lkf)))
else
SCRIPT     :=  $(firstword $(foreach d,$(VPATH),$(wildcard $d$(SCRIPT))))
endif
ifeq ($(strip $(PROJECT)),)
PROJECT    :=     $(notdir $(CURDIR))
endif

#----------------------------------------------------------#

BIN        := $(PROJECT).sm8
ELF        := $(PROJECT).elf
HEX        := $(PROJECT).s19
LIB        := $(PROJECT).sm8
MAP        := $(PROJECT).map

OBJS       := $(AS_SRCS:%.s=%.o)
OBJS       += $(CC_SRCS:%.c=%.o)
LSTS       := $(OBJS:.o=.ls)
TXTS       := $(OBJS:.o=.la)

#----------------------------------------------------------#

AS_FLAGS   := #-ax
AS_FLAGS   += -ep -l
CC_FLAGS   := #+debug
CC_FLAGS   += #+strict +warn
CC_FLAGS   += +$(MODEL) -pc99 +compact -l
LD_FLAGS   := -m $(MAP)

#----------------------------------------------------------#

DEFS_F     := $(DEFS:%=-d%)

INC_DIRS   += $(INCS:%=%/)
INC_DIRS_F := $(INC_DIRS:%=-i%)

LIB_DIRS_F := $(LIB_DIRS:%=-l%)
LIBS_F     := $(LIBS:%=%.sm8)
LIBS_F     += $(LIB_SRCS:$(BIN)=)

CC_FLAGS   += $(DEFS_F) $(INC_DIRS_F)
LD_FLAGS   += $(LIB_DIRS_F)

#----------------------------------------------------------#

all : $(ELF) print_elf_size

lib : $(OBJS)
	$(info Creating library: $(LIB))
	$(AR) -c -p $(LIB) $?

$(BIN) : $(MAKEFILE_LIST) $(OBJS) $(SCRIPT)
	$(info Creating BIN image: $(BIN))
ifeq ($(strip $(SCRIPT)),)
	$(error No linker file in project)
endif
	$(LD) -o $@ $(LD_FLAGS) $(SCRIPT) $(OBJS) $(LIBS_F)

%.o : %.s
	$(AS) $(AS_FLAGS) $<

%.o : %.c
	$(CC) $(CC_FLAGS) $<

$(HEX) : $(BIN)
	$(info Creating HEX image: $(HEX))
	$(DUMP) -o $@ $<

$(ELF) : $(BIN)
	$(info Creating ELF image: $(ELF))
	$(COPY) $<
#	$(info Creating absolute listings)
#	$(LABS) $<

print_elf_size : $(ELF)
	$(info Size of target file:)
	$(SIZE) -B $(ELF)

GENERATED = $(BIN) $(ELF) $(HEX) $(LIB) $(MAP) $(LSTS) $(OBJS) $(TXTS)

clean :
	$(info Removing all generated output files)
	$(RM) $(GENERATED)

flash : all $(HEX)
	$(info Programing device...)
	$(STVP) -FileProg=$(HEX)

.PHONY : all lib clean flash
