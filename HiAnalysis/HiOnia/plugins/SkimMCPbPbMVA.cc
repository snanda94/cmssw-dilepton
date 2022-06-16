#include "../interface/SkimMCPbPbMVA.h"

//Function to calculate NColl Weight
float findNcoll(int hiBin) {
const int nbins = 200;
const float Ncoll[nbins] = {1976.95, 1944.02, 1927.29, 1891.9, 1845.3, 1807.2, 1760.45, 1729.18, 1674.8, 1630.3, 1590.52, 1561.72, 1516.1, 1486.5, 1444.68, 1410.88, 1376.4, 1347.32, 1309.71, 1279.98, 1255.31, 1219.89, 1195.13, 1165.96, 1138.92, 1113.37, 1082.26, 1062.42, 1030.6, 1009.96, 980.229, 955.443, 936.501, 915.97, 892.063, 871.289, 847.364, 825.127, 806.584, 789.163, 765.42, 751.187, 733.001, 708.31, 690.972, 677.711, 660.682, 640.431, 623.839, 607.456, 593.307, 576.364, 560.967, 548.909, 530.475, 519.575, 505.105, 490.027, 478.133, 462.372, 451.115, 442.642, 425.76, 416.364, 405.154, 392.688, 380.565, 371.167, 360.28, 348.239, 340.587, 328.746, 320.268, 311.752, 300.742, 292.172, 281.361, 274.249, 267.025, 258.625, 249.931, 240.497, 235.423, 228.63, 219.854, 214.004, 205.425, 199.114, 193.618, 185.644, 180.923, 174.289, 169.641, 161.016, 157.398, 152.151, 147.425, 140.933, 135.924, 132.365, 127.017, 122.127, 117.817, 113.076, 109.055, 105.16, 101.323, 98.098, 95.0548, 90.729, 87.6495, 84.0899, 80.2237, 77.2201, 74.8848, 71.3554, 68.7745, 65.9911, 63.4136, 61.3859, 58.1903, 56.4155, 53.8486, 52.0196, 49.2921, 47.0735, 45.4345, 43.8434, 41.7181, 39.8988, 38.2262, 36.4435, 34.8984, 33.4664, 31.8056, 30.351, 29.2074, 27.6924, 26.7754, 25.4965, 24.2802, 22.9651, 22.0059, 21.0915, 19.9129, 19.1041, 18.1487, 17.3218, 16.5957, 15.5323, 14.8035, 14.2514, 13.3782, 12.8667, 12.2891, 11.61, 11.0026, 10.3747, 9.90294, 9.42648, 8.85324, 8.50121, 7.89834, 7.65197, 7.22768, 6.7755, 6.34855, 5.98336, 5.76555, 5.38056, 5.11024, 4.7748, 4.59117, 4.23247, 4.00814, 3.79607, 3.68702, 3.3767, 3.16309, 2.98282, 2.8095, 2.65875, 2.50561, 2.32516, 2.16357, 2.03235, 1.84061, 1.72628, 1.62305, 1.48916, 1.38784, 1.28366, 1.24693, 1.18552, 1.16085, 1.12596, 1.09298, 1.07402, 1.06105, 1.02954};
return Ncoll[hiBin];
}

