#include "replace/Replace.h"
#include "initialPlace.h"
#include "nesterovPlace.h"
#include "placerBase.h"
#include "nesterovBase.h"
#include "routeBase.h" 
#include "logger.h"
#include <iostream>

namespace replace {

using namespace std;

Replace::Replace()
  : db_(nullptr), 
  sta_(nullptr), 
  fr_(nullptr),
  pb_(nullptr), nb_(nullptr), 
  ip_(nullptr), np_(nullptr),
  log_(nullptr),
  initialPlaceMaxIter_(20), 
  initialPlaceMinDiffLength_(1500),
  initialPlaceMaxSolverIter_(100),
  initialPlaceMaxFanout_(200),
  initialPlaceNetWeightScale_(800),
  nesterovPlaceMaxIter_(5000),
  binGridCntX_(0), binGridCntY_(0), 
  overflow_(0.1), density_(1.0),
  initDensityPenalityFactor_(0.00008), 
  initWireLengthCoef_(0.25),
  minPhiCoef_(0.95), maxPhiCoef_(1.05),
  referenceHpwl_(446000000),
  routabilityCheckOverflow_(0.20),
  routabilityMaxDensity_(0.99),
  routabilityMaxBloatIter_(1),
  routabilityMaxInflationIter_(4),
  routabilityTargetRcMetric_(1.01),
  routabilityInflationRatioCoef_(2.5),
  routabilityPitchScale_(1.08),
  routabilityMaxInflationRatio_(2.5),
  routabilityRcK1_(1.0),
  routabilityRcK2_(1.0),
  routabilityRcK3_(0.0),
  routabilityRcK4_(0.0),
  timingDrivenMode_(true),
  routabilityDrivenMode_(true),
  incrementalPlaceMode_(false),
  padLeft_(0), padRight_(0),
  verbose_(0) {
};

Replace::~Replace() {
  reset();
}

void Replace::init() {
}

void Replace::reset() {
  // two pointers should not be freed.
  db_ = nullptr;
  sta_ = nullptr;
  fr_ = nullptr;

  ip_.reset();
  np_.reset();

  pb_.reset();
  nb_.reset();

  customNetWeightStor_.clear();
  customNetWeightStor_.shrink_to_fit();

  initialPlaceMaxIter_ = 20;
  initialPlaceMinDiffLength_ = 1500;
  initialPlaceMaxSolverIter_ = 100;
  initialPlaceMaxFanout_ = 200;
  initialPlaceNetWeightScale_ = 800;

  nesterovPlaceMaxIter_ = 5000;
  binGridCntX_ = binGridCntY_ = 0;
  overflow_ = 0.1;
  density_ = 1.0;
  initDensityPenalityFactor_ = 0.00008;
  initWireLengthCoef_ = 0.25;
  minPhiCoef_ = 0.95;
  maxPhiCoef_ = 1.05;
  referenceHpwl_= 446000000;
  routabilityCheckOverflow_ = 0.20;
  routabilityMaxDensity_ = 0.99;
  routabilityMaxBloatIter_ = 1;
  routabilityMaxInflationIter_ = 4;
  routabilityTargetRcMetric_ = 1.01;
  routabilityInflationRatioCoef_ = 2.5;
  routabilityPitchScale_ = 1.08;
  routabilityMaxInflationRatio_ = 2.5;

  timingDrivenMode_ = true;
  routabilityDrivenMode_ = true; 
  incrementalPlaceMode_ = false;
  
  routabilityRcK1_ = routabilityRcK2_ = 1.0;
  routabilityRcK3_ = routabilityRcK4_ = 0.0;

  padLeft_ = padRight_ = 0;
  verbose_ = 0;
}

void Replace::setDb(odb::dbDatabase* db) {
  db_ = db;
}
void Replace::setSta(sta::dbSta* sta) {
  sta_ = sta;
}
void Replace::setFastRoute(FastRoute::FastRouteKernel* fr) {
  fr_ = fr;
}
void Replace::doInitialPlace() {
  log_ = std::make_shared<Logger>("REPL", verbose_);

  PlacerBaseVars pbVars;
  pbVars.padLeft = padLeft_;
  pbVars.padRight = padRight_;

  pb_ = std::make_shared<PlacerBase>(db_, pbVars, log_);

  InitialPlaceVars ipVars;
  ipVars.maxIter = initialPlaceMaxIter_;
  ipVars.minDiffLength = initialPlaceMinDiffLength_;
  ipVars.maxSolverIter = initialPlaceMaxSolverIter_;
  ipVars.maxFanout = initialPlaceMaxFanout_;
  ipVars.netWeightScale = initialPlaceNetWeightScale_;
  ipVars.incrementalPlaceMode = incrementalPlaceMode_;
  
  std::unique_ptr<InitialPlace> ip(new InitialPlace(ipVars, pb_, log_));
  ip_ = std::move(ip);
  ip_->doBicgstabPlace();
}

void Replace::doNesterovPlace() {
  if( !log_ ) {
    log_ = std::make_shared<Logger>("REPL", verbose_);
  }

  if( !pb_ ) {
    PlacerBaseVars pbVars;
    pbVars.padLeft = padLeft_;
    pbVars.padRight = padRight_;

    pb_ = std::make_shared<PlacerBase>(db_, pbVars, log_);
  }
  

  NesterovBaseVars nbVars;
  nbVars.targetDensity = density_;
  
  if( binGridCntX_ != 0 ) {
    nbVars.isSetBinCntX = 1;
    nbVars.binCntX = binGridCntX_;
  }

  if( binGridCntY_ != 0 ) {
    nbVars.isSetBinCntY = 1;
    nbVars.binCntY = binGridCntY_;
  }


  nb_ = std::make_shared<NesterovBase>(nbVars, pb_, log_);
  

  RouteBaseVars rbVars;
  rbVars.maxDensity = routabilityMaxDensity_;
  rbVars.maxBloatIter = routabilityMaxBloatIter_;
  rbVars.maxInflationIter = routabilityMaxInflationIter_;
  rbVars.targetRC = routabilityTargetRcMetric_;
  rbVars.inflationRatioCoef = routabilityInflationRatioCoef_;
  rbVars.gRoutePitchScale = routabilityPitchScale_;
  rbVars.maxInflationRatio = routabilityMaxInflationRatio_;
  rbVars.rcK1 = routabilityRcK1_;
  rbVars.rcK2 = routabilityRcK2_;
  rbVars.rcK3 = routabilityRcK3_;
  rbVars.rcK4 = routabilityRcK4_;

  rb_ = std::make_shared<RouteBase>(rbVars, db_, fr_, nb_, log_);

  NesterovPlaceVars npVars;

  npVars.minPhiCoef = minPhiCoef_;
  npVars.maxPhiCoef = maxPhiCoef_;
  npVars.referenceHpwl = referenceHpwl_;
  npVars.routabilityCheckOverflow = routabilityCheckOverflow_;
  npVars.initDensityPenalty = initDensityPenalityFactor_;
  npVars.initWireLengthCoef = initWireLengthCoef_;
  npVars.targetOverflow = overflow_;
  npVars.maxNesterovIter = nesterovPlaceMaxIter_; 
  npVars.timingDrivenMode = timingDrivenMode_;
  npVars.routabilityDrivenMode = routabilityDrivenMode_;

  std::unique_ptr<NesterovPlace> np(new NesterovPlace(npVars, pb_, nb_, rb_, log_));
  np_ = std::move(np);

  np_->doNesterovPlace();
}


void
Replace::setInitialPlaceMaxIter(int iter) {
  initialPlaceMaxIter_ = iter; 
}

void
Replace::setInitialPlaceMinDiffLength(int length) {
  initialPlaceMinDiffLength_ = length; 
}

void
Replace::setInitialPlaceMaxSolverIter(int iter) {
  initialPlaceMaxSolverIter_ = iter;
}

void
Replace::setInitialPlaceMaxFanout(int fanout) {
  initialPlaceMaxFanout_ = fanout;
}

void
Replace::setInitialPlaceNetWeightScale(float scale) {
  initialPlaceNetWeightScale_ = scale;
}

void
Replace::setNesterovPlaceMaxIter(int iter) {
  nesterovPlaceMaxIter_ = iter;
}

void 
Replace::setBinGridCntX(int binGridCntX) {
  binGridCntX_ = binGridCntX;
}

void 
Replace::setBinGridCntY(int binGridCntY) {
  binGridCntY_ = binGridCntY;
}

void 
Replace::setTargetOverflow(float overflow) {
  overflow_ = overflow;
}

void
Replace::setTargetDensity(float density) {
  density_ = density;
}

void
Replace::setInitDensityPenalityFactor(float penaltyFactor) {
  initDensityPenalityFactor_ = penaltyFactor;
}

void
Replace::setInitWireLengthCoef(float coef) {
  initWireLengthCoef_ = coef;
}

void
Replace::setMinPhiCoef(float minPhiCoef) {
  minPhiCoef_ = minPhiCoef;
}

void
Replace::setMaxPhiCoef(float maxPhiCoef) {
  maxPhiCoef_ = maxPhiCoef;
}

void
Replace::setReferenceHpwl(float refHpwl) {
  referenceHpwl_ = refHpwl;
}

void
Replace::setIncrementalPlaceMode(bool mode) {
  incrementalPlaceMode_ = mode;
}

void
Replace::setVerboseLevel(int verbose) {
  verbose_ = verbose;
}

void
Replace::setTimingDrivenMode(bool mode) {
  timingDrivenMode_ = mode;
}

void
Replace::setRoutabilityDrivenMode(bool mode) {
  routabilityDrivenMode_ = mode;  
}

void
Replace::setRoutabilityCheckOverflow(float overflow) {
  routabilityCheckOverflow_ = overflow;
}

void
Replace::setRoutabilityMaxDensity(float density) {
  routabilityMaxDensity_ = density;
}

void
Replace::setRoutabilityMaxBloatIter(int iter) {
  routabilityMaxBloatIter_ = iter; 
}

void
Replace::setRoutabilityMaxInflationIter(int iter) {
  routabilityMaxInflationIter_ = iter;
}

void
Replace::setRoutabilityTargetRcMetric(float rc) {
  routabilityTargetRcMetric_ = rc;
}

void
Replace::setRoutabilityInflationRatioCoef(float coef) {
  routabilityInflationRatioCoef_ = coef;
}

void
Replace::setRoutabilityPitchScale(float scale) {
  routabilityPitchScale_ = scale;
}

void
Replace::setRoutabilityMaxInflationRatio(float ratio) {
  routabilityMaxInflationRatio_ = ratio;
}

void
Replace::setRoutabilityRcCoefficients(float k1, float k2, float k3, float k4) {
  routabilityRcK1_ = k1;
  routabilityRcK2_ = k2;
  routabilityRcK3_ = k3;
  routabilityRcK4_ = k4;
}

void
Replace::setPadLeft(int pad) {
  padLeft_ = pad;
}

void
Replace::setPadRight(int pad) {
  padRight_ = pad;
}

void
Replace::setCustomNetWeight(odb::dbNet* net, float weight) {
  customNetWeightStor_.push_back(std::make_pair(net, weight)); 
}

}

