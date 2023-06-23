#ifndef READER_H
#define READER_H
#pragma once
#include "datatypes.h"
#include <fstream>
#include <map>

class Reader {
public:
  Reader(const char *filename);
  const Program &get_program();

private:
  std::ifstream file;
  int label_ind{0};
  std::map<std::string, int>
      labels; // while reading, map from given label to internal size_t
  std::vector<int>
      current_labels; // while reading, current things that have been marked
  Program program{};
  int read_number();
  int read_label();
  Instruction read_io();
  Instruction read_stack();
  Instruction read_arith();
  Instruction read_flow();
  Instruction read_heap();
  Instruction read_instruction();
  int get();
  void unget();
};
#endif
