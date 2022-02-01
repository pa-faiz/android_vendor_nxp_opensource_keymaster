# Build Keymaster open source vendor modules
ESE_VENDOR_WEAVER += android.hardware.weaver@1.0-service

ifeq ($(strip $(ENABLE_ESE_WEAVER_SPLIT_MANIFEST)),true)
ESE_VENDOR_WEAVER += android.hardware.weaver@1.0-service.xml
endif

ifeq ($(strip $(TARGET_USES_ESE_WEAVER)),true)
PRODUCT_PACKAGES += $(ESE_VENDOR_WEAVER)
endif
