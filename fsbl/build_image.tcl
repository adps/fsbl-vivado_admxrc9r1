set current_dir [pwd]

set workspace_dir $current_dir/9r1_workspace

cd $workspace_dir
cd ../build_bootimg

puts [pwd]

set OS [lindex $tcl_platform(os) 0]
if { $OS == "Windows" } {
  exec mkimg.bat
} else {
  exec ./mkimg.sh
}

