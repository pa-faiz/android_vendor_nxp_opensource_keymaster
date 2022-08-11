# Build Keymaster open source vendor modules
ESE_VENDOR_SB := libese_transport
ESE_VENDOR_SB += libJavacardKeymaster41
ESE_VENDOR_SB += android.hardware.keymaster@4.1-javacard.service
ESE_VENDOR_SB += init.qti.ese.strongbox.sh

ifeq ($(strip $(ENABLE_SB_SPLIT_MANIFEST)),true)
ESE_VENDOR_SB += android.hardware.keymaster@4.1-javacard.service.xml
endif

ifeq ($(strip $(TARGET_USES_ESESB)),true)
PRODUCT_PACKAGES += $(ESE_VENDOR_SB)
ifeq ($(strip $(ENABLE_SB_FEATURE_XML)),true)
PRODUCT_COPY_FILES += vendor/nxp/opensource/keymaster/keymaster/android.hardware.strongbox_keystore.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.strongbox_keystore.xml
endif
endif
