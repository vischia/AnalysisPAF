#ifndef Plot_h
#define Plot_h 1

#include "Histo.h"
#include "Looper.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLatex.h"
#include "TString.h"
#include <iostream>
#include "TResultsTable.h"

//##################################################
//# Configuration
//##################################################
// Control booleans

// Defaul cases and paths
const TString DefaultPlotfolder = "./Plots/";
const TString DefaultLimitFolder = "./Datacards/";
const Float_t DefaultLumi = 35.87; //fb-1


class Plot {
public:
  bool verbose         = false;
  bool doSys           = true;
  bool doData          = true; 
  bool doYieldsInLeg   = true;
  bool doSingleLep     = false;
  bool doStackOverflow = true;
  bool doSignal        = true;
  bool doSetLogy       = true;
  bool doStatUncInDatacard = true;
  bool doLegend        = true;
  bool doUncInLegend   = false;

  std::vector<Histo*> VBkgs;
  std::vector<Histo*> VSignals;
  std::vector<Histo*> VSignalsErr;
  std::vector<Histo*> VData;
  std::vector<Histo*> VSyst;
  std::vector<Histo*> VSumHistoSystUp;
  std::vector<Histo*> VSumHistoSystDown;
  std::vector<TString> VSystLabel;
  std::vector<TString> VTagSamples;
  std::vector<TString> VTagDataSamples;
  std::vector<TString> VTagProcesses;
  std::vector<TString> VTagOptions;
  std::vector<TString> VBinLabels;
  Histo* hData = NULL;
  THStack* hStack = NULL;
  Histo* hAllBkg = NULL;
 // Histo* hSignal = NULL; // For a default signal if needed

  Int_t nBkgs = 0;
  TPad* plot = NULL; TPad* pratio = NULL;
  TLatex* texlumi = NULL;
  TLatex* texcms = NULL;
  TLatex* texPrelim = NULL;
  TLatex* texchan = NULL;
  TString chlabel = "";
  TH1F* hratio = NULL;
  Float_t* TotalSysUp = NULL;
  Float_t* TotalSysDown = NULL;
  TString sys = "0";
  Float_t sys_lumi = 0.025;

  Plot(){
    plotFolder = DefaultPlotfolder; 
    limitFolder = DefaultLimitFolder; 
    Lumi = DefaultLumi;
    nSignalSamples = 0;
  }
  Plot(TString variable, TString cuts = "", TString channel = "ElMu", Int_t nbins = 0, Double_t bin0 = 0, Double_t binN = 0, TString tit = "title", TString xtit = "VAR"){
    var    = variable;
    cut    = (cuts);
    chan   = channel;
    nb     = nbins;
    x0     = bin0;
    xN     = binN;
    title = tit;
    xtitle = xtit;
    varname = variable; if(variable.Contains(" ")) TString(variable(0,variable.First(" ")));
    f = 0;
    Init();
  }

  Plot(TString variable, TString cuts = "", TString channel = "ElMu", Int_t nbins = 0, Float_t* bins = 0, TString tit = "title", TString xtit = "VAR"){
    var    = variable;
    cut    = (cuts);
    chan   = channel;
    nb     = nbins;
    x0     = 0;
    xN     = 0;
    vbins  = bins;
    title = tit;
    xtitle = xtit;
    f = 0;
    varname = variable; if(variable.Contains(" ")) TString(variable(0,variable.First(" ")));
    Init();
  }
	
	virtual ~Plot(){
	  //if(plot) delete plot;
	  //if(pratio) delete pratio;
	  //		if(texlumi) delete texlumi;
	  //		if(texcms) delete texcms;
	  //		if(texchan) delete texchan;
	  VData.clear();
	  VBkgs.clear();
	  VSignals.clear();
	  VSignalsErr.clear();
	  VSyst.clear();
	  VSumHistoSystUp.clear();
	  VSumHistoSystDown.clear();
	  VSystLabel.clear();
	  VTagSamples.clear();
	  VTagDataSamples.clear();
	  VTagProcesses.clear();
	  if(hratio) delete hratio;
	  if(TotalSysUp) delete TotalSysUp;
	  if(TotalSysDown) delete TotalSysDown;
	  if(hData && doData) delete hData;
	  if(hStack) delete hStack;
	  if(hAllBkg) delete hAllBkg;
	  VBinLabels.clear();
	  if(f)       delete f;
	};            // Destructor

