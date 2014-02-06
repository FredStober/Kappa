import os
import sys
sys.path.append(os.path.abspath(os.path.dirname(sys.argv[0])) + "/Kappa/")

## ------------------------------------------------------------------------
## import skeleton process KAPPA
from Kappa.Producers.KSkimming_template_cfg import *

## test file for EKP
process.source.fileNames			= cms.untracked.vstring('file:/storage/8/dhaitz/testfiles/mc11.root')
## test file for lxplus
#process.source.fileNames			= cms.untracked.vstring('root://eoscms//eos/cms/store/relval/CMSSW_5_3_6-START53_V14/RelValProdTTbar/AODSIM/v2/00000/76ED0FA6-1E2A-E211-B8F1-001A92971B72.root')

process.maxEvents.input				= 100											## number of events to be processed (-1 = all in file)
process.kappaTuple.outputFile 	= 'kappaTuple_treeMetadata.root'				## name of output file
process.kappaTuple.verbose			= cms.int32(0)								## verbosity level

## ------------------------------------------------------------------------
## declare edm OutputModule (expects a path 'p'), uncommented if wanted

process.kappaTuple.active									= cms.vstring('TreeMetadata')
process.kappaTuple.TreeMetadata.dataset				= cms.string("datasetName")
process.kappaTuple.TreeMetadata.generator				= cms.string("pythia")
process.kappaTuple.TreeMetadata.tune 					= cms.string("")
process.kappaTuple.TreeMetadata.productionProcess	= cms.string("DyMuMu")
process.kappaTuple.TreeMetadata.globalTag				= cms.string("START53_V19F")
process.kappaTuple.TreeMetadata.runPeriod				= cms.string("")
process.kappaTuple.TreeMetadata.kappaTag				= cms.string("Kappa_1_0_0")
process.kappaTuple.TreeMetadata.isEmbedded			= cms.bool(False)
process.kappaTuple.TreeMetadata.jetMultiplicity		= cms.int32(0)


process.edmOut = cms.OutputModule("PoolOutputModule",
										fileName = cms.untracked.string('patTuple_muons.root'),						## name of output file 
										SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),	## save only events passing the full path
										outputCommands = cms.untracked.vstring('drop *', 'keep *_*_*_KAPPA')		## save each edm object that has been produced by process KAPPA 
										)
process.kappaOut+=process.edmOut

## ------------------------------------------------------------------------
## KappaMuons
#process.load("Kappa.Producers.KMuons_cff")

## And let it run
process.p = cms.Path(

	)
