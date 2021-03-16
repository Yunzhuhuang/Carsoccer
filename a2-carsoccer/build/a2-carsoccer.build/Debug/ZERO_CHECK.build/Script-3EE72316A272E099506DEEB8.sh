#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/yunzhuhuang/Desktop/2021spring/4611/repo-huan2089/dev/a2-carsoccer/build
  make -f /Users/yunzhuhuang/Desktop/2021spring/4611/repo-huan2089/dev/a2-carsoccer/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/yunzhuhuang/Desktop/2021spring/4611/repo-huan2089/dev/a2-carsoccer/build
  make -f /Users/yunzhuhuang/Desktop/2021spring/4611/repo-huan2089/dev/a2-carsoccer/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/yunzhuhuang/Desktop/2021spring/4611/repo-huan2089/dev/a2-carsoccer/build
  make -f /Users/yunzhuhuang/Desktop/2021spring/4611/repo-huan2089/dev/a2-carsoccer/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/yunzhuhuang/Desktop/2021spring/4611/repo-huan2089/dev/a2-carsoccer/build
  make -f /Users/yunzhuhuang/Desktop/2021spring/4611/repo-huan2089/dev/a2-carsoccer/build/CMakeScripts/ReRunCMake.make
fi

