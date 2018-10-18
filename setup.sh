#!/bin/bash

export CROSS_COMPILE=arm-none-eabi-
export KERNEL_BASE=$(pwd)
export TESTS_BASE=$KERNEL_BASE/tests
export ARCH=arm
export KCONFIG=/usr/local/bin
