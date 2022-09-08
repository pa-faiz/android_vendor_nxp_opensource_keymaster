#Kalama specific build rules
ifeq ($(TARGET_BOARD_PLATFORM),kalama)
TARGET_USES_ESESB := false
ENABLE_SB_SPLIT_MANIFEST := false
ENABLE_SB_FEATURE_XML := false
endif
