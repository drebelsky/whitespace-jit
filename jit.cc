#include "jit.h"
#include "jit_size.h"
#include <cstdint>
#include <ios>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

const size_t kPageSize = sysconf(_SC_PAGESIZE);

using std::vector;

namespace util {
void store(std::unordered_map<int64_t, int64_t> *heap, int64_t key,
           int64_t val) {
  (*heap)[key] = val;
}

int64_t retrieve(std::unordered_map<int64_t, int64_t> *heap, int64_t key) {
  return (*heap)[key];
}

void readc(std::unordered_map<int64_t, int64_t> *heap, int64_t loc) {
  std::cout.flush();
  char c;
  std::cin >> std::noskipws >> c;
  (*heap)[loc] = c;
}

void readn(std::unordered_map<int64_t, int64_t> *heap, int64_t loc) {
  std::cout.flush();
  int64_t n;
  std::cin >> n;
  (*heap)[loc] = n;
}

void writec(char c) { std::cout << c; }

void writen(int64_t n) { std::cout << n; }
} // namespace util

void copy_bytes(uint8_t buf[], uint64_t val) {
  std::copy(reinterpret_cast<uint8_t *>(&val),
            reinterpret_cast<uint8_t *>(&val) + 8, buf);
}

void copy_bytes4(uint8_t buf[], uint32_t val) {
  std::copy(reinterpret_cast<uint8_t *>(&val),
            reinterpret_cast<uint8_t *>(&val) + 4, buf);
}

