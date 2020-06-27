%module replace

%{
#include "openroad/OpenRoad.hh"
#include "replace/Replace.h"

namespace ord {
OpenRoad*
getOpenRoad();

replace::Replace*
getReplace();
}

using ord::getOpenRoad;
using ord::getReplace;
using replace::Replace;
%}

%inline %{

void 
replace_reset_cmd() 
{
  Replace* replace = getReplace();  
  replace->reset();
  replace->setDb(getOpenRoad()->getDb());
  replace->setSta(getOpenRoad()->getSta());
  replace->setFastRoute(getOpenRoad()->getFastRoute());
}

void 
replace_initial_place_cmd()
{
  Replace* replace = getReplace();
  replace->doInitialPlace();
}

void 
replace_nesterov_place_cmd()
{
  Replace* replace = getReplace();
  replace->doNesterovPlace();
}

void
set_replace_density_cmd(float density)
{
  Replace* replace = getReplace();
  replace->setTargetDensity(density);
}

void
set_replace_initial_place_max_iter_cmd(int iter)
{
  Replace* replace = getReplace();
  replace->setInitialPlaceMaxIter(iter); 
}

void
set_replace_initial_place_max_fanout_cmd(int fanout)
{
  Replace* replace = getReplace();
  replace->setInitialPlaceMaxFanout(fanout);
}

void
set_replace_nesv_place_iter_cmd(int iter)
{
  Replace* replace = getReplace();
  replace->setNesterovPlaceMaxIter(iter);
}

void
set_replace_bin_grid_cnt_x_cmd(int cnt_x)
{
  Replace* replace = getReplace();
  replace->setBinGridCntX(cnt_x); 
}

void
set_replace_bin_grid_cnt_y_cmd(int cnt_y)
{
  Replace* replace = getReplace();
  replace->setBinGridCntY(cnt_y);
}

void
set_replace_overflow_cmd(float overflow)
{
  Replace* replace = getReplace();
  replace->setTargetOverflow(overflow);
}

void
set_replace_min_phi_coef_cmd(float min_phi_coef)
{
  Replace* replace = getReplace();
  replace->setMinPhiCoef(min_phi_coef);
}

void
set_replace_max_phi_coef_cmd(float max_phi_coef) 
{
  Replace* replace = getReplace();
  replace->setMaxPhiCoef(max_phi_coef);
}

void
set_replace_init_density_penalty_factor_cmd(float penaltyFactor)
{
  Replace* replace = getReplace();
  replace->setInitDensityPenalityFactor(penaltyFactor);
}

void
set_replace_init_wirelength_coef_cmd(float coef)
{
  Replace* replace = getReplace();
  replace->setInitWireLengthCoef(coef);
}

void
set_replace_incremental_place_mode_cmd()
{
  Replace* replace = getReplace();
  replace->setIncrementalPlaceMode(true);
}

void
set_replace_verbose_level_cmd(int verbose)
{
  Replace* replace = getReplace();
  replace->setVerboseLevel(verbose);
}

void
set_replace_disable_timing_driven_mode_cmd()
{
  Replace* replace = getReplace();
  replace->setTimingDrivenMode(false);
}

void
set_replace_disable_routability_driven_mode_cmd()
{
  Replace* replace = getReplace();
  replace->setRoutabilityDrivenMode(false);
}

void
set_replace_routability_check_overflow_cmd(float overflow) 
{
  Replace* replace = getReplace();
  replace->setRoutabilityCheckOverflow(overflow);
}

void
set_replace_routability_max_density_cmd(float density) 
{
  Replace* replace = getReplace();
  replace->setRoutabilityMaxDensity(density);
}

void
set_replace_routability_max_bloat_iter_cmd(int iter)
{
  Replace* replace = getReplace();
  replace->setRoutabilityMaxBloatIter(iter);
}

void
set_replace_routability_max_inflation_iter_cmd(int iter) 
{
  Replace* replace = getReplace();
  replace->setRoutabilityMaxInflationIter(iter);
}

void
set_replace_routability_target_rc_metric_cmd(float rc)
{
  Replace* replace = getReplace();
  replace->setRoutabilityTargetRcMetric(rc);
}

void
set_replace_routability_inflation_ratio_coef_cmd(float coef)
{
  Replace* replace = getReplace();
  replace->setRoutabilityInflationRatioCoef(coef);
}

void
set_replace_routability_pitch_scale_cmd(float scale) 
{
  Replace* replace = getReplace();
  replace->setRoutabilityPitchScale(scale);
}

void
set_replace_routability_max_inflation_ratio_cmd(float ratio) 
{
  Replace* replace = getReplace();
  replace->setRoutabilityMaxInflationRatio(ratio);
}

void
set_replace_routability_rc_coefficients_cmd(float k1, float k2, float k3, float k4)
{
  Replace* replace = getReplace();
  replace->setRoutabilityRcCoefficients(k1, k2, k3, k4);
}


void
set_replace_pad_left_cmd(int pad) 
{
  Replace* replace = getReplace();
  replace->setPadLeft(pad);
}

void
set_replace_pad_right_cmd(int pad) 
{
  Replace* replace = getReplace();
  replace->setPadRight(pad);
}

void
set_replace_custom_net_weight_cmd(odb::dbNet* net, float weight)
{
  Replace* replace = getReplace();
  replace->setCustomNetWeight(net, weight);
}

%} // inline
