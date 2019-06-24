set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]

#SysRef/Refclk from RF clock distribution IC
set_property PACKAGE_PIN F14 [get_ports fpga_refclk0_p]
set_property PACKAGE_PIN F13 [get_ports fpga_refclk0_n]
set_property PACKAGE_PIN G12 [get_ports fpga_refclk1_p]
set_property PACKAGE_PIN F12 [get_ports fpga_refclk1_n]
set_property IOSTANDARD LVDS_25 [get_ports fpga_refclk0_p]
set_property IOSTANDARD LVDS_25 [get_ports fpga_refclk0_n]
set_property IOSTANDARD LVDS_25 [get_ports fpga_refclk1_p]
set_property IOSTANDARD LVDS_25 [get_ports fpga_refclk1_n]

#Programmable clock from digital clock distribution IC to PS_MGTREFCLK1 (default 150MHz)
set_property PACKAGE_PIN V29 [get_ports progclk0_p]
set_property PACKAGE_PIN V30 [get_ports progclk0_n]

#Programmable clock from digital clock distribution IC to MGTEFCLK0_128 (default 150MHz)
set_property PACKAGE_PIN M28 [get_ports progclk1_p]
set_property PACKAGE_PIN M29 [get_ports progclk1_n]
create_clock -period 6.667 -name progclk [get_ports progclk1_p]

#Programmable clock from digital clock distribution IC to FPGA fabric (default 150MHz)
set_property PACKAGE_PIN G11 [get_ports progclk2_p]
set_property PACKAGE_PIN G10 [get_ports progclk2_n]
set_property IOSTANDARD LVDS_25 [get_ports progclk2_p]
set_property IOSTANDARD LVDS_25 [get_ports progclk2_n]
create_clock -period 6.667 -name progclk [get_ports progclk2_p]

#Fixed 300MHz clock from digital clock distribution IC
set_property PACKAGE_PIN F10 [get_ports fabric_clk_p]
set_property PACKAGE_PIN F9 [get_ports fabric_clk_n]
set_property IOSTANDARD LVDS_25 [get_ports fabric_clk_p]
set_property IOSTANDARD LVDS_25 [get_ports fabric_clk_n]
create_clock -period 3.333 -name refclk300m [get_ports fabric_clk_p]

