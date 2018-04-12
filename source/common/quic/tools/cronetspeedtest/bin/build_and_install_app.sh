#!/bin/sh

# Run this script from google3/ in the google3 repo.

SPEEDTEST_DIR=$SPEEDTEST_DIR_PLACEHOLDER

blaze build -c opt --android_cpu=arm64-v8a gfe/quic/tools/cronetspeedtest/android:cronetspeedtest
adb install -r blaze-bin/gfe/quic/tools/cronetspeedtest/android/cronetspeedtest.apk
adb shell cmd package compile -f -m speed gfe.quic.tools.cronetspeedtest.android

# Copy apk to SPEEDTEST_DIR for use by simpleperf
mkdir -p "$SPEEDTEST_DIR/apk/"
rm -f "$SPEEDTEST_DIR/apk/cronetspeedtest.apk"
cp "blaze-bin/gfe/quic/tools/cronetspeedtest/android/cronetspeedtest.apk" "$SPEEDTEST_DIR/apk/"
