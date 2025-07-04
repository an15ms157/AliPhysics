#ifndef AliAnalysisTaskCVEPIDCMEDiff_cxx
#define AliAnalysisTaskCVEPIDCMEDiff_cxx
#include <TGraphErrors.h>
#include <TH3.h>
#include <TProfile2D.h>

#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "AliAODTrack.h"
#include "AliAODv0.h"
#include "AliAnalysisTaskSE.h"
#include "AliEventCuts.h"
#include "AliMultSelection.h"
#include "AliPIDResponse.h"
#include "TList.h"
#include "TProfile.h"
#include "TProfile3D.h"

enum class PairType { kLambdaProton = 0,
                      kLambdaAntiProton,
                      kAntiLambdaProton,
                      kAntiLambdaAntiProton,
                      kNumPairs = 4 };

class AliAnalysisTaskCVEPIDCMEDiff : public AliAnalysisTaskSE {
 public:
  AliAnalysisTaskCVEPIDCMEDiff();
  AliAnalysisTaskCVEPIDCMEDiff(const char* name);
  virtual ~AliAnalysisTaskCVEPIDCMEDiff();

  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t* option);
  virtual void Terminate(Option_t*);

  // Switch
  void IfCalculateLambdaProton(bool b) { this->isCalculateLambdaProton = b; }
  void IfCalculateLambdaHadron(bool b) { this->isCalculateLambdaHadron = b; }
  void IfCalculateLambdaLambda(bool b) { this->isCalculateLambdaLambda = b; }
  void IfCalculateLambdaPion(bool b) { this->isCalculateLambdaPion = b; }
  void IfDoNUA(bool bDoNUA) { this->isDoNUA = bDoNUA; }
  void IfDoLambdaNUA(bool bDoLambdaNUA) { this->isDoLambdaNUA = bDoLambdaNUA; }
  void IfDoNUE(bool bDoNUE) { this->isDoNUE = bDoNUE; }
  void IfDoLambdaNUE(bool bDoLambdaNUE) { this->isDoLambdaNUE = bDoLambdaNUE; }
  void IfRecentreTPC(bool bRecentreTPC) { this->isRecentreTPC = bRecentreTPC; }
  void IfNarrowDcaCuts768(bool bNarrowDcaCuts768) { this->isNarrowDcaCuts768 = bNarrowDcaCuts768; }
  void IfProtonCustomizedDCACut(bool bProtonCustomizedDCACut) { this->isProtonCustomizedDCACut = bProtonCustomizedDCACut; }
  void IfTightPileUp(bool bTightPileUp) { this->isTightPileUp = bTightPileUp; }

  // read in
  void SetListForNUENUA(TList* flist) { this->fListNUENUA = (TList*)flist->Clone(); }
  void SetListForPlaneNUA(TList* flist) { this->fListPlaneNUA = (TList*)flist->Clone(); }
  void SetListForVZEROCalib(TList* flist) { this->fListVZERO = (TList*)flist->Clone(); }

  // Global
  void SetPeriod(TString period) { this->fPeriod = period; }
  // Event
  void SetVzCut(float vzCut) { this->fVzCut = vzCut; }
  // Plane
  void SetPlaneEstimator(TString planeEstimator) { this->fPlaneEstimator = planeEstimator; }
  // Track
  void SetFilterBit(int filterBit) { this->fFilterBit = filterBit; }
  void SetNclsCut(int nclsCut) { this->fNclsCut = nclsCut; }
  void SetChi2Max(float chi2Max) { this->fChi2Max = chi2Max; }
  void SetChi2Min(float chi2Min) { this->fChi2Min = chi2Min; }
  void SetSpecialHadronDCAzMax(float hadronDCAzMax) { this->fSpecialHadronDCAzMax = hadronDCAzMax; }
  // Proton
  void SetSpecialProtonDCAzMax(float protonDCAzMax) { this->fSpecialProtonDCAzMax = protonDCAzMax; }
  // PID
  void SetNSigmaTPC(float nSigmaTPC) { this->fNSigmaTPC = nSigmaTPC; }
  void SetNSigmaRMS(float nSigmaRMS) { this->fNSigmaRMS = nSigmaRMS; }
  // V0
  void SetV0CPAMin(float v0CPAMin) { this->fV0CPAMin = v0CPAMin; }
  void SetV0DecayLengthMax(float v0DecayLengthMax) { this->fV0DecayLengthMax = v0DecayLengthMax; }
  void SetV0DecayLengthMin(float v0DecayLengthMin) { this->fV0DecayLengthMin = v0DecayLengthMin; }
  void SetV0DcaBetweenDaughtersMax(float v0DcaBetweenDaughtersMax) { this->fV0DcaBetweenDaughtersMax = v0DcaBetweenDaughtersMax; }
  // V0 Daughter Cut
  void SetDaughtersTPCNclsMin(float daughtersTPCNclsMin) { this->fDaughtersTPCNclsMin = daughtersTPCNclsMin; }
  void SetDaughtersDCAToPrimVtxMin(float daughtersDCAToPrimVtxMin) { this->fDaughtersDCAToPrimVtxMin = daughtersDCAToPrimVtxMin; }
  void SetRatioCrossedRowsFindable(float ratioCrossedRowsFindable) { this->fRatioCrossedRowsFindable = ratioCrossedRowsFindable; }
  void SetDaughtersNSigmaTPC(float daughtersNSigmaTPC) { this->fDaughtersNSigmaTPC = daughtersNSigmaTPC; }

  // Min Pt Setting
  void SetLambdaMinPt(float lambdaMinPt) { this->fLambdaMinPt = lambdaMinPt; }
  void SetProtonMinPt(float protonMinPt) { this->fProtonMinPt = protonMinPt; }
  void SetHardonMinPt(float hardonMinPt) { this->fHadronMinPt = hardonMinPt; }
  void SetPionMinPt(float pionMinPt) { this->fPionMinPt = pionMinPt; }

 private:
  ////////////////////////
  // Procedural function
  ////////////////////////
  float GetTPCPlane();
  float GetV0CPlane(float centSPD1);
  void ResetVectors();
  bool LoopTracks();
  bool LoopV0s();
  bool PairV0Trk();
  bool PairV0V0();

  ////////////////////////
  // Functional function
  ////////////////////////
  // Read in
  bool LoadCalibHistForThisRun(); // deal with all the readin
  bool LoadNUENUAGraphForThisCent(); // deal with all the readin
  // Pile-up
  bool RejectEvtTFFit(float centSPD0);
  // Track
  bool AcceptAODTrack(AliAODTrack* track);
  bool CheckPIDofParticle(AliAODTrack* ftrack, int pidToCheck);
  float GetPlaneNUECor(int charge, float pt);
  float GetPlaneNUACor(int charge, float phi, float eta, float vz);
  float GetPIDNUECor(int pdgcode, float pt);
  float GetPIDNUACor(int pdgcode, float pt, float phi);
  // V0
  bool IsGoodV0(AliAODv0* aodV0);
  bool IsGoodDaughterTrack(const AliAODTrack* track);
  int GetLambdaCode(const AliAODTrack* pTrack, const AliAODTrack* ntrack);
  // Plane
  float GetTPCPlaneNoAutoCorr(std::vector<int> vec_id);
  inline float GetEventPlane(float qx, float qy, float harmonic);
  // Get DCA
  bool GetDCA(float& dcaxy, float& dcaz, AliAODTrack* track);
  // Sum pT bin, Delta eta bin
  inline float GetSumPtBin(float sumPt);
  inline float GetDeltaEtaBin(float deltaEta);
  inline float GetDCABin(float dca);

  //////////////////////
  // Switch           //
  //////////////////////
  bool fDebug{false};
  bool isCalculateLambdaHadron{false};
  bool isCalculateLambdaPion{false};
  bool isCalculateLambdaProton{false};
  bool isCalculateLambdaLambda{false};

  bool isDoNUE{true};
  bool isDoLambdaNUE{true};
  bool isDoNUA{true};
  bool isDoLambdaNUA{true};
  bool isRecentreTPC{true};
  bool isNarrowDcaCuts768{true};
  bool isProtonCustomizedDCACut{true};
  bool isTightPileUp{false};

  //////////////////////
  // Cuts and options //
  //////////////////////
  // Global
  TString fTrigger{"kINT7+kSemiCentral"}; //
  TString fPeriod{"LHC18q"};              // period
  float fLambdaMinPt{-1.f};
  float fProtonMinPt{-1.f};
  float fHadronMinPt{-1.f};
  float fPionMinPt{-1.f};
  // Event
  float fVzCut{10}; // vz cut
  // Plane
  TString fPlaneEstimator{"TPC"}; // TPC or V0C
  // Track
  int fFilterBit{768}; // AOD filter bit selection
  int fNclsCut{70};    // ncls cut for all tracks
  float fChi2Max{2.5}; // upper limmit for chi2
  float fChi2Min{0.1}; // lower limmit for chi2
  // Hadron
  float fSpecialHadronDCAzMax{-1}; // upper limit for track DCAz
  // Proton
  float fSpecialProtonDCAzMax{-1}; // upper limit for proton DCAz
  // PID
  float fNSigmaTPC{3.f};
  float fNSigmaRMS{3.f};
  // V0
  float fV0CPAMin{0.997};                //
  float fV0DecayLengthMin{3.f};          //
  float fV0DecayLengthMax{100.f};        //
  float fV0DcaBetweenDaughtersMax{0.5f}; //

  // V0 Daughter
  float fDaughtersTPCNclsMin{70.f};      //
  float fDaughtersDCAToPrimVtxMin{0.05}; //
  float fRatioCrossedRowsFindable{0.8};  //
  float fDaughtersNSigmaTPC{3.f};        //

  ///////////////////The following files are from the data//////////////////////////////////
  /////////////
  // Handles //
  /////////////
  AliAODEvent* fAOD{nullptr};            //!<! aod Event
  AliPIDResponse* fPIDResponse{nullptr}; //!<! PID Handler
  AliMultSelection* fMultSel{nullptr};   //!<!

  ////////////////////////////////
  // Global Variables from data //
  ////////////////////////////////
  std::array<double, 3> fVertex{0.f, 0.f, 0.f};
  int fRunNum{-1};     // runnumber
  int fOldRunNum{-2};  // latest runnumber
  int fRunNumBin{-1};  // runnumer bin; 10:139510...; 11:170387...; 15HIR:246994...
  int fVzBin{-1};      // vertex z bin
  float fCent{0.f};    // centrality
  int fCentBin{-1};    // centrality bin: 0-7
  int fOldCentBin{-2}; // latest centrality bin: 0-7
  // Variable to get TPC Plane
  float fSumQ2x{0.f};
  float fSumQ2y{0.f};
  float fWgtMult{0.f};
  // Plane
  float fPsi2{-1};

  // Plane tracks Map key:id value:(phi,weight)
  std::unordered_map<int, std::pair<float,float>> mapTPCTrksIDPhiWgt{};

  // Vector for particles from Tracks [pt,eta,phi,id,pdgcode,pidweight]
  std::vector<std::array<float, 6>> vecParticle{};
  // Vector for V0s [pt,eta,phi,id,pdgcode,pidweight,mass,id1,id2]
  std::vector<std::array<float, 9>> vecParticleV0{};

  ///////////////////The following files are read from external sources////////////////////
  ////////////////////////
  // Pile up Function
  ////////////////////////
  std::unique_ptr<TF1> fSPDCutPU{nullptr};     //!<!
  std::unique_ptr<TF1> fV0CutPU{nullptr};      //!<!
  std::unique_ptr<TF1> fCenCutLowPU{nullptr};  //!<!
  std::unique_ptr<TF1> fCenCutHighPU{nullptr}; //!<!
  std::unique_ptr<TF1> fMultCutPU{nullptr};    //!<!

  ////////////////////////
  // NUE NUA
  ////////////////////////
  TList* fListNUENUA{nullptr}; //!<! read list for NUE
  // Hadron
  TGraph* gNUEPosHadron_thisCent{nullptr}; //!<!
  TGraph* gNUENegHadron_thisCent{nullptr}; //!<!
  // Proton
  TGraph* gNUEProton_thisCent{nullptr};     //!<!
  TGraph* gNUEAntiProton_thisCent{nullptr}; //!<!
  // Lambda
  TGraph* gNUELambda_thisCent{nullptr};     //!<!
  TGraph* gNUEAntiLambda_thisCent{nullptr}; //!<!

  // Hadron
  TH2F* h2NUAPosHadron_thisCent{nullptr}; //!<!
  TH2F* h2NUANegHadron_thisCent{nullptr}; //!<!
  // Proton
  TH2F* h2NUAProton_thisCent{nullptr};     //!<!
  TH2F* h2NUAAntiProton_thisCent{nullptr}; //!<!
  // Lambda
  TH2F* h2NUALambda_thisCent{nullptr};     //!<!
  TH2F* h2NUAAntiLambda_thisCent{nullptr}; //!<!

  ////////////////////////
  // Plane NUA
  ////////////////////////
  TList* fListPlaneNUA{nullptr}; //!<! read lists for Plane NUA
  TH3F* hCorrectNUAPos{nullptr}; //!<!
  TH3F* hCorrectNUANeg{nullptr}; //!<!

  ////////////////////////
  /// TPC
  ////////////////////////
  TList* fListTPC{nullptr}; //!<!
  TProfile3D* fQxTPCRunCentVz{nullptr}; //!<!
  TProfile3D* fQyTPCRunCentVz{nullptr}; //!<!
  float fQxMeanTPC{0.f};
  float fQyMeanTPC{0.f};

  ////////////////////////
  // VZERO
  ////////////////////////
  TList* fListVZERO{nullptr};           //!<! read list for V0 Calib
  AliOADBContainer* contQxncm{nullptr}; //!<! for recenter
  AliOADBContainer* contQyncm{nullptr}; //!<! for recenter
  TH1D* hQx2mV0C{nullptr};              //!<!
  TH1D* hQy2mV0C{nullptr};              //!<!
  TH2F* fHCorrectV0ChWeghts{nullptr};   //!<! for gain equalization

  ///////////////////The following files will be saved//////////////////////////////////
  //////////////
  // QA Plots //
  //////////////

  TList* fQAList{nullptr}; //!<!
  // General QA
  // Event-wise
  TH1D* fEvtCount{nullptr};                       //!<!
  std::map<int, int>* runNumList{nullptr};        //!<!
  TH1I* fHistRunNumBin{nullptr};                  //!<!
  std::array<TH1D*, 2> fHistCent{nullptr};        //!<!
  std::array<TH1D*, 2> fHistVz{nullptr};          //!<!
  std::array<TH2D*, 6> fHist2CentQA{nullptr};     //!<!
  std::array<TH2D*, 2> fHist2MultCentQA{nullptr}; //!<!
  std::array<TH2D*, 6> fHist2MultMultQA{nullptr}; //!<!
  // Track-wise
  TH1D* fHistPt{nullptr};                 //!<!
  TH1D* fHistEta{nullptr};                //!<!
  TH1D* fHistNhits{nullptr};              //!<!
  TH2D* fHist2PDedx{nullptr};             //!<!
  TH1D* fHistDcaXY{nullptr};              //!<!
  TH1D* fHistDcaZ{nullptr};               //!<!
  std::array<TH2D*, 2> fHistPhi{nullptr}; //!<!

  // Proton QA
  TH1D* fHistProtonPt{nullptr};          //!<!
  TH1D* fHistProtonEta{nullptr};         //!<!
  std::array<TH2D*, 2> fHistProtonPhi{nullptr};         //!<!
  TH2D* fHistProtonPtDcaXY{nullptr};     //!<!
  TH2D* fHistProtonPtDcaZ{nullptr};      //!<!
  TH1D* fHistAntiProtonPt{nullptr};      //!<!
  TH1D* fHistAntiProtonEta{nullptr};     //!<!
  std::array<TH2D*, 2> fHistAntiProtonPhi{nullptr};     //!<!
  TH2D* fHistAntiProtonPtDcaXY{nullptr}; //!<!
  TH2D* fHistAntiProtonPtDcaZ{nullptr};  //!<!

  // V0s QA
  TH1D* fHistV0Pt{nullptr};              //!<! Raw V0s' pT
  TH1D* fHistV0Eta{nullptr};             //!<! Raw V0s' eta
  TH1D* fHistV0DcatoPrimVertex{nullptr}; //!<! Raw V0s' DcatoPV
  TH1D* fHistV0CPA{nullptr};             //!<! Raw V0s' CPA(cosine pointing angle)
  TH1D* fHistV0DecayLength{nullptr};     //!<! Raw V0s' DecayLength
  TH1D* fHistV0NegDaughterDca{nullptr};  //!<! Raw V0s' NegDaughterDca
  TH1D* fHistV0PosDaughterDca{nullptr};  //!<! Raw V0s' PosDaughterDca
  // Lambda QA
  //[0]:Before the Mass Cut [1]:After the Mass Cut
  TH1D* fHistLambdaPt{nullptr};                      //!<!
  TH1D* fHistLambdaEta{nullptr};                     //!<!
  std::array<TH2D*, 2> fHistLambdaPhi{nullptr};      //!<!
  TH1D* fHistLambdaDcaToPrimVertex{nullptr};         //!<!
  TH1D* fHistLambdaNegDaughterDca{nullptr};          //!<!
  TH1D* fHistLambdaPosDaughterDca{nullptr};          //!<!
  TH1D* fHistLambdaCPA{nullptr};                     //!<!
  TH1D* fHistLambdaDecayLength{nullptr};             //!<!
  TH3D* fHist3LambdaCentPtMass{nullptr};             //!<!
  TH3D* fHist3LambdaCentPtMassWeighted{nullptr};     //!<!
  TH2D* fHist2LambdaMassPtY{nullptr};                //!<!
  TH1D* fHistAntiLambdaPt{nullptr};                  //!<!
  TH1D* fHistAntiLambdaEta{nullptr};                 //!<!
  std::array<TH2D*, 2> fHistAntiLambdaPhi{nullptr};  //!<!
  TH1D* fHistAntiLambdaDcaToPrimVertex{nullptr};     //!<!
  TH1D* fHistAntiLambdaNegDaughterDca{nullptr};      //!<!
  TH1D* fHistAntiLambdaPosDaughterDca{nullptr};      //!<!
  TH1D* fHistAntiLambdaCPA{nullptr};                 //!<!
  TH1D* fHistAntiLambdaDecayLength{nullptr};         //!<!
  TH3D* fHist3AntiLambdaCentPtMass{nullptr};         //!<!
  TH3D* fHist3AntiLambdaCentPtMassWeighted{nullptr}; //!<!
  TH2D* fHist2AntiLambdaMassPtY{nullptr};            //!<!

  std::array<TProfile2D*, 2> fProfile2DQxCentVz{nullptr}; //!<!
  std::array<TProfile2D*, 2> fProfile2DQyCentVz{nullptr}; //!<!

  /////////////
  // Results //
  /////////////

  TList* fResultsList{nullptr}; //!<!
  // Plane
  TH2D* fHist2Psi2{nullptr}; //!<!

  // Lambda - Proton
  // Inv Mass
  std::array<TH3D*, 4> fHist3LambdaProtonMassIntg{nullptr}; //!<!  [0]:Λ-p  [1]:Λ-pbar [2]:Λbar-p  [3]:Λbar-pbar
  std::array<TH3D*, 4> fHist3LambdaProtonMassSPt{nullptr};  //!<!  [0]:Λ-p  [1]:Λ-pbar [2]:Λbar-p  [3]:Λbar-pbar
  std::array<TH3D*, 4> fHist3LambdaProtonMassDEta{nullptr}; //!<!  [0]:Λ-p  [1]:Λ-pbar [2]:Λbar-p  [3]:Λbar-pbar
  // Diff δ
  std::array<TProfile3D*, 4> fProfile3DDeltaLambdaProtonMassIntg{nullptr}; //!<!  [0]:Λ-p  [1]:Λ-pbar [2]:Λbar-p  [3]:Λbar-pbar
  std::array<TProfile3D*, 4> fProfile3DDeltaLambdaProtonMassSPt{nullptr};  //!<!  [0]:Λ-p  [1]:Λ-pbar [2]:Λbar-p  [3]:Λbar-pbar
  std::array<TProfile3D*, 4> fProfile3DDeltaLambdaProtonMassDEta{nullptr}; //!<!  [0]:Λ-p  [1]:Λ-pbar [2]:Λbar-p  [3]:Λbar-pbar
  // Diff γ
  std::array<TProfile3D*, 4> fProfile3DGammaLambdaProtonMassIntg{nullptr}; //!<!  [0]:Λ-p  [1]:Λ-pbar [2]:Λbar-p  [3]:Λbar-pbar
  std::array<TProfile3D*, 4> fProfile3DGammaLambdaProtonMassSPt{nullptr};  //!<!  [0]:Λ-p  [1]:Λ-pbar [2]:Λbar-p  [3]:Λbar-pbar
  std::array<TProfile3D*, 4> fProfile3DGammaLambdaProtonMassDEta{nullptr}; //!<!  [0]:Λ-p  [1]:Λ-pbar [2]:Λbar-p  [3]:Λbar-pbar

  // Lambda - Hadron
  // Inv Mass
  std::array<TH3D*, 4> fHist3LambdaHadronMassIntg{nullptr}; //!<!  [0]:Λ-h+ [1]:Λ-h-   [2]:Λbar-h+ [3]:Λbar-h-
  // Diff δ
  std::array<TProfile3D*, 4> fProfile3DDeltaLambdaHadronMassIntg{nullptr}; //!<!  [0]:Λ-h+ [1]:Λ-h-   [2]:Λbar-h+ [3]:Λbar-h-
  // Diff γ
  std::array<TProfile3D*, 4> fProfile3DGammaLambdaHadronMassIntg{nullptr}; //!<!  [0]:Λ-h+ [1]:Λ-h-   [2]:Λbar-h+ [3]:Λbar-h-

  // Lambda - Pion
  std::array<TH3D*, 4> fHist3LambdaPionMassIntg{nullptr}; //!<!
  // Diff δ
  std::array<TProfile3D*, 4> fProfile3DDeltaLambdaPionMassIntg{nullptr}; //!<!  [0]:Λ-h+ [1]:Λ-h-   [2]:Λbar-h+ [3]:Λbar-h-
  // Diff γ
  std::array<TProfile3D*, 4> fProfile3DGammaLambdaPionMassIntg{nullptr}; //!<!  [0]:Λ-h+ [1]:Λ-h-   [2]:Λbar-h+ [3]:Λbar-h-

  // Lambda - Lambda
  std::array<TH3D*, 4> fHist3LambdaLambdaMassMass{nullptr}; //!<!
  // Diff δ
  std::array<TProfile3D*, 4> fProfile3DDeltaLambdaLambdaMassMass{nullptr}; //!<!  [0]:Λ-Λ [1]:Λ-Λbar   [2]:Λbar-Λ [3]:Λbar-Λbar
  // Diff γ
  std::array<TProfile3D*, 4> fProfile3DGammaLambdaLambdaMassMass{nullptr}; //!<!  [0]:Λ-Λ [1]:Λ-Λbar   [2]:Λbar-Λ [3]:Λbar-Λbar

  AliAnalysisTaskCVEPIDCMEDiff(const AliAnalysisTaskCVEPIDCMEDiff&);
  AliAnalysisTaskCVEPIDCMEDiff& operator=(const AliAnalysisTaskCVEPIDCMEDiff&);

  ClassDef(AliAnalysisTaskCVEPIDCMEDiff, 6);
};

#endif
