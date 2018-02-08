################################################################################
#
# snapcaster-ui
#
################################################################################

SNAPCAST_VERSION = master
SNAPCAST_SITE = $(call github,laszloh,snapcaster-ui,$(SNAPCAST_VERSION))
SNAPCAST_DEPENDENCIES = avahi 
SNAPCAST_LICENSE = GPL-3.0+
SNAPCAST_LICENSE_FILES = LICENSE

define SNAPCASTER_UI_INSTALL_INIT_SYSV
	$(INSTALL) -m 0755 -D $(SNAPCASTER_UI_PKGDIR)/S99snapcaster-ui $(TARGET_DIR)/etc/init.d/S99snapcaster-ui
endef

$(eval $(cmake-package))