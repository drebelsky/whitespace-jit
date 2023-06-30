#ifndef JIT_SIZE_H
#define JIT_SIZE_H
#pragma once
#include <unordered_map>
#include "datatypes.h"
const std::unordered_map<InstructionType, size_t> kInstructionSizes {
  {InstructionType::END, 10},
  {InstructionType::RET, 1},
  {InstructionType::END, 10},
  {InstructionType::PUSH, 18},
  {InstructionType::READC, 23},
  {InstructionType::READN, 23},
  {InstructionType::WRITEC, 20},
  {InstructionType::WRITEN, 20},
  {InstructionType::STORE, 28},
  {InstructionType::RETRIEVE, 25},
  {InstructionType::DUP, 13},
  {InstructionType::SWAP, 20},
  {InstructionType::DISCARD, 4},
  {InstructionType::COPY, 16},
  {InstructionType::SLIDE, 17},
  {InstructionType::ADD, 19},
  {InstructionType::SUB, 19},
  {InstructionType::MUL, 20},
  {InstructionType::DIV, 26},
  {InstructionType::MOD, 26},
  {InstructionType::CALL, 13},
  {InstructionType::JUMP, 5},
  {InstructionType::JZ, 15},
  {InstructionType::JN, 15},
};
#endif