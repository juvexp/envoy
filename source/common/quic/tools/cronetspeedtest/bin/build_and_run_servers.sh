#!/bin/bash

# Run this script from google3/ in the google3 repo.

GFE2_CONFIGS_DIR=$GFE2_CONFIGS_DIR_PLACEHOLDER
HOSTNAME=$HOSTNAME_PLACEHOLDER

killall -9 simple_agent upload_server siloed_gfe2_bin

printf "\nBuild simple agent\n"
blaze build -c opt gfe/quic/tools/cronetspeedtest/simple_agent uploader/service/server:upload_server gfe/gfe2:siloed_gfe2_bin
printf "\nRun simple agent in background\n"
./blaze-bin/gfe/quic/tools/cronetspeedtest/simple_agent/simple_agent --alsologtostderr > /tmp/simple_agent.log 2>&1 &

printf "\nRun upload_server in background\n"
./uploader/run_upload_server.py --config_file=./gfe/quic/tools/cronetspeedtest/simple_agent/config.gcl > /tmp/upload_server.log 2>&1 &

printf "\nBuild and run gfe 0 in background\n"
CONFIG_FILE=$GFE2_CONFIGS_DIR/${HOSTNAME}_0.txt  /home/build/google3/gfe/gfe2/tools/run_gfe2.sh > /tmp/gfe1.log 2>&1 &
printf "\nBuild and run gfe 1 in background\n"
CONFIG_FILE=$GFE2_CONFIGS_DIR/${HOSTNAME}_1.txt  /home/build/google3/gfe/gfe2/tools/run_gfe2.sh > /tmp/gfe2.log 2>&1 &
printf "\nBuild and run gfe 2 in background\n"
CONFIG_FILE=$GFE2_CONFIGS_DIR/${HOSTNAME}_2.txt  /home/build/google3/gfe/gfe2/tools/run_gfe2.sh > /tmp/gfe3.log 2>&1 &
printf "\nBuild and run gfe 3 in background\n"
CONFIG_FILE=$GFE2_CONFIGS_DIR/${HOSTNAME}_3.txt  /home/build/google3/gfe/gfe2/tools/run_gfe2.sh > /tmp/gfe4.log 2>&1 &
printf "\nIt may take a few minutes for the GFEs to finish building before they're running.\n"