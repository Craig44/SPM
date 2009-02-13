//============================================================================
// Name        : CEstimateSummaryReporter.cpp
// Author      : S.Rasmussen
// Date        : 30/01/2009
// Copyright   : Copyright NIWA Science �2009 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Local headers
#include "CEstimateSummaryReporter.h"
#include "../../Estimates/CEstimateManager.h"
#include "../../Estimates/CEstimate.h"
#include "../../Helpers/ForEach.h"

//**********************************************************************
// CEstimateSummaryReporter::CEstimateSummaryReporter()
// Default Constructor
//**********************************************************************
CEstimateSummaryReporter::CEstimateSummaryReporter() {
  eExecutionState = STATE_FINALIZATION;
}

//**********************************************************************
// void CEstimateSummaryReporter::execute()
// Execute print state
//**********************************************************************
void CEstimateSummaryReporter::execute() {
  // Check Run-Mode
  if (pRuntimeController->getRunMode() != RUN_MODE_BASIC)
    return;

  // Start IO
  this->start();

  // Get Estimates
  vector<CEstimate*> vEstimates;
  CEstimateManager::Instance()->fillVector(vEstimates);

  cout << CONFIG_ARRAY_START << sLabel << CONFIG_ARRAY_END << "\n";

  foreach(CEstimate *Estimate, vEstimates) {
    cout << Estimate->getParameter() << "\n";
    cout << PARAM_LOWER_BOUND << ": " << Estimate->getLowerBound() << "\n";
    cout << PARAM_UPPER_BOUND << ": " << Estimate->getUpperBound() << "\n";
    cout << PARAM_PRIOR << ": " << Estimate->getPrior() << "\n";
    cout << PARAM_VALUE<< ": " << Estimate->getValue() << "\n" << endl;
  }

  cout << endl;
  // End IO
  this->end();
}

//**********************************************************************
// CEstimateSummaryReporter::~CEstimateSummaryReporter()
// Destructor
//**********************************************************************
CEstimateSummaryReporter::~CEstimateSummaryReporter() {
  // TODO Auto-generated destructor stub
}