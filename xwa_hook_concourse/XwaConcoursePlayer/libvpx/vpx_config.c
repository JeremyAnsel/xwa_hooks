/* Copyright (c) 2011 The WebM project authors. All Rights Reserved. */
/*  */
/* Use of this source code is governed by a BSD-style license */
/* that can be found in the LICENSE file in the root of the source */
/* tree. An additional intellectual property rights grant can be found */
/* in the file PATENTS.  All contributing project authors may */
/* be found in the AUTHORS file in the root of the source tree. */
#include "vpx/vpx_codec.h"
static const char* const cfg = "--target=x86-linux-gcc --disable-sse4_1 --disable-avx --disable-avx2 --disable-avx512 --as=yasm --enable-external-build --enable-realtime-only --enable-pic --disable-runtime-cpu-detect --disable-install-docs --size-limit=4096x3072 --enable-vp9-highbitdepth";
const char *vpx_codec_build_config(void) {return cfg;}
