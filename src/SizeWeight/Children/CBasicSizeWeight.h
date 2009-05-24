//============================================================================
// Name        : CBasicSizeWeight.h
// Author      : A.Dunn
// Date        : 24/05/2009
// Copyright   : Copyright NIWA Science �2009 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================
#ifndef CBASICSIZEWEIGHT_H_
#define CBASICSIZEWEIGHT_H_

#include "../CSizeWeight.h"

//**********************************************************************
//
//
//**********************************************************************
class CBasicSizeWeight: public CSizeWeight {
public:
  // Functions
  CBasicSizeWeight();
  virtual                     ~CBasicSizeWeight();
  void                        validate();
  void                        build();
  void                        rebuild();
  double                      getMeanWeight(double &size);

protected:
  double                      dA;
  double                      dB;
  double                      dCV;

};

#endif /* CBASICSIZEWEIGHT_H_ */