namespace writei {
void start(vector<uint8_t> &instructions) {
  instructions.insert(instructions.end(), {
                                              0x41, 0x54,       // push   %r12
                                              0x41, 0x55,       // push   %r13
                                              0x49, 0x89, 0xfc, // mov %rdi,%r12
                                              0x49, 0x89, 0xf5, // mov %rsi,%r13
                                          });
}

void ret(vector<uint8_t> &instructions) {
  // retq
  instructions.push_back(0xc3);
}

void end(vector<uint8_t> &instructions) {
  instructions.insert(instructions.end(), {
                                              0x41, 0x5d, // pop %r13
                                              0x41, 0x5c, // pop %r12
                                              0xc3        // retq
                                          });
}

void push(vector<uint8_t> &instructions, int64_t val) {
  uint8_t val_buf[8];
  copy_bytes(val_buf, static_cast<uint64_t>(val));
  // movabs $val,%rax
  instructions.insert(instructions.end(), {0x48, 0xb8});
  instructions.insert(instructions.end(), val_buf, val_buf + 8);
  instructions.insert(instructions.end(),
                      {
                          0x49, 0x89, 0x04, 0x24, // mov    %rax,(%r12)
                          0x49, 0x83, 0xc4, 0x08  // add    $0x8,%r12
                      });
}

void readc(vector<uint8_t> &instructions) {
  uint8_t func_addr[8];
  copy_bytes(func_addr, reinterpret_cast<uint64_t>(util::readc));

  instructions.insert(instructions.end(),
                      {0x4c, 0x89, 0xef,       // mov    %r13,%rdi
                       0x49, 0x83, 0xec, 0x08, // sub    $0x8,%r12
                       0x49, 0x8b, 0x34, 0x24, // mov    (%r12),%rsi
                       // movabs $util::readc,%rax
                       0x48, 0xb8});
  instructions.insert(instructions.end(), func_addr, func_addr + 8);
  // callq *%rax
  instructions.insert(instructions.end(), {0xff, 0xd0});
}

void readn(vector<uint8_t> &instructions) {
  uint8_t func_addr[8];
  copy_bytes(func_addr, reinterpret_cast<uint64_t>(util::readn));

  instructions.insert(instructions.end(),
                      {0x4c, 0x89, 0xef,       // mov    %r13,%rdi
                       0x49, 0x83, 0xec, 0x08, // sub    $0x8,%r12
                       0x49, 0x8b, 0x34, 0x24, // mov    (%r12),%rsi
                       // movabs $util::readn,%rax
                       0x48, 0xb8});
  instructions.insert(instructions.end(), func_addr, func_addr + 8);
  // callq *%rax
  instructions.insert(instructions.end(), {0xff, 0xd0});
}

void writec(vector<uint8_t> &instructions) {
  uint8_t func_addr[8];
  copy_bytes(func_addr, reinterpret_cast<uint64_t>(util::writec));

  instructions.insert(instructions.end(),
                      {0x49, 0x83, 0xec, 0x08, // sub    $0x8,%r12
                       0x49, 0x8b, 0x3c, 0x24, // mov    (%r12),%rdi
                       // movabs $util::writec,%rax
                       0x48, 0xb8});
  instructions.insert(instructions.end(), func_addr, func_addr + 8);
  // callq *%rax
  instructions.insert(instructions.end(), {0xff, 0xd0});
}

// Note: this is identical to writei::writec, except for flipping out
// util::writen for util::writec in the copy_bytes call
void writen(vector<uint8_t> &instructions) {
  uint8_t func_addr[8];
  copy_bytes(func_addr, reinterpret_cast<uint64_t>(util::writen));

  instructions.insert(instructions.end(),
                      {0x49, 0x83, 0xec, 0x08, // sub    $0x8,%r12
                       0x49, 0x8b, 0x3c, 0x24, // mov    (%r12),%rdi
                       // movabs $util::writen,%rax
                       0x48, 0xb8});
  instructions.insert(instructions.end(), func_addr, func_addr + 8);
  // callq *%rax
  instructions.insert(instructions.end(), {0xff, 0xd0});
}

void store(vector<uint8_t> &instructions) {
  uint8_t func_addr[8];
  copy_bytes(func_addr, reinterpret_cast<uint64_t>(util::store));

  instructions.insert(instructions.end(),
                      {
                          0x49, 0x83, 0xec, 0x10,       // sub    $0x10,%r12
                          0x4c, 0x89, 0xef,             // mov    %r13,%rdi
                          0x49, 0x8b, 0x34, 0x24,       // mov    (%r12),%rsi
                          0x49, 0x8b, 0x54, 0x24, 0x08, // mov    0x8(%r12),%rdx
                          0x48, 0xb8, // movabs func_addr,%rax (beginning)
                      });
  instructions.insert(instructions.end(), func_addr, func_addr + 8);
  // callq *%rax
  instructions.insert(instructions.end(), {0xff, 0xd0});
}

void retrieve(vector<uint8_t> &instructions) {
  uint8_t func_addr[8];
  copy_bytes(func_addr, reinterpret_cast<uint64_t>(util::retrieve));

  instructions.insert(instructions.end(),
                      {
                          0x4c, 0x89, 0xef,             // mov    %r13,%rdi
                          0x49, 0x8b, 0x74, 0x24, 0xf8, // mov -0x8(%r12),%rsi
                          0x48, 0xb8, // movabs func_addr,%rax (beginning)
                      });
  instructions.insert(instructions.end(), func_addr, func_addr + 8);
  instructions.insert(instructions.end(),
                      {
                          0xff, 0xd0,                   // callq  *%rax
                          0x49, 0x89, 0x44, 0x24, 0xf8, // mov %rax,-0x8(%r12)
                      });
}

void dup(vector<uint8_t> &instructions) {
  instructions.insert(instructions.end(),
                      {
                          0x49, 0x8b, 0x44, 0x24, 0xf8, // mov -0x8(%r12),%rax
                          0x49, 0x89, 0x04, 0x24,       // mov    %rax,(%r12)
                          0x49, 0x83, 0xc4, 0x08,       // add    $0x8,%r12
                      });
}

void swap(vector<uint8_t> &instructions) {
  instructions.insert(
      instructions.end(),
      {
          0x49, 0x8b, 0x44, 0x24, 0xf8, // mov    -0x8(%r12),%rax
          0x49, 0x8b, 0x7c, 0x24, 0xf0, // mov    -0x10(%r12),%rdi
          0x49, 0x89, 0x7c, 0x24, 0xf8, // mov    %rdi,-0x8(%r12)
          0x49, 0x89, 0x44, 0x24, 0xf0, // mov    %rax,-0x10(%r12)
      });
}

void discard(vector<uint8_t> &instructions) {
  // sub    $0x8,%r12
  instructions.insert(instructions.end(), {0x49, 0x83, 0xec, 0x08});
}

void copy(vector<uint8_t> &instructions, int x) {
  x = (-x - 1) * 8;
  uint8_t offset[4];
  copy_bytes4(offset, static_cast<uint32_t>(x));
  // mov    $((-x - 1) * 8)(%r12),%rax
  instructions.insert(instructions.end(), {0x49, 0x8b, 0x84, 0x24});
  instructions.insert(instructions.end(), offset, offset + 4);
  instructions.insert(instructions.end(),
                      {
                          0x49, 0x89, 0x04, 0x24, // mov    %rax,(%r12)
                          0x49, 0x83, 0xc4, 0x08, // add    $0x8,%r12
                      });
}

void slide(vector<uint8_t> &instructions, int x) {
  x *= 8;
  uint8_t offset[4];
  copy_bytes4(offset, static_cast<uint32_t>(x));
  instructions.insert(instructions.end(),
                      {
                          0x49, 0x8b, 0x44, 0x24, 0xf8, // mov -0x8(%r12),%rax
                          0x49, 0x81, 0xec, // sub    $x*8,%r12 (beginning)
                      });
  instructions.insert(instructions.end(), offset, offset + 4);
  // mov    %rax,-0x8(%r12)
  instructions.insert(instructions.end(), {0x49, 0x89, 0x44, 0x24, 0xf8});
}

void add(vector<uint8_t> &instructions) {
  instructions.insert(
      instructions.end(),
      {
          0x49, 0x8b, 0x44, 0x24, 0xf0, // mov    -0x10(%r12),%rax
          0x49, 0x03, 0x44, 0x24, 0xf8, // add    -0x8(%r12),%rax
          0x49, 0x83, 0xec, 0x08,       // sub    $0x8,%r12
          0x49, 0x89, 0x44, 0x24, 0xf8, // mov    %rax,-0x8(%r12)
      });
}

void sub(vector<uint8_t> &instructions) {
  instructions.insert(
      instructions.end(),
      {
          0x49, 0x8b, 0x44, 0x24, 0xf0, // mov    -0x10(%r12),%rax
          0x49, 0x2b, 0x44, 0x24, 0xf8, // sub    -0x8(%r12),%rax
          0x49, 0x83, 0xec, 0x08,       // sub    $0x8,%r12
          0x49, 0x89, 0x44, 0x24, 0xf8, // mov    %rax,-0x8(%r12)
      });
}

void mul(vector<uint8_t> &instructions) {
  instructions.insert(
      instructions.end(),
      {
          0x49, 0x8b, 0x44, 0x24, 0xf0,       // mov    -0x10(%r12),%rax
          0x49, 0x0f, 0xaf, 0x44, 0x24, 0xf8, // imul   -0x8(%r12),%rax
          0x49, 0x83, 0xec, 0x08,             // sub    $0x8,%r12
          0x49, 0x89, 0x44, 0x24, 0xf8,       // mov    %rax,-0x8(%r12)
      });
}

// based on compiling
// int64_t divide(int64_t a, int64_t b) {
//   return b / a;
// }
// with gcc
void div(vector<uint8_t> &instructions) {
  instructions.insert(
      instructions.end(),
      {
          0x41, 0x8b, 0x44, 0x24, 0xf0, // mov    -0xc(%r12),%eax
          0x41, 0x8b, 0x54, 0x24, 0xf4, // mov    -0x10(%r12),%edx
          0x48, 0x99,                   // cqto
          0x49, 0xf7, 0x7c, 0x24, 0xf8, // idivq  -0x8(%r12)
          0x49, 0x83, 0xec, 0x08,       // sub    $0x8,%r12
          0x49, 0x89, 0x44, 0x24, 0xf8, // mov    %rax,-0x8(%r12)
      });
}

// Only the last instruction is different from div
void mod(vector<uint8_t> &instructions) {
  instructions.insert(
      instructions.end(),
      {
          0x41, 0x8b, 0x44, 0x24, 0xf0, // mov    -0xc(%r12),%eax
          0x41, 0x8b, 0x54, 0x24, 0xf4, // mov    -0x10(%r12),%edx
          0x48, 0x99,                   // cqto
          0x49, 0xf7, 0x7c, 0x24, 0xf8, // idivq  -0x8(%r12)
          0x49, 0x83, 0xec, 0x08,       // sub    $0x8,%r12
          0x49, 0x89, 0x54, 0x24, 0xf8, // mov    %rdx,-0x8(%r12)
      });
}

void call(vector<uint8_t> &instructions, int instruction) {
  int delta = instruction - static_cast<int>(instructions.size() + 5);
  uint8_t offset[4];
  copy_bytes4(offset, static_cast<uint32_t>(delta));
  // call     instr
  instructions.insert(instructions.end(), {0xe8});
  instructions.insert(instructions.end(), offset, offset + 4);
}

void jump(vector<uint8_t> &instructions, int instruction) {
  int delta = instruction - static_cast<int>(instructions.size() + 5);
  uint8_t offset[4];
  copy_bytes4(offset, static_cast<uint32_t>(delta));
  // jmp     instr
  instructions.insert(instructions.end(), {0xe9});
  instructions.insert(instructions.end(), offset, offset + 4);
}

void jz(vector<uint8_t> &instructions, int instruction) {
  int delta = instruction - static_cast<int>(instructions.size() + 15);
  uint8_t offset[4];
  copy_bytes4(offset, static_cast<uint32_t>(delta));
  instructions.insert(instructions.end(),
                      {0x49, 0x83, 0xec, 0x08,       // sub    $0x8,%r12
                       0x49, 0x83, 0x3c, 0x24, 0x00, // cmpq   $0x0,(%r12)
                       // je     instr
                       0x0f, 0x84});
  instructions.insert(instructions.end(), offset, offset + 4);
}

void jn(vector<uint8_t> &instructions, int instruction) {
  int delta = instruction - static_cast<int>(instructions.size() + 15);
  uint8_t offset[4];
  copy_bytes4(offset, static_cast<uint32_t>(delta));
  instructions.insert(instructions.end(),
                      {0x49, 0x83, 0xec, 0x08,       // sub    $0x8,%r12
                       0x49, 0x83, 0x3c, 0x24, 0x00, // cmpq   $0x0,(%r12)
                       // jl     instr
                       0x0f, 0x8c});
  instructions.insert(instructions.end(), offset, offset + 4);
}
} // namespace writei

