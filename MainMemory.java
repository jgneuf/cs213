package arch.sm213.machine.student;

import machine.AbstractMainMemory;
import machine.AbstractMainMemory.InvalidAddressException;
import util.UnsignedByte;

/**
 * Main Memory of Simple CPU.
 *
 * Provides an abstraction of main memory (DRAM).
 */

public class MainMemory extends AbstractMainMemory {
	private byte [] mem;

	/**
	 * Allocate memory.
	 * @param byteCapacity size of memory in bytes.
	 */
	public MainMemory (int byteCapacity) {
		mem = new byte [byteCapacity];
	}

	/**
	 * Determine whether an address is aligned to specified length.
	 * @param address memory address.
	 * @param length byte length.
	 * @return true iff address is aligned to length.
	 */
	protected boolean isAccessAligned (int address, int length) {
		/* An address is aligned with respect to a 32-bit word if the address
		 * modulo length is zero. If not, there is an offset.
		 */
		if (address % length == 0)
			return true;
		return false;
	}

	/**
	 * Convert an sequence of four bytes into a Big Endian integer.
	 * @param byteAtAddrPlus0 value of byte with lowest memory address (base address).
	 * @param byteAtAddrPlus1 value of byte at base address plus 1.
	 * @param byteAtAddrPlus2 value of byte at base address plus 2.
	 * @param byteAtAddrPlus3 value of byte at base address plus 3 (highest memory address).
	 * @return Big Endian integer formed by these four bytes.
	 */
	public int bytesToInteger (UnsignedByte byteAtAddrPlus0,
			UnsignedByte byteAtAddrPlus1, UnsignedByte byteAtAddrPlus2,
			UnsignedByte byteAtAddrPlus3) {
		/* Each byte of 32-bit word has an offset. Model this by shifting each byte
		 * the appropriate amount. The first byte doesn't need to be shifted, but each
		 * successive byte must be shifted by four places further than the last.
		 */
		int firstByte, secondByte, thirdByte, fourthByte; // 32-bit representation for each byte
		firstByte = ((int) byteAtAddrPlus0.value()) << 24;
		secondByte = ((int) byteAtAddrPlus1.value()) << 16;
		thirdByte = ((int) byteAtAddrPlus2.value()) << 8;
		fourthByte = (int) byteAtAddrPlus3.value();

		/* Each byte is shifted to its correct position in a 32-bit word, so adding them
		 * will result in the full, 32-bit value.
		 */
		int result = firstByte + secondByte + thirdByte + fourthByte;
		return result;

	}

	/**
	 * Convert a Big Endian integer into an array of 4 bytes organized by memory address.
	 * @param i an Big Endian integer.
	 * @return an array of UnsignedByte where [0] is value of low-address byte of the number etc.
	 */
	public UnsignedByte[] integerToBytes (int i) {
		/* To obtain the first byte, ANDing the integer with a bit mask that tests
		 * only the first four bits, i.e. [1 1 1 1], or in base ten, 15. With this mask
		 * we can grab each successive byte by shifting them four places to align them
		 * with the mask.
		 */
		UnsignedByte[] ub = new UnsignedByte [4]; // Byte array, [0] is least significant

		ub[0] = new UnsignedByte (i >>> 24);
		ub[1] = new UnsignedByte (i >>> 16);
		ub[2] = new UnsignedByte (i >>> 8);
		ub[3] = new UnsignedByte (i);
		return ub;
	}

	/**
	 * Fetch a sequence of bytes from memory.
	 * @param address address of the first byte to fetch.
	 * @param length number of bytes to fetch.
	 * @return an array of UnsignedByte where [0] is memory value at address,
	 * [1] is memory value at address+1 etc.
	 */
	protected UnsignedByte[] get (int address, int length)
			throws InvalidAddressException {
		// Do not allow user to read beyond allocated memory
		if (address + length > mem.length || address < 0)
			throw new InvalidAddressException ();

		// Retrieve and return each byte requested
		UnsignedByte[] ub = new UnsignedByte[length];
		for (int i = 0; i < length; i++) {
			ub[i] = new UnsignedByte (mem[address + i]);
		}
		return ub;
	}

