module MyMult32P_tb;
	bit clk;
	bit [31:0] dataa, datab;
	bit [63:0] dataout;
	int failures;
	
	MyMult32P dut (.clk(clk), .dataa(dataa), .datab(datab), .dataout(dataout));
	
	initial begin		
		int file_handle, i, temp;
		localparam int test_length = 100;
		bit [31:0] in_file_data[test_length * 2];	
		bit [63:0] out_file_data[test_length];
		
		file_handle = $fopen("MyMult/MyMult32P_in.bin", "rb");			
		// code = $fread(memory_array, fd, start, count);
		temp = $fread(in_file_data, file_handle, 0, test_length * 2);
		$fclose(file_handle);				
		file_handle = $fopen("MyMult/MyMult32P_out.bin", "rb");			
		// code = $fread(memory_array, fd, start, count);
		temp = $fread(out_file_data, file_handle, 0, test_length);
		$fclose(file_handle);				
				
		dataa[31:0] = in_file_data[0][31:0];
		datab[31:0] = in_file_data[0 + 1][31:0];
		clk_1;
		
		failures = 0;
		for(i = 1; i < test_length; i++)		begin
			dataa[31:0] = in_file_data[2*i][31:0];
			datab[31:0] = in_file_data[2*i + 1][31:0];
			clk_1;
			//dataout should match out_file_data[i-1]
			//$display("%h , %h", dataout, out_file_data[i-1]);
			check(dataout, out_file_data[i-1], i-1);			
		end
		clk_1;
		check(dataout, out_file_data[test_length-1], test_length-1);
				
		clk_n(4);
		check(dataout, out_file_data[test_length-1], test_length-1);
		
		$display("Total number of failures: %d", failures);		
	end
	
	task check(bit[31:0] dataout, bit[31:0] out_file_data, int test_case);
		if(dataout != out_file_data) begin
			failures++;
			if(failures < 10) begin
				$display("Test case # %d \t dataout = %h \t out_file_data = %h", 
					test_case, dataout, out_file_data);
			end
		end
	endtask
	
	
	task clk_1;
        #1 clk = 1;
        #1 clk = 0;
    endtask 
    
    task clk_n(int n);
        int i;
        for(i = 0; i < n; i = i + 1)
            clk_1;
    endtask
endmodule


module MyMult48_tb;
	bit [47:0] dataa, datab;
	bit [95:0] dataout;
	int failures;
	
	MyMult48 dut (.dataa(dataa), .datab(datab), .dataout(dataout));
	
	initial begin		
		int file_handle, i, temp;
		localparam int test_length = 100;
		bit [47:0] in_file_data[test_length * 2];	
		bit [95:0] out_file_data[test_length];
		
		file_handle = $fopen("MyMult/MyMult48_in.bin", "rb");			
		// code = $fread(memory_array, fd, start, count);
		temp = $fread(in_file_data, file_handle, 0, test_length * 2);
		$fclose(file_handle);				
		file_handle = $fopen("MyMult/MyMult48_out.bin", "rb");			
		// code = $fread(memory_array, fd, start, count);
		temp = $fread(out_file_data, file_handle, 0, test_length);
		$fclose(file_handle);				
		
		failures = 0;
		for(i = 0; i < test_length; i++)		begin
			dataa[47:0] = in_file_data[2*i][47:0];
			datab[47:0] = in_file_data[2*i + 1][47:0];
			#2;
			//dataout should match out_file_data[i]
			//$display("%h , %h", dataout, out_file_data[i]);
			check(dataout[95:0], out_file_data[i], i);			
		end		
		$display("Total number of failures: %d", failures);		
	end
	
	task check(bit[95:0] dataout, bit[95:0] out_file_data, int test_case);
		if(dataout != out_file_data) begin
			failures++;
			if(failures < 10) begin
				$display("Test case # %d \t dataout = %h \t out_file_data = %h", 
					test_case, dataout, out_file_data);
			end
		end
	endtask        
endmodule

