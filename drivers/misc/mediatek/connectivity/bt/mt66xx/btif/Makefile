###############################################################################
# Bluetooth character device driver

###############################################################################

# ---------------------------------------------------
# Compile Options
# ---------------------------------------------------

ifndef TOP
    TOP := $(srctree)/..
endif

ifneq ($(KERNEL_OUT),)
    ccflags-y += -imacros $(KERNEL_OUT)/include/generated/autoconf.h
endif

# Force build fail on modpost warning
KBUILD_MODPOST_FAIL_ON_WARNINGS := y

# platform

ifeq ($(CONFIG_WLAN_DRV_BUILD_IN),y)
  $(info build-in mode!)
  $(info _MTK_BT_CHIP = $(_MTK_BT_CHIP))
  # _MTK_BT_CHIP comes from conninfra setting
  BT_PLATFORM = $(patsubst MTK_CONSYS_MT%,%,$(strip $(_MTK_BT_CHIP)))
endif


$(info $(LOG_TAG) TARGET_BUILD_VARIANT = $(TARGET_BUILD_VARIANT))
ifeq ("$(TARGET_BUILD_VARIANT)","user")
    ccflags-y += -D FW_LOG_DEFAULT_ON=0
else
    ccflags-y += -D FW_LOG_DEFAULT_ON=1
endif


ccflags-y += -D CONNAC20_CHIPID=$(BT_PLATFORM)
$(info $(LOG_TAG) BT_PLATFORM = $(BT_PLATFORM))
$(info $(LOG_TAG) srctree = $(srctree))

# ---------------------------------------------------
# Compile Options: set feature flag (1: enable, 0: disable)
# ---------------------------------------------------

# build btif interface
ccflags-y += -D CHIP_IF_BTIF

# Use device node or hci_dev as native interface
ccflags-y += -D USE_DEVICE_NODE=1

# Customized fw update feature
#ifndef OPLUS_FEATURE_BT_FW_SAU_MTK
#Add for fw sau
#ccflags-y += -D CUSTOMER_FW_UPDATE=0

#else /* OPLUS_FEATURE_BT_FW_SAU_MTK */
ccflags-y += -D CUSTOMER_FW_UPDATE=1
ccflags-y += -D OPLUS_FEATURE_BT_FW_SAU_MTK=1
#endif /* OPLUS_FEATURE_BT_FW_SAU_MTK */

# pm_qos control
ccflags-y += -D PM_QOS_CONTROL=0

# No function, only for build pass
ccflags-y += -D CONFIG_MP_WAKEUP_SOURCE_SYSFS_STAT=1

# Customized feature, load 1b fw bin
#ccflags-y += -D BT_CUS_FEATURE

# ---------------------------------------------------
# Include Path
# ---------------------------------------------------
CONN_INFRA_SRC := $(srctree)/drivers/misc/mediatek/connectivity/conninfra
CONNFEM_SRC := $(srctree)/drivers/misc/mediatek/connectivity/connfem
WMT_SRC := $(srctree)/drivers/misc/mediatek/connectivity/common
BTIF_SRC := $(srctree)/drivers/misc/mediatek/btif
ccflags-y += -I$(srctree)/drivers/misc/mediatek/connectivity/common
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/connectivity/power_throttling
ccflags-y += -I$(srctree)/drivers/gpu/drm/mediatek/mediatek_v2/
ccflags-y += -I$(CONN_INFRA_SRC)/include
ccflags-y += -I$(CONN_INFRA_SRC)/debug_utility/include
ccflags-y += -I$(CONN_INFRA_SRC)/debug_utility/metlog
ccflags-y += -I$(CONN_INFRA_SRC)/debug_utility/
ccflags-y += -I$(CONNFEM_SRC)/include
ccflags-y += -I$(WMT_SRC)/debug_utility
ccflags-y += -I$(BTIF_SRC)/common/inc

ccflags-y += -I$(src)/core/include
ccflags-y += -I$(src)/connsys/connac_2_0
ccflags-y += -I$(src)/../include
ccflags-y += -I$(src)/../include/btif

# ---------------------------------------------------
# Objects List
# ---------------------------------------------------
MODULE_NAME := bt_drv_$(BT_PLATFORM)
ifeq ($(CONFIG_WLAN_DRV_BUILD_IN),y)
  obj-y += $(MODULE_NAME).o
else
  obj-m += $(MODULE_NAME).o
endif

CORE_OBJS := btmtk_dbg.o btmtk_dbg_tp_evt_if.o btmtk_irq.o btmtk_char_dev.o ../btmtk_fw_log.o ../btmtk_main.o
CHIP_OBJS := btmtk_mt66xx.o
HIF_OBJS := btmtk_btif_main.o btmtk_queue.o

$(MODULE_NAME)-objs  += $(CORE_OBJS)
$(MODULE_NAME)-objs  += $(HIF_OBJS)
$(MODULE_NAME)-objs  += $(CHIP_OBJS)
