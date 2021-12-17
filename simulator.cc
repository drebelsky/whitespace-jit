#include "simulator.h"
#include <deque>
#include <iostream>
#include <unordered_map>

void simulate(const Program &program) {
  size_t IP = 0;
  std::deque<int> stack{};
  std::deque<size_t> call_stack{};
  std::unordered_map<int, int> heap{};

  while (true) {
    if (IP >= program.instructions.size()) {
      throw "Finished execution without calling end";
    }
    const Instruction &inst = program.instructions.at(IP);
    switch (inst.type) {
    case READC: {
      char n;
      std::cin >> n;
      heap[stack.back()] = n;
      stack.pop_back();
    } break;
    case READN: {
      int n;
      std::cin >> n;
      heap[stack.back()] = n;
      stack.pop_back();
    } break;
    case WRITEC: {
      char c = stack.back();
      stack.pop_back();
      if (c == '\n')
        std::cout << std::endl;
      else
        std::cout << c;
    } break;
    case WRITEN: {
      int n = stack.back();
      stack.pop_back();
      std::cout << n;
    } break;
    case PUSH:
      stack.push_back(inst.arg);
      break;
    case DUP:
      stack.push_back(stack.back());
      break;
    case SWAP: {
      int a = stack.back();
      stack.pop_back();
      int b = stack.back();
      stack.pop_back();
      stack.push_back(a);
      stack.push_back(b);
    } break;
    case DISCARD:
      stack.pop_back();
      break;
    case COPY:
      stack.push_back(stack[stack.size() - inst.arg - 1]);
      break;
    case SLIDE:
      // TODO: double check
      stack.erase(stack.end() - inst.arg - 1, stack.end() - 1);
      break;
    case ADD: {
      int b = stack.back();
      stack.pop_back();
      int a = stack.back();
      stack.pop_back();
      stack.push_back(a + b);
    } break;
    case SUB: {
      int b = stack.back();
      stack.pop_back();
      int a = stack.back();
      stack.pop_back();
      stack.push_back(a - b);
    } break;
    case MUL: {
      int b = stack.back();
      stack.pop_back();
      int a = stack.back();
      stack.pop_back();
      stack.push_back(a * b);
    } break;
    case DIV: {
      int b = stack.back();
      stack.pop_back();
      int a = stack.back();
      stack.pop_back();
      stack.push_back(a / b);
    } break;
    case MOD: {
      int b = stack.back();
      stack.pop_back();
      int a = stack.back();
      stack.pop_back();
      stack.push_back(a % b);
    } break;
      // note -1 to account for the IP++ at the end
    case CALL:
      call_stack.push_back(IP + 1);
      IP = program.locations.at(inst.arg) - 1;
      break;
    case JUMP:
      IP = program.locations.at(inst.arg) - 1;
      break;
    case JZ: {
      int top = stack.back();
      stack.pop_back();
      if (top == 0) {
        IP = program.locations.at(inst.arg) - 1;
      }
    } break;
    case JN: {
      int top = stack.back();
      stack.pop_back();
      if (top < 0) {
        IP = program.locations.at(inst.arg) - 1;
      }
    } break;
    case RET:
      IP = call_stack.back() - 1;
      call_stack.pop_back();
      break;
    case END:
      return; // Important!
    case STORE: {
      int value = stack.back();
      stack.pop_back();
      int location = stack.back();
      stack.pop_back();
      heap[location] = value;
    } break;
    case RETRIEVE: {
      int top = stack.back();
      stack.pop_back();
      stack.push_back(heap.at(top)); // at for bounds checking
    } break;
    default:
      throw "Encountered invalid instruction";
    }
    IP++;
  }
}
