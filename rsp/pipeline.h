//
// rsp/pipeline.h: RSP processor pipeline.
//
// CEN64: Cycle-Accurate Nintendo 64 Simulator.
// Copyright (C) 2014, Tyler J. Stachecki.
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

#ifndef __rsp_pipeline_h__
#define __rsp_pipeline_h__
#include "common.h"
#include "rsp/decoder.h"
#include "rsp/cp2.h"
#include "rsp/rsp.h"

struct rsp;

enum rsp_mem_request_type {
  RSP_MEM_REQUEST_NONE,
  RSP_MEM_REQUEST_READ,
  RSP_MEM_REQUEST_WRITE,
  RSP_MEM_REQUEST_VECTOR,
  RSP_MEM_REQUEST_FOURTH,
  RSP_MEM_REQUEST_HALF,
  RSP_MEM_REQUEST_PACK,
  RSP_MEM_REQUEST_QUAD,
  RSP_MEM_REQUEST_REST,
  RSP_MEM_REQUEST_UPACK
};

struct rsp_mem_request {
  union aligned_rsp_1vect_t vdqm;
  uint32_t addr;
  uint32_t data;
  uint32_t dqm;

  enum rsp_mem_request_type type;
  unsigned size, element;

  void (*vldst_func)(struct rsp *rsp, uint32_t addr, unsigned element,
    uint16_t *regp, rsp_vect_t reg, rsp_vect_t dqm);
};

struct rsp_latch {
  uint32_t pc;
};

struct rsp_result {
  uint32_t result;
  unsigned dest;
};

struct rsp_ifrd_latch {
  struct rsp_latch common;
  uint32_t pc, iw;
};

struct rsp_rdex_latch {
  struct rsp_latch common;
  struct rsp_opcode opcode;
  uint32_t iw;
};

struct rsp_exdf_latch {
  struct rsp_latch common;
  struct rsp_result result;

  struct rsp_mem_request request;
};

struct rsp_dfwb_latch {
  struct rsp_latch common;
  struct rsp_result result;
};

struct rsp_pipeline {
  struct rsp_dfwb_latch dfwb_latch;
  struct rsp_exdf_latch exdf_latch;
  struct rsp_rdex_latch rdex_latch;
  struct rsp_ifrd_latch ifrd_latch;
};

cen64_cold void rsp_pipeline_init(struct rsp_pipeline *pipeline);

#endif

