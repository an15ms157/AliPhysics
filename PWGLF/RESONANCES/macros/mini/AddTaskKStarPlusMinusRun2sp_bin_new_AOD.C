/***************************************************************************
 //            Modified by Kishora Nayak - 14/06/2016
 //            Modified by Enrico Fragiacomo - 15/01/2014
 //            Modified by Kunal Garg - 13/05/2018 (kgarg@cern.ch)
               Modified for Spherocity analysis by Suman Deb - 03-12-2019 (suman.deb@cern.ch)
	       //             Modified for Spherocity analysis AOD format by Suman Deb - 29-09-2021 (suman.deb@cern.ch)
	       //            Based on AddAnalysisTaskRsnMini
	       //            pPb specific settings from AddTaskKStarPPB.C
	       //
	       // Macro to configure the KStarPlusMinus analysis task
	       // It calls all configs desired by the user, by means
	       // of the boolean switches defined in the first lines.
	       // ---
	       // Inputs:
	       //  1) flag to know if running on MC or data
	       //  2) collision system, whether pp, pPb or PbPb
	       // --
	       // Returns:
	       //  kTRUE  --> initialization successful
	       //  kFALSE --> initialization failed (some config gave errors)
	       //
	       ****************************************************************************/

//enum ERsnCollType_t { kPP=0,      kPPb,      kPbPb};

#ifdef __CLING__
R__ADD_INCLUDE_PATH($ALICE_PHYSICS)
#include <PWGLF/RESONANCES/macros/mini/ConfigKStarPlusMinusRun2sp_bin_new_AOD.C>
#endif

enum pairYCutSet { kPairDefault,    // USED ONLY FOR pA
		   kNegative,       // USED ONLY FOR pA
		   kCentral         // USED ONLY FOR pA
};

enum eventCutSet { kEvtDefault=0,
		   kNoPileUpCut, //=1
		   kDefaultVtx12,//=2
		   kDefaultVtx8, //=3
		   kDefaultVtx5, //=4
		   kMCEvtDefault, //=5
		   kSpecial1, //=6
		   kSpecial2, //=7
		   kNoEvtSel, //=8
		   kSpecial3, //=9
		   kSpecial4, //=10
		   kSpecial5, //=11
		   kSpecial6 //=12 (only for multiplicity analyses)    //Suman
};

enum eventMixConfig { kDisabled = -1,
		      kMixDefault,     //=0 //10 events, Dvz = 1cm, DC = 10
		      k5Evts,          //=1 //5 events, Dvz = 1cm, DC = 10
		      k5Cent,          //=2 //10 events, Dvz = 1cm, DC = 5
};


