#ifndef DATATYPES_H
#define DATATYPES_H
#pragma once

#include <unordered_map>
#include <vector>
enum InstructionType {
  READC,
  READN,
  WRITEC,
  WRITEN,

  PUSH,
  DUP,
  SWAP,
  DISCARD,
  COPY,
  SLIDE,

  ADD,
  SUB,
  MUL,
  DIV,
  MOD,

  MARK,
  CALL,
  JUMP,
  JZ,
  JN,
  RET,
  END,

  STORE,
  RETRIEVE
};

struct Instruction {
  InstructionType type;
  int arg;
  Instruction(InstructionType type) : type{type} {};
  Instruction(InstructionType type, int argument)
      : type{type}, arg{argument} {};
};

struct Program {
  std::unordered_map<int, size_t>
      locations; // map from label to instruction index
  std::vector<Instruction> instructions{};
};

#endif