//Skim is happening here
void SkimMCPbPbMVA(){

    const Int_t XBINS = 9; // Eta Bins to calculate pTEta weight
    float xEdges[XBINS + 1];
    
    for(int i = 0; i < (XBINS + 1); i++){
        
        xEdges[i] = -2.4+(i*(4.8/9));
        
    }
    const Int_t YBINS = 19; //pT Bins to calculate pTEta weight
    float yEdges[YBINS + 1] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0,17.0,18.0,19.0,20.0};

    TH2* hSignalEtapT = new TH2D("hSignalEtapT", "Signal: pT vs. Eta", XBINS, xEdges, YBINS, yEdges);
    TH2* hBkgEtapT = new TH2D("hBkgEtapT", "Bkg: pT vs. Eta", XBINS, xEdges, YBINS, yEdges);
    TH2* hWeight = new TH2D("hWeight", " Weighting Factor ", XBINS, xEdges, YBINS, yEdges);
    TH2* hBkgEtapTWeighted = new TH2D("hBkgEtapTWeighted", " Weighted Bkg: pT vs. Eta ", XBINS, xEdges, YBINS, yEdges);
    
    TH1* hcentEvent = new TH1D("hcentEvent", "centEvent",200, 0., 200.);
    TH1* hcentEventReweight = new TH1D("hcentEventReweight", "centEventReweight",200, 0., 200.);

    bool isMC = kTRUE;
    TChain * mytree = new TChain("hionia/myTree");

    Int_t NIndex = 2; //Number of files to skim
    Int_t FileIndex[NIndex];
    
    for(int index = 0; index < NIndex; index++){
        
        FileIndex[index] = index + 1;
        mytree->Add(Form("/afs/cern.ch/user/s/shnanda/public/OniaTreeMVAPbPb/Oniatree_MC_miniAOD_%i.root",FileIndex[index]));
        cout << "File Name : " << Form("Oniatree_MC_miniAOD_%i.root",FileIndex[index]) << endl;
        
    }
  
    Long64_t nentries = mytree->GetEntries();
    read_tree(mytree, isMC);
  
    for (Long64_t i = 0; i < nentries; i++) {
    
        mytree->GetEntry(i);

        for (int j = 0; j < Reco_mu_size; j++){
        
            Bool_t recoSelection = false;
            Bool_t barrel_muon = false;
            Bool_t endcap_muon = false;
            Bool_t preSelection = false;
            Bool_t Signal = false;
            Bool_t Background = false;
        
            TLorentzVector * p = (TLorentzVector *) Reco_mu_4mom->At(j);
    
            if((fabs(p->Eta()) < 2.4) && ((Reco_mu_isGlobal[j] == 1) || (Reco_mu_isTracker[j] == 1))) recoSelection = true;
            if((p->Pt() > 3.5) && (fabs(p->Eta()) < 1.2)) barrel_muon = true;
            if((p->Pt() > 1.0) && (fabs(p->Eta()) > 1.2)) endcap_muon = true;
            if(recoSelection && (barrel_muon || endcap_muon) && (p->Pt() < 20.0) && (Reco_mu_normChi2_global[j] < 5000) && (Reco_mu_highPurity[j] == 1)) preSelection = true;
            if(preSelection && (Reco_mu_whichGen[j] >= 0 )) Signal = true;
            if(preSelection && (Reco_mu_whichGen[j] == -1 )) Background = true;
    
            if(Signal){hSignalEtapT->Fill(p->Eta(),p->Pt());}
            if(Background){hBkgEtapT->Fill(p->Eta(),p->Pt());}
       
        }
    }
    
    double weight_pTEta[XBINS][YBINS];
    
    for (int i=0; i < XBINS; i++){
        
        for (int j = 0; j < YBINS; j++){
            
            if (hBkgEtapT->GetBinContent(i+1,j+1) == 0){
                weight_pTEta[i][j] = 1;
            }
            else {
                weight_pTEta[i][j] = (hSignalEtapT->GetBinContent(i+1,j+1))/(hBkgEtapT->GetBinContent(i+1,j+1));
            }
            hWeight->SetBinContent(i+1,j+1,weight_pTEta[i][j]);
        }
    }
    
    
    
    TFile* dataFile = TFile::Open( "SkimMCPbPb.root", "RECREATE" );
    TTree* tree = new TTree("tree", "MVATree");
    write_tree(tree, isMC);
    

    for (Long64_t i = 0; i < nentries; i++) {
        
        mytree->GetEntry(i);
        
        hcentEvent->Fill(Centrality);
        hcentEventReweight->Fill(Centrality,findNcoll(Centrality));
        
        centralityEvent = Centrality; //event centrality
        
        matching.clear();
        cent.clear();
        mu_pt.clear();
        mu_et.clear();
        mu_phi.clear();
        Global_muon.clear();
        Tracker_muon.clear();
        Medium_muon.clear();
        Tight_muon.clear();
        Soft_muon.clear();
        HybridSoft_muon.clear();
        Loose_muon.clear();
        PF_muon.clear();
        norm_chi2.clear();
        local_chi2.clear();
        kink.clear();
        segment_comp.clear();
        n_Valid_hits.clear();
        n_mu_segm.clear();
        Valid_pixel.clear();
        tracker_layers.clear();
        validFraction.clear();
        TTrack_MuonSegm_matched.clear();
        pixel_layers.clear();
        TrackHighQual.clear();
        pTEtaReweight.clear();
        NCollReweight.clear();
        Weight.clear();
        
        int muon_count = 0;
        
        for (int j = 0; j < Reco_mu_size; j++){
       
            Bool_t recoSelection = false;
            Bool_t barrel_muon = false;
            Bool_t endcap_muon = false;
            Bool_t preSelection = false;
            Bool_t Signal = false;
            Bool_t Background = false;
            
            TLorentzVector * p = (TLorentzVector *) Reco_mu_4mom->At(j);
        
            if((fabs(p->Eta()) < 2.4) && ((Reco_mu_isGlobal[j] == 1) || (Reco_mu_isTracker[j] == 1))) recoSelection = true;
            if((p->Pt() > 3.5) && (fabs(p->Eta()) < 1.2)) barrel_muon = true;
            if((p->Pt() > 1.0) && (fabs(p->Eta()) > 1.2)) endcap_muon = true;
            if(recoSelection && (barrel_muon || endcap_muon) && (p->Pt() < 20.0) && (Reco_mu_normChi2_global[j] < 5000) && (Reco_mu_highPurity[j] == 1)) preSelection = true;
            if(preSelection && (Reco_mu_whichGen[j] >= 0 )) Signal = true;
            if(preSelection && (Reco_mu_whichGen[j] == -1 )) Background = true;

       
            if(Signal || Background){
                
                muon_count++;
            
                matching.push_back(Reco_mu_whichGen[j]);
                cent.push_back(Centrality);
                mu_pt.push_back(p->Pt());
                mu_et.push_back(p->Eta());
                mu_phi.push_back(p->Phi());
                Global_muon.push_back(Reco_mu_isGlobal[j]);
                Tracker_muon.push_back(Reco_mu_isTracker[j]);
                Medium_muon.push_back(Reco_mu_isMedium[j]);
                Tight_muon.push_back(Reco_mu_InTightAcc[j]);
                Soft_muon.push_back(Reco_mu_isSoft[j]);
                HybridSoft_muon.push_back(Reco_mu_isHybridSoft[j]);
                Loose_muon.push_back(Reco_mu_InLooseAcc[j]);
                PF_muon.push_back(Reco_mu_isPF[j]);
                norm_chi2.push_back(Reco_mu_normChi2_global[j]);
                local_chi2.push_back(Reco_mu_localChi2[j]);
                kink.push_back(Reco_mu_kink[j]);
                segment_comp.push_back(Reco_mu_segmentComp[j]);
                n_Valid_hits.push_back(Reco_mu_nMuValHits[j]);
                n_mu_segm.push_back(Reco_mu_StationsMatched[j]);
                Valid_pixel.push_back(Reco_mu_nPixValHits[j]);
                tracker_layers.push_back(Reco_mu_nTrkWMea[j]);
                validFraction.push_back(Reco_mu_validFraction[j]);
                TTrack_MuonSegm_matched.push_back(Reco_mu_TMOneStaTight[j]);
                pixel_layers.push_back(Reco_mu_nPixWMea[j]);
                TrackHighQual.push_back(Reco_mu_highPurity[j]);
        
                float pTEtaWeight;
                
                if(Signal){
                    pTEtaWeight = 1.0;
                        
                }
                else if(Background){
                    Int_t binx = hWeight->GetXaxis()->FindBin(p->Eta());
                    Int_t biny = hWeight->GetYaxis()->FindBin(p->Pt());
                    pTEtaWeight = hWeight->GetBinContent(binx,biny);
                    hBkgEtapTWeighted->Fill(p->Eta(),p->Pt(),pTEtaWeight);
                
                }
                
                pTEtaReweight.push_back(pTEtaWeight);
                
                float NCollWeight = findNcoll(Centrality);
                NCollReweight.push_back(NCollWeight);
                
                float totalWeight = pTEtaWeight*NCollWeight;
                Weight.push_back(totalWeight);
            
            }
            
        }
        
        mu_part = muon_count;
        tree->Fill();
        
    }
      
    tree->Write();
    hSignalEtapT->Write();
    hBkgEtapT->Write();
    hWeight->Write();
    hBkgEtapTWeighted->Write();
    hcentEvent->Write();
    hcentEventReweight->Write();
    
    dataFile->Close();
    cout << "created data file: " << dataFile->GetName() << endl;

}
 
