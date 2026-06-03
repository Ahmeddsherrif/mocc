#!/bin/sh

set -e

LOG_ENABLED=0


LOG_DIR="log"
TEMP_DIR="tmp"

run() {
    if [ "$LOG_ENABLED" -eq 1 ]; then

        mkdir -p "$TEMP_DIR"

        TMP_LOG="$TEMP_DIR/.tmp"
        "$@" 2>"$TMP_LOG"

        if [ -s "$TMP_LOG" ]; then
            mkdir -p "$LOG_DIR"

            TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
            LOG_FILE="$LOG_DIR/mocc_log_${TIMESTAMP}.log"

            mv "$TMP_LOG" "$LOG_FILE"

        else
            rm -f "$TMP_LOG"
        fi

        rm -rf "$TEMP_DIR"
    else
        "$@"
    fi
}


if [ "$1" = "-l" ]; then
    LOG_ENABLED=1
fi

make clean

make build

run ./out/mocc_test
run ./out/mocc_bench

echo "Tests passed."