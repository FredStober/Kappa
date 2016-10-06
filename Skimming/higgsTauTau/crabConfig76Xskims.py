#!/bin/env python
# crab submission script
# usage: python crabConfig.py submit

from CRABClient.UserUtilities import getUsernameFromSiteDB
from httplib import HTTPException
from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from multiprocessing import Process
from Kappa.Skimming.registerDatasetHelper import get_sample_by_nick,get_n_files_from_nick,get_inputDBS_by_nick
from Kappa.Skimming.datasetsHelper2015 import isData
import sys
from glob import glob
import os, shutil
from Kappa.Skimming.tools import read_grid_control_includes
import datetime
today=datetime.date.today().strftime("%Y-%m-%d")
date = today # feel free to change
from math import ceil
job_submission_limit = 10000 # crab3 can only submit up to 10000 jobs per task

def submit(config):
	try:
		crabCommand('submit', config = config)
	except HTTPException as hte:
		print "Failed submitting task: %s" % (hte.headers)
	except ClientException as cle:
		print "Failed submitting task: %s" % (cle)

def crab_command(command):
	for dir in glob('/nfs/dust/cms/user/%s/kappa/crab_kappa_skim76X-%s/*'%(getUsernameFromSiteDB(), date)):
	#for dir in glob('/net/scratch_cms/institut_3b/%s/kappa/crab_kappa_skim-%s/*'%(getUsernameFromSiteDB(), date)):
		try:
			crabCommand(command, dir = dir)
		except HTTPException as hte:
			print hte

def check_path(path):
	if os.path.exists(path):
		print(path + " already exists! Delete it now in order to re-initialize it by crab? [y/n]")
		yes = set(['yes','y', 'ye', ''])
		no = set(['no','n'])

		choice = raw_input().lower()
		if choice in yes:
			shutil.rmtree(path)
			return
		elif choice in no:
			return
		else:
			sys.stdout.write(path + " already exists! Delete it now in order to re-initialize it by crab?")


def submission():
	from CRABClient.UserUtilities import config
	config = config()
	#config.General.workArea = '/nfs/dust/cms/user/%s/kappa/crab_kappa_skim76X-%s'%(getUsernameFromSiteDB(), date)
	config.General.workArea = '/nfs/dust/cms/user/<your-NAF-username>/kappa/crab_kappa_skim76X-%s'% date
	#config.General.workArea = '/net/scratch_cms/institut_3b/%s/kappa/crab_kappa_skim-%s'%(getUsernameFromSiteDB(), date)
	check_path(config.General.workArea)
	config.General.transferOutputs = True
	config.General.transferLogs = True
	config.User.voGroup = 'dcms'
	
	config.JobType.pluginName = 'Analysis'
	config.JobType.psetName = 'kSkimming_run2_cfg.py'
	#config.JobType.inputFiles = ['Summer15_V5_MC.db']
	config.JobType.allowUndistributedCMSSW = True
	config.Site.blacklist = ["T2_BR_SPRACE"]
	config.Data.splitting = 'FileBased'
	config.Data.unitsPerJob = 1
	config.Data.outLFNDirBase = '/store/user/%s/higgs-kit/skimming/76X_%s'%(getUsernameFromSiteDB(), date)
	config.Data.publication = False
	
	config.Site.storageSite = "T2_DE_DESY"
	# load nicknames form gc-style config files and write them to a flat nicknames list
	nicknames = read_grid_control_includes(["samples/13TeV/Fall15_Higgs_CPAnalysis.conf"])
	#nicknames = read_grid_control_includes(["samples/13TeV/Fall15_SM_Analysis.conf"])
	#nicknames = ['SUSYGluGluToHToTauTauM160_RunIIFall15MiniAODv2_PU25nsData2015v1_13TeV_MINIAOD_pythia8']

	# loop over datasets and get repsective nicks
	for nickname in nicknames:
		config.General.requestName = nickname
		config.Data.inputDBS = get_inputDBS_by_nick(nickname)
		config.JobType.pyCfgParams = ['globalTag=76X_dataRun2_16Dec2015_v0' if isData(nickname) else 'globalTag=76X_mcRun2_asymptotic_RunIIFall15DR76_v1' ,'kappaTag=KAPPA_2_1_0','nickname=%s'%(nickname),'outputfilename=kappa_%s.root'%(nickname),'testsuite=False']
		config.JobType.outputFiles = ['kappa_%s.root'%(nickname)]
		config.Data.inputDataset = get_sample_by_nick(nickname)
		nfiles = get_n_files_from_nick(nickname)
		if float(config.Data.unitsPerJob) > 0 and float(nfiles)/float(config.Data.unitsPerJob) >= job_submission_limit:
			files_per_job = ceil(float(nfiles)/job_submission_limit)
			if files_per_job > 1:
				config.Data.unitsPerJob = int(files_per_job)
		p = Process(target=submit, args=(config,))
		p.start()
		p.join()

if __name__ == "__main__":
	if len(sys.argv) == 1: 
		print "no setting provided"
		sys.exit()
	if sys.argv[1] == "submit":
		submission()
	elif sys.argv[1] in ["status", "resubmit", "kill"]:
		crab_command(sys.argv[1])
	else:
		print "setting \"%s\" is not implemented"% sys.argv[1]