  void Init(){
    plotFolder = DefaultPlotfolder;
    limitFolder = DefaultLimitFolder; 
    Lumi = DefaultLumi;
    VBkgs = std::vector<Histo*>();
    VSignals = std::vector<Histo*>();
    VSignalsErr = std::vector<Histo*>();
    VData = std::vector<Histo*>();
    VSyst = std::vector<Histo*>();
    VSumHistoSystUp = std::vector<Histo*>();
    VSumHistoSystDown =  std::vector<Histo*>();
    VSystLabel = std::vector<TString>();
    VTagSamples = std::vector<TString>();
    VTagDataSamples = std::vector<TString>();
    VTagProcesses = std::vector<TString>();
    VTagOptions = std::vector<TString>();
    VBinLabels  = std::vector<TString>();
    hData = NULL;
    hStack = NULL;
    hAllBkg = NULL;
    //hSignal = NULL; 
    plot = NULL; pratio = NULL;
    texlumi = NULL;
    texcms = NULL;
    texPrelim = NULL;
    texchan = NULL;
    hratio = NULL;
    TotalSysUp = NULL;
    TotalSysDown = NULL;
    nSignalSamples = 0;
    fLegX1 = 0.66; 
    fLegY1 = 0.42;
    fLegX2 = 0.98; 
    fLegY2 = 0.92;
    LegendTextSize  = 0.065;
    RatioPlotLabel  = "";
    SignalDrawStyle = "hist";
    CMSlabel = "CMS";
    CMSmodeLabel = "Preliminary";
    RatioErrorColor = kGray+2;
    RatioErrorStyle = 3444;
    StackErrorColor = kGray+2;
    StackErrorStyle = 3444;
    weight = "TWeight";
    systematics = "";
  }


	
	void AddSample(TString p = "TTbar_Powheg", TString pr = "", Int_t type = itBkg, Int_t color = 0, TString tsys = "0", TString options = "");
  void Multiloop(TString p = "TTbar_Powheg", TString pr = "", Int_t type = itBkg, Int_t color = 0, TString sys = "");
  void PrepareHisto(vector<Histo*> vH, TString name, TString pr, Int_t type = itBkg, Int_t color = 0, TString sys = "");
  void PrepareHisto(Histo* h, TString name, TString pr, Int_t type = itBkg, Int_t color = 0, TString sys = "");
  void Group(Histo* h);

	// ######### Methods ########
  Histo* GetH(TString sample = "TTbar_Powheg", TString s = "0", Int_t type = itBkg);
  TCanvas *SetCanvas();
  TLegend* SetLegend();
  void SetLegendPosition(TString);
  void SetLegendPosition(float x1 = 0.70, float y1 = 0.65, float x2 = 0.93, float y2 = 0.93){ fLegX1 = x1; fLegY1 = y1; fLegX2 = x2; fLegY2 = y2;}
  void SetTexChan(TString cuts); // To be updated
  void SetHRatio(TH1F* h = nullptr); // To be updated

  void SetData();
  void GetStack();
  void GetAllBkg();
  void AllBkgSyst();
  
  void SetPlotStyle();

  void SetGoF(TString thegof="chi2");
  
  void DrawStack(TString tag = "", bool sav = 1);
  void DrawComp(TString tag = "", bool sav = 1, bool doNorm = 0, TString options = "");
  void SaveHistograms();
  TString GetStatUncDatacard(Int_t iSignal = 0);
	TString GetShapeUncLines();
  void MakeDatacard(TString tag, Int_t iSignal);
  void MakeAllDatacards(){ for(Int_t i = 0; i < (Int_t) VSignals.size(); i++) MakeDatacard(VSignals.at(i)->GetTag(), i);};

  void MakeDatacardAllBins(TString tag, Int_t iSignal = 0); // Datacard "bin a bin"
  void MakeDatacardBin(Int_t bin, TString tag = "b", Int_t iSignal = 0);

