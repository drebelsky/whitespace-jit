#include "reader.h"

Reader::Reader(const char *filename) : file{filename} {
  while (true) {
    Instruction inst = read_instruction();
    if (inst.type == InstructionType::MARK) {
      current_labels.push_back(inst.arg);
    } else {
      for (auto label : current_labels) {
        program.locations[label] = program.instructions.size();
      }
      program.instructions.push_back(inst);
      current_labels.clear();
    }
    get();
    if (file.eof()) {
      break;
    }
    unget();
  }
}

int Reader::get() {
  while (true) {
    int res = file.get();
    switch (res) {
    case '\t':
    case '\n':
    case ' ':
    case EOF:
      return res;
    }
  }
}

void Reader::unget() { file.unget(); }

const Program &Reader::get_program() { return program; }
int Reader::read_number() {
  int res = 0;
  bool is_neg;
  switch (get()) {
  case ' ':
    is_neg = false;
    break;
  case '\t':
    is_neg = true;
    break;
  default:
    throw "Invalid sign for number";
  }
  int next;
  while ((next = get()) != '\n') {
    res = (res << 1) | (next == '\t');
  }

  return (is_neg) ? -res : res;
}

int Reader::read_label() {
  std::string label = "";
  int next;
  while ((next = get()) != '\n') {
    label += next;
  }

  if (!labels.count(label)) {
    labels[label] = label_ind++;
  }
  return labels[label];
}

Instruction Reader::read_io() {
  switch (get()) {
  case '\t':
    switch (get()) {
    case ' ':
      return {InstructionType::READC};
    case '\t':
      return {InstructionType::READN};
    }
    break;
  case ' ':
    switch (get()) {
    case ' ':
      return {InstructionType::WRITEC};
    case '\t':
      return {InstructionType::WRITEN};
    }
    break;
  }
  throw "Invalid stack instruction";
}

Instruction Reader::read_stack() {
  switch (get()) {
  case ' ':
    return {InstructionType::PUSH, read_number()};
  case '\n':
    switch (get()) {
    case ' ':
      return {InstructionType::DUP};
    case '\t':
      return {InstructionType::SWAP};
    case '\n':
      return {InstructionType::DISCARD};
    }
    break;
  case '\t':
    switch (get()) {
    case ' ':
      return {InstructionType::COPY, read_number()};
    case '\n':
      return {InstructionType::SLIDE, read_number()};
    }
    break;
  }
  throw "Invalid stack instruction";
}

Instruction Reader::read_arith() {
  switch (get()) {
  case ' ':
    switch (get()) {
    case ' ':
      return {InstructionType::ADD};
    case '\t':
      return {InstructionType::SUB};
    case '\n':
      return {InstructionType::MUL};
    }
    break;
  case '\t':
    switch (get()) {
    case ' ':
      return {InstructionType::DIV};
    case '\t':
      return {InstructionType::MOD};
    }
    break;
  }
  throw "Invalid arithmetic instruction";
}

Instruction Reader::read_flow() {
  switch (get()) {
  case ' ':
    switch (get()) {
    case ' ':
      return {InstructionType::MARK, read_label()};
    case '\t':
      return {InstructionType::CALL, read_label()};
    case '\n':
      return {InstructionType::JUMP, read_label()};
    }
    break;
  case '\t':
    switch (get()) {
    case ' ':
      return {InstructionType::JZ, read_label()};
    case '\t':
      return {InstructionType::JN, read_label()};
    case '\n':
      return {InstructionType::RET};
    }
    break;
  case '\n':
    if (get() == '\n') {
      return {InstructionType::END};
    }
    break;
  }
  throw "Invalid flow instruction";
}

Instruction Reader::read_heap() {
  switch (get()) {
  case ' ':
    return {InstructionType::STORE};
  case '\t':
    return {InstructionType::RETRIEVE};
  }
  throw "Invalid heap instruction";
}

Instruction Reader::read_instruction() {
  switch (get()) {
  case '\t':
    switch (get()) {
    case '\n':
      return read_io();
    case ' ':
      return read_arith();
    case '\t':
      return read_heap();
    default:
      throw "Invalid prefix";
    }
  case ' ':
    return read_stack();
  case '\n':
    return read_flow();
  default:
    throw "Invalid instruction encountered";
  }
}
