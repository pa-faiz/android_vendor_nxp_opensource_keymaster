# Build Keymaster open source vendor modules
ESE_VENDOR_WEAVER += android.hardware.weaver-service.nxp-qti

ifeq ($(strip $(ENABLE_ESE_WEAVER_SPLIT_MANIFEST)),true)
ESE_VENDOR_WEAVER += android.hardware.weaver-service.nxp.xml
endif

ifeq ($(strip $(TARGET_USES_ESE_WEAVER)),true)
PRODUCT_PACKAGES += $(ESE_VENDOR_WEAVER)
endif
