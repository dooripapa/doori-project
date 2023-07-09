#!/bin/bash

PUB_BIN_PATH="/home/jaeseong/CLionProjects/doori-project/cmake-build-debug/doori_service/pub --file /home/jaeseong/CLionProjects/doori-project/doori_bin/pub.dictionary"
PUB="cmake-build-debug/doori_service/pub"

function main() {

  ${PUB_BIN_PATH}

  local pid=$(ps -ef | grep -v grep | grep "${PUB}" | awk '{print $2}')

  echo $pid

}

main "$@"
