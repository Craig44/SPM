//============================================================================
// Name        : CDESolver.cpp
// Author      : S.Rasmussen
// Date        : 1/05/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Global Headers
#include <iostream>

// Local Headers
#include "CDESolver.h"
#include "CDESolverCallback.h"
#include "../../Estimates/CEstimateManager.h"
#include "../../Estimates/CEstimate.h"

// Namespaces
using std::cout;
using std::endl;

//**********************************************************************
// CDESolverInterface::CDESolverInterface()
// Default Constructor
//**********************************************************************
CDESolver::CDESolver() {


  // Register some Parameters
  pParameterList->registerAllowed(PARAM_POPULATION_SIZE);
  pParameterList->registerAllowed(PARAM_CROSSOVER_PROBABILITY);
  pParameterList->registerAllowed(PARAM_DIFFERENCE_SCALE);
  pParameterList->registerAllowed(PARAM_MAX_GENERATIONS);
}

//**********************************************************************
// void CDESolver::validate()
// Validate our DE Solver Minimizer
//**********************************************************************
void CDESolver::validate() {
  try {
    // Base
    CMinimizer::validate();

    iPopulationSize         = pParameterList->getInt(PARAM_POPULATION_SIZE);
    dCrossoverProbability   = pParameterList->getDouble(PARAM_CROSSOVER_PROBABILITY);
    dDifferenceScale        = pParameterList->getDouble(PARAM_DIFFERENCE_SCALE);
    iMaxGenerations         = pParameterList->getInt(PARAM_MAX_GENERATIONS);

  } catch (string Ex) {
    Ex = "CDESolver.validate()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CDESolver::runEstimation()
// run our Estimation
//**********************************************************************
void CDESolver::runEstimation() {
  // Variables
  CEstimateManager      *pEstimateManager    = CEstimateManager::Instance();
  int                   iCount               = 0;

  try {
    // Get Number of Estimate Variables.
    iCount = pEstimateManager->getEnabledEstimateCount();

    if (iCount == 0)
      throw string(ERROR_ENABLED_ESTIMATES);

    // Setup our Solver
    CDESolverCallback     clDESolver = CDESolverCallback(iCount, iPopulationSize);

    // Setup Lower, Upper Bounds
    vector<double> vLowerBounds;
    vector<double> vUpperBounds;
    vector<double> vStartValues;

    for (int i = 0; i < iCount; ++i) {
      CEstimate *pEstimate = pEstimateManager->getEnabledEstimate(i);
      vLowerBounds.push_back(pEstimate->getLowerBound());
      vUpperBounds.push_back(pEstimate->getUpperBound());
      vStartValues.push_back(pEstimate->getValue());
    }

    clDESolver.Setup(vStartValues, vLowerBounds, vUpperBounds, stBest1Exp, dDifferenceScale, dCrossoverProbability);

    if (clDESolver.Solve(iMaxGenerations)) {
      cout << "Convergence was successful" << endl;
      cout << "Final Score: " << clDESolver.getEnergy() << endl;
      cout << "Score took " << clDESolver.getGenerations() << " generations to find" << endl;
    } else
      cout << "Failed to converge, ran out of generations" << endl;

  } catch (string Ex) {
    Ex = "CDESolverInterface.runEstimation()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// CDESolver::~CDESolver()
// Default De-Constructor
//**********************************************************************
CDESolver::~CDESolver() {
}
