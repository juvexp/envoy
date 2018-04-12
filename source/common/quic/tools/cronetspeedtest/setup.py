#!/usr/bin/python
"""Performs initial setup of an empty cronet speed test base directory."""

import argparse
import os
import re
import shutil
import socket
import stat
import subprocess
import sys
import tempfile

DEPOT_BASE_DIR = '/google/src/cloud/ckrasic/quic-dev/google3/'
DEPOT_SPEEDTEST_DIR = os.path.join(DEPOT_BASE_DIR,
                                   'gfe/quic/tools/cronetspeedtest')
DEPOT_RUN_BENCHMARKS_PY_DIR = os.path.join(DEPOT_BASE_DIR,
                                           'third_party/py/cronetspeedtest')
DEPOT_CERTS_DIR = os.path.join(DEPOT_SPEEDTEST_DIR, 'certs')
DEPOT_GFE2_CONFIG_PATH = os.path.join(DEPOT_SPEEDTEST_DIR,
                                      'gfe2_configs/gfe2_config.txt')
DEPOT_SCRIPTS_DIR = os.path.join(DEPOT_SPEEDTEST_DIR, 'bin')

FASTER_STATS_PORT_NUM = 6441
HTTP_PORT_NUM = 8888
HTTPS_PORT_NUM = 8443
PORT_NUM = 8443
NUM_GFES = 4


def chmod_x(path):
  st = os.stat(path)
  os.chmod(path, st.st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)


def mkdir(path):
  if not os.path.isdir(path):
    os.makedirs(path)


def rm_file(path):
  if os.path.isfile(path):
    os.remove(path)


def gen_file_replace_strings(src_path, dst_path, replacement_dict):
  regexp = re.compile('|'.join(map(re.escape, replacement_dict.keys())))
  with open(src_path, 'rt') as fin:
    with open(dst_path, 'wt') as fout:
      for line in fin:
        line = regexp.sub(lambda match: replacement_dict[match.group(0)], line)
        fout.write(line)