AliRsnMiniAnalysisTask *AddTaskKStarPlusMinusRun2sp_bin_new_AOD
(
 Int_t       binning_sphero=100, ///change [03-12-2019]
 Double_t    min_sphero=0.0,
 Double_t    max_sphero=1.0,
 Int_t       mult_binning=11,
 Double_t    mult_min=0.0,
 Double_t    mult_max=11.0,
 Bool_t      HIST = kFALSE,
 Bool_t      isMC =kFALSE, 
 Bool_t      isPP = kTRUE,  
 Bool_t      Sphero = kTRUE,
 Float_t     cutV = 10.0,
 Bool_t      isGT = 0,
 Int_t       aodFilterBit = 5,//0====ESD,
 Bool_t      enableMonitor=kTRUE,
 TString     monitorOpt="pp",
 Float_t     piPIDCut = 3.0,
 Int_t       evtCutSetID = 0,
 Int_t       customQualityCutsID=1,
 AliRsnCutSetDaughterParticle::ERsnDaughterCutSet cutPiCandidate = AliRsnCutSetDaughterParticle::kTPCpidphipp2015,
 Float_t     pi_k0s_PIDCut = 5.0,
 Float_t     massTolVeto = 0.0043,
 Int_t       tol_switch = 1,
 Double_t    tol_sigma = 6,
 Float_t     pLife = 20,
 Float_t     radiuslow = 0.5,
 Bool_t      Switch = kTRUE,
 Float_t     k0sDCA = 1000.0,
 Float_t     k0sCosPoinAn = 0.97,
 Float_t     k0sDaughDCA = 1.0,
 Int_t       NTPCcluster = 70,
 Float_t     maxDiffVzMix = 1.0,
 Float_t     maxDiffMultMix = 5.0,
 Float_t     maxDiffAngleMixDeg = 20.0,
 Int_t       aodN = 68,
 TString     outNameSuffix ="KStarPlusMinus_V0Mass_Pt",
 Float_t     DCAxy = 0.06,
 Bool_t      enableSys = kFALSE,
 Float_t     crossedRows = 70,
 Float_t     rowsbycluster = 0.8,
 Float_t     v0rapidity= 0.8,
 Int_t       Sys= 0
 )
{
  //-------------------------------------------
  // event cuts
  //-------------------------------------------
  Float_t     massTol = 0.03;
  //Float_t     massTolVeto = 0.0043;
  //Int_t       evtCutSetID = 0;
  Int_t       pairCutSetID = 0;
  Int_t       mixingConfigID = 0;  
  UInt_t      triggerMask=AliVEvent::kINT7;
  Bool_t      rejectPileUp=kTRUE;
  Double_t    vtxZcut=10.0;//cm, default cut on vtx z
  Int_t       MultBins=aodFilterBit/100;
  //   cout<<"EVENTCUTID is    "<<evtCutSetID<<endl;
  if(evtCutSetID==eventCutSet::kDefaultVtx12) vtxZcut=12.0; //cm
  if(evtCutSetID==eventCutSet::kDefaultVtx8) vtxZcut=8.0; //cm
  if(evtCutSetID==eventCutSet::kDefaultVtx5) vtxZcut=5.0; //cm
  if(evtCutSetID==eventCutSet::kNoPileUpCut) rejectPileUp=kFALSE;
  if(evtCutSetID==eventCutSet::kSpecial2) vtxZcut=1.e6;//off        //suman  
    
    
  // if(isMC) rejectPileUp=kFALSE;
  if(!isPP || isMC || MultBins) rejectPileUp=kFALSE; //suman
    
  //-------------------------------------------
  //mixing settings
  //-------------------------------------------
    
  Int_t       nmix = 10;
  if (mixingConfigID == eventMixConfig::kMixDefault) nmix = 10;
  if (mixingConfigID == eventMixConfig::k5Evts)      nmix = 5;
  if (mixingConfigID == eventMixConfig::k5Cent)      maxDiffMultMix = 5;
    
  //
  // -- INITIALIZATION ----------------------------------------------------------------------------
  // retrieve analysis manager
  //
    
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddTaskKStarPlusMinus", "No analysis manager to connect to.");
    return NULL;
  }
    
  // create the task and configure
  TString taskName = Form("KStarPlusMinus%s%s", (isPP? "pp" : "PbPb"), (isMC ? "MC" : "Data"));
    
  AliRsnMiniAnalysisTask* task = new AliRsnMiniAnalysisTask(taskName.Data(),isMC);
    
  //suman  //  if(evtCutSetID==eventCutSet::kSpecial4 || evtCutSetID==eventCutSet::kSpecial5) task->UseESDTriggerMask(triggerMask); //ESD ****** check this *****
  if (Sphero)
    {
      if(evtCutSetID!=eventCutSet::kNoEvtSel && evtCutSetID!=eventCutSet::kSpecial3 && evtCutSetID!=eventCutSet::kSpecial4  && evtCutSetID!=eventCutSet::kSpecial6) task->SelectCollisionCandidates(triggerMask); //AOD----SUMAN            
    }
  else
    {
      if(evtCutSetID!=eventCutSet::kNoEvtSel && evtCutSetID!=eventCutSet::kSpecial3 && evtCutSetID!=eventCutSet::kSpecial4 && evtCutSetID!=eventCutSet::kSpecial6)task->UseESDTriggerMask(triggerMask); //ESD suman 

    }
  if(isPP){
    if(MultBins==1) task->UseMultiplicity("AliMultSelection_V0M");
    else if(MultBins==2) task->UseMultiplicity("AliMultSelection_RefMult08");
    else task->UseMultiplicity("QUALITY");
  }else task->UseCentrality("V0M");
    
    
  // set event mixing options
  task->UseContinuousMix();
  //task->UseBinnedMix();
  task->SetNMix(nmix);
  task->SetMaxDiffVz(maxDiffVzMix);
  task->SetMaxDiffMult(maxDiffMultMix);
  //::Info("AddAnalysisTaskTOFKStar", Form("Event mixing configuration: \n events to mix = %i \n max diff. vtxZ = cm %5.3f \n max diff multi = %\5.3f", nmix, maxDiffVzMix, maxDiffMultMix));//=======SD ========03-NOV-21
    
  mgr->AddTask(task);
    
  //
  // -- EVENT CUTS (same for all configs) ---------------------------------------------------------
  //
  // cut on primary vertex:
  // - 2nd argument --> |Vz| range
  // - 3rd argument --> minimum required number of contributors
  // - 4th argument --> tells if TPC stand-alone vertexes must be accepted
    
  AliRsnCutPrimaryVertex *cutVertex = 0;
  if(evtCutSetID!=eventCutSet::kSpecial1 && evtCutSetID!=eventCutSet::kNoEvtSel && (!MultBins || fabs(vtxZcut-10.)>1.e-10)){
    cutVertex=new AliRsnCutPrimaryVertex("cutVertex",vtxZcut,0,kFALSE);
    if(!MultBins && evtCutSetID!=eventCutSet::kSpecial3){
      cutVertex->SetCheckZResolutionSPD();
      cutVertex->SetCheckDispersionSPD();
      cutVertex->SetCheckZDifferenceSPDTrack();
    }
    if(evtCutSetID==eventCutSet::kSpecial3 || evtCutSetID==eventCutSet::kSpecial6) cutVertex->SetCheckGeneratedVertexZ(); //suman
  }
     
  AliRsnCutEventUtils* cutEventUtils=0;
  if(evtCutSetID!=eventCutSet::kNoEvtSel && evtCutSetID!=eventCutSet::kSpecial3){
    cutEventUtils=new AliRsnCutEventUtils("cutEventUtils",kTRUE,rejectPileUp);
    if(!MultBins){
      cutEventUtils->SetCheckIncompleteDAQ();
      cutEventUtils->SetCheckSPDClusterVsTrackletBG();
      //cutEventUtils->SetCheckInelGt0MC(kFALSE);
    }else{
      //cutEventUtils->SetCheckInelGt0SPDtracklets();
      cutEventUtils->SetRemovePileUppA2013(kFALSE);
      // cutEventUtils->SetCheckAcceptedMultSelection();
      if (evtCutSetID!=eventCutSet::kSpecial6) cutEventUtils->SetCheckAcceptedMultSelection();  //suman
    }
  }

  //////////Suman/////////// change [03-12-2019

  if(isPP && (!isMC) && cutVertex){ 
    cutVertex->SetCheckPileUp(rejectPileUp);// set the check for pileup  
    // ::Info("AddTaskKStarPlusMinusRun2sp_bin_new_AOD", Form(":::::::::::::::::: Pile-up rejection mode: %s", (rejectPileUp)?"ON":"OFF"));//=======SD ========03-NOV-21
  }

  ///////////////////////////////////
    
  //    if(!isMC){ //assume pp data
  //      cutVertex->SetCheckPileUp(rejectPileUp);// set the check for pileup
  //      ::Info("AddAnalysisTaskTOFKStar", Form(":::::::::::::::::: Pile-up rejection mode: %s", (rejectPileUp)?"ON":"OFF"));
  //  }
    
    
  // define and fill cut set for event cut
  AliRsnCutSet* eventCuts=0;
  if(cutEventUtils || cutVertex){
    eventCuts=new AliRsnCutSet("eventCuts",AliRsnTarget::kEvent);
        
    if(cutEventUtils && cutVertex){
      eventCuts->AddCut(cutEventUtils);
      eventCuts->AddCut(cutVertex);
      eventCuts->SetCutScheme(Form("%s&%s",cutEventUtils->GetName(),cutVertex->GetName()));
    }else if(cutEventUtils && !cutVertex){
      eventCuts->AddCut(cutEventUtils);
      eventCuts->SetCutScheme(Form("%s",cutEventUtils->GetName()));
    }else if(!cutEventUtils && cutVertex){
      eventCuts->AddCut(cutVertex);
      eventCuts->SetCutScheme(Form("%s",cutVertex->GetName()));
    }
        
    task->SetEventCuts(eventCuts);
  }
    
    
  // -- EVENT-ONLY COMPUTATIONS -------------------------------------------------------------------
  //vertex
  Int_t vtxID=task->CreateValue(AliRsnMiniValue::kVz,kFALSE);
  AliRsnMiniOutput* outVtx=task->CreateOutput("eventVtx","HIST","EVENT");
  outVtx->AddAxis(vtxID,240,-12.0,12.0);
    
  //multiplicity
  Int_t multID=task->CreateValue(AliRsnMiniValue::kMult,kFALSE);
  AliRsnMiniOutput* outMult=task->CreateOutput("eventMult","HIST","EVENT");

  /*//if(isPP)
    outMult->AddAxis(multID,400,0.5,400.5);
    //else outMult->AddAxis(multID,100,0.,100.);*/
  if(isPP && !MultBins) outMult->AddAxis(multID,400,0.5,400.5);//change-SD-[03-12-2019]
  else outMult->AddAxis(multID,110,0.,110.);
    
  // TH2F* hvz=new TH2F("hVzVsCent","",100,0.,100., 240,-12.0,12.0);
  TH2F* hvz=new TH2F("hVzVsCent","",110,0.,110., 240,-12.0,12.0);//change-SD-[03-12-2019]
  task->SetEventQAHist("vz",hvz);//plugs this histogram into the fHAEventVz data member
    
  //TH2F* hmc=new TH2F("MultiVsCent","", 100,0.,100., 400,0.5,400.5);
  TH2F* hmc=new TH2F("MultiVsCent","", 110,0.,110., 400,0.5,400.5);//change-SD-[03-12-2019]
  hmc->GetYaxis()->SetTitle("QUALITY");
  task->SetEventQAHist("multicent",hmc);//plugs this histogram into the fHAEventMultiCent data member

  //============================suman  //Spherocity QA===================================
  //  if (Sphero) AliRsnMiniAnalysisTask::SetComputeSpherocity();

  //TH2F* hsp=new TH2F("hSpherocityVsCent","",110,0.,110., 1000.,0.,1.);
  TH2F* hsp=new TH2F("hSpherocityVsCent","",110,0.,110., 500.,0.,1.0);//change-SD-[03-12-2019]
  task->SetEventQAHist("spherocitycent",hsp);//plugs this histogram into the fHASpherocityCent data member
    
  // -- PAIR CUTS (common to all resonances) ------------------------------------------------------
  Double_t    minYlab =  -0.5;
  Double_t    maxYlab =  0.5;
    
  AliRsnCutMiniPair* cutY=new AliRsnCutMiniPair("cutRapidity", AliRsnCutMiniPair::kRapidityRange);
  cutY->SetRangeD(-0.5,0.5);
    
  AliRsnCutMiniPair* cutV0=new AliRsnCutMiniPair("cutV0", AliRsnCutMiniPair::kContainsV0Daughter);
    
  AliRsnCutSet* PairCutsSame=new AliRsnCutSet("PairCutsSame",AliRsnTarget::kMother);
  PairCutsSame->AddCut(cutY);
  PairCutsSame->AddCut(cutV0);
  PairCutsSame->SetCutScheme(TString::Format("%s&(!%s)",cutY->GetName(),cutV0->GetName()).Data());
  //note the use of the ! operator in this cut scheme
    
  AliRsnCutSet* PairCutsMix=new AliRsnCutSet("PairCutsMix",AliRsnTarget::kMother);
  PairCutsMix->AddCut(cutY);
  PairCutsMix->SetCutScheme(cutY->GetName());
  //
  // -- CONFIG ANALYSIS --------------------------------------------------------------------------
  
