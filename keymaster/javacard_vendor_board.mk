#Taro specific build rules
ifeq ($(TARGET_BOARD_PLATFORM),taro)
TARGET_USES_ESESB := true
ENABLE_SB_SPLIT_MANIFEST := false
endif
