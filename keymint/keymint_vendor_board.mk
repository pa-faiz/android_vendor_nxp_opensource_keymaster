#pineapple specific build rules
ifeq ($(TARGET_BOARD_PLATFORM),pineapple)
TARGET_USES_ESE_KEYMINT := false
ENABLE_ESE_KEYMINT_SPLIT_MANIFEST := false
endif
