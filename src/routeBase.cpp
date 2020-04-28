#include "routeBase.h"
#include "logger.h"
#include "nesterovBase.h"

#include "opendb/db.h"
#include "FastRouteKernel.h"

#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>

using std::vector;
using std::string;
using std::to_string; 
using std::pair;
using std::make_pair;
using std::sort;
using FastRoute::FastRouteKernel;

namespace replace {

static bool
inflationListCompare(std::pair<Tile*, float> l, 
    std::pair<Tile*, float> r);


Tile::Tile()
: x_(0), y_(0), 
  lx_(0), ly_(0), ux_(0), uy_(0),
  pinCnt_(0),
  usageH_(0), usageV_(0),
  supplyH_(0), supplyV_(0),
  supplyHL_(0), supplyHR_(0),
  supplyVL_(0), supplyVR_(0), 
  inflationRatioH_(0),
  inflationRatioV_(0),
  inflationRatio_(1.0),
  inflationArea_(0),
  inflationAreaDelta_(0),
  inflatedRatio_(0), 
  isMacroIncluded_(false) {}

Tile::Tile(int x, int y, int lx, int ly, int ux, int uy, int layers) 
: Tile() {
  x_ = x;
  y_ = y;
  lx_ = lx;
  ly_ = ly;
  ux_ = ux; 
  uy_ = uy;

  blockage_.resize(layers, 0);
  capacity_.resize(layers, 0);
  route_.resize(layers, 0);

  usageHL_.resize(layers, 0);
  usageHR_.resize(layers, 0);
  usageVL_.resize(layers, 0);
  usageVR_.resize(layers, 0);
}

Tile::~Tile() {
  reset();
}

void
Tile::reset() {
  x_ = y_ = lx_ = ly_ = ux_ = uy_ = pinCnt_ = 0;
  usageH_ = usageV_ = supplyH_ = supplyV_ = 0;
  supplyHL_ = supplyHR_ = supplyVL_ = supplyVR_ = 0;
  inflationRatioH_ = inflationRatioV_ = 0;
  inflationRatio_ = 1.0;

  inflationArea_ = inflationAreaDelta_ = 0;
  inflatedRatio_ = 0;
  isMacroIncluded_ = false;

  blockage_.clear();
  capacity_.clear();
  route_.clear();

  usageHL_.clear();
  usageHR_.clear();
  usageVL_.clear();
  usageVR_.clear();
  
  usageHL_.shrink_to_fit();
  usageHR_.shrink_to_fit();
  usageVL_.shrink_to_fit();
  usageVR_.shrink_to_fit();
}

int
Tile::blockage(int layer) const {
  return blockage_[layer];
}

int
Tile::capacity(int layer) const {
  return capacity_[layer];
}

int
Tile::route(int layer) const {
  return route_[layer];
}

int
Tile::usageHL(int layer) const {
  return usageHL_[layer];
}

int
Tile::usageHR(int layer) const {
  return usageHR_[layer];
}

int
Tile::usageVL(int layer) const {
  return usageVL_[layer];
}

int 
Tile::usageVR(int layer) const {
  return usageVR_[layer];
}

float 
Tile::usageH() const {
  return usageH_;
}

float
Tile::usageV() const {
  return usageV_; 
}

float
Tile::supplyHL() const {
  return supplyHL_;
}
float
Tile::supplyHR() const {
  return supplyHR_;
}

float
Tile::supplyVL() const {
  return supplyVL_;
}
float
Tile::supplyVR() const {
  return supplyVR_;
}

float
Tile::supplyH() const {
  return supplyH_; 
}

float
Tile::supplyV() const {
  return supplyV_;
}

float 
Tile::inflationRatioH() const {
  return inflationRatioH_;
}

float 
Tile::inflationRatioV() const {
  return inflationRatioV_;
}

float
Tile::inflationRatio() const {
  return inflationRatio_;
}

float
Tile::inflationArea() const {
  return inflationArea_;
}

float
Tile::inflationAreaDelta() const {
  return inflationAreaDelta_;
}

float
Tile::inflatedRatio() const {
  return inflatedRatio_;
}

bool
Tile::isMacroIncluded() const {
  return isMacroIncluded_;
}

int
Tile::pinCnt() const {
  return pinCnt_;
}



void
Tile::setBlockage(int layer, int block) {
  blockage_[layer] = block;
}

void
Tile::setCapacity(int layer, int capacity) {
  capacity_[layer] = capacity;
}

void
Tile::setCapacity(vector<int>& capacity) {
  capacity_ = capacity; 
}

void
Tile::setRoute(int layer, int route) {
  route_[layer] = route;
}


void
Tile::setUsageHL(int layer, int usage) { 
  usageHL_[layer] = usage;
}

void
Tile::setUsageHR(int layer, int usage) { 
  usageHR_[layer] = usage;
}

void
Tile::setUsageVL(int layer, int usage) { 
  usageVL_[layer] = usage;
}

void
Tile::setUsageVR(int layer, int usage) { 
  usageVR_[layer] = usage;
}


void
Tile::setSupplyH(float supply) {
  supplyH_ = supply;
}

void
Tile::setSupplyV(float supply) {
  supplyV_ = supply;
}

// set func for supply*
void
Tile::setSupplyHL(float supply) {
  supplyHL_ = supply;
}

void
Tile::setSupplyHR(float supply) {
  supplyHR_ = supply;
}

void
Tile::setSupplyVL(float supply) {
  supplyVL_ = supply; 
}

void
Tile::setSupplyVR(float supply) {
  supplyVR_ = supply; 
}

void
Tile::setInflationRatioH(float val) {
  inflationRatioH_ = val;
}

void
Tile::setInflationRatioV(float val) {
  inflationRatioV_ = val;
}

void
Tile::setInflationRatio(float val) {
  inflationRatio_ = val;
}

void
Tile::setInflationArea(float area) {
  inflationArea_ = area;
}

void
Tile::setInflationAreaDelta(float delta) {
  inflationAreaDelta_ = delta;
}

void
Tile::setInflatedRatio(float ratio) {
  inflatedRatio_ = ratio;
}


void
Tile::setMacroIncluded(bool mode) {
  isMacroIncluded_ = mode;
}

void 
Tile::setPinCnt(int cnt) {
  pinCnt_ = cnt;
}

void 
Tile::updateUsages() {
  usageH_ = 0;
  usageV_ = 0;

  int grSumUsageH = 0, grSumUsageV = 0;

  for(int i=0; i<usageHL_.size(); i++) {
    grSumUsageH += std::max( usageHL_[i], usageHR_[i] );
    grSumUsageV += std::max( usageVL_[i], usageVR_[i] );
  }

  // scaled by tileSizeX and tileSizeY
  usageH_ = static_cast<float>(grSumUsageH) 
    * static_cast<float>(ux() - lx());
  usageV_ = static_cast<float>(grSumUsageV) 
    * static_cast<float>(uy() - ly());
}



TileGrid::TileGrid()
  : lx_(0), ly_(0), 
  tileCntX_(0), tileCntY_(0), 
  tileSizeX_(0), tileSizeY_(0),
  numRoutingLayers_(0) {}

TileGrid::~TileGrid() {
  reset();
}

void
TileGrid::reset() {
  lx_ = ly_ = 0; 
  tileCntX_ = tileCntY_ = 0;
  tileSizeX_ = tileSizeY_ = 0;
  numRoutingLayers_ = 0;

  tileStor_.clear();
  tiles_.clear();

  tileStor_.shrink_to_fit();
  tiles_.shrink_to_fit();
}

void
TileGrid::setLogger(std::shared_ptr<Logger> log) {
  log_ = log;
}

void
TileGrid::setTileCnt(int tileCntX, int tileCntY) {
  tileCntX_ = tileCntX;
  tileCntY_ = tileCntY;
}

void
TileGrid::setTileCntX(int tileCntX) {
  tileCntX_ = tileCntX;
}

void
TileGrid::setTileCntY(int tileCntY) {
  tileCntY_ = tileCntY;  
}

void
TileGrid::setTileSize(int tileSizeX, int tileSizeY) {
  tileSizeX_ = tileSizeX;
  tileSizeY_ = tileSizeY;
}

void
TileGrid::setTileSizeX(int tileSizeX) {
  tileSizeX_ = tileSizeX;
}

void
TileGrid::setTileSizeY(int tileSizeY) {
  tileSizeY_ = tileSizeY; 
}

void
TileGrid::setNumRoutingLayers(int num) {
  numRoutingLayers_ = num;
}

void
TileGrid::setLx(int lx) {
  lx_ = lx;
}

void
TileGrid::setLy(int ly) {
  ly_ = ly;
}

void
TileGrid::initTiles() {
  log_->infoIntPair("TileLxLy", lx_, ly_);
  log_->infoIntPair("TileSize", tileSizeX_, tileSizeY_);
  log_->infoIntPair("TileCnt", tileCntX_, tileCntY_);
  log_->infoInt("numRoutingLayers", numRoutingLayers_);

  // 2D tile grid structure init
  int x = lx_, y = ly_;
  int idxX = 0, idxY = 0;
  tileStor_.resize(tileCntX_ * tileCntY_);
  for(auto& tile : tileStor_) {
    tile = Tile(idxX, idxY, x, y, 
        x + tileSizeX_, y + tileSizeY_,
        numRoutingLayers_); 

    x += tileSizeX_;
    idxX += 1;
    if( x >= ux() ) {
      y += tileSizeY_;
      x = lx_;

      idxY ++;
      idxX = 0;
    }

    tiles_.push_back( &tile );
  }
  log_->infoInt("NumTiles", tiles_.size());
}

int
TileGrid::lx() const {
  return lx_; 
}

int 
TileGrid::ly() const {
  return ly_;
}

