LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := NE10_test_demo
LOCAL_SRC_FILES := NE10_test_demo.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../inc
LOCAL_CFLAGS := -L
LOCAL_LDLIBS := -lNE10_test

include $(BUILD_SHARED_LIBRARY)
