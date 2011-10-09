/*
 * Name: Jonathan Neufeld
 * Student Number: 30671093
 * CS ID: p9d8 
 */
package arch.sm213.machine.student;

import arch.sm213.machine.AbstractSM213CPU;
import machine.AbstractMainMemory;
import machine.AbstractMainMemory.InvalidAddressException;
import machine.RegisterSet;
import machine.Register;
import util.DataModel;
import util.HalfByteNumber;
import util.SixByteNumber;
import util.UnsignedByte;

/**
 * The Simple Machine CPU.
 *
 * Simulate the execution of a single cycle of the Simple Machine SM213 CPU. 
 */

public class CPU extends AbstractSM213CPU {

	/**
	 * Create a new CPU.
	 *
	 * @param name   fully-qualified name of CPU implementation.
	 * @param memory main memory used by CPU.
	 */
	public CPU (String name, AbstractMainMemory memory) {
		super (name, memory);
	}

	/**
	 * Fetch Stage of CPU Cycle.
	 * Fetch instruction at address stored in "pc" register from memory into instruction register 
	 * and set "pc" to point to the next instruction to execute.
	 *
	 * Input register:   pc.
	 * Output registers: pc, instruction, insOpCode, insOp0, insOp1, insOp2, insOpImm, insOpExt
	 * @see AbstractSM213CPU for pc, instruction, insOpCode, insOp0, insOp1, insOp2, insOpImm, 
	 * insOpExt
	 *
	 * @throws MainMemory.InvalidAddressException when program counter contains an invalid 
	 * memory address.
	 */
	protected void fetch () throws MainMemory.InvalidAddressException {
		// Bit masks for ANDing
		long UPPER_BIT_MASK = 0xf0;
		long LOWER_BIT_MASK = 0x0f;

		// Get full instruction from memory at PC
		int baseAddress = pc.get();
		instruction.set((((long)mem.readIntegerUnaligned(baseAddress)) << 16) 
				+ (mem.readIntegerUnaligned(baseAddress + 2)));
		UnsignedByte[] insArr = mem.readUnaliged(baseAddress, 6);

		/* Store opcode locally. insOpCode doesn't update until fetch() is complete,
		 but we need the opcode later to check instruction length. */
		long opcode = (insArr[0].value() & UPPER_BIT_MASK) >> 4;
		insOpCode.set(opcode); 

		// Set each special register by ANDing with appropriate bit mask
		insOp0.set(insArr[0].value()  & LOWER_BIT_MASK);	   // op0: lower order bits, first byte
		insOp1.set((insArr[1].value() & UPPER_BIT_MASK) >> 4); // op1: higher order bits, second byte
		insOp2.set(insArr[1].value()  & LOWER_BIT_MASK);	   // op2: lower order bits, second byte
		insOpImm.set(insArr[1].value());					   // imm: full second byte

		// Opcode 0 requires four more bytes, adjust PC and insOpExt
		if (opcode == 0 || opcode == 0xb) {
			insOpExt.set(mem.readIntegerUnaligned(baseAddress + 2));
			pc.set(baseAddress + 6);
		}
		// Non-zero opcodes are two bytes long, adjust PC by two
		else {
			pc.set(baseAddress + 2);
		}
	}

