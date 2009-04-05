//============================================================================
// Name        : CInitializationPhaseManager.cpp
// Author      : S.Rasmussen
// Date        : 29/04/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Global headers
#include <iostream>

// Local Headers
#include "CInitializationPhaseManager.h"
#include "CInitializationPhase.h"
#include "../Reports/CReportManager.h"
#include "../Helpers/ForEach.h"
#include "../Helpers/CError.h"

// Using
using std::cout;
using std::endl;

// Singleton Variable
boost::thread_specific_ptr<CInitializationPhaseManager> CInitializationPhaseManager::clInstance;

//**********************************************************************
// CInitializationPhaseManager::CInitializationPhaseManager()
// Default Constructor
//**********************************************************************
CInitializationPhaseManager::CInitializationPhaseManager() {
  // Variables
  lastExecutedInitializationPhase = -1;
  pReportManager                  = 0;
}

//**********************************************************************
// CInitializationPhaseManager* CInitializationPhaseManager::Instance()
// Instance Method - Singleton
//**********************************************************************
CInitializationPhaseManager* CInitializationPhaseManager::Instance() {
  if (clInstance.get() == 0)
    clInstance.reset(new CInitializationPhaseManager());
  return clInstance.get();
}

//**********************************************************************
// void CInitializationPhaseManager::Destroy()
// Destroy Method - Singleton
//**********************************************************************
void CInitializationPhaseManager::Destroy() {
  if (clInstance.get() != 0) {
    clInstance.reset();
  }
}

//**********************************************************************
// void CInitializationPhaseManager::addInitializationPhase(CInitializationPhase *value)
// Add our Timestep To The List
//**********************************************************************
void CInitializationPhaseManager::addInitializationPhase(CInitializationPhase *value) {
  vInitializationPhases.push_back(value);
}

//**********************************************************************
// void CInitializationPhaseManager::loadInitializationPhaseOrder(vector<string> &order)
// Populate the ordered vector with phases based on the vector parameter
//**********************************************************************
void CInitializationPhaseManager::loadInitializationPhaseOrder(vector<string> &order) {
  vInitializationPhaseOrder.clear();

  foreach(string Label, order) {
    foreach(CInitializationPhase *Phase, vInitializationPhases) {
      if (Phase->getLabel() == Label) {
        vInitializationPhaseOrder.push_back(Phase);
        break;
      }
    }
  }
}

//**********************************************************************
// int CInitializationPhaseManager::getInitializationPhaseOrderIndex(string label)
// Get the Index for our Initializaton Phase when Ordered
//**********************************************************************
int CInitializationPhaseManager::getInitializationPhaseOrderIndex(string label) {
  try {
    // Get the Index for our Initialization Phase
    for (int i = 0; i < (int)vInitializationPhaseOrder.size(); ++i)
      if (vInitializationPhaseOrder[i]->getLabel() == label)
        return i;

    CError::errorUnknown(PARAM_INITIALIZATION_PHASE, label);
    return -1;

  } catch (string Ex) {
    Ex = "CInitializationPhaseManager.getInitializationPhaseOrderIndex(" + label + ")->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CInitializationPhaseManager::clone(CInitializationPhaseManager *Manager)
// Clone the manager passed in as a parameter
//**********************************************************************
void CInitializationPhaseManager::clone(CInitializationPhaseManager *Manager) {
  try {
    for (int i = 0; i < (int)Manager->vInitializationPhases.size(); ++i) {
      CInitializationPhase *pInitializationPhase = Manager->vInitializationPhases[i];
      vInitializationPhases.push_back(pInitializationPhase->clone());
    }

  } catch (string Ex) {
    Ex = "CInitialisationManager.clone()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CInitializationPhaseManager::validate()
// Validate our TimeSteps
//**********************************************************************
void CInitializationPhaseManager::validate() {
  try {
    foreach( CInitializationPhase *InitializationPhase, vInitializationPhases) {
      InitializationPhase->validate();
    }

  } catch(string Ex) {
    Ex = "CInitialisationManager.validate()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CInitializationPhaseManager::build()
// Build our Time Steps
//**********************************************************************
void CInitializationPhaseManager::build() {
  try {
    // Now, Call TimeStep Validations
    foreach( CInitializationPhase *InitializationPhase, vInitializationPhases) {
      InitializationPhase->build();
    }

    // Variables
    pReportManager = CReportManager::Instance();

  } catch(string Ex) {
    Ex = "CInitialisationManager.build()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CInitializationPhaseManager::execute()
// Execute our Initialization Phases
//**********************************************************************
void CInitializationPhaseManager::execute() {

  // Reset Lasr Executed
  lastExecutedInitializationPhase = -1;

  foreach(CInitializationPhase *Phase, vInitializationPhaseOrder) {
    Phase->execute();
    // Incremenet Place holder
    lastExecutedInitializationPhase++;
    pReportManager->execute(STATE_INITIALIZATION_MODELLING);
  }
}

//**********************************************************************
// CInitializationPhaseManager::~CInitializationPhaseManager()
// Default De-Constructor
//**********************************************************************
CInitializationPhaseManager::~CInitializationPhaseManager() {
  // Delete Our Layers
  foreach( CInitializationPhase *InitializationPhase, vInitializationPhases) {
    delete InitializationPhase;
  }

  // Clear Vector
  vInitializationPhases.clear();
}
