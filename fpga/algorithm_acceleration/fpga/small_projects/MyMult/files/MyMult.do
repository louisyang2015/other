#MyMult32P_tb
if 0 {
vsim work.MyMult32P_tb
add wave \
{sim:/MyMult32P_tb/clk } \
-decimal {sim:/MyMult32P_tb/dataa } \
{sim:/MyMult32P_tb/datab } \
{sim:/MyMult32P_tb/dataout } 
run -all
}


#MyMult48_tb
if 0 {
vsim work.MyMult48_tb
add wave \
-decimal {sim:/MyMult48_tb/dataa } \
{sim:/MyMult48_tb/datab } \
{sim:/MyMult48_tb/dataout } 
run -all
}


#MyMult48P_tb
if 0 {
vsim work.MyMult48P_tb
add wave \
{sim:/MyMult48P_tb/clk } \
-decimal {sim:/MyMult48P_tb/dataa } \
{sim:/MyMult48P_tb/datab } \
{sim:/MyMult48P_tb/dataout } 
run -all
}

#MyMult40P_tb
#if 0 {
vsim work.MyMult40P_tb
add wave \
{sim:/MyMult40P_tb/clk } \
-decimal {sim:/MyMult40P_tb/dataa } \
{sim:/MyMult40P_tb/datab } \
{sim:/MyMult40P_tb/dataout } 
run -all
#}