    // this is points
int 
TileGrid::ux() const {
  return lx_ + tileCntX_ * tileSizeX_;
}
int 
TileGrid::uy() const {
  return ly_ + tileCntY_ * tileSizeY_;
}

int
TileGrid::tileCntX() const {
  return tileCntX_;
}

int
TileGrid::tileCntY() const {
  return tileCntY_; 
}

int
TileGrid::tileSizeX() const {
  return tileSizeX_;
}

int
TileGrid::tileSizeY() const {
  return tileSizeY_;
}

int
TileGrid::numRoutingLayers() const {
  return numRoutingLayers_;
}

/////////////////////////////////////////////
// EdgeCapacityInfo 
//
EdgeCapacityInfo::EdgeCapacityInfo()
: lx(0), ly(0), ll(0), ux(0), uy(0), ul(0), capacity(0) {}

EdgeCapacityInfo::EdgeCapacityInfo(int lx1, int ly1, int ll1,
  int ux1, int uy1, int ul1, int capacity1)
  : lx(lx1), ly(ly1), ll(ll1), ux(ux1), uy(uy1), ul(ul1), capacity(capacity1) {}


/////////////////////////////////////////////
// RoutingTrack  
//
RoutingTrack::RoutingTrack() 
: lx(0), ly(0), ux(0), uy(0), layer(0), gNet(nullptr) {}

RoutingTrack::RoutingTrack(int lx1, int ly1, int ux1, int uy1, int layer1, GNet* gNet1) 
: RoutingTrack() {
  lx = lx1;
  ly = ly1;
  ux = ux1;
  uy = uy1;
  layer = layer1;
  gNet = gNet1;
}


/////////////////////////////////////////////
// RouteBaseVars

RouteBaseVars::RouteBaseVars()
  : gRoutePitchScale(1.09), 
  edgeAdjustmentCoef(1.19),
  pinInflationCoef(1.66), 
  pinBlockageFactor(0.05),
  inflationRatioCoef(2.33), 
  maxInflationRatio(2.5), 
  blockagePorosity(0),
  maxDensity(0.90),
  ignoreEdgeRatio(0.8),
  targetRC(1.01),
  maxBloatIter(1),
  maxInflationIter(4),
  minPinBlockLayer(1),
  maxPinBlockLayer(2) {}

void 
RouteBaseVars::reset() {
  gRoutePitchScale = 1.09;
  edgeAdjustmentCoef = 1.19;
  pinInflationCoef = 1.66;
  pinBlockageFactor = 0.05;
  inflationRatioCoef = 2.33;
  maxInflationRatio = 2.5;
  blockagePorosity = 0;
  maxDensity = 0.90;
  ignoreEdgeRatio = 0.8;
  targetRC = 1.01;
  maxBloatIter = 1;
  maxInflationIter = 4;
  minPinBlockLayer = 1;
  maxPinBlockLayer = 2;
}

/////////////////////////////////////////////
// RouteBase

RouteBase::RouteBase()
  : rbVars_(), 
  db_(nullptr), nb_(nullptr), log_(nullptr),
  inflatedAreaDelta_(0), 
  bloatIterCnt_(0), inflationIterCnt_(0), numCall_(0) {}

RouteBase::RouteBase(
    RouteBaseVars rbVars, 
    odb::dbDatabase* db, 
    std::shared_ptr<NesterovBase> nb,
    std::shared_ptr<Logger> log)
  : RouteBase() {
  rbVars_ = rbVars;
  db_ = db;
  nb_ = nb;
  log_ = log;

  init();
}

RouteBase::~RouteBase() {
  reset();
}

void 
RouteBase::reset() {
  rbVars_.reset();
  db_ = nullptr;
  nb_ = nullptr;
  log_ = nullptr;

  bloatIterCnt_ = inflationIterCnt_ = 0;
  numCall_ = 0;
  resetRoutabilityResources();
}

void
RouteBase::resetRoutabilityResources() {
  inflatedAreaDelta_ = 0;

  fr_->reset();
  tg_.reset();
  verticalCapacity_.clear();
  horizontalCapacity_.clear();
  minWireWidth_.clear();
  minWireSpacing_.clear();
  edgeCapacityStor_.clear();
  routingTracks_.clear();
  inflationList_.clear();

  verticalCapacity_.shrink_to_fit();
  horizontalCapacity_.shrink_to_fit();
  minWireWidth_.shrink_to_fit();
  minWireSpacing_.shrink_to_fit();
  edgeCapacityStor_.shrink_to_fit();
  routingTracks_.shrink_to_fit();
  inflationList_.shrink_to_fit();
}

void
RouteBase::init() {
  // tg_ init
  std::unique_ptr<TileGrid> tg(new TileGrid());
  tg_ = std::move(tg);
  
  tg_->setLogger(log_);
  
  // call FR only once
  std::unique_ptr<FastRouteKernel> fr(new FastRouteKernel());
  fr_ = std::move(fr);
}

void
RouteBase::getGlobalRouterResult() {
  // update gCells' location to DB for GR
  nb_->updateDbGCells(); 

//  db_->getChip()->getBlock()->writeDb("./route01_mem_err.db");
  
  // fr_ init
  fr_->setDbId(db_->getId());

  // FR init funcs
  fr_->setMinRoutingLayer(1);
  fr_->setMaxRoutingLayer(9);
  fr_->setUnidirectionalRoute(1);
  fr_->setOverflowIterations(100);
  fr_->setAdjustment(0.15);

  fr_->addLayerAdjustment(2, 50);
  fr_->addLayerAdjustment(3, 50);

  /*
  fr_->setAlpha(0.3);
  fr_->setOverflowIterations(500);
  fr_->setClockNetRouting(false);
  fr_->setPDRev(false);
  fr_->setPDRevForHighFanout(-1);
  fr_->setAdjustment(0);
  fr_->setGridOrigin(0,0);
  fr_->setVerbose(0);
  */

  fr_->startFastRoute();
  fr_->runFastRoute();

  // Note that *.route info is unique.
  // TODO: read *.route only once.
  updateRoute();
  log_->infoString("route parsing is done");

  updateEst();
  log_->infoString("est parsing is done");
  tg_->initTiles();
}

void
RouteBase::updateCongestionMap() {
  updateSupplies();
  updateUsages();
  updatePinCount();
  updateRoutes();
  updateInflationRatio();

  log_->infoString("Congestion Map building is done");
}

int64_t 
RouteBase::inflatedAreaDelta() const {
  return inflatedAreaDelta_; 
}

int
RouteBase::numCall() const {
  return numCall_;
}

int 
RouteBase::bloatIterCnt() const {
  return bloatIterCnt_;
}

int
RouteBase::inflationIterCnt() const {
  return inflationIterCnt_;
}

// fill 
//
// TileGrids' 
// lx_ ly_ 
// tileCntX_ tileCntY_
// tileSizeX_ tileSizeY_ 
//
// rbVars_' 
// blockagePorosity_
//
// RouteBases' 
// verticalCapacity_
// horizontalCapacity_
//
// minWireWidth_
// minWireSpacing_
//
// edgeCapacityStor_
void 
RouteBase::updateRoute() {
  using FastRoute::FastRouteKernel;
  FastRouteKernel::ROUTE_ route = fr_->getRoute();
  
  tg_->setTileCnt(route.gridCountX, route.gridCountY);
  tg_->setNumRoutingLayers(route.numLayers);

  tg_->setLx(route.gridOriginX);
  tg_->setLy(route.gridOriginY);

  tg_->setTileSize( route.tileWidth, route.tileHeight);

  rbVars_.blockagePorosity
    = route.blockPorosity;

  verticalCapacity_ = route.verticalEdgesCapacities;
  horizontalCapacity_ = route.horizontalEdgesCapacities;
  minWireWidth_ = route.minWireWidths;
  minWireSpacing_ = route.minWireSpacings;

  edgeCapacityStor_.reserve(route.adjustments.size());
  for(auto& e : route.adjustments) {
    edgeCapacityStor_.push_back(
        EdgeCapacityInfo(e.firstX, e.firstY, e.firstLayer,
          e.finalX, e.finalY, e.finalLayer, 
          e.edgeCapacity));
  }
}

// Fill routingTracks_;
void
RouteBase::updateEst() {
  using FastRoute::FastRouteKernel;
  std::vector<FastRouteKernel::EST_> estStor 
    = fr_->getEst();

  for(auto& netEst : estStor) {
    odb::dbNet* dbNet = 
        db_->getChip()->getBlock()->findNet(netEst.netName.c_str());
    GNet* gNet = nb_->dbToNb(dbNet);

    for(int i=0; i<netEst.numSegments; i++) {
      // only focus on the same layer!
      if( netEst.initLayer[i] != netEst.finalLayer[i] ) {
        continue;
      }

      routingTracks_.push_back(
          RoutingTrack(
            netEst.initX[i], netEst.initY[i], 
            netEst.finalX[i], netEst.finalY[i],
            netEst.initLayer[i], 
            gNet));
    } 
  }
}

void
RouteBase::updateSupplies() {
  int numHTracks = 0;
  int numVTracks = 0;
  for(int i=0; i<tg_->numRoutingLayers(); i++) {
    numVTracks += 
      verticalCapacity_[i] 
      / (minWireWidth_[i] + minWireSpacing_[i]);
    numHTracks +=
      horizontalCapacity_[i] 
      / (minWireWidth_[i] + minWireSpacing_[i]);
  }
  
  log_->infoInt("NumHTracks", numHTracks);
  log_->infoInt("NumVTracks", numVTracks);
  
  int pitchH = std::round(static_cast<float>(tg_->tileSizeY()) 
      / numHTracks * rbVars_.gRoutePitchScale);
  int pitchV = std::round(static_cast<float>(tg_->tileSizeX()) 
      / numVTracks * rbVars_.gRoutePitchScale);

  for(auto& tile : tg_->tiles()) {
    tile->setSupplyHL( tile->area() / pitchH );
    tile->setSupplyHR( tile->supplyHL() );
    tile->setSupplyVL( tile->area() / pitchV );
    tile->setSupplyVR( tile->supplyVL() );
  }

  log_->infoInt("PitchH", pitchH );
  log_->infoInt("PitchV", pitchV );

  // apply edgeCapacityInfo from *.route
  // update supplyH/V
  for(auto& ecInfo : edgeCapacityStor_) {
    bool isHorizontal = (ecInfo.ly == ecInfo.uy);
    
    // l : lower
    // u : upper
    // index
    int lx = std::min( ecInfo.lx, ecInfo.ux );
    int ux = std::max( ecInfo.lx, ecInfo.ux );
    int ly = std::min( ecInfo.ly, ecInfo.uy );
    int uy = std::max( ecInfo.ly, ecInfo.uy );

    // Note that ecInfo.ll == ecInfo.ul
    assert( ecInfo.ll == ecInfo.ul );
    int layer = ecInfo.ll - 1;
    int capacity = ecInfo.capacity;

    Tile* lTile = tg_->tiles()[ly * tg_->tileCntX() + lx];
    Tile* uTile = tg_->tiles()[uy * tg_->tileCntX() + ux];

    if( isHorizontal ) {
      // lower -> right edge
      lTile->setSupplyHR( 
          lTile->supplyHR() - 
          static_cast<float>((horizontalCapacity_[layer] - capacity)) /
          static_cast<float>((minWireWidth_[layer] + minWireSpacing_[layer])) /
          rbVars_.edgeAdjustmentCoef * tg_->tileSizeX() );

      // upper -> left edge
      uTile->setSupplyHL(
          uTile->supplyHL() -
          static_cast<float>((horizontalCapacity_[layer] - capacity)) /
          static_cast<float>((minWireWidth_[layer] + minWireSpacing_[layer])) /
          rbVars_.edgeAdjustmentCoef * tg_->tileSizeX() );

      // lower layer check
      if( layer <= 4 && horizontalCapacity_[layer] > 0 &&
          capacity < 0.01 ) {
        lTile->setMacroIncluded(true);
      }
    }
    else {
      // lower -> right edge
      lTile->setSupplyVR( 
          lTile->supplyVR() - 
          static_cast<float>((verticalCapacity_[layer] - capacity)) /
          static_cast<float>((minWireWidth_[layer] + minWireSpacing_[layer])) /
          rbVars_.edgeAdjustmentCoef * tg_->tileSizeY() );
      
      // upper -> left edge
      uTile->setSupplyVL(
          uTile->supplyVL() -
          static_cast<float>((verticalCapacity_[layer] - capacity)) /
          static_cast<float>((minWireWidth_[layer] + minWireSpacing_[layer])) /
          rbVars_.edgeAdjustmentCoef * tg_->tileSizeY() );
      
      // lower layer check
      if( layer <= 4 && verticalCapacity_[layer] > 0 &&
          capacity < 0.01 ) {
        lTile->setMacroIncluded(true);
      }
    }
  } 
  
  // fill capacity
  std::vector<int> capacity(tg_->numRoutingLayers(), 0);
  for(int i=0; i<tg_->numRoutingLayers(); i++) {
    if( horizontalCapacity_[i] > 0 ) {
      capacity[i] = horizontalCapacity_[i]; 
    }
    else {
      capacity[i] = verticalCapacity_[i];
    }
  }

  for(auto& tile : tg_->tiles()) {
    // set H, V from L, R
    tile->setSupplyH( std::fmin( tile->supplyHL(), tile->supplyHR() ) );
    tile->setSupplyV( std::fmin( tile->supplyVL(), tile->supplyVR() ) );

    // set capacity initially
    tile->setCapacity( capacity );
  }
  
}


// update tiles' usageHR, usageHL, usageVR, usage VL
void
RouteBase::updateUsages() {
  for (auto& rTrack : routingTracks_) {
    bool isHorizontal = ( rTrack.ly == rTrack.uy );
    
    // points
    int lx = std::min( rTrack.lx, rTrack.ux );
    int ux = std::max( rTrack.lx, rTrack.ux );
    int ly = std::min( rTrack.ly, rTrack.uy );
    int uy = std::max( rTrack.ly, rTrack.uy );

    int layer = rTrack.layer - 1;

    // getIdx from coordinates.
    int lIdxX = (lx - tg_->lx())/tg_->tileSizeX();
    int lIdxY = (ly - tg_->ly())/tg_->tileSizeY();
    int uIdxX = (ux - tg_->lx())/tg_->tileSizeX();
    int uIdxY = (uy - tg_->ly())/tg_->tileSizeY();


    if( lIdxX < 0 || lIdxX >= tg_->tileCntX() ) {
      log_->error("lIdxX is wrong. Check the *.est file. lIdx: " + to_string(lIdxX), 100);
    }
    if( lIdxY < 0 || lIdxY >= tg_->tileCntY() ) {
      log_->error("lIdxY is wrong. Check the *.est file. lIdxY: " + to_string(lIdxY), 100);
    }
    if( uIdxX < 0 || uIdxX >= tg_->tileCntX() ) {
      log_->error("uIdxX is wrong. Check the *.est file. uIdxX: " + to_string(uIdxX), 100);
    }
    if( uIdxY < 0 || uIdxY >= tg_->tileCntY() ) {
      log_->error("uIdxY is wrong. Check the *.est file. uIdxY: " + to_string(uIdxY), 100);
    }
   
    // get lTile and uTile using lx, ly, ux, uy 
    Tile* lTile = tg_->tiles()[lIdxY * tg_->tileCntX() + lIdxX];
    Tile* uTile = tg_->tiles()[uIdxY * tg_->tileCntX() + uIdxX];
    // horizontal
    if( isHorizontal ) {
      lTile->setUsageHR( layer, lTile->usageHR(layer) + 1 );
      uTile->setUsageHL( layer, uTile->usageHL(layer) + 1 );
    }
    // vertical
    else {
      lTile->setUsageVR( layer, lTile->usageVR(layer) + 1 );
      uTile->setUsageVL( layer, uTile->usageVL(layer) + 1 );
    }

    // update route info
    lTile->setRoute(layer, lTile->route(layer) 
        + minWireWidth_[layer] + minWireSpacing_[layer] );
  }

  
  // update usageH and usageV
  for(auto& tile : tg_->tiles()) {
    tile->updateUsages();
  }

}

// update pin density on tiles
void
RouteBase::updatePinCount() {
  for(auto& gCell : nb_->gCells()) {
    for(auto& gPin : gCell->gPins()) {
      int idxX = (gPin->cx() - tg_->lx())/tg_->tileSizeX();
      int idxY = (gPin->cy() - tg_->ly())/tg_->tileSizeY();
      Tile* tile = tg_->tiles()[idxY * tg_->tileCntX() + idxX];
      tile->setPinCnt( tile->pinCnt() + 1 );
    }
  }
}


void
RouteBase::updateRoutes() {
  // apply edgeCapacityInfo from *.route
  // update blockage from possible capacity
  //
  // Note that route += blockage
  //
  
  // edgeCapacity will generate blockage
  for(auto& ecInfo : edgeCapacityStor_) {
    int lx = std::min( ecInfo.lx, ecInfo.ux );
    int ly = std::min( ecInfo.ly, ecInfo.uy );
    int layer = ecInfo.ll - 1;
    int capacity = ecInfo.capacity;
    
    Tile* tile = tg_->tiles()[ly * tg_->tileCntX() + lx];
    tile->setBlockage(layer, 
        tile->blockage(layer) 
        + tile->capacity(layer) - capacity);
  }

  // blockage is merged into route resources.
  for(auto& tile : tg_->tiles()) {
    for(int i=0; i<tg_->numRoutingLayers(); i++) {
      tile->setRoute( i, tile->route(i) + tile->blockage(i) );
    }
  }
  
  // update routes based on pin Density
  //
  // Note that 
  // minPinBlockLayer and 
  // maxPinBlockLayer can be changed by
  // user tuning; Default: M2~M3 
  //
  for(auto& tile : tg_->tiles()) {
    for(int i=rbVars_.minPinBlockLayer; 
        i<=rbVars_.maxPinBlockLayer; i++) {

      if( horizontalCapacity_[i] > 0 ) {
        tile->setRoute(i,
            tile->route(i)
            + static_cast<int>(round(rbVars_.pinBlockageFactor * tile->pinCnt()))
            * (minWireWidth_[i] + minWireSpacing_[i])
            );


        if( tile->x() <= tg_->tileCntX() - 2 ) {
          Tile* rightTile = tg_->tiles()
            [tile->y() * tg_->tileCntX() + (tile->x()+1)];

          tile->setRoute(i,
              tile->route(i)
              + static_cast<int>(round(rbVars_.pinBlockageFactor * rightTile->pinCnt()))
              * (minWireWidth_[i] + minWireSpacing_[i])
              ); 

        }
      }
      else if( verticalCapacity_[i] > 0 ) {
        tile->setRoute(i,
            tile->route(i)
            + static_cast<int>(round(rbVars_.pinBlockageFactor * tile->pinCnt()))
            * (minWireWidth_[i] + minWireSpacing_[i])
            );

        if( tile->y() <= tg_->tileCntY() - 2 ) {
          Tile* upperTile = tg_->tiles()
            [ (tile->y()+1) * tg_->tileCntX() + tile->x()];

          tile->setRoute(i,
              tile->route(i)
              + static_cast<int>(round(rbVars_.pinBlockageFactor * upperTile->pinCnt()))
              * (minWireWidth_[i] + minWireSpacing_[i])
              ); 
        }
      }
    }  
  }
}

// inflationRatio
void
RouteBase::updateInflationRatio() {
  float maxInflV = 0, maxInflH = 0;

  for(auto& tile : tg_->tiles()) {
    float newRatioV 
      = (tile->usageH() + rbVars_.pinInflationCoef * tile->pinCnt()) / tile->supplyH();
    newRatioV = pow(newRatioV, rbVars_.inflationRatioCoef);

    // Vertical InflationRatio
    tile->setInflationRatioV( 
        std::fmax(
          tile->inflationRatioV(), newRatioV));

    // upper bound
    tile->setInflationRatioV(
        std::fmin( 
          rbVars_.maxInflationRatio, 
          tile->inflationRatioV()));

    maxInflV = std::max(maxInflV, tile->inflationRatioV());
    
    float newRatioH 
      = (tile->usageV() + rbVars_.pinInflationCoef * tile->pinCnt()) / tile->supplyV();
    newRatioH = pow(newRatioH, rbVars_.inflationRatioCoef);

    // Horizontal InflationRatio
    tile->setInflationRatioH( 
        std::fmax(
          tile->inflationRatioH(), newRatioH));

    // upper bound
    tile->setInflationRatioH(
        std::fmin( 
          rbVars_.maxInflationRatio, 
          tile->inflationRatioH()));

    maxInflH = std::max(maxInflH, tile->inflationRatioH());
  }
  log_->infoFloatPair("MaxInflationRatioHV", maxInflH, maxInflV);


  // newly set inflationRatio from route arr
  // for each tile
  for(auto& tile : tg_->tiles()) {

    // for each layer
    for(int i=0; i<tg_->numRoutingLayers(); i++) {
      // horizontal
      if( horizontalCapacity_[i] > 0 ) {
        
        if( tile->blockage(i) 
            <= rbVars_.ignoreEdgeRatio 
            * horizontalCapacity_[i] ) {
          
          tile->setInflationRatio(
              fmax( 
                tile->inflationRatio(), 
                static_cast<float>(tile->route(i)) 
                / horizontalCapacity_[i]
                * rbVars_.gRoutePitchScale)
              );
        }

        // left tile exists
        if( tile->x() >= 1 ) {

          Tile* leftTile = tg_->tiles()
            [ tile->y() * tg_->tileCntX() 
            + (tile->x()-1) ];
        
          if( leftTile->blockage(i) 
            <= rbVars_.ignoreEdgeRatio 
            * horizontalCapacity_[i] ) {

            tile->setInflationRatio(
                fmax(
                  tile->inflationRatio(),
                  static_cast<float>(leftTile->route(i)) 
                  / horizontalCapacity_[i]
                  * rbVars_.gRoutePitchScale)
                );
          }
        }
      }
      // vertical
      else if( verticalCapacity_[i] > 0 ) {
        if( tile->blockage(i) 
            <= rbVars_.ignoreEdgeRatio 
            * verticalCapacity_[i] ) {
          tile->setInflationRatio(
              fmax(
                tile->inflationRatio(),
                static_cast<float>(tile->route(i))
                / verticalCapacity_[i]
                * rbVars_.gRoutePitchScale)
              );
        }
           
        // lower tile exists
        if( tile->y() >= 1 ) {
          Tile* lowerTile = tg_->tiles()
            [ (tile->y()-1) * tg_->tileCntX()
            + tile->x() ];

          if( lowerTile->blockage(i) 
              <= rbVars_.ignoreEdgeRatio 
              * verticalCapacity_[i] ) {

            tile->setInflationRatio(
                fmax(
                  tile->inflationRatio(),
                  static_cast<float>(lowerTile->route(i))
                  / verticalCapacity_[i]
                  * rbVars_.gRoutePitchScale)
                );
          }
        }
      } 
    } 

    if( tile->inflationRatio() > 1.0 ) {
      tile->setInflationRatio( pow(tile->inflationRatio(), 
            rbVars_.inflationRatioCoef) );
    }

    /*
    if( tile->x() == 91 && 
        tile->y() == 55 ) {
      for(int i=0; i<tg_->numRoutingLayers(); i++) {
        std::cout << "9155 route: " << tile->route(i) << std::endl;
      }

      Tile* leftTile = tg_->tiles()
        [ tile->y() * tg_->tileCntX() 
        + (tile->x()-1) ];
          
      Tile* lowerTile = tg_->tiles()
        [ (tile->y()-1) * tg_->tileCntX()
        + tile->x() ];

      std::cout << "leftTile XY: " << leftTile->x() << " " << leftTile->y() << std::endl;
      
      for(int i=0; i<tg_->numRoutingLayers(); i++) {
        std::cout << "leftTile route: " << leftTile->route(i) << std::endl;
      }

      std::cout << "lowerTile XY: " << lowerTile->x() << " " << lowerTile->y() << std::endl;
      
      for(int i=0; i<tg_->numRoutingLayers(); i++) {
        std::cout << "lowerTile route: " << lowerTile->route(i) << std::endl;
      }
    }

    */
  }
    
  for(auto& tile : tg_->tiles()) {
    if( tile->inflationRatio() > 1.0 ) {
      log_->infoIntPair("xy", tile->x(), tile->y(), 5); 
      log_->infoIntPair("minxy", tile->lx(), tile->ly(), 5);
      log_->infoIntPair("maxxy", tile->ux(), tile->uy(), 5);
      log_->infoFloatPair("usageHV", 
          tile->usageH(), tile->usageV(), 5); 
      log_->infoFloatPair("supplyHV", 
          tile->supplyH(), tile->supplyV(), 5); 
      log_->infoInt("pinCnt", tile->pinCnt(), 5);
      log_->infoFloatPair("calcInflRatioHV", 
          tile->inflationRatioH(),
          tile->inflationRatioV(), 5);
      log_->infoFloat("calcInflationRatio", tile->inflationRatio(), 5);
    }
  }
}


bool 
RouteBase::routability() {
  increaseCounter();

  // create Tile Grid
  std::unique_ptr<TileGrid> tg(new TileGrid());
  tg_ = std::move(tg);
  tg_->setLogger(log_);
  
  getGlobalRouterResult();
  updateCongestionMap();

  // no need routing if RC is lower than targetRC val
  if( getRC() < rbVars_.targetRC ) {
    resetRoutabilityResources();  
    std::cout << "reset and quit RouteBase" << std::endl;
    return false;
  }

  // set inflated ratio
  for(auto& tile : tg_->tiles()) {
    if( tile->inflationRatio() > 1 ) {
      tile->setInflatedRatio( tile->inflationRatio() );
    }
    else {
      tile->setInflatedRatio( 1.0 );
    }
  }
  
  using std::cout;
  using std::endl;

  inflatedAreaDelta_ = 0;

  // run bloating and get inflatedAreaDelta_
  for(auto& gCell : nb_->gCells()) {
    // only care about "standard cell"
    if( !gCell->isStdInstance() ) {
      continue;
    }

    int idxX = (gCell->dCx() - tg_->lx())/tg_->tileSizeX();
    int idxY = (gCell->dCy() - tg_->ly())/tg_->tileSizeY();

    Tile* tile = tg_->tiles()[idxY * tg_->tileCntX() + idxX];

    // Don't care when inflRatio <= 1
    if( tile->inflatedRatio() <= 1.0 ) {
      continue;
    } 

    int64_t prevCellArea 
      = static_cast<int64_t>(gCell->dx())
      * static_cast<int64_t>(gCell->dy());

    // bloat
    gCell->setSize(
        static_cast<int>(round(gCell->dx() 
            * sqrt(tile->inflatedRatio()))), 
        static_cast<int>(round(gCell->dy() 
            * sqrt(tile->inflatedRatio()))));


    int64_t newCellArea
      = static_cast<int64_t>(gCell->dx())
      * static_cast<int64_t>(gCell->dy());
    
    // deltaArea is equal to area * deltaRatio
    // both of original and density size will be changed 
    inflatedAreaDelta_ 
      += newCellArea - prevCellArea;

//    inflatedAreaDelta_ 
//      = static_cast<int64_t>(round( 
//        static_cast<int64_t>(gCell->dx()) 
//        * static_cast<int64_t>(gCell->dy())
//        * (tile->inflatedRatio() - 1.0)));
  }

  // update inflationList_
  for(auto& tile : tg_->tiles()) {
    inflationList_.push_back(
        make_pair(tile, tile->inflatedRatio()));
  }

  // sort by inflatedRatio
  sort(inflationList_.begin(), inflationList_.end(), 
      inflationListCompare);

  // target ratio
  float targetInflationDeltaAreaRatio  
    = 1.0 / static_cast<float>(rbVars_.maxInflationIter);

  // TODO: looks weird
  if( inflatedAreaDelta_ >
     targetInflationDeltaAreaRatio * 
     (nb_->whiteSpaceArea() - nb_->nesterovInstsArea() + nb_->totalFillerArea())) {
    cout << "Needs dynamic inflation proc:" << endl; 
  } 

  log_->infoInt64("InflatedAreaDelta", inflatedAreaDelta_ );
  log_->infoFloat("TargetDensity", nb_->targetDensity());

  int64_t totalGCellArea = inflatedAreaDelta_ 
        + nb_->nesterovInstsArea() + nb_->totalFillerArea();

  // newly set Density
  nb_->setTargetDensity( static_cast<float>(totalGCellArea) 
      / static_cast<float>(nb_->whiteSpaceArea()) );

  // TODO: looks weird, too
  if( nb_->targetDensity() > rbVars_.maxDensity ) {
    cout << "Need to shrink fillercells" << endl;
//    nb_->updateFillerCellSize(dx, dy);
  }

  log_->infoInt64("WhiteSpaceArea", nb_->whiteSpaceArea());
  log_->infoInt64("NesterovInstsArea", nb_->nesterovInstsArea());
  log_->infoInt64("TotalFillerArea", nb_->totalFillerArea());
  log_->infoInt64("TotalGCellsArea", nb_->nesterovInstsArea() 
      + nb_->totalFillerArea());
  log_->infoInt64("NewTotalGCellsArea", inflatedAreaDelta_ 
      + nb_->nesterovInstsArea() + nb_->totalFillerArea());
 
  // updateArea 
  log_->infoString("UpdateArea");
  nb_->updateAreas();

  log_->infoFloat("NewTargetDensity", nb_->targetDensity());
  log_->infoInt64("NewWhiteSpaceArea", nb_->whiteSpaceArea());
  log_->infoInt64("MovableArea", nb_->movableArea());
  log_->infoInt64("NewNesterovInstsArea", nb_->nesterovInstsArea());
  log_->infoInt64("NewTotalFillerArea", nb_->totalFillerArea());
  log_->infoInt64("NewTotalGCellsArea", nb_->nesterovInstsArea() 
      + nb_->totalFillerArea());

  // update densitySizes for all gCell
  nb_->updateDensitySize();

  // reset
  resetRoutabilityResources();  

  return true;
}

// extract RC values
float
RouteBase::getRC() const {
  double totalRouteOverflowH = 0;
  double totalRouteOverflowV = 0;

  int overflowTileCnt = 0;
  for(auto& tile : tg_->tiles()) {
    totalRouteOverflowH += (double)fmax(0.0, (double)tile->usageH() - tile->supplyH());
    totalRouteOverflowV += (double)fmax(0.0, (double)tile->usageV() - tile->supplyV());
    if(tile->usageH() > tile->supplyH() 
        || tile->usageV() > tile->supplyV()) {
      overflowTileCnt++;
    }
  }

  log_->infoFloatSignificant("TotalRouteOverflowH", totalRouteOverflowH);
  log_->infoFloatSignificant("TotalRouteOverflowV", totalRouteOverflowV);
  log_->infoInt("OverflowTileCnt", overflowTileCnt);

  // what's the difference between 1 and 2?
  double totalRouteOverflowH2 = 0;
  double totalRouteOverflowV2 = 0;
  int overflowTileCnt2 = 0;
  
  std::vector< double > horEdgeCongArray;
  std::vector< double > verEdgeCongArray;

  for(auto& tile : tg_->tiles()) {
    for(int j = 0; j < tg_->numRoutingLayers() ; j++) {
      if(horizontalCapacity_[j] != 0) {
        if(tile->blockage(j) 
            > rbVars_.ignoreEdgeRatio 
            * horizontalCapacity_[j]) {
          continue;
        }
        totalRouteOverflowH2 +=
            (double)fmax(0.0, -1 + tile->route(j) * 1.0 / horizontalCapacity_[j]);
        horEdgeCongArray.push_back(tile->route(j) * 1.0 / horizontalCapacity_[j]);
        if(tile->route(j) - horizontalCapacity_[j] > 0) {
          overflowTileCnt2++;
        }
      }
      else if(verticalCapacity_[j] != 0) {
        if(tile->blockage(j) 
            > rbVars_.ignoreEdgeRatio 
            * verticalCapacity_[j]) {
          continue;
        }
        totalRouteOverflowV2 +=
            (double)fmax(0.0, -1 + tile->route(j) * 1.0 / verticalCapacity_[j]);
        verEdgeCongArray.push_back(tile->route(j) * 1.0 / verticalCapacity_[j]);
        if(tile->route(j) - verticalCapacity_[j] > 0) {
          overflowTileCnt2++;
        }
      }
    }
  }

  log_->infoFloatSignificant("TotalRouteOverflowH2", totalRouteOverflowH2);
  log_->infoFloatSignificant("TotalRouteOverflowV2", totalRouteOverflowV2);
  log_->infoInt("OverflowTileCnt2", overflowTileCnt2);

  int horArraySize = horEdgeCongArray.size();
  int verArraySize = verEdgeCongArray.size();

  std::sort(horEdgeCongArray.rbegin(), 
      horEdgeCongArray.rend());
  std::sort(verEdgeCongArray.rbegin(), 
      verEdgeCongArray.rend());

  double horAvg005RC = 0;
  double horAvg010RC = 0;
  double horAvg020RC = 0;
  double horAvg050RC = 0;
  for(int i = 0; i < horArraySize; ++i) {
    if(i < 0.005 * horArraySize) {
      horAvg005RC += horEdgeCongArray[i];
    }
    if(i < 0.01 * horArraySize) {
      horAvg010RC += horEdgeCongArray[i];
    }
    if(i < 0.02 * horArraySize) {
      horAvg020RC += horEdgeCongArray[i];
    }
    if(i < 0.05 * horArraySize) {
      horAvg050RC += horEdgeCongArray[i];
    }
  }
  horAvg005RC /= 1.0 * 0.005 * horArraySize;
  horAvg010RC /= 1.0 * 0.010 * horArraySize;
  horAvg020RC /= 1.0 * 0.020 * horArraySize;
  horAvg050RC /= 1.0 * 0.050 * horArraySize;

  double verAvg005RC = 0;
  double verAvg010RC = 0;
  double verAvg020RC = 0;
  double verAvg050RC = 0;
  for(int i = 0; i < verArraySize; ++i) {
    if(i < 0.005 * verArraySize) {
      verAvg005RC += verEdgeCongArray[i];
    }
    if(i < 0.01 * verArraySize) {
      verAvg010RC += verEdgeCongArray[i];
    }
    if(i < 0.02 * verArraySize) {
      verAvg020RC += verEdgeCongArray[i];
    }
    if(i < 0.05 * verArraySize) {
      verAvg050RC += verEdgeCongArray[i];
    }
  }
  verAvg005RC /= 1.0 * 0.005 * verArraySize;
  verAvg010RC /= 1.0 * 0.010 * verArraySize;
  verAvg020RC /= 1.0 * 0.020 * verArraySize;
  verAvg050RC /= 1.0 * 0.050 * verArraySize;

  log_->infoFloatSignificant("0.5%RC", fmax(horAvg005RC, verAvg005RC));
  log_->infoFloatSignificant("1.0%RC", fmax(horAvg010RC, verAvg010RC));
  log_->infoFloatSignificant("2.0%RC", fmax(horAvg020RC, verAvg020RC));
  log_->infoFloatSignificant("5.0%RC", fmax(horAvg050RC, verAvg050RC));

  float finalRC = (1.0 * fmax(horAvg005RC, verAvg005RC) +
                  1.0 * fmax(horAvg010RC, verAvg010RC) +
                  1.0 * fmax(horAvg020RC, verAvg020RC) +
                  1.0 * fmax(horAvg050RC, verAvg050RC)) /
                 (1.0 + 1.0 + 1.0 + 1.0);

  log_->infoFloatSignificant("FinalRC", finalRC);
  return finalRC;
}

void
RouteBase::increaseCounter() {
  numCall_ ++;
  inflationIterCnt_ ++;
  if( inflationIterCnt_ > rbVars_.maxInflationIter ) {
    inflationIterCnt_ = 0;
    bloatIterCnt_ ++;
  }
}


// compare based on the inflatedRatio
static bool
inflationListCompare(std::pair<Tile*, float> l, 
   std::pair<Tile*, float> r) {

  // inflatedRatio order
  if ( l.second < r.second ) {
    return true;
  }
  else if( l.second > r.second ) {
    return false;
  }

  // x/y index sorts for deterministic
  // when inflatedRatio is the same
  //
  // x sort
  if( l.first->x() < r.first->x() ) {
    return true;
  }
  else if( l.first->x() > r.first->x() ) { 
    return false;
  }

  // y sort
  if( l.first->y() < r.first->y() ) {
    return true;
  }
  else if( l.first->y() > r.first->y() ) {
    return false;
  }
  return true;
}


}