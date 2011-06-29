#include "../interface/KDebug.h"

// Need thin wrapper class for ostream operator overloading
// - overkill here, but simplifies vector output later
template<typename T> struct KLVWrap { KLVWrap(const T &_p4) : p4(_p4) {}; const T &p4; };
template<typename T> std::ostream &operator<<(std::ostream &os, const KLVWrap<T> &lv)
{
	return os << "(pt=" << lv.p4.pt() << ", eta=" << lv.p4.eta() << ", phi="
		<< lv.p4.phi() << ", E=" << lv.p4.E()  << ", m=" << lv.p4.M() << ")";
}

std::ostream &operator<<(std::ostream &os, const KLV &lv)
{
	return os << KLVWrap<KLV::KInternalLV>(lv.p4);
}

std::ostream &operator<<(std::ostream &os, const KDataLV &lv)
{
	return os << KLVWrap<KDataLV::KInternalLV>(lv.p4);
}

std::ostream &operator<<(std::ostream &os, const KDataBeamSpot &bs)
{
	return os << bs.position << " betaStar=" << bs.betaStar;
}

std::ostream &operator<<(std::ostream &os, const KDataMET &met)
{
	return os << static_cast<const KDataLV>(met) << " sumEt=" << met.sumEt;
}

std::ostream &operator<<(std::ostream &os, const KDataPFMET &met)
{
	os << static_cast<const KDataMET>(met) << std::endl;
	os << "\tCharged (em,had): " << met.chargedEMEtFraction << ", " << met.chargedHadEtFraction << std::endl;
	os << "\tNeutral (em,had): " << met.neutralEMEtFraction << ", " << met.neutralHadEtFraction << std::endl;
	os << "\tMuonF=" << met.muonEtFraction << " type6F=" << met.type6EtFraction << " type7F=" << met.type7EtFraction;
	return os;
}

std::ostream &operator<<(std::ostream &os, const KTrackSummary &s)
{
	return os << "#Tracks=" << s.nTracks << " #HQ Tracks=" << s.nTracksHQ;
}

std::ostream &operator<<(std::ostream &os, const KParton &p)
{
	return os << KLVWrap<KParton::KInternalLV>(p.p4)
		<< " status=" << p.status() << " pdgid=" << p.pdgId();
}

std::ostream &operator<<(std::ostream &os, const KDataJet &jet)
{
	os << static_cast<const KDataLV>(jet) << std::endl;
	os << "\t#Const=" << jet.nConst << " n90=" << jet.n90 << " n90Hits=" << jet.n90Hits << std::endl;
	os << "\tfHPD=" << jet.fHPD << " fRBX=" << jet.fRBX << " HCAL noise=" << jet.noiseHCAL << std::endl;
	return os << "\tfEM=" << jet.fEM << " fHO=" << jet.fHO << " area=" << jet.area;
}

std::ostream &operator<<(std::ostream &os, const KDataPFJet &jet)
{
	os << static_cast<const KDataLV>(jet) << std::endl;
	os << "\tCharged (em,had): " << jet.chargedEMFraction << ", " << jet.chargedHadFraction << std::endl;
	os << "\tNeutral (em,had): " << jet.neutralEMFraction << ", " << jet.neutralHadFraction << std::endl;
	return os << "\t#Const=" << jet.nConst << " #Charged=" << jet.nCharged << " area=" << jet.area;
}

std::ostream &operator<<(std::ostream &os, const KDataTau &tau)
{
	return os << static_cast<const KDataLV>(tau) << " charge=" << tau.charge;
}

std::ostream &operator<<(std::ostream &os, const KDataCaloTau &tau)
{
	return os << static_cast<const KDataLV>(tau) << " charge=" << tau.charge;
}

std::ostream &operator<<(std::ostream &os, const KDataGenTau &tau)
{
	return os << static_cast<const KParton>(tau)
		<< " p4_vis=" << KLVWrap<KDataLV::KInternalLV>(tau.p4_vis)
		<< " decayMode=" << tau.decayMode << " vertex=" << tau.vertex;
}

std::ostream &operator<<(std::ostream &os, const KDataVertex &vertex)
{
	return os << vertex.position
		<< " #Tracks=" << vertex.nTracks << " fake=" << vertex.fake
		<< " chi2=" << vertex.chi2 << " DOF=" << vertex.nDOF;
}

std::ostream &operator<<(std::ostream &os, const KVertexSummary &vs)
{
	return os << vs.pv << " #Vertices=" << vs.nVertices;
}

std::ostream &operator<<(std::ostream &os, const KJetArea &ja)
{
	return os << "median=" << ja.median << " sigma=" << ja.sigma;
}

std::ostream &operator<<(std::ostream &os, const KDataHit &hit)
{
	return os << "theta=" << hit.theta << " phi=" << hit.phi
		<< " pAbs=" << hit.pAbs << " Eloss=" << hit.energyLoss;
}

std::ostream &operator<<(std::ostream &os, const KPFCandidate &cand)
{
	return os << static_cast<const KDataLV>(cand)
		<< " charge=" << cand.charge() << " pdgid=" << cand.pdgId()
		<< " deltaP=" << cand.deltaP
		<< " Eecal=" << cand.ecalEnergy << " Ehcal=" << cand.hcalEnergy;
}

////////////////////////////////////////////////////////////
// KMetadata.h
////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &os, const KProvenance &p)
{
	if (p.names.size() != p.branches.size())
		return os << "Provenance corrupted!" << std::endl;
	for (size_t i = 0; i < p.names.size(); ++i)
		os << p.names[i] << " = " << p.branches[i] << std::endl;
	return os;
}

std::ostream &operator<<(std::ostream &os, const KLumiMetadata &m)
{
	os << "Run: " << m.nRun << " " << m.nLumi << std::endl;
	os << "HLT names "; displayVector(os, m.hltNames);
	return os;
}

std::ostream &operator<<(std::ostream &os, const KGenLumiMetadata &m)
{
	os << static_cast<const KLumiMetadata>(m) << std::endl;
	return os
		<< "ext. xSec: " << m.xSectionExt << " "
		<< "int. xSec: " << m.xSectionInt;
}

std::ostream &operator<<(std::ostream &os, const KDataLumiMetadata &m)
{
	os << static_cast<const KLumiMetadata>(m) << std::endl;
	return os
		<< "Delivered=" << m.avgInsDelLumi << "+/-" << m.avgInsDelLumiErr << std::endl
		<< "Recorded=" << m.avgInsRecLumi << "+/-" << m.avgInsRecLumiErr << std::endl
		<< "deadtime=" << m.deadFrac << " length=" << m.lumiSectionLength << " quality=" << m.lumiSecQual;
}

std::ostream &operator<<(std::ostream &os, const KEventMetadata &m)
{
	return os << "Event ID = " << m.nRun << ":" << m.nLumi << ":" << m.nBX << ":" << m.nEvent;
}

std::ostream &operator<<(std::ostream &os, const KGenEventMetadata &m)
{
	os << static_cast<const KEventMetadata>(m) << std::endl;
	return os
		<< "Weight: " << m.weight
		<< "#PU: " << m.numPUInteractions;
}

std::ostream &displayHLT(std::ostream &os, const KLumiMetadata &metaLumi, const KEventMetadata &metaEvent)
{
	for (size_t hltIdx = 0; hltIdx < metaLumi.hltNames.size(); ++hltIdx)
		if (metaEvent.bitsHLT & ((unsigned long long)1 << hltIdx))
			os << hltIdx << ":" << metaLumi.hltNames[hltIdx] << " ";
		os << std::endl;
	return os;
}