	/**
	 * Execution Stage of CPU Cycle.
	 * Execute instruction that was fetched by Fetch stage.
	 *
	 * Input state: pc, instruction, insOpCode, insOp0, insOp1, insOp2, insOpImm, insOpExt, reg, mem
	 * Ouput state: pc, reg, mem
	 * @see AbstractSM213CPU for pc, instruction, insOpCode, insOp0, insOp1, insOp2, insOpImm, insOpExt
	 * @see MainMemory       for mem
	 * @see machine.AbstractCPU      for reg
	 *
	 * @throws InvalidInstructionException                when instruction format is invalid.
	 * @throws MachineHaltException                       when instruction is the HALT instruction.
	 * @throws RegisterSet.InvalidRegisterNumberException when instruction references an invalid register (i.e, not 0-7).
	 * @throws MainMemory.InvalidAddressException         when instruction references an invalid memory address.
	 */
	protected void execute () throws InvalidInstructionException, 
	MachineHaltException, RegisterSet.InvalidRegisterNumberException, 
	MainMemory.InvalidAddressException {
		// Storage for source and destination registers, value and immediate value.
		int rs, rd, v, i, address;

		// Use the opcode to determine what instruction to execute.
		switch (insOpCode.get()) {
		// Load immediate:
		case 0:
			rd = insOp0.get();
			v  = insOpExt.get();
			reg.set(rd, v);
			break;

		// Load base + offset:
		case 1:
			rs = insOp1.get();
			rd = insOp2.get();
			i  = insOp0.get();
			address = (i * 4) + reg.get(rs);
			v = mem.readIntegerUnaligned(address);
			reg.set(rd, v);
			break;

		// Load indexed:
		case 2:
			rd = insOp2.get();
			rs = insOp0.get();
			i  = insOp1.get();
			address = reg.get(rs) + reg.get(i) * 4;
			v = mem.readIntegerUnaligned(address);
			reg.set(rd, v);
			break;

		// Store base + offset:
		case 3:			
			rs = insOp0.get();
			rd = insOp2.get();
			i  = insOp1.get();
			address = (i * 4) + reg.get(rd);
			mem.writeIntegerUnaligned(address, reg.get(rs));
			break;

		// Store indexed:
		case 4:
			rs = insOp0.get();
			rd = insOp1.get();
			i  = insOp2.get();
			v  = reg.get(rs);
			address = reg.get(rd) + (reg.get(i) * 4);
			mem.writeInteger(address, v);
			break;

		// ALU instructions, check specific instruction with second hexit:
		case 6:
			switch (insOp0.get()) {
			// Move:
			case 0:
				rs = insOp1.get();
				rd = insOp2.get();
				reg.set(rd, reg.get(rs));
				break;

			// Addition:
			case 1:
				rs = insOp1.get();
				rd = insOp2.get();
				reg.set(rd, reg.get(rs) + reg.get(rd));
				break;

			// Bitwise and:
			case 2:
				rs = insOp1.get();
				rd = insOp2.get();
				reg.set(rd, reg.get(rs) & reg.get(rd));
				break;

			// Increment by one:
			case 3:
				rd = insOp2.get();
				v  = reg.get(rd);
				v++;
				reg.set(rd, v);
				break;

			// Increment address by one:
			case 4:
				rd = insOp2.get();
				v  = reg.get(rd);
				v += 4;
				reg.set(rd, v);
				break;

			// Decrement by one:
			case 5:
				rd = insOp2.get();
				v  = reg.get(rd);
				v--;
				reg.set(rd, v);
				break;

			// Decrement address by one:
			case 6:
				rd = insOp2.get();
				v  = reg.get(rd);
				v -= 4;
				reg.set(rd, v);
				break;

			// Bitwise not:
			case 7:
				rd = insOp2.get();
				reg.set(rd, ~(reg.get(rd)));
				break;
				
			// Get PC:
			case 0xf:
				v  = 2 * insOp1.get();
				/* Extract rd from imm using a bitmask. */
				rd = insOp2.get(); 
				reg.set(rd, pc.get() + v);
				break;
			}
			break;

		// Shift:
		case 7:
			rd = insOp0.get();
			v  = insOpImm.get();
			// Negative Imm means shift right
			if (v < 0)
				reg.set(rd, rd >> v);
			// Positive Imm means shift left
			else
				reg.set(rd,  rd << v);
			break;
			
		// Branch:
		case 8:
			v = insOpImm.get() * 2;
			pc.set(pc.get() + v);
			break;
			
		// Branch if equal:
		case 9:
			rd = reg.get(insOp0.get()); 
			if (rd == 0) {
				v = insOpImm.get() * 2;
				pc.set(pc.get() + v);
			}
			break;

		// Branch if greater:
		case 0xa:
			rd = reg.get(insOp0.get());
			if (rd > 0) {
				v = insOpImm.get() * 2;
				pc.set(pc.get() + v);
			}
			break;
			
		// Unconditional jump:
		case 0xb:
			v = insOpExt.get();
			pc.set(v);
			break;
			
		// Indirect jump:
		case 0xc:
			v  = insOpImm.get() * 2;
			rs = reg.get(insOp0.get());
			pc.set(rs + v);
			break;
		
		// HALT and NOP instructions, use nested case to determine which:
		case 0xf:
			switch (insOp0.get()) {
			// HALT:
			case 0:
				throw new MachineHaltException();
			// NOP case, do nothing:
			case 0xf:
				break;
			}
			break;

		// No matching opcode, something bad happened:
		default:
			System.out.println("Error: Opcode not found ( " + insOpCode.get() + " )");
			throw new InvalidInstructionException();
		}
	}
}
