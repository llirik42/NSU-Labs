#!/bin/bash

PROXY_HOST="127.0.0.1"
PROXY_PORT="1080"
URL=""
WGET_FLAGS="--no-http-keep-alive"
WGET_OPTIONS="-e use_proxy=on"

result=$(wget ${WGET_FLAGS} ${URL} ${CURL_OPTIONS} ${WGET_OPTIONS} -e "http_proxy=${PROXY_HOST}:${PROXY_PORT}")
echo "${result}"
