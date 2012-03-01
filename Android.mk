LOCAL_PATH:= $(call my-dir)

ne10_neon_source := \
    source/NE10_setc.neon.c \
    source/NE10_mulc.neon.c \
    source/NE10_mla.neon.c \
    source/NE10_subc.neon.c \
    source/NE10_addc.neon.c \
    source/NE10_normalize.neon.s \
    source/NE10_mlac.neon.c \
    source/NE10_abs.neon.c \
    source/NE10_div.neon.c \
    source/NE10_add.neon.c \
    source/NE10_divc.neon.c \
    source/NE10_mul.neon.c \
    source/NE10_len.neon.s \
    source/NE10_sub.neon.c \
    source/NE10_rsbc.neon.c \

ne10_source_files := \
    source/NE10_subc.c \
    source/NE10_add.asm.s \
    source/NE10_rsbc.asm.s \
    source/NE10_addc.c \
    source/NE10_setc.c \
    source/NE10_subc.asm.s \
    source/NE10_rsbc.c \
    source/NE10_mla.asm.s \
    source/NE10_mlac.c \
    source/NE10_setc.asm.s \
    source/NE10_mul.asm.s \
    source/NE10_addc.asm.s \
    source/NE10_mul.c \
    source/NE10_mulc.c \
    source/NE10_mulc.asm.s \
    source/NE10_mla.c \
    source/NE10_mlac.asm.s \
    source/NE10_div.asm.s \
    source/NE10_div.c \
    source/NE10_normalize.asm.s \
    source/NE10_len.c \
    source/NE10_len.asm.s \
    source/NE10_abs.asm.s \
    source/NE10_sub.c \
    source/NE10_abs.c \
    source/NE10_add.c \
    source/NE10_divc.asm.s \
    source/NE10_divc.c \
    source/NE10_sub.asm.s \
    source/NE10_normalize.c 

include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=     $(LOCAL_PATH)/headers/ \
                        $(LOCAL_PATH)/inc 

LOCAL_SRC_FILES :=  \
    $(ne10_source_files)

ifeq ($(ARCH_ARM_HAVE_NEON),true)
LOCAL_SRC_FILES += $(ne10_neon_source)
endif

LOCAL_CFLAGS := -D_ARM_ASSEM_

LOCAL_ARM_MODE := arm

LOCAL_MODULE_TAGS := eng
LOCAL_MODULE := libne10

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cc

LOCAL_CFLAGS := -D_ARM_ASSEM_

LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES :=     $(LOCAL_PATH)/headers/ \
                        $(LOCAL_PATH)/inc 

LOCAL_SRC_FILES :=  \
    $(ne10_source_files)

ifeq ($(ARCH_ARM_HAVE_NEON),true)
LOCAL_SRC_FILES += $(ne10_neon_source)
endif

LOCAL_MODULE_TAGS := eng
LOCAL_MODULE := libne10

include $(BUILD_SHARED_LIBRARY)



