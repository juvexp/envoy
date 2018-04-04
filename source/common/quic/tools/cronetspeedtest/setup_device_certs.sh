#!/bin/bash
set -e

CERTS_DIR=$CERTS_DIR_PLACEHOLDER

# First remount /system as read-write
printf "Make sure Android device is connected via USB and USB debugging is enabled.\n"
printf "Press enter to continue...\n"
read
adb root
adb disable-verity
adb reboot
printf "\nRebooting device. Press enter when it finishes rebooting...\n"
read
printf "Installing certificate...\n"

# Install the cert
adb root
adb remount
adb push "$CERTS_DIR/f440b82c.0 /system/etc/security/cacerts/f440b82c.0"
printf "\nCertificate installed successfully.\n"
adb reboot
printf "Rebooting device.\n"
