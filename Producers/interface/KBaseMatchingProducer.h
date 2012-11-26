#ifndef KAPPA_MATCHINGPRODUCER_H
#define KAPPA_MATCHINGPRODUCER_H

#include "KBaseProducer.h"
#include <FWCore/Utilities/interface/InputTag.h>

template<typename Tout>
class KBaseMatchingProducer : public KBaseProducerWP
{
public:
	KBaseMatchingProducer(const edm::ParameterSet &cfg, TTree *_event_tree, TTree *_run_tree) :
		KBaseProducerWP(cfg, _event_tree, _run_tree, Tout::producer()),
		event_tree(_event_tree), matchingCounter(0),
		viManual(cfg.getParameter<std::vector<edm::InputTag> >("manual")),

		vsWhitelist(cfg.getParameter<std::vector<std::string> >("whitelist")),
		vsBlacklist(cfg.getParameter<std::vector<std::string> >("blacklist")),

		vsRename(cfg.getParameter<std::vector<std::string> >("rename")),
		vsRenameWhitelist(cfg.getParameter<std::vector<std::string> >("rename_whitelist")),
		vsRenameBlacklist(cfg.getParameter<std::vector<std::string> >("rename_blacklist"))
	{
	}
	virtual ~KBaseMatchingProducer() {}

	virtual bool onFirstEvent(const edm::Event &event, const edm::EventSetup &setup)
	{
		addPSetRequests(event, setup);
		addRegExRequests(event, setup);
		if (this->verbosity > 0)
			std::cout << "Accepted number of matched products: " << matchingCounter << std::endl;
		return KBaseProducerWP::onFirstEvent(event, setup);
	}

protected:
	virtual bool onMatchingInput(const std::string targetName, const std::string inputName,
		const edm::ParameterSet &pset, const edm::InputTag &tag)
	{
		++matchingCounter;
		vsMatched.push_back(targetName);
		return true;
	}

	typename Tout::type *allocateBronch(const std::string bronchName)
	{
		// Static storage of ROOT bronch target - never changes, only accessed here:
		bronchStorage[bronchName] = new typename Tout::type();
		this->event_tree->Bronch(bronchName.c_str(), Tout::id().c_str(), &(bronchStorage[bronchName]));
		return bronchStorage[bronchName];
	}

private:
	TTree *event_tree;

	int matchingCounter;
	std::vector<edm::InputTag> viManual;
	std::vector<std::string> vsWhitelist;
	std::vector<std::string> vsBlacklist;

	std::vector<std::string> vsRename;
	std::vector<std::string> vsRenameWhitelist;
	std::vector<std::string> vsRenameBlacklist;

	std::vector<std::string> vsMatched;
	std::map<std::string, typename Tout::type*> bronchStorage;

	bool addPSetRequests(const edm::Event &event, const edm::EventSetup &setup)
	{
		std::cout << "Requesting entries: ";

		const edm::ParameterSet &psBase = this->psBase;
		std::vector<std::string> names = psBase.getParameterNamesForType<edm::ParameterSet>();

		for (size_t i = 0; i < names.size(); ++i)
		{
			std::cout << "\"" << names[i] << "\" ";
			const edm::ParameterSet pset = psBase.getParameter<edm::ParameterSet>(names[i]);
			// Notify about match
			onMatchingInput(names[i], names[i], pset, pset.getParameter<edm::InputTag>("src"));
		}
		std::cout << std::endl;

		return true;
	}

	bool addRegExRequests(const edm::Event &event, const edm::EventSetup &setup)
	{
		std::vector<edm::Provenance const*> plist;
		event.getAllProvenance(plist);

		for (std::vector<edm::Provenance const*>::const_iterator piter = plist.begin(); piter < plist.end(); ++piter)
		{
			if (this->verbosity > 1)
				std::cout << "Product: ";

			// Check if branch was selected
			if (!(this->regexMatch((*piter)->branchName(), vsWhitelist, vsBlacklist)
				|| this->tagMatch(*piter, viManual)))
			{
				continue;
			}

			// Rename branch if requested
			if ((this->verbosity > 0) && (vsRename.size() > 0))
			{
				std::cout << " => " + Tout::producer() + " will use: ";
				std::cout.flush();
			}
			const std::string targetName = this->regexRename((*piter)->moduleLabel(), vsRename);
			if ((this->verbosity > 0) && (vsRename.size() > 0))
				std::cout << targetName << " as matching name." << std::endl;

			// Filter on the new name
			if (!this->regexMatch(targetName, vsRenameWhitelist, vsRenameBlacklist))
			{
				if (this->verbosity > 0)
					std::cout << " => Branch was vetoed by post-rename Black/Whitelist!" << std::endl;
				continue;
			}

			// Avoid name collisions: Ignore or Fail
			if (std::find(vsMatched.begin(), vsMatched.end(), targetName) != vsMatched.end())
			{
				if (this->verbosity > 0)
					std::cout << " => Matching name was already processed!" << std::endl;
				continue;
			}

			// Create selection tag
			const edm::InputTag tag((*piter)->moduleLabel(), (*piter)->productInstanceName(), (*piter)->processName());
			if (this->verbosity > 1)
				std::cout << " => Branch will be selected with " << tag << std::endl;

			// Notify about match
			onMatchingInput(targetName, (*piter)->branchName(), this->psBase, tag);
		}

		return true;
	}
};

#endif
