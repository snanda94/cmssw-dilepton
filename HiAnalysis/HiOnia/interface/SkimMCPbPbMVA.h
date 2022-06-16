#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TVector3.h"
#include <TRandom1.h>
#include <vector>
#include <TLorentzVector.h>
#include "THnSparse.h"
#include "TRandom3.h"
#include <cstring>
#include <ctime>
#include <iostream>
#include <cmath>
#include <math.h>
#include <fstream>
#include <vector>
#include <map>
#include "TFrame.h"
#include "TH1F.h"
#include "TBenchmark.h"
#include "TSystem.h"
#include <stdlib.h>
#ifndef __CINT__
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#endif
using namespace ROOT::Math;
using namespace std;


Short_t Reco_mu_size;
Int_t Centrality;
const Int_t maxN = 50;
Short_t* Reco_mu_whichGen = new Short_t[maxN];
Bool_t*  Reco_mu_InTightAcc = new Bool_t[maxN];
Bool_t*  Reco_mu_InLooseAcc = new Bool_t[maxN];
Bool_t*  Reco_mu_highPurity = new Bool_t[maxN];
Bool_t*  Reco_mu_TMOneStaTight = new Bool_t[maxN];
Bool_t*  Reco_mu_isPF = new Bool_t[maxN];
Bool_t*  Reco_mu_isTracker = new Bool_t[maxN];
Bool_t*  Reco_mu_isGlobal = new Bool_t[maxN];
Bool_t*  Reco_mu_isSoft = new Bool_t[maxN];
Bool_t*  Reco_mu_isHybridSoft = new Bool_t[maxN];
Bool_t*  Reco_mu_isMedium = new Bool_t[maxN];
Int_t*   Reco_mu_nPixValHits = new Int_t[maxN];
Int_t*   Reco_mu_nMuValHits = new Int_t[maxN];
Int_t*   Reco_mu_nMuValHits_inner = new Int_t[maxN];
Int_t*   Reco_mu_nMuValHits_bestTracker = new Int_t[maxN];
Int_t*   Reco_mu_nTrkHits = new Int_t[maxN];
Float_t* Reco_mu_segmentComp = new Float_t[maxN];
Float_t* Reco_mu_kink = new Float_t[maxN];
Float_t* Reco_mu_localChi2 = new Float_t[maxN];
Float_t* Reco_mu_validFraction = new Float_t[maxN];
Float_t* Reco_mu_normChi2_bestTracker = new Float_t[maxN];
Float_t* Reco_mu_normChi2_inner = new Float_t[maxN];
Float_t* Reco_mu_normChi2_global = new Float_t[maxN];
Int_t*   Reco_mu_nPixWMea = new Int_t[maxN];
Int_t*   Reco_mu_nTrkWMea = new Int_t[maxN];
Int_t*   Reco_mu_StationsMatched = new Int_t[maxN];

TClonesArray *Reco_mu_4mom = new TClonesArray("TLorentzVector");


void read_tree(TTree *mytree, bool isMC){
   
    mytree->SetBranchAddress("Reco_mu_size", &Reco_mu_size);
    mytree->SetBranchAddress("Centrality", &Centrality);
    mytree->SetBranchAddress("Reco_mu_whichGen", Reco_mu_whichGen);
    mytree->SetBranchAddress("Reco_mu_InTightAcc", Reco_mu_InTightAcc);
    mytree->SetBranchAddress("Reco_mu_InLooseAcc", Reco_mu_InLooseAcc);
    mytree->SetBranchAddress("Reco_mu_highPurity", Reco_mu_highPurity);
    mytree->SetBranchAddress("Reco_mu_TMOneStaTight", Reco_mu_TMOneStaTight);
    mytree->SetBranchAddress("Reco_mu_isPF", Reco_mu_isPF);
    mytree->SetBranchAddress("Reco_mu_isTracker", Reco_mu_isTracker);
    mytree->SetBranchAddress("Reco_mu_isGlobal", Reco_mu_isGlobal);
    mytree->SetBranchAddress("Reco_mu_isSoft", Reco_mu_isSoft);
    mytree->SetBranchAddress("Reco_mu_isHybridSoft", Reco_mu_isHybridSoft);
    mytree->SetBranchAddress("Reco_mu_isMedium", Reco_mu_isMedium);
    mytree->SetBranchAddress("Reco_mu_nPixValHits", Reco_mu_nPixValHits);
    mytree->SetBranchAddress("Reco_mu_nMuValHits", Reco_mu_nMuValHits);
    mytree->SetBranchAddress("Reco_mu_nMuValHits_inner", Reco_mu_nMuValHits_inner);
    mytree->SetBranchAddress("Reco_mu_nMuValHits_bestTracker", Reco_mu_nMuValHits_bestTracker);
    mytree->SetBranchAddress("Reco_mu_nTrkHits", Reco_mu_nTrkHits);
    mytree->SetBranchAddress("Reco_mu_segmentComp", Reco_mu_segmentComp);
    mytree->SetBranchAddress("Reco_mu_kink", Reco_mu_kink);
    mytree->SetBranchAddress("Reco_mu_localChi2", Reco_mu_localChi2);
    mytree->SetBranchAddress("Reco_mu_validFraction", Reco_mu_validFraction);
    mytree->SetBranchAddress("Reco_mu_normChi2_bestTracker", Reco_mu_normChi2_bestTracker);
    mytree->SetBranchAddress("Reco_mu_normChi2_inner", Reco_mu_normChi2_inner);
    mytree->SetBranchAddress("Reco_mu_normChi2_global", Reco_mu_normChi2_global);
    mytree->SetBranchAddress("Reco_mu_nPixWMea", Reco_mu_nPixWMea);
    mytree->SetBranchAddress("Reco_mu_nTrkWMea", Reco_mu_nTrkWMea);
    mytree->SetBranchAddress("Reco_mu_StationsMatched", Reco_mu_StationsMatched);
    mytree->SetBranchAddress("Reco_mu_4mom", &Reco_mu_4mom);

}

