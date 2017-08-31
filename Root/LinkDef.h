/** @file LinkDef.h
 *  @brief ROOT Dictionary LinkDef
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#include <TopLoop/Core/Variables.h>
#include <TopLoop/Core/FileManager.h>
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/Core/Job.h>
#include <TopLoop/Core/SampleMetaSvc.h>
#include <TopLoop/EDM/PhysicsObject.h>
#include <TopLoop/EDM/Lepton.h>
#include <TopLoop/EDM/LeptonPair.h>
#include <TopLoop/EDM/Jet.h>
#include <TopLoop/EDM/MET.h>
#include <TopLoop/EDM/FinalState.h>

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

#pragma link C++ class TL::Variables+;
#pragma link C++ class TL::FileManager+;
#pragma link C++ class TL::Algorithm+;
#pragma link C++ class TL::Job+;

#pragma link C++ enum TL::kMeta;
#pragma link C++ class std::map<int,std::tuple<TL::kMeta,TL::kMeta,TL::kMeta>>+;
#pragma link C++ class TL::SampleMetaSvc+;

#pragma link C++ class std::pair<size_t,size_t>+;
#pragma link C++ class TL::EDM::PhysicsObject+;
#pragma link C++ class TL::EDM::Jet+;
#pragma link C++ enum  TL::EDM::Jet::WP_mv2cbm+;
#pragma link C++ class std::map<TL::EDM::Jet::WP_mv2cbm,float>+;
#pragma link C++ class TL::EDM::Lepton+;
#pragma link C++ class TL::EDM::LeptonPair+;
#pragma link C++ class std::vector<TL::EDM::Jet>+;
#pragma link C++ class std::vector<TL::EDM::Lepton>+;
#pragma link C++ class std::vector<TL::EDM::LeptonPair>+;
#pragma link C++ class TL::EDM::MET+;
#pragma link C++ class TL::EDM::FinalState+;

#endif
