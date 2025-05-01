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
//---------------------------------------------------------------------------
//
// ClassName:   RefConstructor
//
// Author: 2002 J.P. Wellisch
//
// Modified:
// 10.11.2005 V.Ivanchenko edit to provide a standard
// 05.12.2005 V.Ivanchenko add controlled verbosity
// 25.04.2006 V.Ivanchenko fix problem of destructor 
// 01.05.2025 M.Naito add particle reflection at surface boundary
//
//----------------------------------------------------------------------------
//

#include "RefConstructor.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4BuilderType.hh"
#include "G4PhysListUtil.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(RefConstructor);

RefConstructor::RefConstructor(const G4String& name, G4int ver)
  :  G4VPhysicsConstructor(name), verbose(ver)
{
  //SetPhysicsType(bTransportation);
  G4PhysListUtil::InitialiseParameters();
}

RefConstructor::RefConstructor(G4int ver)
  :  RefConstructor("Reflection", ver)
{}

RefConstructor::~RefConstructor()
{}

void RefConstructor::ConstructParticle()
{

// G4cout << "RefConstructor::ConstructParticle" << G4endl;
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();

}

void RefConstructor::ConstructProcess()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  // Add Reflection Process
  Reflection* fReflection = new Reflection();
  auto myParticleIterator=GetParticleIterator();
  myParticleIterator->reset();
  G4ParticleDefinition* particle=0;

  while( (*myParticleIterator)() )
  {
    particle = myParticleIterator->value();
    ph->RegisterProcess( fReflection, particle );

    // This method is also allowed while it is for old geant4 version.
    //G4ProcessManager* pmanager = particle->GetProcessManager();
    //pmanager->AddDiscreteProcess( fReflection );
  }
}