module MyMult48P_tb;
	bit clk;
	bit [47:0] dataa, datab;
	bit [95:0] dataout;
	int failures;
	
	MyMult48P dut (.clk(clk), .dataa(dataa), .datab(datab), .dataout(dataout));
	
	initial begin		
		int file_handle, i, temp;
		localparam int test_length = 100;
		bit [47:0] in_file_data[test_length * 2];	
		bit [95:0] out_file_data[test_length];
		
		file_handle = $fopen("MyMult/MyMult48_in.bin", "rb");			
		// code = $fread(memory_array, fd, start, count);
		temp = $fread(in_file_data, file_handle, 0, test_length * 2);
		$fclose(file_handle);				
		file_handle = $fopen("MyMult/MyMult48_out.bin", "rb");			
		// code = $fread(memory_array, fd, start, count);
		temp = $fread(out_file_data, file_handle, 0, test_length);
		$fclose(file_handle);				
				
		dataa[47:0] = in_file_data[0][47:0];
		datab[47:0] = in_file_data[0 + 1][47:0];
		clk_1;
		dataa[47:0] = in_file_data[2][47:0];
		datab[47:0] = in_file_data[2 + 1][47:0];
		clk_1;
		
		failures = 0;
		for(i = 2; i < test_length; i++)		begin
			dataa[47:0] = in_file_data[2*i][47:0];
			datab[47:0] = in_file_data[2*i + 1][47:0];
			clk_1;
			//dataout should match out_file_data[i-1]
			//$display("%h , %h", dataout, out_file_data[i-1]);
			check(dataout, out_file_data[i-2], i-2);
		end
		clk_1;
		check(dataout, out_file_data[test_length-2], test_length-2);
		clk_1;
		check(dataout, out_file_data[test_length-1], test_length-1);
				
		clk_n(4);
		check(dataout, out_file_data[test_length-1], test_length-1);
		
		$display("Total number of failures: %d", failures);		
	end
	
	task check(bit[95:0] dataout, bit[95:0] out_file_data, int test_case);
		if(dataout != out_file_data) begin
			failures++;
			if(failures < 10) begin
				$display("Test case # %d \t dataout = %h \t out_file_data = %h", 
					test_case, dataout, out_file_data);
			end
		end
	endtask
	
	
	task clk_1;
        #1 clk = 1;
        #1 clk = 0;
    endtask 
    
    task clk_n(int n);
        int i;
        for(i = 0; i < n; i = i + 1)
            clk_1;
    endtask
endmodule

module MyMult40P_tb;
	bit clk;
	bit [31:0] dataa;
	bit [47:0] datab;
	bit [79:0] dataout;
	int failures;
	
	MyMult40P dut (.clk(clk), .dataa(dataa), .datab(datab), .dataout(dataout));
	
	initial begin		
		int file_handle, i, temp;
		localparam int test_length = 100;
		bit [79:0] in_file_data[test_length];	
		bit [79:0] out_file_data[test_length];
		
		file_handle = $fopen("MyMult/MyMult40_in.bin", "rb");			
		// code = $fread(memory_array, fd, start, count);
		temp = $fread(in_file_data, file_handle, 0, test_length);
		$fclose(file_handle);
		file_handle = $fopen("MyMult/MyMult40_out.bin", "rb");			
		// code = $fread(memory_array, fd, start, count);
		temp = $fread(out_file_data, file_handle, 0, test_length);
		$fclose(file_handle);				
				
		dataa[31:0] = in_file_data[0][79:48];
		datab[47:0] = in_file_data[0][47:0];
		clk_1;
		dataa[31:0] = in_file_data[1][79:48];
		datab[47:0] = in_file_data[1][47:0];
		clk_1;
		
		failures = 0;
		for(i = 2; i < test_length; i++)		begin
			dataa[31:0] = in_file_data[i][79:48];
			datab[47:0] = in_file_data[i][47:0];
			clk_1;
			//dataout should match out_file_data[i-1]
			//$display("%h , %h", dataout, out_file_data[i-1]);
			check(dataout, out_file_data[i-2], i-2);
		end
		clk_1;
		check(dataout, out_file_data[test_length-2], test_length-2);
		clk_1;
		check(dataout, out_file_data[test_length-1], test_length-1);
				
		clk_n(4);
		check(dataout, out_file_data[test_length-1], test_length-1);
		
		$display("Total number of failures: %d", failures);		
	end
	
	task check(bit[79:0] dataout, bit[79:0] out_file_data, int test_case);
		if(dataout != out_file_data) begin
			failures++;
			if(failures < 10) begin
				$display("Test case # %d \t dataout = %h \t out_file_data = %h", 
					test_case, dataout, out_file_data);
			end
		end
	endtask
	
	
	task clk_1;
        #1 clk = 1;
        #1 clk = 0;
    endtask 
    
    task clk_n(int n);
        int i;
        for(i = 0; i < n; i = i + 1)
            clk_1;
    endtask
endmodule
