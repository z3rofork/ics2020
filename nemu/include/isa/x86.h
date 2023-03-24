#ifndef __ISA_X86_H__
#define __ISA_X86_H__

#include <common.h>


// memory
#define x86_IMAGE_START 0x100000
#define x86_PMEM_BASE 0x0

// reg

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
  union 
  {
    union {
      uint32_t _32;
      uint16_t _16;
      uint8_t _8[2];   
    } gpr[8];
    struct {
      /* registers */
      rtlreg_t eax;
      rtlreg_t ecx;
      rtlreg_t edx;
      rtlreg_t ebx;
      rtlreg_t esp;
      rtlreg_t ebp;
      rtlreg_t esi;
      rtlreg_t edi;  
    };
    struct{
    uint32_t CF : 1;    // 进位标志位
    uint32_t reserved1 : 1;
    uint32_t PF : 1;    // 奇偶标志位
    uint32_t reserved2 : 1;
    uint32_t AF : 1;    // 辅助进位标志位
    uint32_t reserved3 : 1;
    uint32_t ZF : 1;    // 零标志位
    uint32_t SF : 1;    // 符号标志位
    uint32_t TF : 1;    // 跟踪标志位
    uint32_t IF : 1;    // 中断标志位
    uint32_t DF : 1;    // 方向标志位
    uint32_t OF : 1;    // 溢出标志位
    uint32_t IOPL : 2;  // I/O 特权级
    uint32_t NT : 1;    // 扩展处理器状态标志位
    uint32_t reserved4 : 1;
    uint32_t RF : 1;    // 保留标志位
    uint32_t VM : 1;    // 虚拟模式标志位
    uint32_t AC : 1;    // 对准检查标志位
    uint32_t VIF : 1;   // 虚拟中断标志位
    uint32_t VIP : 1;   // 虚拟中断屏蔽标志位
    uint32_t ID : 1;    // 检测处理器是否支持 CPUID 指令
    };   
  };
  /* Do NOT change the order of the GPRs' definitions. */

  /* In NEMU, rtlreg_t is exactly uint32_t. This makes RTL instructions
   * in PA2 able to directly access these registers.
   */
  //rtlreg_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
  vaddr_t pc;
} x86_CPU_state;

// decode
typedef struct {
  bool is_operand_size_16;
  uint8_t ext_opcode;
  const rtlreg_t *mbase;
  rtlreg_t mbr;
  word_t moff;
} x86_ISADecodeInfo;

#define suffix_char(width) ((width) == 4 ? 'l' : ((width) == 1 ? 'b' : ((width) == 2 ? 'w' : '?')))
#define isa_vaddr_check(vaddr, type, len) (MEM_RET_OK)
#define x86_has_mem_exception() (false)

#endif
