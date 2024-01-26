#!/bin/bash
# for i in {1..100}; do wget --no-http-keep-alive http://ccfit.nsu.ru/~rzheutskiy/test_files/200mb.dat -e use_proxy=on -e http_proxy=127.0.0.1:1234 -O /dev/null && date & done

PROXY_HOST="127.0.0.1"
PROXY_PORT="1080"
URL=""
WGET_FLAGS="--no-http-keep-alive"
WGET_OPTIONS="-e use_proxy=on"

for i in {1..512}; do wget ${WGET_FLAGS} ${URL} ${CURL_OPTIONS} ${WGET_OPTIONS} -e "http_proxy=${PROXY_HOST}:${PROXY_PORT}" -O "/dev/null" && date & done
