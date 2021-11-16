# Corstone1000 machines specific TFM support

COMPATIBLE_MACHINE = "(corstone1000)"

TFM_DEBUG = "1"

# Default is the FVP
TFM_PLATFORM_IS_FVP ?= "TRUE"
EXTRA_OECMAKE += "-DPLATFORM_IS_FVP=${TFM_PLATFORM_IS_FVP}"

SRCBRANCH_tfm = "master"
SRCREV_tfm = "ccd82e35f539c0d7261b2935d6d30c550cfc6736"

# The install task signs the TF-A BL2 and FIP binaries.
# So they need to be copied to the sysroot. Hence the dependencies below:
do_prepare_recipe_sysroot[depends]+= "virtual/trusted-firmware-a:do_populate_sysroot"

# adding host images signing support
require trusted-firmware-m-sign-host-images.inc

do_install() {
  install -D -p -m 0644 ${B}/install/outputs/ARM/CORSTONE1000/tfm_s_signed.bin ${D}/firmware/tfm_s_signed.bin
  install -D -p -m 0644 ${B}/install/outputs/ARM/CORSTONE1000/bl2_signed.bin ${D}/firmware/bl2_signed.bin
  install -D -p -m 0644 ${B}/install/outputs/ARM/CORSTONE1000/bl1.bin ${D}/firmware/bl1.bin

  #
  # Signing TF-A BL2 and the FIP image
  #

  sign_host_image ${TFA_BL2_BINARY} ${RECIPE_SYSROOT}/firmware ${TFA_BL2_RE_IMAGE_LOAD_ADDRESS} ${TFA_BL2_RE_SIGN_BIN_SIZE}

  fiptool update \
      --tb-fw ${D}/firmware/signed_${TFA_BL2_BINARY} \
      ${RECIPE_SYSROOT}/firmware/${TFA_FIP_BINARY}

  sign_host_image ${TFA_FIP_BINARY} ${RECIPE_SYSROOT}/firmware ${TFA_FIP_RE_IMAGE_LOAD_ADDRESS} ${TFA_FIP_RE_SIGN_BIN_SIZE}
}