import java.io.*;
import java.lang.Thread;

class MipsCPU
{
	public static final int MAXMEM =8192;

	private int PC, IR, MDR;
	private int[] Rgf;
	static byte[] Memory;

	MipsCPU()
	{
		Rgf = new int[32];
		Memory = new byte[MAXMEM];

		Rgf[0]=0; //$zero
	}
	
	class boot_error{
		
	}
	
	public void boot(String fn)
	{
		try{
			DataInputStream in = 
					new DataInputStream(
							new FileInputStream(
									new File(fn)));
			in.read(Memory);
			in.close();
		}
		catch(IOException ex){
			ex.printStackTrace();
		}
		PC=0;
	}

	public int execute()
	{
		int op, rs, rt, rd, dat, adr, sft, fun;

		for(;;){
			IR = 0x00000000;
			/*c file form*/
			IR = unsignedByteToInt(Memory[PC+3]) << 24
					|unsignedByteToInt(Memory[PC+2]) << 16
					|unsignedByteToInt(Memory[PC+1]) << 8
					|unsignedByteToInt(Memory[PC+0]);
			/*
			IR = unsignedByteToInt(Memory[PC+0]) << 24 
					| unsignedByteToInt(Memory[PC+1]) << 16
					| unsignedByteToInt(Memory[PC+2]) << 8
					| unsignedByteToInt(Memory[PC+3]);
			*/
			/*
			IR = (Memory[PC+0]<<24)
					|(Memory[PC+1]<<16)
					|(Memory[PC+2]<<8)
					|Memory[PC+3]; //Big-Endian
			*/
            //IR = (Memory[PC+3]<<24)|(Memory[PC+2]<<16)|(Memory[PC+1]<<8)|Memory[PC+0]; //Little-Endian
			PC+=4;
			
			op =(IR>>26)&63; //IR{31..26}
			rs =(IR>>21)&31; //IR{25..21}
			rt =(IR>>16)&31; //IR{20..16}
			rd =(IR>>11)&31; //IR{15..11}
			sft=(IR>>6)&31; //IR{11..6}
			fun=IR&63; //IR{5..0}
			dat=(int)(short)(IR&0xFFFF); //IR{15..0}
			adr=(IR&0x3FFFFFF)<<2; //IR{25..0}<<2
			
			//System.out.println(IR);
			//System.out.println(code_to_str(IR));
			//System.out.println(op + " "  + rs + " "  + rt + " "  + rd + " "  + sft + " "  + fun);
			//System.out.println(dat);
			switch(op){
			case 0: //R-type
				switch(fun){
				case 32: //ADD
					Rgf[rd] = Rgf[rs]+Rgf[rt];
					break;
				case 34: //SUB
					Rgf[rd] = Rgf[rs]-Rgf[rt];
					break;
				case 0: //SLL
					Rgf[rd] = Rgf[rt] << sft;
					break;
				case 2: //SRL
					Rgf[rd] = Rgf[rt] >> sft;
					break;
				case 36: //AND
					Rgf[rd] = Rgf[rs] & Rgf[rt];
					break;
				case 39: //NOR
					Rgf[rd] = Rgf[rs] ^ Rgf[rt];
					break;
				case 37: //OR
					Rgf[rd] = Rgf[rs] | Rgf[rt];
					break;
				case 42: //SLT
					Rgf[rd] = Rgf[rs] < Rgf[rt] ? 1 : 0;
					break;
				case 13:
					return -1;
				default:
					return -1;
				}
				break;
			case 35: //LW
				Rgf[rt] =
				(Memory[Rgf[rs]+dat+0]<<24)
				|(Memory[Rgf[rs]+dat+1]<<16)
				|(Memory[Rgf[rs]+dat+2]<<8)
				| Memory[Rgf[rs]+dat+3];
				break;
			case 43: //SW
				Memory[Rgf[rs]+dat+0] = (byte)((Rgf[rt]>>24)&0xFF);
				Memory[Rgf[rs]+dat+1] = (byte)((Rgf[rt]>>16)&0xFF);
				Memory[Rgf[rs]+dat+2] = (byte)((Rgf[rt]>>8)&0xFF);
				Memory[Rgf[rs]+dat+3] = (byte)( Rgf[rt]&0xFF);
				break;
			case 4: //BEQ
				if(Rgf[rs] == Rgf[rt])PC+=(dat<<2);
				break;
			case 2: //J
				PC=PC&0xFFFFFFF | adr;
				break;
			case 8: //ADDi
				Rgf[rt] = Rgf[rs] + dat;
				break;
			case 40: //SB
				Memory[Rgf[rs]+dat] = (byte)((Rgf[rt])&0xFF);
				break;
			case 32: //LB
				Rgf[rt] = Memory[Rgf[rs]+dat+0] & 0xFF;
				break;
			default:
				//System.out.println("Instruction Error!");
				return -1;
				// break;
			}
            //break;
			//System.out.print(Rgf[rt]);
			//System.out.print(Memory[1024]);
		} //for
	}

	public int get_rgf(int index){
		return Rgf[index];
	}
	public void set_rgf(int index, int data){
		Rgf[index] = data;
		//System.out.println(Rgf[index]);
	}
	public byte[] get_memory(){
		return Memory;
	}
	public String code_to_str(int code){
		String tmp = "";
		String ret = "";
		for(int i = 0; i < 32; i++){
			tmp += code & 0x00000001;
			code >>= 1;
		}
		for(int i=tmp.length()-1;i>=0;i--)
		{
			ret += String.valueOf(tmp.charAt(i));
		}
		return ret;
	}
	
	public static int unsignedByteToInt(byte b) {  
		return (int) b & 0xFF;  
	}

	
	public static void main(String[] args)
	{
		
		MipsCPU cpu = new MipsCPU();
		Thread text = new Text_mode(Memory);
		cpu.boot("mips.os" +
				""); //OS load
		cpu.execute();
		text.start();
		
		//System.out.println("Hello World!");
		//System.out.println((char)41);
	}
}

class Text_mode extends Thread
{
	private byte[] VM;
	public void run(){
		for(int row = 0; row < 16; row++){
			for(int col = 0; col < 16; col++){
				if(VM[1024+row*16+col] == 0)
					System.out.print(" ");
				else
					System.out.print((char)VM[1024+row*4+col]);
			}
			System.out.println();
		}
	}
	Text_mode(byte[] in_VM){
		VM = in_VM;
	}
}