void jit(const Program &program) {
  using namespace writei;
  vector<uint8_t> instructions;
  writei::start(instructions);

  size_t offset = instructions.size();
  vector<size_t> offsets;
  for (const Instruction &inst : program.instructions) {
    offsets.push_back(offset);
    offset += kInstructionSizes.at(inst.type);
  }

  for (const Instruction &inst : program.instructions) {
    switch (inst.type) {
    case ADD:
      add(instructions);
      break;
    case CALL:
      call(instructions, offsets[program.locations.at(inst.arg)]);
      break;
    case COPY:
      copy(instructions, inst.arg);
      break;
    case DISCARD:
      discard(instructions);
      break;
    case DIV:
      div(instructions);
      break;
    case DUP:
      dup(instructions);
      break;
    case END:
      end(instructions);
      break;
    case JN:
      jn(instructions, offsets[program.locations.at(inst.arg)]);
      break;
    case JUMP:
      jump(instructions, offsets[program.locations.at(inst.arg)]);
      break;
    case JZ:
      jz(instructions, offsets[program.locations.at(inst.arg)]);
      break;
    case MOD:
      mod(instructions);
      break;
    case MUL:
      mul(instructions);
      break;
    case PUSH:
      push(instructions, inst.arg);
      break;
    case READC:
      readc(instructions);
      break;
    case READN:
      readn(instructions);
      break;
    case RET:
      ret(instructions);
      break;
    case RETRIEVE:
      retrieve(instructions);
      break;
    case SLIDE:
      slide(instructions, inst.arg);
      break;
    case STORE:
      store(instructions);
      break;
    case SUB:
      sub(instructions);
      break;
    case SWAP:
      swap(instructions);
      break;
    case WRITEC:
      writec(instructions);
      break;
    case WRITEN:
      writen(instructions);
      break;

    case MARK:
      throw "Encountered mark when trying to write instructions";
    }
  }

  writei::end(instructions);
  uint8_t *stack = new uint8_t[67108864]; // allocate 64MB stack
  std::unordered_map<int64_t, int64_t> heap;

  size_t alloc_size =
      (instructions.size() + kPageSize - 1) / kPageSize * kPageSize;
  // https://eli.thegreenplace.net/2013/11/05/how-to-jit-an-introduction
  uint8_t *inst_memory = static_cast<uint8_t *>(
      mmap(nullptr, alloc_size, PROT_READ | PROT_WRITE | PROT_EXEC,
           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
  std::copy(instructions.begin(), instructions.end(), inst_memory);
  reinterpret_cast<void (*)(uint8_t *, void *)>(inst_memory)(stack, &heap);

  munmap(inst_memory, alloc_size);
  delete[] stack;
}
