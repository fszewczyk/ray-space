#!/bin/bash
set -e 

mkdir build -p
cd build
cmake ..
make
cd ..

RUN="false"


while [[ $# -gt 0 ]]; do
  case "$1" in
    -r|--run)
      RUN="true"
      shift
      ;;
    -*|--*)
      echo "Unknown option $1"
      exit
      ;;
  esac
done
shift "$(($OPTIND -1))"

if [ "$RUN" == "true" ]; then
  NO_COLOR='\033[0m'
  LIGHT_GREEN='\033[1;32m'

  echo -e "${LIGHT_GREEN}Running Shkyera Engine${NO_COLOR}"

  ./build/bin/shkyera

fi