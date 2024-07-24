#!/bin/bash

usage() {
  echo "Usage: $0 -f <PayloadFile> -i <BinloaderIP> -p <BinloaderPort>"
  exit 1
}

while getopts ":f:i:p:" opt; do
  case $opt in
    f) PayloadFile="$OPTARG" ;;
    i) BinloaderIP="$OPTARG" ;;
    p) BinloaderPort="$OPTARG" ;;
    *) usage ;;
  esac
done

if [ -z "$PayloadFile" ] || [ -z "$BinloaderIP" ] || [ -z "$BinloaderPort" ]; then
  usage
fi

if [ ! -f "$PayloadFile" ]; then
  PayloadFile="$(pwd)/$PayloadFile"
fi

curl -X POST "http://$BinloaderIP:$BinloaderPort" --upload-file "$PayloadFile"
