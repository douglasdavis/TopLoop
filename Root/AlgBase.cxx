/** @file AlgBase.cxx
 *  @brief TL::AlgBase class implementation
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 *  @author Kevin Finelli < kevin.finelli@cern.ch >
 */

// TL
#include <TopLoop/Core/AlgBase.h>
#include <TopLoop/Core/FileManager.h>

ANA_MSG_SOURCE(msgAlgBase,"TL::AlgBase")

TL::AlgBase::AlgBase() :
m_datasetName(),
  m_isMC(true),
  m_isNominal(true),
  m_showTTRVwarning(true)
{
  SetName("TopLoop");
}

TL::AlgBase::~AlgBase() {}

float TL::AlgBase::countSumWeights() {
  using namespace msgAlgBase;
  //sum up the weighted number of events in the metadata tree.  This works for
  //MC (to get the MC lumi) and data (perhaps as a cross-check)
  float sumWeights = 0;

  while ( m_weightsReader->Next() ) {
    if ( m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid ) {
      FATAL("countSumWeights(): Tree reader does not return kEntryValid");
    }
    sumWeights += *(*totalEventsWeighted);
  }
  m_weightsReader->Restart();
  //todo: cross-check the value with Ami, warn if different?

  return sumWeights;
}

std::vector<float> TL::AlgBase::generatorVariedSumWeights() {
  using namespace msgAlgBase;
  //sum up the weighted number of events in the metadata tree.  This works for
  //MC (to get the MC lumi) and data (perhaps as a cross-check)
  std::size_t vsize = 0;
  while ( m_weightsReader->Next() ) {
    vsize = (*totalEventsWeighted_mc_generator_weights)->size();
    break;
  }
  std::vector<float> weights(vsize,0.0);
  m_weightsReader->Restart();

  while ( m_weightsReader->Next() ) {
    if ( m_weightsReader->GetEntryStatus() != TTreeReader::kEntryValid ) {
      FATAL("generatorVariedSumWeights(): Tree reader does not return kEntryValid");
    }
    // now get all the rest
    for ( std::size_t j = 0; j < (*totalEventsWeighted_mc_generator_weights)->size(); ++j ) {
      weights[j] += (*totalEventsWeighted_mc_generator_weights)->at(j);
    }
  }
  m_weightsReader->Restart();

  //todo: cross-check the value with Ami, warn if different?
  return weights;
}

std::vector<std::string> TL::AlgBase::generatorWeightNames() {
  std::vector<std::string> retvec;
  while ( m_weightsReader->Next() ) {
    retvec = *(*names_mc_generator_weights);
    break;
  }
  m_weightsReader->Restart();
  return retvec;
}

unsigned int TL::AlgBase::get_dsid() {
  unsigned int ret_dsid = 999999;
  while ( m_weightsReader->Next() ) {
    ret_dsid = *(*dsid);
    break;
  }
  m_weightsReader->Restart();

  return ret_dsid;
}

StatusCode TL::AlgBase::init() {
  using namespace msgAlgBase;
  ANA_CHECK_SET_TYPE(StatusCode);
  ANA_CHECK(init_core_vars());
  return StatusCode::SUCCESS;
}

StatusCode TL::AlgBase::setupOutput() {
  ANA_CHECK_SET_TYPE(StatusCode);
  return StatusCode::SUCCESS;
}

StatusCode TL::AlgBase::execute() {
  ANA_CHECK_SET_TYPE(StatusCode);
  return StatusCode::SUCCESS;
}

StatusCode TL::AlgBase::finish() {
  ANA_CHECK_SET_TYPE(StatusCode);
  return StatusCode::SUCCESS;
}

void TL::AlgBase::progress(long cur, long total, int range) const {
  using namespace msgAlgBase;
  if ( total > range ) {
    auto progress = 100.0*cur/total;
    int gap = total/range;
    if ( cur%gap == 0 ) {
      ANA_MSG_INFO("Event " << cur << ", " << std::round(progress) << "%");
    }
  }
  return;
}
  
