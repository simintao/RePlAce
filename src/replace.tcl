sta::define_cmd_args "global_placement" {
  [-skip_initial_place]\
  [-density target_density]\
  [-disable_timing_driven]\
  [-disable_routability_driven]\
  [-timing_driven]\
  [-incremental]\
  [-density target_density]\
    [-bin_grid_count grid_count]}

proc global_placement { args } {
  sta::parse_key_args "global_placement" args \
    keys {-bin_grid_count -wire_res -wire_cap -density \
      -init_density_penalty -init_wirelength_coef \
      -min_phi_coef -max_phi_coef -overflow \
      -initial_place_max_iter -initial_place_max_fanout \
      -routability_check_overflow -routability_max_density \
      -routability_max_bloat_iter -routability_max_inflation_iter \
      -verbose_level} \
      flags {-skip_initial_place -disable_timing_driven -disable_routability_driven -incremental}


  # flow control for initial_place
  if { [info exists flags(-skip_initial_place)] } {
    set_replace_initial_place_max_iter_cmd 0
  } elseif { [info exists keys(-initial_place_max_iter)] } { 
    set initial_place_max_iter $keys(-initial_place_max_iter)
    sta::check_positive_integer "-initial_place_max_iter" $initial_place_max_iter
    set_replace_initial_place_max_iter_cmd $initial_place_max_iter
  } 

  # flow control for timing-driven 
  if { [info exists flags(-disable_timing_driven)] } { 
    set_replace_disable_timing_driven_mode_cmd
  }

  # flow control for routability-driven 
  if { [info exists flags(-disable_routability_driven)] } {
    set_replace_disable_routability_driven_mode_cmd
  }
  
  # flow control for incremental GP
  if { [info exists flags(-incremental)] } {
    set_replace_initial_place_max_iter_cmd 0
    set_replace_incremental_place_mode_cmd
  }

  if { [info exists keys(-initial_place_max_fanout)] } { 
    set initial_place_max_fanout $keys(-initial_place_max_fanout)
    sta::check_positive_integer "-initial_place_max_fanout" $initial_place_max_fanout
    set_replace_initial_place_max_fanout_cmd $initial_place_max_fanout
  }
  
  # density settings    
  set target_density 0.7
  if { [info exists keys(-density)] } {
    set target_density $keys(-density) 
    sta::check_positive_float "-density" $target_density
  }
  set_replace_density_cmd $target_density

  if { [info exists keys(-routability_max_density)] } {
    set routability_max_density $keys(-routability_max_density)
    sta::check_positive_float "-routability_max_density" $routability_max_density
    set_replace_routability_max_density_cmd $routability_max_density
  }


  # parameter to control the RePlAce divergence
  if { [info exists keys(-min_phi_coef)] } { 
    set min_phi_coef $keys(-min_phi_coef)
    sta::check_positive_float "-min_phi_coef" $min_phi_coef
    set_replace_min_phi_coef_cmd $min_phi_coef
  } 

  if { [info exists keys(-max_phi_coef)] } { 
    set max_phi_coef $keys(-max_phi_coef)
    sta::check_positive_float "-max_phi_coef" $max_phi_coef
    set_replace_max_phi_coef_cmd $max_phi_coef  
  }

  if { [info exists keys(-init_density_penalty)] } {
    set density_penalty $keys(-init_density_penalty)
    sta::check_positive_float "-init_density_penalty" $density_penalty
    set_replace_init_density_penalty_factor_cmd $density_penalty
  }
  
  if { [info exists keys(-init_wirelength_coef)] } {
    set coef $keys(-init_wirelength_coef)
    sta::check_positive_float "-init_wirelength_coef" $coef
    set_replace_init_wirelength_coef_cmd $coef
  }
  
  if { [info exists keys(-bin_grid_count)] } {
    set bin_grid_count  $keys(-bin_grid_count)
    sta::check_positive_integer "-bin_grid_count" $bin_grid_count
    set_replace_bin_grid_cnt_x_cmd $bin_grid_count
    set_replace_bin_grid_cnt_y_cmd $bin_grid_count    
  }
 
  # overflow 
  if { [info exists keys(-overflow)] } {
    set overflow $keys(-overflow)
    sta::check_positive_float "-overflow" $overflow
    set_replace_overflow_cmd $overflow
  }
  
  # routability check overflow
  if { [info exists keys(-routability_check_overflow)] } {
    set routability_check_overflow $keys(-routability_check_overflow)
    sta::check_positive_float "-routability_check_overflow" $routability_check_overflow
    set_replace_routability_check_overflow $routability_check_overflow
  }
  
  # routability bloat iter
  if { [info exists keys(-routability_max_bloat_iter)] } {
    set routability_max_bloat_iter $keys(-routability_max_bloat_iter)
    sta::check_positive_float "-routability_max_bloat_iter" $routability_max_bloat_iter
    set_replace_routability_max_bloat_iter $routability_max_bloat_iter
  }
  
  # routability inflation iter
  if { [info exists keys(-routability_max_inflation_iter)] } {
    set routability_max_inflation_iter $keys(-routability_max_inflation_iter)
    sta::check_positive_float "-routability_max_inflation_iter" $routability_max_inflation_iter
    set_replace_routability_max_inflation_iter $routability_max_inflation_iter
  }


  if { [info exists keys(-verbose_level)] } {
    set verbose_level $keys(-verbose_level)
    sta::check_positive_integer "-verbose_level" $verbose_level
    set_replace_verbose_level_cmd $verbose_level
  } 



  if { [ord::db_has_rows] } {
    sta::check_argc_eq0 "global_placement" $args
  
    replace_initial_place_cmd
    replace_nesterov_place_cmd
    replace_reset_cmd
  } else {
    puts "Error: no rows defined in design. Use initialize_floorplan to add rows."
  }
}
