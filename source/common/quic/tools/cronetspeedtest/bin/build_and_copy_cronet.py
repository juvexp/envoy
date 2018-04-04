#!/usr/bin/python
"""Builds cronet and copies compiled lib files to google3 repo."""

# Run this script from google3/ in the google3 repo.

import glob
import os
import re
import shutil
import subprocess
import sys

CLANKIUM_SRC_DIR = 'CLANKIUM_SRC_DIR_PLACEHOLDER'
CRONET_SRC_DIR = './third_party/java/android_libs/cronet/current'


def copy(src, dst):
  shutil.copy(src, dst)
  print 'Copied %s to %s' % (src, dst)


def main():
  subprocess.check_call([
      'autoninja', '-C',
      os.path.join(CLANKIUM_SRC_DIR, 'out/cronetspeedtest_arm64'),
      'cronet_package'
  ])
  subprocess.check_call([
      'autoninja', '-C',
      os.path.join(CLANKIUM_SRC_DIR, 'out/cronetspeedtest_armeabi-v7a'),
      'cronet_package'
  ])

  # Determine which cronet version was built. If multiple versions were built,
  # just pick one.
  libcronet_filepath = glob.glob(
      os.path.join(
          CLANKIUM_SRC_DIR,
          'out/cronetspeedtest_arm64/lib.unstripped/libcronet.*.so'))[0]
  libcronet_filename = os.path.basename(libcronet_filepath)

  # Modify the cronet lib BUILD file with the name of the libcronet file that
  # was built.
  with open(os.path.join(CRONET_SRC_DIR, 'BUILD'), 'rt') as f:
    lines = f.readlines()
  with open(os.path.join(CRONET_SRC_DIR, 'BUILD'), 'wt') as f:
    regexp = re.compile('libcronet\\..*\\.so')
    for line in lines:
      line = regexp.sub(lambda match: libcronet_filename, line)
      f.write(line)

  # Copy compiled libcronet files
  copy(
      os.path.join(CLANKIUM_SRC_DIR, 'out/cronetspeedtest_arm64/lib.unstripped',
                   libcronet_filename), os.path.join(CRONET_SRC_DIR,
                                                     'arm64-v8a'))

  copy(
      os.path.join(CLANKIUM_SRC_DIR,
                   'out/cronetspeedtest_armeabi-v7a/lib.unstripped',
                   libcronet_filename),
      os.path.join(CRONET_SRC_DIR, 'armeabi-v7a'))

  for f in glob.glob(
      os.path.join(CLANKIUM_SRC_DIR, 'out/cronetspeedtest_arm64/cronet/*.jar')):
    copy(f, CRONET_SRC_DIR)


if __name__ == '__main__':
  sys.exit(main())
