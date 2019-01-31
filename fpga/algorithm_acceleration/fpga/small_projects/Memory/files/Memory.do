# Mem16 test bench
if 0 {
vsim -L altera_mf_ver work.Mem16_tb 
add wave \
{sim:/Mem16_tb/wren } \
{sim:/Mem16_tb/clk } \
-color "orange" -unsigned {sim:/Mem16_tb/wraddress } \
-hex {sim:/Mem16_tb/data } \
-color "yellow" -unsigned {sim:/Mem16_tb/rdaddress } \
-hex {sim:/Mem16_tb/q }
run -all
}

# Mem32 test bench
if 0 {
vsim -L altera_mf_ver work.Mem32_tb 
add wave \
{sim:/Mem32_tb/wren } \
{sim:/Mem32_tb/clk } \
-color "orange" -unsigned {sim:/Mem32_tb/wraddress } \
-hex {sim:/Mem32_tb/data } \
-color "yellow" -unsigned {sim:/Mem32_tb/rdaddress } \
-hex {sim:/Mem32_tb/q }
run -all
}

# Mem64 test bench
if 0 {
vsim -L altera_mf_ver work.Mem64_tb 
add wave \
{sim:/Mem64_tb/wren } \
{sim:/Mem64_tb/clk } \
-color "orange" -unsigned {sim:/Mem64_tb/wraddress } \
-hex {sim:/Mem64_tb/data } \
-color "yellow" -unsigned {sim:/Mem64_tb/rdaddress } \
-hex {sim:/Mem64_tb/q }
run -all
}

# Rom48 test bench
vsim -L altera_mf_ver work.Rom48_tb
add wave \
{sim:/Rom48_tb/clk } \
-unsigned {sim:/Rom48_tb/address } \
-hex {sim:/Rom48_tb/q } 
run -all
