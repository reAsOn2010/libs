import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class Display {
	
	GridLayout experimentLayout = new GridLayout(0,2);
	
	MipsCPU cpu = new MipsCPU();
	Text_mode tm;
	
	JFrame f = new JFrame("Mips CPU");  
	JButton reg = new JButton("读取reg");
	JButton reg_in = new JButton("写入reg");
	JButton run = new JButton("运行");
	JTextArea ta = new JTextArea(8, 20);
	
	String[] regfile = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2",
			"$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
			"$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8",
			"$t9", "$gp", "$sp", "$fp", "$ra"};
	JComboBox reg_chooser = new JComboBox(regfile);
	JTextField code = new JTextField(24);
	JTextField code_in = new JTextField(24);
	public static void main(String[] args)
    {  
		//cpu.boot("mips.os"); //OS load
		//cpu.execute();
        //设置Swing窗口使用Java风格  
        JFrame.setDefaultLookAndFeelDecorated(true);   
        new Display().init();

    }
	
    public void init(){
    	JPanel west = new JPanel();
        west.setLayout(experimentLayout);
        west.add(reg_chooser);
        west.add(ta);
        
        JPanel east = new JPanel();
        east.setLayout(experimentLayout);
    	east.add(reg);
    	east.add(code);
    	east.add(reg_in);
    	east.add(code_in);
    	
        f.add(west , BorderLayout.WEST);
        f.add(east , BorderLayout.EAST);
        
        JPanel bottom = new JPanel();
        bottom.add(run);
        f.add(bottom, BorderLayout.SOUTH);
        reg.addActionListener(new ActionListener()  
        {  
            public void actionPerformed(ActionEvent e)  
            {
            	int temp;
        		temp = cpu.get_rgf(reg_chooser.getSelectedIndex());
                code.setText(cpu.code_to_str(temp));
            }  
        });
        reg_in.addActionListener(new ActionListener()  
        {  
            public void actionPerformed(ActionEvent e)  
            {
            	int temp;
            	temp = Integer.parseInt(code_in.getText());
            	//System.out.println(code_in.getText());
            	//System.out.println(Integer.parseInt(code_in.getText()));
                cpu.set_rgf(reg_chooser.getSelectedIndex(), temp);
            }  
        });
        run.addActionListener(new ActionListener()  
        {  
            public void actionPerformed(ActionEvent e)  
            {
            	cpu.boot("out");
            	cpu.execute();
            	byte[] mem = cpu.get_memory();
            	tm = new Text_mode(mem);
            	tm.start();
            	//ta.append("something");
            }  
        });

        /*
        reg_in.addActionListener(new ActionListener()  
        {  
            public void actionPerformed(ActionEvent e)  
            {
                code.setText(reg_chooser.getSelectedItem().toString());
            }  
        });
        */
    	f.pack();
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
        f.setVisible(true);  
    }
    
    class Text_mode extends Thread
    {
    	private byte[] VM;
    	public void run(){
    		String temp;
    		while(true){
    		for(int row = 0; row < 8; row++){
    			temp = "";
    			for(int col = 0; col < 20; col++){
    				if(VM[1024+row*20+col] == 0)
    					temp += " ";
    				else
    					temp += ((char)VM[256+row*20+col]);
    			}
    			ta.append(temp);
    			ta.append("\n");
    		}
    		ta.setText("");
    		}
    	}
    	Text_mode(byte[] in_VM){
    		VM = in_VM;
    	}
    }
    
}



