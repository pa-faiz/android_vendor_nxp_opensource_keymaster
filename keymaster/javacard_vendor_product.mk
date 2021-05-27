# Build Keymaster open source vendor modules
ESE_VENDOR_SB := libese_transport
ESE_VENDOR_SB += libJavacardKeymaster41
ESE_VENDOR_SB += android.hardware.keymaster@4.1-javacard.service

ifeq ($(strip $(TARGET_USES_ESESB)),true)
PRODUCT_PACKAGES += $(ESE_VENDOR_SB)
endif
