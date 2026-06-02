#!/bin/sh

set -e

VERBOSE_ENABLED=0
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

if [ "$1" = "-v" ]; then
    VERBOSE_ENABLED=1
elif [ "$1" = "-vl" ]; then
    VERBOSE_ENABLED=1
    LOG_ENABLED=1
fi

make clean

make build VERBOSE_ENABLED="$VERBOSE_ENABLED"

run ./out/mocc_test

echo "Tests passed."