#if !defined (__CINT__) || defined (__CLING__)
  if (!ConfigKStarPlusMinusRun2sp_bin_new_AOD(task,binning_sphero,min_sphero,max_sphero,mult_binning,mult_min,mult_max,HIST,isPP, isMC, isGT, piPIDCut,customQualityCutsID, cutPiCandidate, pi_k0s_PIDCut, aodFilterBit, enableMonitor, monitorOpt.Data(), massTol, massTolVeto, tol_switch, tol_sigma, pLife, radiuslow, Switch, k0sDCA, k0sCosPoinAn, k0sDaughDCA, NTPCcluster, "", PairCutsSame, PairCutsMix, DCAxy, enableSys, crossedRows, rowsbycluster, v0rapidity, Sys)) return 0x0;

#else
  
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGLF/RESONANCES/macros/mini/ConfigKStarPlusMinusRun2sp_bin_new_AOD.C");
    
  if (isMC) {
    Printf("========================== MC analysis - PID cuts not used");
  } else
    Printf("========================== DATA analysis - PID cuts used");
   
  if (!ConfigKStarPlusMinusRun2sp_bin_new_AOD(task,binning_sphero,min_sphero,max_sphero,mult_binning,mult_min,mult_max,HIST,isPP, isMC, isGT, piPIDCut,customQualityCutsID, cutPiCandidate, pi_k0s_PIDCut, aodFilterBit, enableMonitor, monitorOpt.Data(), massTol, massTolVeto, tol_switch, tol_sigma, pLife, radiuslow, Switch, k0sDCA, k0sCosPoinAn, k0sDaughDCA, NTPCcluster, "", PairCutsSame, PairCutsMix, DCAxy, enableSys, crossedRows, rowsbycluster, v0rapidity, Sys)) return 0x0;

