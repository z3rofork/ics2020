#include <isa.h>
#include <stdlib.h>
#include <time.h>
#include "local-include/reg.h"

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
  srand(time(0));
  word_t sample[8];
  word_t pc_sample = rand();
  cpu.pc = pc_sample;

  int i;
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
    reg_l(i) = sample[i];
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(pc_sample == cpu.pc);
}

void isa_reg_display() {
  printf("eax\t 0x%08x\t %d\t\n",cpu.gpr[0]._32,cpu.gpr[0]._32);
  printf("ecx\t 0x%08x\t %d\t\n",cpu.gpr[1]._32,cpu.gpr[1]._32);
  printf("edx\t 0x%08x\t %d\t\n",cpu.gpr[2]._32,cpu.gpr[2]._32);
  printf("ebx\t 0x%08x\t %d\t\n",cpu.gpr[3]._32,cpu.gpr[3]._32);
  printf("esp\t 0x%08x\t %d\t\n",cpu.gpr[4]._32,cpu.gpr[4]._32);
  printf("ebp\t 0x%08x\t %d\t\n",cpu.gpr[5]._32,cpu.gpr[5]._32);
  printf("esi\t 0x%08x\t %d\t\n",cpu.gpr[6]._32,cpu.gpr[6]._32);
  printf("edi\t 0x%08x\t %d\t\n",cpu.gpr[7]._32,cpu.gpr[7]._32);
  printf("eip\t 0x%08x\t\n",cpu.pc);
}

word_t isa_reg_str2val(const char *s, bool *success) {
  return 0;
}
