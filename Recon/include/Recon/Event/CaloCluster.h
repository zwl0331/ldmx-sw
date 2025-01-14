/**
 * @file CaloCluster.h
 * @brief Class that stores calorimeter cluster information
 */

#ifndef EVENT_CALOCLUSTER_H_
#define EVENT_CALOCLUSTER_H_

// ROOT
#include "TObject.h"  //For ClassDef
#include "TString.h"

// STL
#include <iostream>
#include <set>

// ldmx-sw
#include "Recon/Event/CalorimeterHit.h"

namespace ldmx {

/**
 * @class CaloCluster
 * @brief Stores cluster information from the ECal
 */
class CaloCluster {
 public:
  /**
   * Class constructor.
   */
  CaloCluster();

  /**
   * Class destructor.
   */
  virtual ~CaloCluster();

  /**
   * Print a description of this object.
   */
  void Print() const;

  /**
   * Reset the CaloCluster object.
   */
  void Clear();

  /**
   * Take in the hits that make up the cluster.
   * @param hit The digi hit's entry number in the events digi
   * collection.
   */
  void addHits(const std::vector<const ldmx::CalorimeterHit*> hitsVec);

  /**
   * Sets total energy for the cluster.
   * @param energy The total energy of the cluster.
   */
  void setEnergy(double energy) { energy_ = energy; }

  /**
   * Sets total number of hits in the cluster.
   * @param nHits The total number of hits.
   */
  void setNHits(int nHits) { nHits_ = nHits; }

  /**
   * Sets a sorted vector for the IDs of the hits
   * that make up the cluster.
   * @param IDs Sorted vector of hit IDs.
   */
  void setIDs(std::vector<unsigned int>& hitIDs) { hitIDs_ = hitIDs; }

  void setHitValsX(std::vector<float>& x) { hitX_ = x; }
  void setHitValsY(std::vector<float>& x) { hitY_ = x; }
  void setHitValsZ(std::vector<float>& x) { hitZ_ = x; }
  void setHitValsE(std::vector<float>& x) { hitE_ = x; }

  /**
   * Sets the three coordinates of the cluster centroid
   * @param x The x coordinate.
   * @param y The y coordinate.
   * @param z The z coordinate.
   */
  void setCentroidXYZ(double x, double y, double z) {
    centroidX_ = x;
    centroidY_ = y;
    centroidZ_ = z;
  }
  void setRMSXYZ(double x, double y, double z) {
    rmsX_ = x;
    rmsY_ = y;
    rmsZ_ = z;
  }
  void setDXDZ(double x) { DXDZ_ = x; }

  void setDYDZ(double x) { DYDZ_ = x; }

  void setEDXDZ(double x) { errDXDZ_ = x; }

  void setEDYDZ(double x) { errDYDZ_ = x; }

  /////////////////////////////////////////////

  // energy of cluster
  double getEnergy() const { return energy_; }

  // number of hits - equivalent to number of strips
  int getNHits() const { return nHits_; }

  // position (weighted by energy)
  double getCentroidX() const { return centroidX_; }
  double getCentroidY() const { return centroidY_; }
  double getCentroidZ() const { return centroidZ_; }
  double getRMSX() const { return rmsX_; }
  double getRMSY() const { return rmsY_; }
  double getRMSZ() const { return rmsZ_; }

  double getDXDZ() const { return DXDZ_; }

  double getDYDZ() const { return DYDZ_; }

  double getEDXDZ() const { return errDXDZ_; }

  double getEDYDZ() const { return errDYDZ_; }

  // get hit rawIDs (unused)
  const std::vector<unsigned int>& getHitIDs() const { return hitIDs_; }

  // ability to store limited hit info
  const std::vector<float>& getHitX() const { return hitX_; }
  const std::vector<float>& getHitY() const { return hitY_; }
  const std::vector<float>& getHitZ() const { return hitZ_; }
  const std::vector<float>& getHitE() const { return hitE_; }

  bool operator<(const CaloCluster& rhs) const {
    return this->getEnergy() < rhs.getEnergy();
  }

 protected:
  std::vector<unsigned int> hitIDs_;
  double energy_{0};
  int nHits_{0};
  double centroidX_{0};
  double centroidY_{0};
  double centroidZ_{0};
  double rmsX_{0};
  double rmsY_{0};
  double rmsZ_{0};
  double DXDZ_{0};
  double DYDZ_{0};
  double errDXDZ_{0};
  double errDYDZ_{0};
  std::vector<float> hitX_;
  std::vector<float> hitY_;
  std::vector<float> hitZ_;
  std::vector<float> hitE_;

 private:
  ClassDef(CaloCluster, 1);
};
}  // namespace ldmx

#endif
