LOCAL_PATH:= $(call my-dir)

ne10_neon_source := \
    modules/math/NE10_abs.neon.s \
    modules/math/NE10_addc.neon.c \
    modules/math/NE10_addmat.neon.c \
    modules/math/NE10_add.neon.s \
    modules/math/NE10_cross.neon.s \
    modules/math/NE10_detmat.neon.s \
    modules/math/NE10_divc.neon.c \
    modules/math/NE10_div.neon.s \
    modules/math/NE10_dot.neon.s \
    modules/math/NE10_identitymat.neon.s \
    modules/math/NE10_invmat.neon.s \
    modules/math/NE10_len.neon.s \
    modules/math/NE10_mla.neon.s \
    modules/math/NE10_mlac.neon.c \
    modules/math/NE10_mulcmatvec.neon.s \
    modules/math/NE10_mulc.neon.c \
    modules/math/NE10_mulmat.neon.s \
    modules/math/NE10_mul.neon.c \
    modules/math/NE10_normalize.neon.s \
    modules/math/NE10_rsbc.neon.c \
    modules/math/NE10_setc.neon.c \
    modules/math/NE10_subc.neon.c \
    modules/math/NE10_submat.neon.c \
    modules/math/NE10_sub.neon.s \
    modules/math/NE10_transmat.neon.s \

ne10_source_files := \
    modules/math/NE10_abs.asm.s \
    modules/math/NE10_addc.asm.s \
    modules/math/NE10_addmat.asm.s \
    modules/math/NE10_add.asm.s \
    modules/math/NE10_cross.asm.s \
    modules/math/NE10_detmat.asm.s \
    modules/math/NE10_divc.asm.s \
    modules/math/NE10_div.asm.s \
    modules/math/NE10_dot.asm.s \
    modules/math/NE10_identitymat.asm.s \
    modules/math/NE10_invmat.asm.s \
    modules/math/NE10_len.asm.s \
    modules/math/NE10_mla.asm.s \
    modules/math/NE10_mlac.asm.s \
    modules/math/NE10_mulcmatvec.asm.s \
    modules/math/NE10_mulc.asm.s \
    modules/math/NE10_mulmat.asm.s \
    modules/math/NE10_mul.asm.s \
    modules/math/NE10_normalize.asm.s \
    modules/math/NE10_rsbc.asm.s \
    modules/math/NE10_setc.asm.s \
    modules/math/NE10_subc.asm.s \
    modules/math/NE10_submat.asm.s \
    modules/math/NE10_sub.asm.s \
    modules/math/NE10_transmat.asm.s \
    modules/math/NE10_abs.c \
    modules/math/NE10_addc.c \
    modules/math/NE10_addmat.c \
    modules/math/NE10_add.c \
    modules/math/NE10_cross.c \
    modules/math/NE10_detmat.c \
    modules/math/NE10_divc.c \
    modules/math/NE10_div.c \
    modules/math/NE10_dot.c \
    modules/math/NE10_identitymat.c \
    modules/math/NE10_invmat.c \
    modules/math/NE10_len.c \
    modules/math/NE10_mla.c \
    modules/math/NE10_mlac.c \
    modules/math/NE10_mulcmatvec.c \
    modules/math/NE10_mulc.c \
    modules/math/NE10_mulmat.c \
    modules/math/NE10_mul.c \
    modules/math/NE10_normalize.c \
    modules/math/NE10_rsbc.c \
    modules/math/NE10_setc.c \
    modules/math/NE10_subc.c \
    modules/math/NE10_submat.c \
    modules/math/NE10_sub.c \
    modules/math/NE10_transmat.c \

include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=     $(LOCAL_PATH)/common/ \
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

LOCAL_C_INCLUDES :=     $(LOCAL_PATH)/common/ \
                        $(LOCAL_PATH)/inc

LOCAL_SRC_FILES :=  \
    $(ne10_source_files)

ifeq ($(ARCH_ARM_HAVE_NEON),true)
LOCAL_SRC_FILES += $(ne10_neon_source)
endif

LOCAL_MODULE_TAGS := eng
LOCAL_MODULE := libne10

include $(BUILD_SHARED_LIBRARY)



