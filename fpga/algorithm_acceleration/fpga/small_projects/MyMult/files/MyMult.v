/// Implements a 32x32 multiplier using four 18x18 multipliers
module MyMult32(input [31:0] dataa, datab, output [63:0] dataout);
	wire [15:0] a, b, c, d;	
	assign a = dataa[31:16];
	assign b = dataa[15:0];
	assign c = datab[31:16];
	assign d = datab[15:0];
	
	wire [35:0] bd, ad, cb, ac;
	Mult18 mult0(.dataa({2'b0, b}), .datab({2'b0, d}), .dataout(bd));
	Mult18 mult1(.dataa({{2{a[15]}}, a}), .datab({2'b0, d}), .dataout(ad));
	Mult18 mult2(.dataa({{2{c[15]}}, c}), .datab({2'b0, b}), .dataout(cb));
	Mult18 mult3(.dataa({{2{a[15]}}, a}), .datab({{2{c[15]}}, c}), .dataout(ac));
	
	assign dataout[15:0] = bd[15:0];
	assign dataout[63:16] = {ac[31:0], bd[31:16]} + {{12{cb[35]}}, cb} + {{12{ad[35]}}, ad};
endmodule

/// This is a pipelined version of MyMult32
module MyMult32P(input clk, input [31:0] dataa, datab, output reg [63:0] dataout);
	wire [15:0] a, b, c, d;	
	assign a = dataa[31:16];
	assign b = dataa[15:0];
	assign c = datab[31:16];
	assign d = datab[15:0];
	
	wire [35:0] bd_w, ad_w, cb_w, ac_w;
	Mult18 mult0(.dataa({2'b0, b}), .datab({2'b0, d}), .dataout(bd_w));
	Mult18 mult1(.dataa({{2{a[15]}}, a}), .datab({2'b0, d}), .dataout(ad_w));
	Mult18 mult2(.dataa({{2{c[15]}}, c}), .datab({2'b0, b}), .dataout(cb_w));
	Mult18 mult3(.dataa({{2{a[15]}}, a}), .datab({{2{c[15]}}, c}), .dataout(ac_w));
	
	reg [35:0] bd, ad, cb, ac;
	always @ (posedge clk) begin
		bd <= bd_w;     ad <= ad_w;
		cb <= cb_w;     ac <= ac_w;
		dataout[15:0] <= bd[15:0];
		dataout[63:16] <= {ac[31:0], bd[31:16]} + {{12{cb[35]}}, cb} + {{12{ad[35]}}, ad};
	end
	
endmodule


/// Altera template that will compile into an 18bit multiplier
module Mult18
#(parameter WIDTH=18)
(
	input signed [WIDTH-1:0] dataa,
	input signed [WIDTH-1:0] datab,
	output [2*WIDTH-1:0] dataout
);

	assign dataout = dataa * datab;

endmodule

/// Implements a 48x48 multiplier using nine 18x18 multipliers
module MyMult48(input [47:0] dataa, datab, output [95:0] dataout);
	wire [15:0] a, b, c, x, y, z;	
	assign a = dataa[47:32];
	assign b = dataa[31:16];
	assign c = dataa[15:0];
	assign x = datab[47:32];
	assign y = datab[31:16];
	assign z = datab[15:0];
		
	wire [35:0] ax, ay, az, bx, by, bz, cx, cy, cz;
	Mult18 mult0(.dataa({2'b0, b}), .datab({2'b0, z}), .dataout(bz));
	Mult18 mult1(.dataa({2'b0, c}), .datab({2'b0, z}), .dataout(cz));
	Mult18 mult2(.dataa({2'b0, b}), .datab({2'b0, y}), .dataout(by));
	Mult18 mult3(.dataa({2'b0, c}), .datab({2'b0, y}), .dataout(cy));
	
	Mult18 mult4(.dataa({{2{a[15]}}, a}), .datab({2'b0, y}), .dataout(ay));
	Mult18 mult5(.dataa({{2{a[15]}}, a}), .datab({2'b0, z}), .dataout(az));
	Mult18 mult6(.dataa({{2{x[15]}}, x}), .datab({2'b0, b}), .dataout(bx));	
	Mult18 mult7(.dataa({{2{x[15]}}, x}), .datab({2'b0, c}), .dataout(cx));	
	
	Mult18 mult8(.dataa({{2{a[15]}}, a}), .datab({{2{x[15]}}, x}), .dataout(ax));
	
	assign dataout[15:0] = cz[15:0];
	assign dataout[95:16] = {{28{az[35]}}, az, cz[31:16]}
		+ {{12{ay[35]}}, ay, bz[31:0]} + {{12{bx[35]}}, bx, cy[31:0]}
		+ {ax[31:0], by[31:0], {16{1'b0}}} + {{28{cx[35]}}, cx, {16{1'b0}}};
endmodule

/// This is the pipelined version of the 48x48 multiplier
module MyMult48P(input clk, input [47:0] dataa, datab, output reg [95:0] dataout);	
	wire [15:0] a, b, c, x, y, z;	
	assign a = dataa[47:32];
	assign b = dataa[31:16];
	assign c = dataa[15:0];
	assign x = datab[47:32];
	assign y = datab[31:16];
	assign z = datab[15:0];
		
	wire [35:0] ax_w, ay_w, az_w, bx_w, by_w, bz_w, cx_w, cy_w, cz_w;
	reg [35:0] ax, ay, az, bx, by, bz, cx, cy, cz;
	Mult18 mult0(.dataa({2'b0, b}), .datab({2'b0, z}), .dataout(bz_w));
	Mult18 mult1(.dataa({2'b0, c}), .datab({2'b0, z}), .dataout(cz_w));
	Mult18 mult2(.dataa({2'b0, b}), .datab({2'b0, y}), .dataout(by_w));
	Mult18 mult3(.dataa({2'b0, c}), .datab({2'b0, y}), .dataout(cy_w));
	
	Mult18 mult4(.dataa({{2{a[15]}}, a}), .datab({2'b0, y}), .dataout(ay_w));
	Mult18 mult5(.dataa({{2{a[15]}}, a}), .datab({2'b0, z}), .dataout(az_w));
	Mult18 mult6(.dataa({{2{x[15]}}, x}), .datab({2'b0, b}), .dataout(bx_w));	
	Mult18 mult7(.dataa({{2{x[15]}}, x}), .datab({2'b0, c}), .dataout(cx_w));	
	
	Mult18 mult8(.dataa({{2{a[15]}}, a}), .datab({{2{x[15]}}, x}), .dataout(ax_w));
	
	reg [31:0] cz_delayed;
	reg [63:0] sum1;
	reg [64:0] sum2;
	
	always @ (posedge clk) begin
		// multiplier stage
		ax <= ax_w; ay <= ay_w; az <= az_w;
		bx <= bx_w; by <= by_w; bz <= bz_w;
		cx <= cx_w; cy <= cy_w; cz <= cz_w;
		
		//first adder stage
		cz_delayed <= cz[31:0];
		sum1 <= {{28{az[35]}},az} + {{28{cx[35]}},cx} + {ax[31:0],by[31:0]};
		sum2 <= {ay[32:0], bz[31:0]} + {bx[32:0], cy[31:0]};
		
		//second adder stage
		dataout[15:0] <= cz_delayed[15:0];
		dataout[95:16] <= {sum1, cz_delayed[31:16]} + {{16{sum2[64]}}, sum2[63:0]};
		
		// if a 2 stage pipeline is desired:
		/*dataout[95:16] <= {{28{az[35]}}, az, cz[31:16]}
			+ {{12{ay[35]}}, ay, bz[31:0]} + {{12{bx[35]}}, bx, cy[31:0]}
			+ {ax[31:0], by[31:0], {16{1'b0}}} + {{28{cx[35]}}, cx, {16{1'b0}}};*/
	end
endmodule

/// This is the smaller pipeliend version of the 48x48 multiplier, 
/// with the "cz" multiplier dropped
module MyMult48Ps(input clk, input [47:0] dataa, datab, output reg [95:16] dataout);	
	wire [15:0] a, b, c, x, y, z;	
	assign a = dataa[47:32];
	assign b = dataa[31:16];
	assign c = dataa[15:0];
	assign x = datab[47:32];
	assign y = datab[31:16];
	assign z = datab[15:0];
		
	wire [35:0] ax_w, ay_w, az_w, bx_w, by_w, bz_w, cx_w, cy_w;
	reg [35:0] ax, ay, az, bx, by, bz, cx, cy;
	Mult18 mult0(.dataa({2'b0, b}), .datab({2'b0, z}), .dataout(bz_w));
	Mult18 mult2(.dataa({2'b0, b}), .datab({2'b0, y}), .dataout(by_w));
	Mult18 mult3(.dataa({2'b0, c}), .datab({2'b0, y}), .dataout(cy_w));
	
	Mult18 mult4(.dataa({{2{a[15]}}, a}), .datab({2'b0, y}), .dataout(ay_w));
	Mult18 mult5(.dataa({{2{a[15]}}, a}), .datab({2'b0, z}), .dataout(az_w));
	Mult18 mult6(.dataa({{2{x[15]}}, x}), .datab({2'b0, b}), .dataout(bx_w));	
	Mult18 mult7(.dataa({{2{x[15]}}, x}), .datab({2'b0, c}), .dataout(cx_w));	
	
	Mult18 mult8(.dataa({{2{a[15]}}, a}), .datab({{2{x[15]}}, x}), .dataout(ax_w));
	
	reg [63:0] sum1;
	reg [64:0] sum2;
	
	always @ (posedge clk) begin
		// multiplier stage
		ax <= ax_w; ay <= ay_w; az <= az_w;
		bx <= bx_w; by <= by_w; bz <= bz_w;
		cx <= cx_w; cy <= cy_w;
		
		//first adder stage
		sum1 <= {{28{az[35]}},az} + {{28{cx[35]}},cx} + {ax[31:0],by[31:0]};
		sum2 <= {ay[32:0], bz[31:0]} + {bx[32:0], cy[31:0]};
		
		//second adder stage
		dataout[95:16] <= {sum1, {16{1'b0}}} + {{16{sum2[64]}}, sum2[63:0]};
	end
endmodule


/// This is a pipelined 32x48 multiplier, using six 18x18 multipliers
module MyMult40P(input clk, input [31:0] dataa, input [47:0] datab, output reg [79:0] dataout);	
	wire [15:0] a, b, c, x, y, z;	
	assign b = dataa[31:16];
	assign c = dataa[15:0];
	assign x = datab[47:32];
	assign y = datab[31:16];
	assign z = datab[15:0];
		
	wire [35:0] bx_w, by_w, bz_w, cx_w, cy_w, cz_w;
	reg [35:0] bx, by, bz, cx, cy, cz;
	Mult18 mult1(.dataa({2'b0, c}), .datab({2'b0, z}), .dataout(cz_w));
	Mult18 mult3(.dataa({2'b0, c}), .datab({2'b0, y}), .dataout(cy_w));
	
	Mult18 mult4(.dataa({{2{b[15]}}, b}), .datab({2'b0, y}), .dataout(by_w));
	Mult18 mult5(.dataa({{2{b[15]}}, b}), .datab({2'b0, z}), .dataout(bz_w));
	Mult18 mult7(.dataa({{2{x[15]}}, x}), .datab({2'b0, c}), .dataout(cx_w));	
	
	Mult18 mult8(.dataa({{2{b[15]}}, b}), .datab({{2{x[15]}}, x}), .dataout(bx_w));
	
	reg [31:0] cz_delayed;
	reg [32:0] sum1;
	reg [63:0] sum2;
	
	always @ (posedge clk) begin
		// product stage
		bx <= bx_w; by <= by_w; bz <= bz_w;
		cx <= cx_w; cy <= cy_w; cz <= cz_w;
		
		// first add stage
		cz_delayed <= cz[31:0];
		sum1 <= by[32:0] + cx[32:0];
		sum2 <= {{28{bz[35]}}, bz} + {bx[31:0], cy[31:0]};
		
		// second add stage
		dataout[15:0] <= cz_delayed[15:0];
		dataout[79:16] <= {{16{sum1[32]}}, sum1[31:0], cz_delayed[31:16]} + sum2;
		
		/* The old 2 stage pipeline case
		dataout[79:16] <= {{12{by[35]}}, by, cz[31:16]} + {{28{bz[35]}}, bz} 
			+ {bx[31:0], cy[31:0]} + {{12{cx[35]}}, cx, {16{1'b0}}};*/
	end
endmodule