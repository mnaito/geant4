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
// File:        Reflection.cc
// Description: Discrete Process -- particle reflection at
//                                  surface boundary
// Based on G4OpBoundaryProcess
// Modified:    
//
// Author:      01.05.2025 M.Naito
//
////////////////////////////////////////////////////////////////////////

#include "Reflection.hh"

#include "G4ios.hh"
#include "G4GeometryTolerance.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4ParallelWorldProcess.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4TransportationManager.hh"
#include "G4VSensitiveDetector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Reflection::Reflection(const G4String& processName,
                                         G4ProcessType ptype)
  : G4VDiscreteProcess(processName, ptype)
{
  Initialise();

  if(verboseLevel > 0)
  {
    G4cout << GetProcessName() << " is created " << G4endl;
  }
  SetProcessSubType(32);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Reflection::~Reflection() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Reflection::Initialise()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VParticleChange* Reflection::PostStepDoIt(const G4Track& aTrack,
                                                     const G4Step& aStep)
{
  aParticleChange.Initialize(aTrack);
  aParticleChange.ProposeVelocity(aTrack.GetVelocity());

  const G4Step* pStep = &aStep;
  const G4Step* hStep = G4ParallelWorldProcess::GetHyperStep();
  if (hStep != nullptr)
    pStep = hStep;

  G4StepPoint* PreS  = pStep->GetPreStepPoint();
  G4StepPoint* PostS  = pStep->GetPostStepPoint();
  G4ThreeVector PostPos = PostS->GetPosition();
  fOldMomentum = PostS->GetMomentumDirection();

  // Check physical volume in the post step
  if (PreS->GetPhysicalVolume()->GetName()!="PhysVol_Pre"){
	  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  // Check physical volume in the post step
  if (PostS->GetPhysicalVolume()->GetName()!="PhysVol_Post"){
	  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  // Reflection at x = ± 10000. This parameter should be arranged.
  if (PostPos.x()<=-10000 || PostPos.x()>=10000) {
    // Perpendicular momentum.
    G4ThreeVector pPerp(fOldMomentum.x(), 0, 0);
	  fNewMomentum = fOldMomentum - 2*pPerp;
  }

  // Reflection at y = ± 10000. This parameter should be arranged.
  if (PostPos.y()<=-10000 || PostPos.y()>=10000){
    // Perpendicular momentum.
	  G4ThreeVector pPerp(0,fOldMomentum.y(),0);
	  fNewMomentum = fOldMomentum -2*pPerp;
  }

  if(verboseLevel > 1)
  {
    G4cout << " New Momentum Direction: " << fNewMomentum << G4endl;
  }

  aParticleChange.ProposeMomentumDirection(fNewMomentum);
  
  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double Reflection::GetMeanFreePath(const G4Track&, G4double,
                                              G4ForceCondition* condition)
{
  *condition = Forced;
  return DBL_MAX;
}
