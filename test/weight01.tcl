source helpers.tcl
set test_name weight01  
read_lef ./nangate45.lef
read_def ./$test_name.def

# apply net weight as 1.2 on the _007_ net
set db [ord::get_db]
set block [[$db getChip] getBlock]
set dbNet [$block findNet _007_]
set_replace_custom_net_weight_cmd $dbNet 1.2

global_placement -init_density_penalty 0.01 -skip_initial_place -disable_routability_driven

set def_file [make_result_file $test_name.def]
write_def $def_file
diff_file $def_file $test_name.defok