def main():
  parser = argparse.ArgumentParser()
  parser.add_argument(
      'clankium_src_dir', help="""Path to /src dir of clankium repo""")
  parser.add_argument(
      'simpleperf_scripts_dir', help="""Path to /scripts dir of simpleperf""")
  args = parser.parse_args()

  clankium_src_dir = os.path.abspath(args.clankium_src_dir)
  simpleperf_scripts_dir = os.path.abspath(args.simpleperf_scripts_dir)

  speedtest_dir = os.getcwd()
  certs_dir = os.path.abspath('certs')
  configs_dir = os.path.abspath('gfe2_configs')
  scripts_dir = os.path.abspath('bin')

  # Get hostname of this machine
  hostname = socket.gethostname()

  # Make local copy of host root cert and device cert
  mkdir(certs_dir)
  shutil.copy(os.path.join(DEPOT_CERTS_DIR, 'rootCA.key'), certs_dir)
  shutil.copy(os.path.join(DEPOT_CERTS_DIR, 'rootCA.pem'), certs_dir)
  shutil.copy(os.path.join(DEPOT_CERTS_DIR, 'f440b82c.0'), certs_dir)
  rm_file(os.path.join(certs_dir, 'rootCA.srl'))

  root_pem_path = os.path.join(certs_dir, 'rootCA.pem')
  root_key_path = os.path.join(certs_dir, 'rootCA.key')
  key_path = os.path.join(certs_dir, hostname + '.key')
  csr_path = os.path.join(certs_dir, hostname + '.csr')
  crt_path = os.path.join(certs_dir, hostname + '.crt')

  rm_file(key_path)
  rm_file(csr_path)
  rm_file(crt_path)

  # Generate certs for this machine
  subprocess.check_call(['openssl', 'genrsa', '-out', key_path, '2048'])
  subprocess.check_call([
      'openssl', 'req', '-key', key_path, '-subj',
      '/C=CN/ST=GD/L=SZ/O=Acme, Inc./CN=%s' % (hostname), '-new', '-sha256',
      '-out', csr_path
  ])
  tf = tempfile.NamedTemporaryFile(delete=False)
  tf.write('subjectAltName=DNS:' + hostname)
  tf.close()
  subprocess.check_call([
      'openssl', 'x509', '-req', '-extfile', tf.name, '-days', '365', '-in',
      csr_path, '-CA', root_pem_path, '-CAkey', root_key_path,
      '-CAcreateserial', '-out', crt_path
  ])
  rm_file(tf.name)
  subprocess.check_call(['openssl', 'x509', '-in', crt_path, '-text', '-noout'])

  # Generate gfe2 configs for this machine
  mkdir(configs_dir)
  for i in range(0, NUM_GFES):
    replacement_dict = {
        'HTTPS_PORT_PLACEHOLDER': str(HTTPS_PORT_NUM + i),
        'HTTP_PORT_PLACEHOLDER': str(HTTP_PORT_NUM + i),
        'FASTER_STATS_PORT_PLACEHOLDER': str(FASTER_STATS_PORT_NUM + i),
        'CERTS_DIR_PLACEHOLDER': certs_dir,
        'HOSTNAME_PLACEHOLDER': hostname
    }
    gen_file_replace_strings(DEPOT_GFE2_CONFIG_PATH,
                             '%s/%s_%d.txt' % (configs_dir, hostname, i),
                             replacement_dict)

  # Generate setup_clankium_repo.py for this machine
  replacement_dict = {'CLANKIUM_SRC_DIR_PLACEHOLDER': clankium_src_dir}
  gen_file_replace_strings(
      os.path.join(DEPOT_SPEEDTEST_DIR, 'setup_clankium_repo.py'),
      os.path.join(speedtest_dir, 'setup_clankium_repo.py'), replacement_dict)
  chmod_x(os.path.join(speedtest_dir, 'setup_clankium_repo.py'))

  # Generate setup_device_certs.sh for this machine
  replacement_dict = {'CERTS_DIR_PLACEHOLDER': certs_dir}
  gen_file_replace_strings(
      os.path.join(DEPOT_SPEEDTEST_DIR, 'setup_device_certs.sh'),
      os.path.join(speedtest_dir, 'setup_device_certs.sh'), replacement_dict)
  chmod_x(os.path.join(speedtest_dir, 'setup_device_certs.sh'))

  # Generate user scripts for this machine
  mkdir(scripts_dir)

  # Generate build_and_copy_cronet.py for this machine
  replacement_dict = {'CLANKIUM_SRC_DIR_PLACEHOLDER': clankium_src_dir}
  gen_file_replace_strings(
      os.path.join(DEPOT_SCRIPTS_DIR, 'build_and_copy_cronet.py'),
      os.path.join(scripts_dir, 'build_and_copy_cronet.py'), replacement_dict)
  chmod_x(os.path.join(scripts_dir, 'build_and_copy_cronet.py'))

  # Generate build_and_install_app.sh for this machine
  replacement_dict = {'$SPEEDTEST_DIR_PLACEHOLDER': speedtest_dir}
  gen_file_replace_strings(
      os.path.join(DEPOT_SCRIPTS_DIR, 'build_and_install_app.sh'),
      os.path.join(scripts_dir, 'build_and_install_app.sh'), replacement_dict)
  chmod_x(os.path.join(scripts_dir, 'build_and_install_app.sh'))

  # Generate build_and_run_servers.sh for this machine
  replacement_dict = {
      '$GFE2_CONFIGS_DIR_PLACEHOLDER': configs_dir,
      '$HOSTNAME_PLACEHOLDER': hostname
  }
  gen_file_replace_strings(
      os.path.join(DEPOT_SCRIPTS_DIR, 'build_and_run_servers.sh'),
      os.path.join(scripts_dir, 'build_and_run_servers.sh'), replacement_dict)
  chmod_x(os.path.join(scripts_dir, 'build_and_run_servers.sh'))

  # Generate run_benchmark.py for this machine
  replacement_dict = {
      'SIMPLEPERF_SCRIPTS_DIR_PLACEHOLDER': simpleperf_scripts_dir,
      'CLANKIUM_SRC_DIR_PLACEHOLDER': clankium_src_dir,
      'SPEEDTEST_DIR_PLACEHOLDER': speedtest_dir
  }
  gen_file_replace_strings(
      os.path.join(DEPOT_RUN_BENCHMARKS_PY_DIR, 'run_benchmark.py'),
      os.path.join(scripts_dir, 'run_benchmark.py'), replacement_dict)
  chmod_x(os.path.join(scripts_dir, 'run_benchmark.py'))

  print '\nSetup complete.\n'


if __name__ == '__main__':
  sys.exit(main())
