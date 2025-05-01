//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// File:        Reflection.hh
// Description: Discrete Process -- particle reflection at
//                                  surface boundary
// Based on G4OpBoundaryProcess
// Modified:    
//
// Author:      01.05.2025 M.Naito
//
////////////////////////////////////////////////////////////////////////

#ifndef Reflection_h
#define Reflection_h 1

#include "G4Ions.hh"
#include "G4RandomTools.hh"
#include "G4VDiscreteProcess.hh"

class Reflection : public G4VDiscreteProcess
{
 public:
  explicit Reflection(const G4String& processName = "Reflection",
                               G4ProcessType type          = fOptical);
  virtual ~Reflection();

  virtual G4double GetMeanFreePath(const G4Track&, G4double,
		                   G4ForceCondition* condition) override;


  G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
                                  const G4Step& aStep) override;

  virtual void Initialise();

  void SetVerboseLevel(G4int);

 private:

  G4ThreeVector fOldMomentum;
  G4ThreeVector fNewMomentum;

};

#endif /* Reflection_h */
