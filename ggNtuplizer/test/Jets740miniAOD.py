import FWCore.ParameterSet.Config as cms

process = cms.Process('ggNtuplizer')
process.load("Configuration.Geometry.GeometryIdeal_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = 'MCRUN2_74_V7'
process.load("Configuration.StandardSequences.MagneticField_cff")

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.options.allowUnscheduled = cms.untracked.bool(True)

process.load("ggAnalysis.ggNtuplizer.ggNtuplizer_miniAOD_cfi")

##Taus
#from PhysicsTools.PatAlgos.tools.tauTools import *
process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")
#process.cleanPatTaus.preselection = cms.string(' tauID("decayModeFinding") > 0.5 ')
#process.cleanPatTaus.finalCut = cms.string(' pt > 15.0 & abs(eta) < 2.5 ')
process.load("ggAnalysis.ggNtuplizer.ggTau_cff")
##Jets

process.ggNtuplizer.dumpSubJets=cms.bool(True)
process.ggNtuplizer.dumpJets=cms.bool(True)
process.ggNtuplizer.dumpTaus=cms.bool(False)

process.TFileService = cms.Service("TFileService", fileName = cms.string('ggtree_mc.root'))

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
#        'root://cmsxrootd-site.fnal.gov//store/mc/Phys14DR/DYJetsToLL_M-50_13TeV-madgraph-pythia8/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/0432E62A-7A6C-E411-87BB-002590DB92A8.root'
'/store/relval/CMSSW_7_4_0_pre9_ROOT6/RelValWpToENu_M-2000_13TeV/MINIAODSIM/MCRUN2_74_V7-v1/00000/4A75C5D1-DCD1-E411-BE48-002618943951.root'
        )
     )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.p = cms.Path(
  process.ggNtuplizer
)


