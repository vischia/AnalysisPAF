#ifndef LEPTONSF_H
#define LEPTONSF_H 1

#include <iostream>
#include "TFile.h"
#include "TH2F.h"
#include "Lepton.h"
#include "Functions.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include "TSystem.h"

const TString path_to_SF_histos = gSystem->WorkingDirectory() + TString("/InputFiles/");

class LeptonSF {
 public:
  LeptonSF();
  ~LeptonSF() {}
  void loadHisto(Int_t iHisto, Int_t wp = -1);
  Float_t GetTrackerMuonSF(Float_t eta);
	Float_t GetLeptonSF(Float_t pt, Float_t ieta);
	Float_t GetLeptonSFerror(Float_t pt, Float_t ieta);
	TGraphAsymmErrors* LoadTrackerMuonSF(const char* file, const char* histo);
  
  // Trigger SFs
  Float_t GetTrigDoubleMuSF(Float_t eta1, Float_t eta2) const;
  Float_t GetTrigDoubleElSF(Float_t eta1, Float_t eta2) const;
  Float_t GetTrigElMuGSF   (Float_t eta1, Float_t eta2) const;
  // Trigger SF errors
  Float_t GetTrigDoubleMuSF_err(Float_t eta1, Float_t eta2) const;
  Float_t GetTrigDoubleElSF_err(Float_t eta1, Float_t eta2) const;
  Float_t GetTrigElMuSF_err    (Float_t eta1, Float_t eta2) const;
  
	std::vector<Int_t> loadedHistos;

 protected:
  TH2D* GetHistogramFromFileD(const char* file, const char* histo, const char* hname);
  TH2F* GetHistogramFromFileF(const char* file, const char* histo, const char* hname) const;
  
 private:
  // Muon SFs
  TGraphAsymmErrors*  fMuonTrackerSF;    
  TH2D*  fMuonIdSF;     
  TH2D*  fMuonIsoSF;     
  TH2D*  fMuonIP2DSF;     
  TH2D*  fMuonSIP3DSF;     
 
  // Elec SFs
  TH2D*  fElecTrackerSF;
  TH2D*  fElecIdSF;
  TH2D*  fElecIsoSF; 
  TH2D*  fElecIP2DSF;     
  TH2D*  fElecSIP3DSF;     

  // Trigger SFs
  TH2F *fDoubleMuSF;
  TH2F *fDoubleElSF;
  TH2F *fMuEGSF;
};
#endif