################################################################################
#
# fft_module
#
################################################################################

FFT_MODULE_VERSION = 1.0
FFT_MODULE_SITE = $(TOPDIR)/../fft_module
FFT_MODULE_SITE_METHOD = local
FFT_MODULE_MODULE_SUBDIRS = .
FFT_MODULE_LICENSE = GPL-2.0

$(eval $(kernel-module))
$(eval $(generic-package))
