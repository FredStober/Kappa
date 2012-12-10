#ifndef KAPPA_PFTAUPRODUCER_H
#define KAPPA_PFTAUPRODUCER_H

#include "KMetadataProducer.h"
#include "KTauProducer.h"

#include <DataFormats/TauReco/interface/PFTau.h>
#include <DataFormats/TauReco/interface/PFTauDiscriminator.h>

struct KPFTauProducer_Product
{
	typedef std::vector<KDataPFTau> type;
	static const std::string id() { return "std::vector<KDataPFTau>"; };
	static const std::string producer() { return "KDataPFTauProducer"; };
};

class KPFTauProducer : public KTauProducer<reco::PFTau, reco::PFTauDiscriminator, KPFTauProducer_Product>
{
public:
	KPFTauProducer(const edm::ParameterSet &cfg, TTree *_event_tree, TTree *_lumi_tree) :
		KTauProducer<reco::PFTau, reco::PFTauDiscriminator, KPFTauProducer_Product>(cfg, _event_tree, _lumi_tree)
	{
	}

protected:
	virtual bool isCorrectType(std::string className)
	{
		return className == "reco::PFTauDiscriminator";
	}
	virtual void fillSingle(const SingleInputType &in, SingleOutputType &out)
	{
		// Fill fields of KDataTau via base class
		KTauProducer<reco::PFTau, reco::PFTauDiscriminator, KPFTauProducer_Product>::fillSingle(in, out);

		// Fill additional fields from KDataPFTau
		out.emFraction = in.emFraction();
		out.cntSignalChargedHadrCands = in.signalPFChargedHadrCands().size();
		out.cntSignalGammaCands = in.signalPFGammaCands().size();
		out.cntSignalNeutrHadrCands = in.signalPFNeutrHadrCands().size();
		out.cntSignalCands = in.signalPFCands().size();

		if(in.leadPFCand().isNonnull())
		{
			// Vertex
			out.vertex.fake = false;
			out.vertex.position = in.leadPFCand()->vertex();
			out.vertex.chi2 = in.leadPFCand()->vertexChi2();
			out.vertex.nDOF = in.leadPFCand()->vertexNdof();
			out.vertex.nTracks = 1;
			out.vertex.covariance = ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> >(); //in.leadPFCand()->vertexCovariance());
/*			out.errDxy = in.leadPFCand()->dxyError();
			out.errDz = in.leadPFCand()->dzError();*/

			copyP4(in.leadPFCand()->p4(), out.leadTrack);
		}
		else
		{
			out.vertex.fake = true;
		}

		if(in.leadPFChargedHadrCand().isNonnull())
			copyP4(in.leadPFChargedHadrCand()->p4(), out.leadChargedHadrTrack);
		if(in.leadPFNeutralCand().isNonnull())
			copyP4(in.leadPFNeutralCand()->p4(), out.leadNeutralTrack);

		out.cntSignalTracks = in.signalTracks().size();
	}
};

#endif
