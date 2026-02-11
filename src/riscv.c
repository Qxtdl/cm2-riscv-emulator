#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "util.h"

#define ASM_BUF_SIZE 128

static inline void address(char *s, char *offset, char *reg) {
	strcat(s, offset);
	strcat(s, "(");
	strcat(s, reg);
	strcat(s, ")");
}

char *rv32i_instruction_to_str(uint32_t instruction) {
	uint32_t ir = instruction;

	uint8_t opcode = ir & 0x7f;
	uint8_t funct3 = (ir >> 12) & 0x7;
	uint8_t funct7 = (ir >> 25) & 0x7f;
	uint8_t rd = (ir >> 7) & 0x1f;
	uint8_t rs1 = (ir >> 15) & 0x1f;
	uint8_t rs2 = (ir >> 20) & 0x1f;

	uint32_t u_imm = (ir & 0xfffff000);
	uint32_t j_imm = ( ((ir & 0x80000000) ? 0xfff00000 : 0x0) |
	                   ((ir & 0x00100000) ? 0x00000800 : 0x0) |
	                   ((ir & 0x7fe00000) >> 20) |
	                   ((ir & 0x000ff000)) );
	uint32_t i_imm = ( ((ir & 0x80000000) ? 0xfffff000 : 0x0) |
	                   (ir >> 20) );
	uint32_t b_imm = ( ((ir & 0x80000000) ? 0xfffff000 : 0x0) |
	                   ((ir & 0x7e000000) >> 20) |
	                   ((ir & 0x00000f00) >> 7) |
	                   ((ir & 0x00000080) ? 0x00000800 : 0x0) );
	uint32_t s_imm = ( ((ir & 0x80000000) ? 0xfffff000 : 0x0) |
	                   ((ir & 0xfe000000) >> 20) |                  
	                   ((ir & 0x00000f80) >> 7) );

	static char assembly[ASM_BUF_SIZE];
	assembly[0] = '\0';

	char xrd[6], xrs1[6], xrs2[4],
					 _xrs1[4], _xrs2[6];
	snprintf(xrd, sizeof(xrd), "x%d, ", rd);
	snprintf(xrs1, sizeof(xrs1), "x%d, ", rs1);
	snprintf(_xrs1, sizeof(_xrs1), "x%d", rs1);
	snprintf(xrs2, sizeof(xrs2), "x%d", rs2);
	snprintf(_xrs2, sizeof(_xrs2), "x%d, ", rs2);

   switch (opcode) {
      case 0x37: // LUI
         strcat(assembly, "lui ");
         strcat(assembly, xrd);
         strcat(assembly, u32_to_hex(u_imm));
         break;
      case 0x17: // AUIPC
         strcat(assembly, "auipc ");
         strcat(assembly, xrd);
         strcat(assembly, u32_to_hex(u_imm));
         break;
      case 0x6f: // JAL
         strcat(assembly, "jal ");
         strcat(assembly, xrd);
         strcat(assembly, u32_to_hex(j_imm));
         break;
      case 0x67: // JALR
         strcat(assembly, "jalr ");
         strcat(assembly, xrd);
         strcat(assembly, xrs1);
         strcat(assembly, u32_to_hex(i_imm));
         break;
      case 0x63: // Branch
         switch (funct3) {
            case 0: strcat(assembly, "beq "); break;
            case 1: strcat(assembly, "bne "); break;
            case 4: strcat(assembly, "blt "); break;
            case 5: strcat(assembly, "bge "); break;
            case 6: strcat(assembly, "bltu "); break;
            case 7: strcat(assembly, "bgeu "); break;
         }
         strcat(assembly, xrs1);
         strcat(assembly, _xrs2);
         strcat(assembly, u32_to_hex(b_imm));    
         break;
      case 0x03: { // Load
         switch (funct3) {
            case 0: strcat(assembly, "lb "); break;
            case 1: strcat(assembly, "lh "); break;
            case 2: strcat(assembly, "lw "); break;
            case 4: strcat(assembly, "lbu "); break;
            case 5: strcat(assembly, "lhu "); break;
         }
         strcat(assembly, xrd);
         address(assembly, u32_to_hex(i_imm), _xrs1);
         break;
      }
      case 0x23: { // Store
         switch (funct3) {
            case 0: strcat(assembly, "sb "); break;
            case 1: strcat(assembly, "sh "); break;
            case 2: strcat(assembly, "sw "); break;
         }
         address(assembly, u32_to_hex(s_imm), _xrs1);
         strcat(assembly, ", ");
         strcat(assembly, xrs2);
         break;
      }
      case 0x13: // ARITH-I
      case 0x33: { // ARITH
	      if (opcode == 0x33 && funct3 == 0x0 && !funct7) strcat(assembly, "add "); // ADD
	      else if (opcode == 0x13 && funct3 == 0x0) { // ADDI
				strcat(assembly, "addi ");
				goto imm;
	      }
	      else if (funct3 == 0x0 && funct7) { // SUB
				strcat(assembly, "sub ");
				goto imm;		
	      }
	      else if (funct3 == 0x1) strcat(assembly, "sll "); // SLL
	      else if (funct3 == 0x2) strcat(assembly, "slt "); // SLT
	      else if (funct3 == 0x3) strcat(assembly, "sltu "); // SLTU
	      else if (funct3 == 0x4) { // XORI
	         strcat(assembly, "xori ");
	         goto imm;
			}
	      else if (funct3 == 0x5 && !funct7) strcat(assembly, "srl "); // SRL
	      else if (funct3 == 0x5 && funct7) strcat(assembly, "sra "); // SRA
	      else if (funct3 == 0x6) strcat(assembly, "or "); // OR
	      else if (funct3 == 0x7) strcat(assembly, "and "); // AND
         
      /* A bit ugly but works */
      // reg
		strcat(assembly, xrd);
		strcat(assembly, xrs1);
		strcat(assembly, xrs2);
		break;
		imm:
         strcat(assembly, xrd);
         strcat(assembly, xrs1);
         strcat(assembly, u32_to_hex(i_imm));
         break;
      }
      default: strcat(assembly, u32_to_hex(ir));
   }	
   return assembly;	
}
