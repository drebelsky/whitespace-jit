#include "datatypes.h"
#include "jit.h"
#include "reader.h"
#include "simulator.h"

#include <iostream>
#include <sstream>
#include <stack>

void debug_inst(const Instruction &inst) {
  switch (inst.type) {
  case READC:
    std::cout << "READC";
    break;
  case READN:
    std::cout << "READN";
    break;
  case WRITEC:
    std::cout << "WRITEC";
    break;
  case WRITEN:
    std::cout << "WRITEN";
    break;
  case PUSH:
    std::cout << "PUSH";
    break;
  case DUP:
    std::cout << "DUP";
    break;
  case SWAP:
    std::cout << "SWAP";
    break;
  case DISCARD:
    std::cout << "DISCARD";
    break;
  case COPY:
    std::cout << "COPY";
    break;
  case SLIDE:
    std::cout << "SLIDE";
    break;
  case ADD:
    std::cout << "ADD";
    break;
  case SUB:
    std::cout << "SUB";
    break;
  case MUL:
    std::cout << "MUL";
    break;
  case DIV:
    std::cout << "DIV";
    break;
  case MOD:
    std::cout << "MOD";
    break;
  case MARK:
    std::cout << "MARK";
    break;
  case CALL:
    std::cout << "CALL";
    break;
  case JUMP:
    std::cout << "JUMP";
    break;
  case JZ:
    std::cout << "JZ";
    break;
  case JN:
    std::cout << "JN";
    break;
  case RET:
    std::cout << "RET";
    break;
  case END:
    std::cout << "END";
    break;
  case STORE:
    std::cout << "STORE";
    break;
  case RETRIEVE:
    std::cout << "RETRIEVE";
    break;
  }
  std::cout << " " << inst.arg << "\n";
}

int main(int argc, char *argv[]) {
  std::ios::sync_with_stdio(false);
  if (argc != 3 || (argv[1][0] != 't' && argv[1][0] != 'f')) {
    std::cout << "Usage: " << argv[0] << " <use_jit:t|f> <filename>"
              << std::endl;
    return 1;
  }
  try {
    Reader reader{argv[2]};
    if (argv[1][0] == 't') {
      jit(reader.get_program());
    } else {
      simulate(reader.get_program());
    }
  } catch (char const *exc) {
    std::cerr << exc << std::endl;
    return 2;
  }
  return 0;
}