  TString GetVar(){  return var;}
  TString GetChan(){ return chan;}
  TString GetSignal(){ return signal;}
  Float_t GetLumi(){ return Lumi;}
  Float_t GetLumiUnc(){ return sys_lumi;}
  TString GetGoF(){ return gof;}
  void SetVar(TString variable){ var = variable; if(varname == "") varname = variable;}
  void SetVarName(TString variable){ varname = variable;}
  void SetChan(TString ch){chan = ch;}
  void SetLumi(Float_t lum){Lumi = lum;} 
  void SetLumiUnc(Float_t lum){sys_lumi = lum;} 
  void SetPlotFolder(TString f){plotFolder = f;} 
  void SetLimitFolder(TString f){limitFolder = f;}   
	void SetCut(TString cuts){cut = (cuts);}
	void SetBins(Int_t nbins, Double_t bin0, Double_t binN){
		nb = nbins; x0 = bin0; xN = binN;
  }
	void SetTitle(TString tit){title = tit;}
	void SetTitleX(TString xtit){xtitle = xtit;}
  void SetBinLabels(TString t, char separator = ','){VBinLabels = TStringToVector(t, separator);}


  void AddToHistos(Histo* p);
	void AddVarHistos(TString sys);
	void AddSystematic(TString s, TString pr = "");
  void AddStatError(TString process = "");
  void AddToSystematicLabels(TString sys){
    if(sys.Contains(",")){
      vector<TString> v = TStringToVector(sys, ',');
      for(Int_t i = 0; i < (Int_t) v.size(); i++) AddToSystematicLabels(v.at(i));
      return;
    }
    sys.ReplaceAll("Up", ""); sys.ReplaceAll("Down", "");
    for(Int_t i = 0; i < (Int_t) VSystLabel.size(); i++){
      if(VSystLabel.at(i) == sys) return;
    }
    VSystLabel.push_back(sys);
  }
  void GroupSystematics();
  void AddSumHistoSystematicUp(Histo* hsys){hsys->SetStyle(); VSumHistoSystUp.push_back(hsys);}
  void AddSumHistoSystematicDown(Histo* hsys){hsys->SetStyle(); VSumHistoSystDown.push_back(hsys);}
  void IncludeBkgSystematics();

  void SetPath(TString p){ path = p; if(pathSignal == "") pathSignal = path; if(pathData == "") pathData = path;}
  void SetPathToHeppyTrees(TString p){ pathToHeppyTrees = p;}
  void SetPathSignal(TString p){ pathSignal = p; }
  void SetPathData(TString p){ pathData = p; }
  void SetTreeName(TString p){ treeName = p;}
  void SetOutputName(TString p){ outputName = p;}
  void SetYieldsTableName(TString p){ YieldsTableName = p;}
  TString GetOutputName(){ return outputName;}
  TString GetPathToHeppyTrees(){ return pathToHeppyTrees;}

  Histo* GetHistoNormLHE(TString sampleName);

  void PrintSamples();
  void PrintSystematics(); 
  void PrintYields(TString cuts = "", TString labels = "", TString channels = "", TString options = "");
  void PrintBinsYields(TString options = "txt,tex,html");
	void PrintSystYields();
	Float_t GetYield(TString pr = "ttbar", TString systag = "0");
	Float_t GetBinYield(TString pr = "ttbar", Int_t bin = 1, TString systag = "0");
	Histo* GetHisto(TString pr = "ttbar", TString systag = "0");
  void AddNormUnc(TString pr, Float_t systUp = 1, Float_t systDown = - 99);
	void SetLineStyle(TString pr = "ttbar", Int_t s = 0){  Histo* h = GetHisto(pr, "0"); h->SetLineStyle(s);}
	void SetLineColor(TString pr = "ttbar", Int_t s = 0){  Histo* h = GetHisto(pr, "0"); h->SetLineColor(s);}
	void SetMarkerColor(TString pr = "ttbar", Int_t s = 0){  Histo* h = GetHisto(pr, "0"); h->SetMarkerColor(s);}
	void SetMarkerStyle(TString pr = "ttbar", Int_t s = 0){  Histo* h = GetHisto(pr, "0"); h->SetMarkerStyle(s);}
  Histo* GetSymmetricHisto(TString pr = "ttbar", TString systag = "0");
  void AddSymmetricHisto(TString pr, TString systag);
  Float_t GetData();
  Histo* GetHData();
  Float_t GetTotalSystematic(TString pr);
  Int_t GetColorOfProcess(TString pr);
  Plot* NewPlot(TString newVar = "", TString newCut = "", TString newChan = "", Int_t newnbins = -1, Float_t newbin0 = -999, Float_t newbinN = -999, TString newtitle = "", TString newXtitle = "");
  void RemoveSystematic(TString sys);
  void UseEnvelope(TString pr, TString tags, TString newname = "");

