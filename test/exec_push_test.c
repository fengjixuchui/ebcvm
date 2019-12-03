/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"
#include <assert.h>

static void push_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = PUSH;
  _inst->op1_indirect = false;
  /* XXX: when operand1 is R0, the behavior is undefined */
  for (_inst->operand1 = R1; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[R0] = 0x14;
    _vm->regs->regs[_inst->operand1] = 0x01234567;
    _vm = exec_op(_vm, _inst);
    assert(read_mem32(_vm->mem, _vm->regs->regs[R0]) == 0x01234567);
    assert(_vm->regs->regs[R0] == 0x10);
    fini_vm(_vm);
  }
}

static void push_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->opcode = PUSH;
  _inst->op1_indirect = false;
  /* XXX: when operand1 is R0, the behavior is undefined */
  for (_inst->operand1 = R1; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[R0] = 0x18;
    _vm->regs->regs[_inst->operand1] = 0x0123456789abcdef;
    _vm = exec_op(_vm, _inst);
    assert(read_mem64(_vm->mem, _vm->regs->regs[R0]) == 0x0123456789abcdef);
    assert(_vm->regs->regs[R0] == 0x10);
    fini_vm(_vm);
  }
}

static void push_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->is_64op = false;
  _inst->opcode = PUSH;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;
  _inst->imm = 0x4567;
  _vm = init_vm();
  _vm->regs->regs[R0] = 0x14;
  _vm->regs->regs[_inst->operand1] = 0x01230000;
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, _vm->regs->regs[R0]) == 0x01234567);
  assert(_vm->regs->regs[R0] == 0x10);
  fini_vm(_vm);
}

static void push_test04() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->is_64op = true;
  _inst->opcode = PUSH;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;
  _inst->imm = 0xcdef;
  _vm = init_vm();
  _vm->regs->regs[R0] = 0x18;
  _vm->regs->regs[_inst->operand1] = 0x0123456789ab0000;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, _vm->regs->regs[R0]) == 0x0123456789abcdef);
  assert(_vm->regs->regs[R0] == 0x10);
  fini_vm(_vm);
}

static void push_test05() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = PUSH;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;
  _vm = init_vm();
  _vm->regs->regs[R0] = 0x14;
  _vm->regs->regs[_inst->operand1] = 0x20;
  write_mem32(_vm->mem, 0x20, 0x01234567);
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, _vm->regs->regs[R0]) == 0x01234567);
  assert(_vm->regs->regs[R0] == 0x10);
  fini_vm(_vm);
}

static void push_test06() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->opcode = PUSH;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;
  _vm = init_vm();
  _vm->regs->regs[R0] = 0x18;
  _vm->regs->regs[_inst->operand1] = 0x20;
  write_mem64(_vm->mem, _vm->regs->regs[_inst->operand1], 0x0123456789abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, _vm->regs->regs[R0]) == 0x0123456789abcdef);
  assert(_vm->regs->regs[R0] == 0x10);
  fini_vm(_vm);
}

static void push_test07() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->is_64op = false;
  _inst->opcode = PUSH;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _vm = init_vm();
  _vm->regs->regs[R0] = 0x14;
  _vm->regs->regs[_inst->operand1] = 0x20;
  write_mem32(_vm->mem, 0x30, 0x01234567);
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, _vm->regs->regs[R0]) == 0x01234567);
  assert(_vm->regs->regs[R0] == 0x10);
  fini_vm(_vm);
}

static void push_test08() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->is_64op = true;
  _inst->opcode = PUSH;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _vm = init_vm();
  _vm->regs->regs[R0] = 0x18;
  _vm->regs->regs[_inst->operand1] = 0x20;
  write_mem64(_vm->mem, 0x30, 0x0123456789abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, _vm->regs->regs[R0]) == 0x0123456789abcdef);
  assert(_vm->regs->regs[R0] == 0x10);
  fini_vm(_vm);
}

int main() {
  push_test01();
  push_test02();
  push_test03();
  push_test04();
  push_test05();
  push_test06();
  push_test07();
  push_test08();

  return 0;
}
