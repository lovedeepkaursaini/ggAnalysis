#include "ggAnalysis/ggNtuplizer/interface/ggNtuplizer.h"

using namespace std;
using namespace edm;

ggNtuplizer::ggNtuplizer(const edm::ParameterSet& ps) {

  doGenParticles_            = ps.getParameter<bool>("doGenParticles");
  runOnParticleGun_          = ps.getParameter<bool>("runOnParticleGun");
  dumpJets_                  = ps.getParameter<bool>("dumpJets");
  dumpSubJets_               = ps.getParameter<bool>("dumpSubJets");
  dumpTaus_                  = ps.getParameter<bool>("dumpTaus");
  isAOD_                     = ps.getParameter<bool>("isAOD");

  vtxLabel_                  = consumes<reco::VertexCollection>     (ps.getParameter<InputTag>("VtxLabel"));
  vtxBSLabel_                = consumes<reco::VertexCollection>     (ps.getParameter<InputTag>("VtxBSLabel"));
  rhoLabel_                  = consumes<double>                     (ps.getParameter<InputTag>("rhoLabel"));
  generatorLabel_            = consumes<GenEventInfoProduct>        (ps.getParameter<InputTag>("generatorLabel"));
  puCollection_              = consumes<vector<PileupSummaryInfo> > (ps.getParameter<InputTag>("pileupCollection"));
  genParticlesCollection_    = consumes<vector<reco::GenParticle> > (ps.getParameter<InputTag>("genParticleSrc"));
  pfMETlabel_                = consumes<View<pat::MET> >            (ps.getParameter<InputTag>("pfMETLabel"));
  electronCollection_        = consumes<View<pat::Electron> >       (ps.getParameter<InputTag>("electronSrc"));
  photonCollection_          = consumes<View<pat::Photon> >         (ps.getParameter<InputTag>("photonSrc"));
  muonCollection_            = consumes<View<pat::Muon> >           (ps.getParameter<InputTag>("muonSrc"));
  ebReducedRecHitCollection_ = consumes<EcalRecHitCollection>       (ps.getParameter<InputTag>("ebReducedRecHitCollection"));
  eeReducedRecHitCollection_ = consumes<EcalRecHitCollection>       (ps.getParameter<InputTag>("eeReducedRecHitCollection"));
  esReducedRecHitCollection_ = consumes<EcalRecHitCollection>       (ps.getParameter<InputTag>("esReducedRecHitCollection")); 
  recophotonCollection_      = consumes<reco::PhotonCollection>     (ps.getParameter<InputTag>("recoPhotonSrc"));
  tracklabel_                = consumes<reco::TrackCollection>      (ps.getParameter<InputTag>("TrackLabel"));
  gsfElectronlabel_          = consumes<reco::GsfElectronCollection>(ps.getParameter<InputTag>("gsfElectronLabel"));
  tauCollection_             = consumes<vector<pat::Tau> >          (ps.getParameter<InputTag>("tauSrc"));
  pfAllParticles_            = consumes<reco::PFCandidateCollection>(ps.getParameter<InputTag>("PFAllCandidates"));
  jetCollection_             = consumes<View<pat::Jet> >            (ps.getParameter<InputTag>("jetSrc"));
  jetsCHSLabel_              = consumes<View<pat::Jet> >            (ps.getParameter<InputTag>("selectedPatJetsCA8PFCHS"));
  newparticles_              = ps.getParameter< vector<int > >("newParticles");

  pfLooseId_                 = ps.getParameter<ParameterSet>("pfLooseId");

  cicPhotonId_ = new CiCPhotonID(ps);

  Service<TFileService> fs;
  tree_    = fs->make<TTree>("EventTree", "Event data");
  hEvents_ = fs->make<TH1F>("hEvents",    "total processed and skimmed events",   2,  0,   2);

  branchesGlobalEvent(tree_);

  if (doGenParticles_) {
    branchesGenInfo(tree_, fs);
    branchesGenPart(tree_);
  }

  branchesPhotons(tree_);
  branchesElectrons(tree_);
  branchesMuons(tree_);

  if (dumpTaus_) branchesTaus(tree_);
  if (dumpJets_) branchesJets(tree_);
}

ggNtuplizer::~ggNtuplizer() {
  cleanupPhotons();
  delete cicPhotonId_;
}

void ggNtuplizer::analyze(const edm::Event& e, const edm::EventSetup& es) {

  hEvents_->Fill(0.5);

  edm::Handle<reco::VertexCollection> vtxHandle;
  e.getByToken(vtxLabel_, vtxHandle);

  // best-known primary vertex coordinates
  math::XYZPoint pv(0, 0, 0);
  for (vector<reco::Vertex>::const_iterator v = vtxHandle->begin(); v != vtxHandle->end(); ++v) {
    // replace isFake() for miniAOD since it requires tracks while miniAOD vertices don't have tracks:
    // Vertex.h: bool isFake() const {return (chi2_==0 && ndof_==0 && tracks_.empty());}
    bool isFake = isAOD_ ? v->isFake() : (v->chi2() == 0 && v->ndof() == 0);

    if (!isFake) {
      pv.SetXYZ(v->x(), v->y(), v->z());
      break;
    }
  }

  fillGlobalEvent(e);

  if (!e.isRealData()) {
    fillGenInfo(e);
    if (doGenParticles_)
      fillGenPart(e);
  }

  fillPhotons(e, es); // FIXME: photons have different vertex (not pv)
  fillElectrons(e, es, pv);
  fillMuons(e, pv);

  if (dumpTaus_) fillTaus(e);
  if (dumpJets_) fillJets(e);

  hEvents_->Fill(1.5);
  tree_->Fill();
}


// void ggNtuplizer::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
// {
//   //The following says we do not know what parameters are allowed so do no validation
//   // Please change this to state exactly what you do use, even if it is no parameters
//   edm::ParameterSetDescription desc;
//   desc.setUnknown();
//   descriptions.addDefault(desc);
// }

DEFINE_FWK_MODULE(ggNtuplizer);
