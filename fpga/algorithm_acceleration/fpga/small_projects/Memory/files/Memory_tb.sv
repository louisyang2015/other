module Mem16_tb;
	bit clk, wren;
	bit [15:0] data;
	bit [8:0] rdaddress, wraddress; 
	
	bit [15:0] q;
	
	Mem16 dut(.clock(clk), .data(data), .rdaddress(rdaddress),
		.wraddress(wraddress), .wren(wren), .q(q));

	initial begin
		read_array(0, 30);		
		write(8'd0, 16'h0234);
		write(8'd1, 16'h1234);
		write(8'd2, 16'h2234);
		read(8'd0);
		read(8'd1);
		read(8'd2);
	end
	
	task print_read_status;
		$display("rdaddress=%d \t q=%h", rdaddress, q);
	endtask
	
	task read_array(int start_index, int end_index);		
		int i;
		$display("Reading from %d to %d", start_index, end_index);
		for(i=start_index; i<=end_index; i++) begin
			rdaddress=i; print_read_status; clk_1;
		end
		print_read_status; clk_1;
		print_read_status; clk_1;
		print_read_status; clk_1;
	endtask
	
	task read(bit[8:0] address);
		$display("Reading address %d", address);
		rdaddress = address;
		clk_n(3);
		$display("Result = %h", q);
	endtask
	
	task write(bit[8:0] address, bit[15:0] write_data);
		$display("Writing %h to address %d", write_data, address);
		wren = 1;
		wraddress = address;
		data = write_data;
		clk_1;
		wren = 0;
		clk_1;
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


module Mem32_tb;
	bit clk, wren;
	bit [31:0] data;
	bit [8:0] rdaddress, wraddress; 
	
	bit [31:0] q;
	
	Mem32 dut(.clock(clk), .data(data), .rdaddress(rdaddress),
		.wraddress(wraddress), .wren(wren), .q(q));

	initial begin
		read_array(0, 30);		
		write(8'd0, 32'h0234);
		write(8'd1, 32'h1234);
		write(8'd2, 32'h2234);
		read(8'd0);
		read(8'd1);
		read(8'd2);
	end
	
	task print_read_status;
		$display("rdaddress=%d \t q=%h", rdaddress, q);
	endtask
	
	task read_array(int start_index, int end_index);		
		int i;
		$display("Reading from %d to %d", start_index, end_index);
		for(i=start_index; i<=end_index; i++) begin
			rdaddress=i; print_read_status; clk_1;
		end
		print_read_status; clk_1;
		print_read_status; clk_1;
		print_read_status; clk_1;
	endtask
	
	task read(bit[8:0] address);
		$display("Reading address %d", address);
		rdaddress = address;
		clk_n(3);
		$display("Result = %h", q);
	endtask
	
	task write(bit[8:0] address, bit[31:0] write_data);
		$display("Writing %h to address %d", write_data, address);
		wren = 1;
		wraddress = address;
		data = write_data;
		clk_1;
		wren = 0;
		clk_1;
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



module Mem64_tb;
	bit clk, wren;
	bit [63:0] data;
	bit [8:0] rdaddress, wraddress; 
	
	bit [63:0] q;
	
	Mem64 dut(.clock(clk), .data(data), .rdaddress(rdaddress),
		.wraddress(wraddress), .wren(wren), .q(q));

	initial begin
		read_array(0, 30);		
		write(8'd0, 64'h0234);
		write(8'd1, 64'h1234);
		write(8'd2, 64'h2234);
		read(8'd0);
		read(8'd1);
		read(8'd2);
	end
	
	task print_read_status;
		$display("rdaddress=%d \t q=%h", rdaddress, q);
	endtask
	
	task read_array(int start_index, int end_index);		
		int i;
		$display("Reading from %d to %d", start_index, end_index);
		for(i=start_index; i<=end_index; i++) begin
			rdaddress=i; print_read_status; clk_1;
		end
		print_read_status; clk_1;
		print_read_status; clk_1;
		print_read_status; clk_1;
	endtask
	
	task read(bit[8:0] address);
		$display("Reading address %d", address);
		rdaddress = address;
		clk_n(3);
		$display("Result = %h", q);
	endtask
	
	task write(bit[8:0] address, bit[63:0] write_data);
		$display("Writing %h to address %d", write_data, address);
		wren = 1;
		wraddress = address;
		data = write_data;
		clk_1;
		wren = 0;
		clk_1;
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



module Rom48_tb;
	bit clk;
	bit [8:0] address; 
	
	bit [47:0] q;
	
	Rom48 dut(.clock(clk), .address(address), .q(q));

	initial begin
		read_array(0, 30);		
	end
	
	task print_read_status;
		$display("address=%d \t q=%h", address, q);
	endtask
	
	task read_array(int start_index, int end_index);		
		int i;
		$display("Reading from %d to %d", start_index, end_index);
		for(i=start_index; i<=end_index; i++) begin
			address=i; print_read_status; clk_1;
		end
		print_read_status; clk_1;
		print_read_status; clk_1;
		print_read_status; clk_1;
	endtask
	
	task read(bit[8:0] rdaddress);
		$display("Reading address %d", address);
		address = rdaddress;
		clk_n(3);
		$display("Result = %h", q);
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

