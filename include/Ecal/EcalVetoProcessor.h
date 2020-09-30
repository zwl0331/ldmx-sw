/**
 * @file EcalVetoProcessor.h
 * @brief Class that determines if event is vetoable using ECAL hit information
 * @author Owen Colegrove, UCSB
 */

#ifndef EVENTPROC_ECALVETOPROCESSOR_H_
#define EVENTPROC_ECALVETOPROCESSOR_H_

// LDMX
#include "DetDescr/EcalHexReadout.h"
#include "DetDescr/EcalID.h"
#include "Event/EventDef.h"
#include "Framework/EventProcessor.h"
#include "Framework/Parameters.h"

#include "Tools/ONNXRuntime.h"

// ROOT (MIP tracking)
#include "TVector3.h"

//C++
#include <map>
#include <memory>

namespace ldmx {

    /**
     * @class EcalVetoProcessor
     * @brief Determines if event is vetoable using ECAL hit information
     */
    class EcalVetoProcessor: public Producer {

        public:

            typedef std::pair<EcalID, float> CellEnergyPair;

            typedef std::pair<float, float> XYCoords;

            EcalVetoProcessor(const std::string& name, Process& process) :
                    Producer(name, process) {
            }

            virtual ~EcalVetoProcessor() { }

            /**
             * Configure the processor using the given user specified parameters.
             *
             * @param parameters Set of parameters used to configure this processor.
             */
            void configure(Parameters& parameters) final override;

            void produce(Event& event);

        private:

            /** Wrappers for ecalHexReadout functions. See hitToPair().
             *  Necessary to easily combine cellID with moduleID to get unique ID of
             *  hit in layer. In future: combine celID+moduleID+layerID.
             */
            bool isInShowerInnerRing(EcalID centroidID, EcalID probeID){
                return hexReadout_->isNN(centroidID, probeID);
            }
            bool isInShowerOuterRing(EcalID centroidID, EcalID probeID){
                return hexReadout_->isNNN(centroidID, probeID);
            }
            std::pair<double,double> getCellCentroidXYPair(EcalID centroidID){
                return hexReadout_->getCellCenterAbsolute(centroidID);
            }
            std::vector<EcalID> getInnerRingCellIds(EcalID id){
                return hexReadout_->getNN(id);
            }
            std::vector<EcalID> getOuterRingCellIds(EcalID id){
                return hexReadout_->getNNN(id);
            }

            void clearProcessor();

            EcalID hitID(const EcalHit &hit) const { return EcalID(hit.getID()); }

            /* Function to calculate the energy weighted shower centroid */
            EcalID GetShowerCentroidIDAndRMS(const std::vector< EcalHit > &ecalRecHits, double & showerRMS);

            /* Function to load up empty vector of hit maps */
            void fillHitMap(const std::vector< EcalHit > &ecalRecHits,
                    std::map<EcalID, float>& cellMap_);

            /* Function to take loaded hit maps and find isolated hits in them */
            void fillIsolatedHitMap(const std::vector< EcalHit > &ecalRecHits,
                    EcalID globalCentroid,
                    std::map<EcalID, float>& cellMap_,
                    std::map<EcalID, float>& cellMapIso_,
                    bool doTight = false);

            std::vector<XYCoords> getTrajectory(std::vector<double> momentum, std::vector<float> position);

            void buildBDTFeatureVector(const ldmx::EcalVetoResult& result);

            // MIP tracking
            /* Function to find distance between two lines (line 1 passing through v1 and v2, etc.) */
            float distTwoLines(TVector3 v1, TVector3 v2, TVector3 w1, TVector3 w2);
            /* Function to find the minimum point-line distance */
            float distPtToLine(TVector3 h1, TVector3 p1, TVector3 p2);

        private:
            std::map<EcalID, float> cellMap_;
            std::map<EcalID, float> cellMapTightIso_;

            std::vector<float> ecalLayerEdepRaw_;
            std::vector<float> ecalLayerEdepReadout_;
            std::vector<float> ecalLayerTime_;
            std::vector<float> mapsx;
            std::vector<float> mapsy;


            int nEcalLayers_{0};
            int backEcalStartingLayer_{0};
            int nReadoutHits_{0};
            int deepestLayerHit_{0};
            int doBdt_{0};


            double summedDet_{0};
            double summedTightIso_{0};
            double maxCellDep_{0};
            double showerRMS_{0};
            double xStd_{0};
            double yStd_{0};
            double avgLayerHit_{0};
            double stdLayerHit_{0};
            double ecalBackEnergy_{0};
            // MIP tracking
            int nStraightTracks_{0};
            int nLinregTracks_{0};
            int firstNearPhLayer_{0};
            float epAng_{0};
            float epSep_{0};

            double bdtCutVal_{0};

            bool verbose_{false};
            bool doesPassVeto_{false};

            std::unique_ptr<EcalHexReadout> hexReadout_;

            std::string bdtFileName_;
            std::string cellFileNamexy_;
            std::vector<float> bdtFeatures_;

            /** Name of the collection which will containt the results. */
            std::string collectionName_{"EcalVeto"};

            std::unique_ptr<Ort::ONNXRuntime> rt_;

    };

    // MIP tracking
    struct HitData {
        int layer;
        TVector3 pos;
        //float x, y, z, layer;
    };

}

#endif
