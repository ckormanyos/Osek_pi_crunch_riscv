# ******************************************************************************************
#   Filename    : Makefile
#
#   Author      : Chalandi Amine
#
#   Owner       : Chalandi Amine
#
#   Date        : 22.11.2022
#
#   Description : Build system
#
# ******************************************************************************************

############################################################################################
# Defines
############################################################################################

PRJ_NAME   = Osek_pi_crunch_riscv

OUTPUT_DIR = $(CURDIR)/Output
OBJ_DIR    = $(OUTPUT_DIR)/Obj
SRC_DIR    = $(CURDIR)/Code

CC_ERR_FORMAT_SCRIPT = CompilerErrorFormater.py
LD_SCRIPT            = $(SRC_DIR)/Memory_Map.ld

############################################################################################
# Toolchain
############################################################################################

TOOLCHAIN = riscv64-unknown-elf

AS        = $(TOOLCHAIN)-g++
CC        = $(TOOLCHAIN)-g++
CPP       = $(TOOLCHAIN)-g++
CPPFILT   = $(TOOLCHAIN)-c++filt
LD        = $(TOOLCHAIN)-g++
NM        = $(TOOLCHAIN)-nm
OBJDUMP   = $(TOOLCHAIN)-objdump
OBJCOPY   = $(TOOLCHAIN)-objcopy
READELF   = $(TOOLCHAIN)-readelf

PYTHON = python3

############################################################################################
# C Compiler flags
############################################################################################

OPT_MODIFIED_O2 = -O2                                             \
                  -fno-reorder-blocks-and-partition               \
                  -fno-reorder-functions

NO_OPT = -O0

OPT = $(OPT_MODIFIED_O2)

OPS_BASE     = -Wall                                          \
               -Wextra                                        \
               -Wpedantic                                     \
               -Wconversion                                   \
               -Wsign-conversion                              \
               -Wshadow                                       \
               -mcpu=sifive-e31                               \
               -mabi=ilp32                                    \
               -msmall-data-limit=0                           \
               -falign-functions=4                            \
               -ffast-math                                    \
               -fomit-frame-pointer                           \
               -fno-exceptions                                \
               -Wa,-adhln=$(OBJ_DIR)/$(basename $(@F)).lst    \
               -g3                                            \
               -gdwarf-2

COPS         = -x c                                           \
               -std=c99                                       \
               $(OPT)                                         \
               $(OPS_BASE)                                    \
               -fno-inline-functions

############################################################################################
# C++ Compiler flags
############################################################################################

CPPOPS       = -x c++                                         \
               $(OPT)                                         \
               $(OPS_BASE)                                    \
               -std=c++14                                     \
               -fno-rtti                                      \
               -fno-use-cxa-atexit                            \
               -fno-nonansi-builtins                          \
               -fno-threadsafe-statics                        \
               -finline-functions                             \
               -finline-limit=32                              \
               -fno-enforce-eh-specs                          \
               -ftemplate-depth=128                           \
               -Wzero-as-null-pointer-constant

############################################################################################
# Assembler flags
############################################################################################

ifeq ($(AS), $(TOOLCHAIN)-as)
ASOPS        =  -march=rv32imac                                         \
         -      alh 
else
ASOPS        = -x assembler                                   \
               $(OPS_BASE)
endif

############################################################################################
# Linker flags
############################################################################################

LOPS         = -x none                                              \
               -nostartfiles                                        \
               -nostdlib                                            \
               -specs=nano.specs                                    \
               -specs=nosys.specs                                   \
               -e Startup_Init                                      \
               $(OPS_BASE)                                          \
               -Wl,--print-memory-usage                             \
               -Wl,-Map,$(OUTPUT_DIR)/$(PRJ_NAME).map               \
               -T $(LD_SCRIPT)


