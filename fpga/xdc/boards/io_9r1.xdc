#External IO ports to RF connector
set_property PACKAGE_PIN A14  [get_ports FPGA_EXTIO0];
set_property PACKAGE_PIN A13  [get_ports EXTIO0_DIR];
set_property PACKAGE_PIN B12  [get_ports FPGA_EXTIO1];
set_property PACKAGE_PIN A12  [get_ports EXTIO1_DIR];

#GPIO ports to P6 connector
set_property PACKAGE_PIN H10  [get_ports FPGA_GP0_p];
set_property PACKAGE_PIN H9   [get_ports FPGA_GP0_n];
set_property PACKAGE_PIN D9   [get_ports FPGA_GP1_p];
set_property PACKAGE_PIN C9   [get_ports FPGA_GP1_n];
set_property PACKAGE_PIN A10  [get_ports FPGA_GP2_p];
set_property PACKAGE_PIN A9   [get_ports FPGA_GP2_n];
set_property PACKAGE_PIN E10  [get_ports FPGA_GP3_p];
set_property PACKAGE_PIN E9   [get_ports FPGA_GP3_n];
set_property PACKAGE_PIN E11  [get_ports FPGA_GP4_p];
set_property PACKAGE_PIN D11  [get_ports FPGA_GP4_n];

#User LEDs
set_property PACKAGE_PIN B11  [get_ports LED_D3];
set_property PACKAGE_PIN B10  [get_ports LED_D4];
set_property PACKAGE_PIN C11  [get_ports LED_D5];
set_property PACKAGE_PIN C10  [get_ports LED_D6];

#On-Board System Monitor serial interface
set_property PACKAGE_PIN H11  [get_ports AVR_HS_CLK];
set_property PACKAGE_PIN J11  [get_ports AVR_HS_B2U];
set_property PACKAGE_PIN K11  [get_ports AVR_HS_U2B];

#Signals from/to XMC P5 connector
set_property PACKAGE_PIN K12  [get_ports PCIE_MRSTO_L];
set_property PACKAGE_PIN J12  [get_ports PCIE_MRSTI_L];
set_property PACKAGE_PIN K10  [get_ports MBIST_L];

