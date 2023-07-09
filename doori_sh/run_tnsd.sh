#!/bin/bash

TNSD_BIN_PATH="/home/jaeseong/CLionProjects/doori-project/cmake-build-debug/doori_service/tnsd --file /home/jaeseong/CLionProjects/doori-project/doori_bin/tnsd.dictionary"
TNSD="cmake-build-debug/doori_service/tnsd"

function main() {

  ${TNSD_BIN_PATH} &

  local pid=$(ps -ef | grep -v grep | grep "${TNSD}" | awk '{print $2}')

  echo $pid

}

main "$@"