  void SetWeight(TString t){ weight = t;}
  void SetSystematics(TString t){ weight = t;}
  void SetRatioMin(Float_t r){ RatioMin = r;}
  void SetRatioMax(Float_t r){ RatioMax = r;}
  void SetScaleMax(Float_t s){ ScaleMax = s;}
  void SetScaleLog(Float_t s){ ScaleLog = s;}
  void SetPlotMinimum(Float_t p){ PlotMinimum = p;}
  void SetPlotMaximum(Float_t p){ PlotMaximum = p;}
  void ScaleProcess(TString process, Float_t factor = 1);
  void ScaleSignal(Float_t factor = 1);
  void ScaleSys(TString processSys, Float_t factor = 1);
  void SetTableFormats(TString t){ tableFormats = t;}
  void SetLoopOptions(TString t){LoopOptions = t;}
  void SetRatioOptions(TString t){RatioOptions = t;}
  void SetSignalDrawStyle(TString t){ SignalDrawStyle = t;}
  void SetLegendTextSize(Float_t t){ LegendTextSize = t;} 
  void SetRatioPlotLabel(TString t){ RatioPlotLabel = t;} 
  void SetRatioErrorColor(Int_t b){ RatioErrorColor = b;}
  void SetRatioErrorStyle(Int_t b){ RatioErrorStyle = b;}
  void SetStackErrorColor(Int_t b){ StackErrorColor = b;}
  void SetStackErrorStyle(Int_t b){ StackErrorStyle = b;}

  void SetSignalProcess(TString p){ SignalProcess = p;}
  void SetSignalStyle(TString p){ SignalStyle = p;} 
  TString GetSignalProcess(){ return SignalProcess;}

  void SetCMSlabel(TString t){ CMSlabel = t;}
  void SetCMSmodeLabel(TString t){ CMSmodeLabel = t;}
  void SetChLabel(TString t){chlabel= t;}

protected: 
  TString pathToHeppyTrees = "";
  TString path = "";
  TString pathSignal = "";
  TString pathData = "";
  TString treeName = "";
  TString outputName = "";
  TString YieldsTableName = "yields";
  TString tableFormats = "%1.2f";
  TString gof = "";
  TString weight;
  TString systematics;
  
  TFile *f;

  Int_t nSignalSamples;
  
  // Maximum and minimum value of the ratio plot
  Float_t RatioMin = 0.8;
  Float_t RatioMax = 1.2;
  
  // Factor to multiply the maximum of the plot to set the maximum
  Float_t ScaleMax = 1.2;
  Float_t ScaleLog = 500;
  Float_t PlotMinimum = -999;
  Float_t PlotMaximum = -999;

  TString varname = "";
	TString var;
  TString chan;
  TString signal;
  TString cut;
  Int_t nb; Double_t x0; Double_t xN; Float_t *vbins;
  TString  title;
  TString xtitle;
 
  TString SignalProcess;
  TString LoopOptions = "";
  TString RatioOptions = "";
  TString SignalStyle = "";
  TString SignalDrawStyle;
  TString CMSlabel;
  TString CMSmodeLabel;
  Int_t  RatioErrorColor;
  Int_t  RatioErrorStyle;
  Int_t  StackErrorColor;
  Int_t  StackErrorStyle;

  TString SystVar;
  Int_t iS;
  Float_t Lumi;
  TString plotFolder;
  TString limitFolder;
  Bool_t ShowSystematics = false;

  float fLegX1;
  float fLegY1;
  float fLegX2;
  float fLegY2;
  Float_t LegendTextSize;
  TString RatioPlotLabel;
  
};

#endif
