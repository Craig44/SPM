//============================================================================
// Name        : CAgeingError.h
// Author      : S.Rasmussen
// Date        : 18/05/2009
// Copyright   : Copyright NIWA Science �2009 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================
#ifndef CAGEINGERROR_H_
#define CAGEINGERROR_H_

// Local headers
#include "../BaseClasses/CBaseValidate.h"
#include "../BaseClasses/CBaseBuild.h"

//**********************************************************************
//
//
//**********************************************************************
class CAgeingError : public CBaseValidate {
public:
  // Functions
  CAgeingError();
  virtual                     ~CAgeingError();
  void                        validate();
  void                        build() { };
  void                        execute() { };
  int                         getMinAge() { return iMinAge; }
  int                         getMaxAge() { return iMaxAge; }
  bool                        getAgePlusGroup() { return bAgePlusGroup; }


protected:
  int                       iMaxAge;
  int                       iMinAge;
  bool                      bAgePlusGroup;
  int                       iNAges;
  vector<vector<double> >   mMisMatrix;

};

#endif /* CAGEINGERROR_H_ */