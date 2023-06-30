#include <vector>
#include <iostream>
#include <cstdint>

using std::vector;
using std::cout;

namespace writei {
  void start(vector<uint8_t>& instructions);
  void ret(vector<uint8_t>& instructions);
  void end(vector<uint8_t>& instructions);
  void push(vector<uint8_t>& instructions, int64_t val);
  void readc(vector<uint8_t>& instructions);
  void readn(vector<uint8_t>& instructions);
  void writec(vector<uint8_t>& instructions);
  void writen(vector<uint8_t>& instructions);
  void store(vector<uint8_t>& instructions);
  void retrieve(vector<uint8_t>& instructions);
  void dup(vector<uint8_t>& instructions);
  void swap(vector<uint8_t>& instructions);
  void discard(vector<uint8_t>& instructions);
  void copy(vector<uint8_t>& instructions, int x);
  void slide(vector<uint8_t>& instructions, int x);
  void add(vector<uint8_t>& instructions);
  void sub(vector<uint8_t>& instructions);
  void mul(vector<uint8_t>& instructions);
  void div(vector<uint8_t>& instructions);
  void mod(vector<uint8_t>& instructions);
  void call(vector<uint8_t>& instructions, int instruction);
  void jump(vector<uint8_t>& instructions, int instruction);
  void jz(vector<uint8_t>& instructions, int instruction);
  void jn(vector<uint8_t>& instructions, int instruction);
}

int main() {
  using namespace writei;
  vector<uint8_t> instructions;
  cout
    << "#ifndef JIT_SIZE_H\n"
    << "#define JIT_SIZE_H\n"
    << "#pragma once\n"
    << "#include <unordered_map>\n"
    << "#include \"datatypes.h\"\n"
    << "const std::unordered_map<InstructionType, size_t> kInstructionSizes {\n";
  end(instructions); cout << "  {InstructionType::END, " << instructions.size() << "},\n"; instructions.clear();
  ret(instructions); cout << "  {InstructionType::RET, " << instructions.size() << "},\n"; instructions.clear();
  end(instructions); cout << "  {InstructionType::END, " << instructions.size() << "},\n"; instructions.clear();
  push(instructions, 0); cout << "  {InstructionType::PUSH, " << instructions.size() << "},\n"; instructions.clear();
  readc(instructions); cout << "  {InstructionType::READC, " << instructions.size() << "},\n"; instructions.clear();
  readn(instructions); cout << "  {InstructionType::READN, " << instructions.size() << "},\n"; instructions.clear();
  writec(instructions); cout << "  {InstructionType::WRITEC, " << instructions.size() << "},\n"; instructions.clear();
  writen(instructions); cout << "  {InstructionType::WRITEN, " << instructions.size() << "},\n"; instructions.clear();
  store(instructions); cout << "  {InstructionType::STORE, " << instructions.size() << "},\n"; instructions.clear();
  retrieve(instructions); cout << "  {InstructionType::RETRIEVE, " << instructions.size() << "},\n"; instructions.clear();
  dup(instructions); cout << "  {InstructionType::DUP, " << instructions.size() << "},\n"; instructions.clear();
  swap(instructions); cout << "  {InstructionType::SWAP, " << instructions.size() << "},\n"; instructions.clear();
  discard(instructions); cout << "  {InstructionType::DISCARD, " << instructions.size() << "},\n"; instructions.clear();
  copy(instructions, 0); cout << "  {InstructionType::COPY, " << instructions.size() << "},\n"; instructions.clear();
  slide(instructions, 0); cout << "  {InstructionType::SLIDE, " << instructions.size() << "},\n"; instructions.clear();
  add(instructions); cout << "  {InstructionType::ADD, " << instructions.size() << "},\n"; instructions.clear();
  sub(instructions); cout << "  {InstructionType::SUB, " << instructions.size() << "},\n"; instructions.clear();
  mul(instructions); cout << "  {InstructionType::MUL, " << instructions.size() << "},\n"; instructions.clear();
  div(instructions); cout << "  {InstructionType::DIV, " << instructions.size() << "},\n"; instructions.clear();
  mod(instructions); cout << "  {InstructionType::MOD, " << instructions.size() << "},\n"; instructions.clear();
  call(instructions, 0); cout << "  {InstructionType::CALL, " << instructions.size() << "},\n"; instructions.clear();
  jump(instructions, 0); cout << "  {InstructionType::JUMP, " << instructions.size() << "},\n"; instructions.clear();
  jz(instructions, 0); cout << "  {InstructionType::JZ, " << instructions.size() << "},\n"; instructions.clear();
  jn(instructions, 0); cout << "  {InstructionType::JN, " << instructions.size() << "},\n"; instructions.clear();
  // start(instructions); cout << "const kJitStartSize = " << instructions.size() << ";\n"; instructions.clear();
  cout << "};\n";
  cout << "#endif";
}
