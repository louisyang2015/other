module XXX_tb;
	bit clk;

	XXX dut(...);

	initial begin
		// ...
	end
	
	task print_status;
		$display("xxx");
	endtask
	
	task run_n_cycles(int n);
		int i; 
		for(i=0; i<n; i++) begin
			clk_1; print_status;
		end
	endtask

	task clk_1_short;
		#1 clk = 1;
		#2 clk = 0;
	endtask

	task clk_1;
		#2 clk = 1;
		#2 clk = 0;
	endtask

	task clk_n(int n);
		int i;
		for(i = 0; i < n; i = i + 1)
			clk_1;
	endtask
endmodule


