set current_dir [pwd]

set workspace_dir $current_dir/9r1_workspace
set fsbl_sources_dir $current_dir/sources/

set hw_proj 9r1_hw
set bsp_proj 9r1_bsp
set pmufw_bsp_proj 9r1_pmufw_bsp
set fsbl_proj 9r1_fsbl
set pmufw_proj 9r1_pmufw

sdk setws $workspace_dir

cd $workspace_dir

#Genreate hw, bsp and application projects
sdk createhw -name $hw_proj -hwspec $fsbl_sources_dir/hdf/admxrc9r1_simple.hdf
sdk createbsp -name $bsp_proj -hwproject $hw_proj -proc psu_cortexa53_0 -os standalone -arch 64
setlib -bsp $bsp_proj -lib xilffs
setlib -bsp $bsp_proj -lib xilsecure
setlib -bsp $bsp_proj -lib xilpm
setlib -bsp $bsp_proj -lib xilfpga
updatemss -mss $bsp_proj/system.mss
regenbsp -bsp $bsp_proj
sdk createbsp -name $pmufw_bsp_proj -hwproject $hw_proj -proc psu_pmu_0 -os standalone
setlib -bsp $pmufw_bsp_proj -lib xilfpga
setlib -bsp $pmufw_bsp_proj -lib xilsecure
setlib -bsp $pmufw_bsp_proj -lib xilskey
updatemss -mss $pmufw_bsp_proj/system.mss
regenbsp -bsp $pmufw_bsp_proj
sdk createapp -name $fsbl_proj -app {Zynq MP FSBL} -proc psu_cortexa53_0 -hwproject $hw_proj -bsp $bsp_proj -lang c
sdk createapp -name $pmufw_proj -app {ZynqMP PMU Firmware} -proc psu_pmu_0 -hwproject $hw_proj -bsp $pmufw_bsp_proj -lang c

#Delete XSDK generated sources
file delete -force ./$fsbl_proj/src
#Copy Alpha-Data FSBL sources to project
file copy $fsbl_sources_dir/src $workspace_dir/$fsbl_proj

#build the project
sdk build

cd ../build_bootimg

set OS [lindex $tcl_platform(os) 0]
if { $OS == "Windows" } {
  exec mkimg.bat
} else {
  exec ./mkimg.sh
}
