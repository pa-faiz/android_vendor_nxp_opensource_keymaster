#Kalama specific build rules
ifeq ($(TARGET_BOARD_PLATFORM),kalama)
TARGET_USES_ESE_WEAVER := false
ENABLE_ESE_WEAVER_SPLIT_MANIFEST := false
endif