############################################################################################
# Source Files
############################################################################################
SRC_FILES := $(SRC_DIR)/Mcal/mtimer.c                                                                 \
             $(SRC_DIR)/Mcal/Clock.c                                                                  \
             $(SRC_DIR)/Mcal/Mcu.c                                                                    \
             $(SRC_DIR)/Startup/boot.s                                                                \
             $(SRC_DIR)/Startup/intvect.c                                                             \
             $(SRC_DIR)/Startup/Startup.c                                                             \
             $(SRC_DIR)/main.c                                                                        \
             $(SRC_DIR)/tasks.c                                                                       \
             $(SRC_DIR)/ref_app/src/mcal/mcal_gcc_cxx_completion                                      \
             $(SRC_DIR)/ref_app/src/mcal/riscvfe310/mcal_irq.cpp                                      \
             $(SRC_DIR)/ref_app/src/mcal/riscvfe310/mcal_led.cpp                                      \
             $(SRC_DIR)/ref_app/target/micros/riscvfe310/startup/crt0.cpp                             \
             $(SRC_DIR)/ref_app/target/micros/riscvfe310/startup/crt0_init_ram.cpp                    \
             $(SRC_DIR)/ref_app/target/micros/riscvfe310/startup/crt1.cpp                             \
             $(SRC_DIR)/OSEK/HwPlatform/RISC-V/OsSwClz.s                                              \
             $(SRC_DIR)/OSEK/HwPlatform/RISC-V/OsAsm.s                                                \
             $(SRC_DIR)/OSEK/HwPlatform/RISC-V/OsHwSchedPrio.s                                        \
             $(SRC_DIR)/OSEK/HwPlatform/RISC-V/OsHwPltfm.c                                            \
             $(SRC_DIR)/OSEK/HwPlatform/RISC-V/OsDummy.asm                                            \
             $(SRC_DIR)/OSEK/OsAlarm.c                                                                \
             $(SRC_DIR)/OSEK/OsCore.c                                                                 \
             $(SRC_DIR)/OSEK/OsEvt.c                                                                  \
             $(SRC_DIR)/OSEK/OsSched.c                                                                \
             $(SRC_DIR)/OSEK/OsTask.c                                                                 \
             $(SRC_DIR)/OSEK/OsTcb.c                                                                  \
             $(SRC_DIR)/pi_spigot/pi_spigot.cpp

############################################################################################
# Include Paths
############################################################################################
INC_FILES := $(SRC_DIR)/OSEK/HwPlatform/RISC-V                                                        \
             $(SRC_DIR)/OSEK                                                                          \
             $(SRC_DIR)/Mcal                                                                          \
             $(SRC_DIR)/ref_app/src                                                                   \
             $(SRC_DIR)/ref_app/src/mcal/riscvfe310                                                   \
             $(SRC_DIR)

############################################################################################
# Rules
############################################################################################

VPATH := $(subst \,/,$(sort $(dir $(SRC_FILES)) $(OBJ_DIR)))

FILES_O := $(addprefix $(OBJ_DIR)/, $(notdir $(addsuffix .o, $(basename $(SRC_FILES)))))


ifeq ($(MAKECMDGOALS),build)
-include $(subst .o,.d,$(FILES_O))
endif

all : clean $(OUTPUT_DIR)/$(PRJ_NAME).elf


.PHONY : clean
clean :
	@-rm -f $(OBJ_DIR)/*.o            2>/dev/null || true
	@-rm -f $(OBJ_DIR)/*.err          2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)            2>/dev/null || true
	@-mkdir -p $(subst \,/,$(OBJ_DIR))
	@-mkdir -p $(subst \,/,$(OUTPUT_DIR))

$(OBJ_DIR)/%.o : %.c
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@-$(CC) $(COPS) $(addprefix -I, $(INC_FILES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(PYTHON) $(CC_ERR_FORMAT_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

$(OBJ_DIR)/%.o : %.s
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@$(AS) $(ASOPS) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err >$(OBJ_DIR)/$(basename $(@F)).lst
	@-$(PYTHON) $(CC_ERR_FORMAT_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

$(OBJ_DIR)/%.o : %.asm
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@$(CC) -E -P $(addprefix -I, $(INC_FILES)) -x c $< > $(OBJ_DIR)/$(basename $(@F)).asm.pre
	@-$(CC) $(ASOPS) $(addprefix -I, $(INC_FILES)) -c $(OBJ_DIR)/$(basename $(@F)).asm.pre -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(PYTHON) $(CC_ERR_FORMAT_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

$(OBJ_DIR)/%.o : %.cpp
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@$(CPP) $(CPPOPS) $(addprefix -I, $(INC_FILES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(PYTHON) $(CC_ERR_FORMAT_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

$(OUTPUT_DIR)/$(PRJ_NAME).elf : $(FILES_O) $(LD_SCRIPT)
	@$(LD) $(LOPS) $(FILES_O) -o $(OUTPUT_DIR)/$(PRJ_NAME).elf
	@$(OBJCOPY) $(OUTPUT_DIR)/$(PRJ_NAME).elf -O ihex $(OUTPUT_DIR)/$(PRJ_NAME).hex
	@$(NM) --numeric-sort --print-size $(OUTPUT_DIR)/$(PRJ_NAME).elf | $(CPPFILT) > $(OUTPUT_DIR)/$(PRJ_NAME)_cppfilt.txt
