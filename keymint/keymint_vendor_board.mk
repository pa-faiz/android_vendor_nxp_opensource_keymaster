#Kalama specific build rules
ifeq ($(TARGET_BOARD_PLATFORM),kalama)
TARGET_USES_ESE_KEYMINT := false
ENABLE_ESE_KEYMINT_SPLIT_MANIFEST := false
ENABLE_ESE_KEYMINT_FEATURE_XML := false
endif