#endif
    
  //
  // -- CONTAINERS --------------------------------------------------------------------------------
  //
  TString outputFileName = AliAnalysisManager::GetCommonFileName();
  //  outputFileName += ":Rsn";
  Printf("AddTaskKStarPlusMinus - Set OutputFileName : \n %s\n", outputFileName.Data() );
    
  if(!HIST)//change-SD-[03-12-2019]
    {
      AliAnalysisDataContainer *output = mgr->CreateContainer(Form("RsnOut_%s_%.1f_%.1d_%.1f_%.2f_%.3f_%.f_%.f_%.f_%.1f_%.2f_%.1f_%.3d", outNameSuffix.Data(),piPIDCut,customQualityCutsID,pi_k0s_PIDCut,massTol,massTolVeto,pLife,radiuslow, k0sDCA,k0sCosPoinAn,k0sDaughDCA, DCAxy, Sys), TList::Class(), AliAnalysisManager::kOutputContainer, outputFileName);

    
      mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());
      mgr->ConnectOutput(task, 1, output);
    }

  if(HIST)
    {
      AliAnalysisDataContainer *output = mgr->CreateContainer(Form("RsnOut_%s_%.1f_%.1d_%.1f_%.2f_%.3f_%.f_%.f_%.f_%.1f_%.2f_%.1f_%.3d", outNameSuffix.Data(),piPIDCut,customQualityCutsID,pi_k0s_PIDCut,massTol,massTolVeto,pLife,radiuslow, k0sDCA,k0sCosPoinAn,k0sDaughDCA, DCAxy, Sys), TList::Class(), AliAnalysisManager::kOutputContainer, outputFileName);

    
      mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());
      mgr->ConnectOutput(task, 1, output);
    }
  return task;
}

