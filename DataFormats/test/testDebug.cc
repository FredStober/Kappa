/* This file is autogenerated by classes.UP. Do not edit!

    Copyright (c) 2010 - 2015, All Rights Reserved
    The authors are listed in the AUTHORS file.

    Standalone script to test the debug outputs of Kappa

    compile with
    g++ testDebug.cc $(root-config --cflags) $(root-config --ldflags --libs) -I../.. -o testdebug  -L../../lib -lKappa -lGenVector
*/

#include "DataFormats/interface/KDebug.h"
#include <iostream>

using namespace std;

int main() {
	cout << "Kappa object debug output:" << endl;
	cout << "* KEventInfo: " << KEventInfo() << endl << endl;
	cout << "* KGenEventInfo: " << KGenEventInfo() << endl << endl;
	cout << "* KBasicJet: " << KBasicJet() << endl << endl;
	cout << "* KBasicTau: " << KBasicTau() << endl << endl;
	cout << "* KBeamSpot: " << KBeamSpot() << endl << endl;
	cout << "* KCaloJet: " << KCaloJet() << endl << endl;
	cout << "* KElectron: " << KElectron() << endl << endl;
	cout << "* KLV: " << KLV() << endl << endl;
	cout << "* KGenJet: " << KGenJet() << endl << endl;
	cout << "* KGenParticle: " << KGenParticle() << endl << endl;
	cout << "* KGenPhoton: " << KGenPhoton() << endl << endl;
	cout << "* KGenTau: " << KGenTau() << endl << endl;
	cout << "* KHit: " << KHit() << endl << endl;
	cout << "* KJet: " << KJet() << endl << endl;
	cout << "* KL1Muon: " << KL1Muon() << endl << endl;
	cout << "* KLepton: " << KLepton() << endl << endl;
	cout << "* KLeptonPair: " << KLeptonPair() << endl << endl;
	cout << "* KMET: " << KMET() << endl << endl;
	cout << "* KMuon: " << KMuon() << endl << endl;
	cout << "* KMuonTriggerCandidate: " << KMuonTriggerCandidate() << endl << endl;
	cout << "* KParticle: " << KParticle() << endl << endl;
	cout << "* KPFCandidate: " << KPFCandidate() << endl << endl;
	cout << "* KPhoton: " << KPhoton() << endl << endl;
	cout << "* KPileupDensity: " << KPileupDensity() << endl << endl;
	cout << "* KTau: " << KTau() << endl << endl;
	cout << "* KExtendedTau: " << KExtendedTau() << endl << endl;
	cout << "* KTaupairVerticesMap: " << KTaupairVerticesMap() << endl << endl;
	cout << "* KTrack: " << KTrack() << endl << endl;
	cout << "* KTriggerObjects: " << KTriggerObjects() << endl << endl;
	cout << "* KVertex: " << KVertex() << endl << endl;
	cout << "* KFilterSummary: " << KFilterSummary() << endl << endl;
	cout << "* KHCALNoiseSummary: " << KHCALNoiseSummary() << endl << endl;
	cout << "* KTrackSummary: " << KTrackSummary() << endl << endl;
	cout << "* KVertexSummary: " << KVertexSummary() << endl << endl;
	cout << "* KLumiInfo: " << KLumiInfo() << endl << endl;
	cout << "* KGenLumiInfo: " << KGenLumiInfo() << endl << endl;
	cout << "* KDataLumiInfo: " << KDataLumiInfo() << endl << endl;
	cout << "* KElectronMetadata: " << KElectronMetadata() << endl << endl;
	cout << "* KFilterMetadata: " << KFilterMetadata() << endl << endl;
	cout << "* KMuonMetadata: " << KMuonMetadata() << endl << endl;
	cout << "* KGenEventInfoMetadata: " << KGenEventInfoMetadata() << endl << endl;
	cout << "* KJetMetadata: " << KJetMetadata() << endl << endl;
	cout << "* KTauMetadata: " << KTauMetadata() << endl << endl;
	cout << "* KTriggerObjectMetadata: " << KTriggerObjectMetadata() << endl << endl;
	cout << "* KProvenance: " << KProvenance() << endl << endl;
}
