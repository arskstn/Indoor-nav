#!/bin/bash

# handle commandline options
usage()
{
   echo "Script for building projects with PSDK."
   echo
   echo "Usage: build.sh <options>"
   echo
   echo "Options:"
   echo "-a | --application     Path to application directory for build."
   echo "-s | --spec            Path to spec file."
   echo "-t | --target          PSDK target to build RPM with."
   echo "-h | --help            Print this help and exit."
}

while [[ ${1:-} ]]; do
    case "$1" in
    -a | --application ) shift
        OPT_PROJECT_PATH=$1; shift
        ;;
    -s | --spec ) shift
        OPT_SPEC_PATH=$1; shift
        ;;
    -t | --target ) shift
        OPT_PSDK_TARGET=$1; shift
        ;;
    -h | --help ) shift
        usage
        exit;;
    * )
        usage
        exit;;
    esac
done

if [[ -z $OPT_PROJECT_PATH ]]; then
    echo "Enter path to application directory"
    exit
fi

if [[ -z $OPT_PSDK_TARGET ]]; then
    echo "Incorrect target of PSDK"
    exit
fi

mkdir -p build
mkdir -p artifacts
cd build

sdk-assistant list

# Build project
echo "Build project for $OPT_PSDK_TARGET"
if [[ -z $OPT_SPEC_PATH ]]; then
    mb2 --target $OPT_PSDK_TARGET build ../$OPT_PROJECT_PATH
else
    mb2 --target $OPT_PSDK_TARGET -s ../$OPT_SPEC_PATH build ../$OPT_PROJECT_PATH
fi

# Copy build artifacts
cp RPMS/* ../artifacts/
