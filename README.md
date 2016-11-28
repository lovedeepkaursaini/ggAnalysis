## to work in cmssw_8_0_22

cmsrel CMSSW_8_0_22

cd CMSSW_8_0_22/src/

cmsenv

git cms-merge-topic -u cms-met:CMSSW_8_0_X-METFilterUpdate

git cms-merge-topic cms-met:METRecipe_8020

git cms-init

git cms-merge-topic emanueledimarco:ecal_smear_fix_80X

cd EgammaAnalysis/ElectronTools/data

git clone -b ICHEP2016_v2 https://github.com/ECALELFS/ScalesSmearings.git

git clone https://github.com/cmkuo/HiggsAnalysis.git

git clone https://github.com/cmkuo/ggAnalysis.git

scramv1 b -j 10
