
C_TOOL    = gcc
EXE_TOOL    = gcc
ASM_TOOL    = as

#BJ_FLAGS   = -mthumb-interwork -march=armv7-a -mcpu=cortex-a9 -mfpu=vfp3
ARM_FLAGS   = -mthumb-interwork -march=armv7-a -mcpu=cortex-a9
C_FLAGS     = -lm -lrt -I./inc/
#DEBUG_FLAGS = -gstabs
OPTIMIZE_FLAGS = -O3
# -save-temps -O3

LDFLAGS+=-L.  -L/usr/local/lib -L/client/lib -L/lib/arm-linux-gnueabi
LDFLAGS+=-lm

#TARGET_ARCH = stdc

.PHONY: all clean

all : NE10_addc.test_r.ex

clean:
	./cleanall.sh

%.test_r.ex : %.asm_r.o %.c_r.o %.neon_r.o ./source/%_test.c  ./inc/NE10.h
		$(EXE_TOOL) $(OPTIMIZE_FLAGS) $(ARM_FLAGS) $^ -o $@ $(C_FLAGS) -L/lib/arm-linux-gnueabi
 
%.c_r.o : ./source/%.c ./inc/NE10.h
		$(C_TOOL) $(OPTIMIZE_FLAGS) $(ARM_FLAGS) -mfpu=vfp3 -c $< -o $@ $(C_FLAGS) -L/lib/arm-linux-gnueabi 

%.asm_r.o : ./source/%.asm.s
		$(ASM_TOOL) $(ARM_FLAGS) -mfpu=vfp3 $< -o $@

# Either use the C version or use the Assembly version for compiling the NEON routines

# Rules for the Assembly version
%.neon_r.o : ./source/%.neon.s
		$(ASM_TOOL) $(ARM_FLAGS) -mfpu=neon $< -o $@

# Rules for the C version
%.neon_r.o : ./source/%.neon.c ./inc/NE10.h
		$(C_TOOL) $(OPTIMIZE_FLAGS) $(ARM_FLAGS) -mfpu=neon -c $< -o $@ $(C_FLAGS)