Int_t mu_part;
Int_t centralityEvent;
std::vector<int> matching;
std::vector<int> cent;
std::vector<float> mu_pt;
std::vector<float> mu_et;
std::vector<float> mu_phi;
std::vector<bool> Global_muon;
std::vector<bool> Tracker_muon;
std::vector<bool> Medium_muon;
std::vector<bool> Tight_muon;
std::vector<bool> Soft_muon;
std::vector<bool> HybridSoft_muon;
std::vector<bool> Loose_muon;
std::vector<bool> PF_muon;
std::vector<float> norm_chi2;
std::vector<float> local_chi2;
std::vector<float> kink;
std::vector<float> segment_comp;
std::vector<int> n_Valid_hits;
std::vector<int> n_mu_segm;
std::vector<int> Valid_pixel;
std::vector<int> tracker_layers;
std::vector<float> validFraction;
std::vector<bool> TTrack_MuonSegm_matched;
std::vector<int> pixel_layers;
std::vector<bool> TrackHighQual;
std::vector<float> pTEtaReweight;
std::vector<float> NCollReweight;
std::vector<float> Weight;

void write_tree(TTree *tree, bool isMC){
  
    tree->Branch("mu_part", &mu_part, "mu_part/I");
    tree->Branch("centralityEvent", &centralityEvent, "centralityEvent/I");
    tree->Branch("matching", &matching);
    tree->Branch("cent", &cent);
    tree->Branch("mu_pt", &mu_pt);
    tree->Branch("mu_et", &mu_et);
    tree->Branch("mu_phi", &mu_phi);
    tree->Branch("Global_muon", &Global_muon);
    tree->Branch("Tracker_muon", &Tracker_muon);
    tree->Branch("Medium_muon", &Medium_muon);
    tree->Branch("Tight_muon", &Tight_muon);
    tree->Branch("Soft_muon", &Soft_muon);
    tree->Branch("HybridSoft_muon", &HybridSoft_muon);
    tree->Branch("Loose_muon", &Loose_muon);
    tree->Branch("PF_muon", &PF_muon);
    tree->Branch("norm_chi2", &norm_chi2);
    tree->Branch("local_chi2", &local_chi2);
    tree->Branch("kink", &kink);
    tree->Branch("segment_comp", &segment_comp);
    tree->Branch("n_Valid_hits", &n_Valid_hits);
    tree->Branch("n_mu_segm", &n_mu_segm);
    tree->Branch("Valid_pixel", &Valid_pixel);
    tree->Branch("tracker_layers", &tracker_layers);
    tree->Branch("validFraction", &validFraction);
    tree->Branch("TTrack_MuonSegm_matched", &TTrack_MuonSegm_matched);
    tree->Branch("pixel_layers", &pixel_layers);
    tree->Branch("TrackHighQual", &TrackHighQual);
    tree->Branch("pTEtaReweight", &pTEtaReweight);
    tree->Branch("NCollReweight", &NCollReweight);
    tree->Branch("Weight", &Weight);

}



