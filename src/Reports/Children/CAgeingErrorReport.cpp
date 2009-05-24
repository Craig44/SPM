//============================================================================
// Name        : CAgeingErrorReport.cpp
// Author      : S.Rasmussen
// Date        : 26/03/2009
// Copyright   : Copyright NIWA Science �2009 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Local headers
#include "CAgeingErrorReport.h"
#include "../../AgeingError/CAgeingErrorManager.h"
#include "../../AgeingError/CAgeingError.h"

//**********************************************************************
// CAgeingErrorReport::CAgeingErrorReport()
// Constructor
//**********************************************************************
CAgeingErrorReport::CAgeingErrorReport() {
  // Variables
  eExecutionState   = STATE_FINALIZATION;
  pAgeingError      = 0;

  // Register allowed
  pParameterList->registerAllowed(PARAM_AGEING_ERROR);
}

//**********************************************************************
// void CAgeingErrorReport::validate()
// Validate our selectivity
//**********************************************************************
void CAgeingErrorReport::validate() {
  try {
    // Parent
    CFileReport::validate();

    sAgeingError = pParameterList->getString(PARAM_AGEING_ERROR);

  } catch (string ex) {
    ex = "CAgeingErrorReport.validate(" + getLabel() + ")->" + ex;
    throw ex;
  }
}

//**********************************************************************
// void CAgeingErrorReport::build()
// Build our Report
//**********************************************************************
void CAgeingErrorReport::build() {
  try {
    // Parent
    CFileReport::build();

    // Get our selectivity
    CAgeingErrorManager *pManager = CAgeingErrorManager::Instance();
    pAgeingError = pManager->getAgeingError(sAgeingError);

  } catch (string ex) {
    ex = "CAgeingErrorReport.build(" + getLabel() + ")->" + ex;
    throw ex;
  }
}

//**********************************************************************
// void CAgeingErrorReport::execute()
// Execute
//**********************************************************************
void CAgeingErrorReport::execute() {
  // Check for correct state
  if (pRuntimeController->getRunMode() != RUN_MODE_BASIC)
    return;

  this->start();

  // Work our how many viable ages we have.
  int iSpread = pWorld->getAgeSpread();
  int iMinAge = pWorld->getMinAge();

  // Output Header
  cout << CONFIG_ARRAY_START << sLabel << CONFIG_ARRAY_END << "\n";
  cout << PARAM_REPORT << "." << PARAM_TYPE << CONFIG_RATIO_SEPARATOR << " " << pParameterList->getString(PARAM_TYPE) << "\n";
  cout << PARAM_AGEING_ERROR << "." << PARAM_LABEL << CONFIG_RATIO_SEPARATOR << " " << sAgeingError << "\n";

  cout << PARAM_AGES << CONFIG_RATIO_SEPARATOR << " " << iMinAge;
  for (int i = 1; i < iSpread; ++i)
    cout << CONFIG_SEPERATOR_ESTIMATE_VALUES << " " << (i + iMinAge);
  cout << "\n";

  vector<vector<double> > mMisMatrix = pAgeingError->getMisMatrix();

  for (int i = 0; i < iSpread; ++i) {
    cout << i+1 << CONFIG_RATIO_SEPARATOR << " ";
    for (int j = 0; j < (iSpread - 1); ++j) {
      cout << mMisMatrix[i][j] << CONFIG_SEPERATOR_ESTIMATE_VALUES << " ";
    }
    cout << mMisMatrix[i][iSpread-1] << "\n";
  }
  cout << "\n";
  cout << "*end\n" << endl;

  this->end();
}

//**********************************************************************
// CAgeingErrorReport::~CAgeingErrorReport()
// Destuctor
//**********************************************************************
CAgeingErrorReport::~CAgeingErrorReport() {
}
