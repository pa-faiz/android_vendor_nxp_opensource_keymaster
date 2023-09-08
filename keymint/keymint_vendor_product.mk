# Build Keymaster open source vendor modules
ESE_VENDOR_KEYMINT += android.hardware.security.keymint-service.strongbox-nxp

ifeq ($(strip $(ENABLE_ESE_KEYMINT_SPLIT_MANIFEST)),true)
ESE_VENDOR_KEYMINT += android.hardware.security.keymint-service.strongbox.xml
ESE_VENDOR_KEYMINT += android.hardware.security.sharedsecret-service.strongbox.xml
endif

ifeq ($(strip $(ENABLE_ESE_KEYMINT_FEATURE_XML)),true)
ESE_VENDOR_KEYMINT += android.hardware.strongbox_keystore.xml
endif

ifeq ($(strip $(TARGET_USES_ESE_KEYMINT)),true)
PRODUCT_PACKAGES += $(ESE_VENDOR_KEYMINT)
endif
