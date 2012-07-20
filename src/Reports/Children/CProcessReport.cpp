//============================================================================
// Name        : CProcessReport.cpp
// Author      : S.Rasmussen
// Date        : 23/04/2009
// Copyright   : Copyright NIWA Science �2009 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Headers
#include <boost/lexical_cast.hpp>

#include "CProcessReport.h"
#include "../../Helpers/CError.h"
#include "../../Helpers/ForEach.h"
#include "../../Estimates/CEstimateManager.h"
#include "../../Estimates/CEstimate.h"
#include "../../Processes/CProcessManager.h"
#include "../../Processes/CProcess.h"
#include "../../Processes/Population/CBHRecruitmentProcess.h"
#include "../../Processes/Population/CHollingMortalityRateProcess.h"
#include "../../Selectivities/CSelectivityManager.h"
#include "../../Selectivities/CSelectivity.h"

//**********************************************************************
// CProcessReport::CProcessReport()
// Constructor
//**********************************************************************
CProcessReport::CProcessReport() {
  // Variables
  eExecutionState   = STATE_FINALIZATION;

  // Reg
  pParameterList->registerAllowed(PARAM_PROCESS);
}

//**********************************************************************
// void CProcessReport::validate()
// Validate our Parameter List
//**********************************************************************
void CProcessReport::validate() {
  try {

    // Get var
    sParameter = pParameterList->getString(PARAM_PROCESS);

    // Validate parent
    CFileReport::validate();

    // Local validation

  } catch (string &Ex) {
    Ex = "CProcessReport.validate()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
//
//
//**********************************************************************
void CProcessReport::build() {
  try {
    // Base
    CFileReport::build();

    // Now grab our variable
    pTarget = CProcessManager::Instance()->getProcess(sParameter);

  } catch (string &Ex) {
    Ex = "CProcessReport.build()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CProcessReport::execute()
// Execute our Report
//**********************************************************************
void CProcessReport::execute() {

  try {
    // Check for correct state
    if (pRuntimeController->getRunMode() != RUN_MODE_BASIC)
      if (pRuntimeController->getRunMode() != RUN_MODE_PROFILE)
        return;

    this->start();

    CEstimateManager *pEstimateManager = CEstimateManager::Instance();

    CParameterList *pList = pTarget->getParameterList();

    cout << CONFIG_ARRAY_START << sLabel << CONFIG_ARRAY_END << "\n";
    cout << PARAM_REPORT << "." << PARAM_TYPE << CONFIG_RATIO_SEPARATOR << " " << pParameterList->getString(PARAM_TYPE) << "\n";

    vector<string> vDefinedParameters;
    pList->fillDefinedParameterVector(vDefinedParameters);

    vector<string> vValues;
    foreach(string Parameter, vDefinedParameters) {
      pList->fillVector(vValues, Parameter);

      cout << Parameter << ": ";

      /**
       * Check if this parameter is estimated
       * First check if there is only 1 value
       */
      if (vValues.size() == 1) {
        string estimableName = "process[" + pTarget->getLabel() + "]." + Parameter;

        if (pEstimateManager->hasEstimate(estimableName)) {
          CEstimate *pEstimate = pEstimateManager->getEstimate(estimableName);
          cout << pEstimate->getValue();
        } else {
          cout << vValues[0];
        }

      } else if (vValues.size() > 1) {

        for (unsigned i = 0; i < vValues.size(); ++i) {
          string estimableName = "process[" + pTarget->getLabel() + "]." + Parameter + "(" + boost::lexical_cast<string>(i + 1) + ")";

          if (pEstimateManager->hasEstimate(estimableName)) {
            CEstimate *pEstimate = pEstimateManager->getEstimate(estimableName);
            cout << pEstimate->getValue() << " ";
          } else {
            cout << vValues[i] << " ";
          }
        }

      } else {
        foreach(string Value, vValues) {
          cout << Value << " ";
        }
      }

      cout << endl;
    }

    // Specific private reporting for individual processes
    CBHRecruitmentProcess *pRecruit = dynamic_cast<CBHRecruitmentProcess*>(pTarget);
    if (pRecruit != 0) {
      //B0
      cout << PARAM_B0_VALUE << ": " << pRecruit->getB0Value() << "\n";
      //YCS years
      vector<int> vYCSYears = pRecruit->getYCSYears();
      cout << PARAM_YCS_YEARS << ": " ;
      for (int i=0; i < (int)vYCSYears.size(); ++i) {
        cout << vYCSYears[i] << " ";
      }
      cout << "\n";
      //SSBs
      vector<double> vSSBValues = pRecruit->getSSBValues();
      cout << PARAM_SSB_VALUES << ": " ;
      for (int i=0; i < (int)vSSBValues.size(); ++i) {
        cout << vSSBValues[i] << " ";
      }
      cout << "\n";
      //Recruitments
      vector<double> vRecruitmentValues = pRecruit->getRecruitmentValues();
      cout << PARAM_RECRUITMENT_VALUES << ": " ;
      for (int i=0; i < (int)vRecruitmentValues.size(); ++i) {
        cout << vRecruitmentValues[i] << " ";
      }
      cout << "\n";
      //True YCS
      vector<double> vTrueYCSValues = pRecruit->getTrueYCSValues();
      cout << PARAM_TRUE_YCS_VALUES << ": " ;
      for (int i=0; i < (int)vTrueYCSValues.size(); ++i) {
        cout << vTrueYCSValues[i] << " ";
      }
      cout << "\n";
    }
    CHollingMortalityRateProcess *pHolling = dynamic_cast<CHollingMortalityRateProcess*>(pTarget);
    if (pHolling != 0) {
      cout << "layer_type" << ": " << (pHolling->isAbundance()?PARAM_ABUNDANCE:PARAM_BIOMASS) << "\n";
      vector<double> vMortalityRate = pHolling->getMortalityRate();
      vector<double> vMortalityN = pHolling->getMortalityN();
      cout << PARAM_PROPORTION << ": ";
      for (int i = 0; i < (int)vMortalityRate.size(); ++i)
        cout << vMortalityRate[i] << " ";
      cout << "\n";
      cout << PARAM_ABUNDANCE << ": ";
      for (int i = 0; i < (int)vMortalityN.size(); ++i)
        cout << vMortalityN[i] << " ";
      cout << "\n";
      if(!(pHolling->isAbundance())) {
        vector<double> vMortalityBiomass = pHolling->getMortalityBiomass();
        cout << PARAM_BIOMASS << ": ";
        for (int i = 0; i < (int)vMortalityBiomass.size(); ++i)
          cout << vMortalityBiomass[i] << " ";
        cout << "\n";
      }
    }
    cout << CONFIG_END_REPORT << "\n" << endl;

    this->end();

  } catch (string &Ex) {
    Ex = "CProcessReport.build(" + getLabel() + ")->" + Ex;
    throw Ex;
  }

}

//**********************************************************************
// CProcessReport::~CProcessReport()
// Destructor
//**********************************************************************
CProcessReport::~CProcessReport() {
}