	/**
	 * Store a sequence of bytes into memory.
	 * @param address address of the first byte in memory to receive the specified value.
	 * @param value an array of UnsignedByte values to store in memory at the specified address.
	 * @throws InvalidAddressException if any address in the range address to
	 * address+value.length-1 is invalid.
	 */
	protected void set (int address, UnsignedByte[] value)
			throws InvalidAddressException {
		// Do not allow user to read beyond allocated memory
		if (address + value.length > mem.length || address < 0)
			throw new InvalidAddressException ();

		// Write the bytes into memory
		for (int i = 0; i < value.length; i++) {
			mem[address + i] = (byte) value[i].value();
		}
	}

	/**
	 * Determine the size of memory.
	 * @return the number of bytes allocated to this memory.
	 */
	public int length () {
		return mem.length;
	}

	public static void main (String[] args) {
		MainMemory mm = new MainMemory (256);

		// Tests for isAccessAligned
		if (mm.isAccessAligned(0, 4) == false)
			System.out.println ("Buggy");
		if (mm.isAccessAligned(16,4) == false)
			System.out.println ("Buggy");
		if (mm.isAccessAligned(3, 4) == true)
			System.out.println ("Buggy");
		if (mm.isAccessAligned(15,8) == true)
			System.out.println ("Buggy");

		// Tests for bytesToInteger
		UnsignedByte byte1 = new UnsignedByte (0);
		UnsignedByte byte2 = new UnsignedByte (0);
		UnsignedByte byte3 = new UnsignedByte (0);
		UnsignedByte byte4 = new UnsignedByte (0);
		if (mm.bytesToInteger(byte1, byte2, byte3, byte4) != 0)
			System.out.println ("Buggy");

		byte1 = new UnsignedByte (13);
		byte2 = new UnsignedByte (5);
		byte3 = new UnsignedByte (1);
		byte4 = new UnsignedByte (0);
		if (mm.bytesToInteger(byte1, byte2, byte3, byte4) != 349)
			System.out.println ("Buggy");

		// Tests for integerToBytes
		UnsignedByte[] arr = mm.integerToBytes(0);
		if (arr[0].value() != arr[1].value()
				&& arr[1].value() != arr[2].value()
				&& arr[2].value() != arr[3].value()
				&& arr[3].value () != 0)
			System.out.println ("Buggy");

		arr = mm.integerToBytes(349);
		if (arr[0].value() != 13
				&& arr[1].value() != 5
				&& arr[2].value() != 1
				&& arr[3].value () != 0)
			System.out.println ("Buggy");

		// Tests for set
		try {
			System.out.println ("-- Testing Memory --");

			// Create some bytes to test
			UnsignedByte[] b1 = new UnsignedByte[4];
			b1[0] = new UnsignedByte(2);
			b1[1] = new UnsignedByte(3);
			b1[2] = new UnsignedByte(4);
			b1[3] = new UnsignedByte(5);

			UnsignedByte[] b2 = new UnsignedByte[4];
			b2[0] = new UnsignedByte(6);
			b2[1] = new UnsignedByte(7);
			b2[2] = new UnsignedByte(8);
			b2[3] = new UnsignedByte(9);

			UnsignedByte[] b3 = new UnsignedByte[4];
			b3[0] = new UnsignedByte(0);
			b3[1] = new UnsignedByte(1);
			b3[2] = new UnsignedByte(2);
			b3[3] = new UnsignedByte(3);

			UnsignedByte[] b4 = new UnsignedByte[4];
			b4[0] = new UnsignedByte(13);
			b4[1] = new UnsignedByte(12);
			b4[2] = new UnsignedByte(11);
			b4[3] = new UnsignedByte(10);

			// Add bytes to memory
			mm.set(0, b4);
			mm.set(4, b3);
			mm.set(8, b2);
			mm.set(12, b1);

			// Print memory
			UnsignedByte[] allMem = mm.get(0, 16);
			for (int i = 0; i < 16; i++) {
				System.out.println("MEM[" + i + "] : " + allMem[i].value());
			}
		} catch (InvalidAddressException e) {
			System.out.println ("Invalid Address Exception -- Expected");
		}
	}
}