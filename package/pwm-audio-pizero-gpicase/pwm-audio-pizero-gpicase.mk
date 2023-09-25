################################################################################
#
# pwm-audio-pizero-gpicase
#
################################################################################

PWM_AUDIO_PIZERO_GPICASE_VERSION = 2e7aed359fcc537cf085f021242f5c9e692d8aaf
PWM_AUDIO_PIZERO_GPICASE_SITE = $(call github,digitalLumberjack,pwm-audio-pizero-gpicase,$(PWM_AUDIO_PIZERO_GPICASE_VERSION))
PWM_AUDIO_PIZERO_GPICASE_LICENSE = GPL-2.0
PWM_AUDIO_PIZERO_GPICASE_LICENSE_FILES = LICENSE

PWM_AUDIO_PIZERO_GPICASE_DEPENDENCIES += linux

define PWM_AUDIO_PIZERO_GPICASE_BUILD_CMDS
	$(HOST_DIR)/bin/linux-dtc $(@D)/pwm-audio-pizero-gpicase.dts -o $(@D)/pwm-audio-pizero-gpicase.dtbo
endef

define PWM_AUDIO_PIZERO_GPICASE_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0644 $(@D)/pwm-audio-pizero-gpicase.dtbo $(BINARIES_DIR)/boot-data/overlays/pwm-audio-pizero-gpicase.dtbo
endef

$(eval $(generic-package))
