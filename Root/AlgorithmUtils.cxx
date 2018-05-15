/** @file AlgorithmUtils.cxx
 *  @brief TL::Algorithm class utility function implementations
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 *  @author Kevin Finelli, <kevin.finelli@cern.ch>
 */

// TL
#include <TopLoop/Core/Algorithm.h>

// C++
#include <cmath>

unsigned int TL::Algorithm::get_dsid() {
  unsigned int ret_dsid = 999999;
  m_weightsReader->Restart();
  while (m_weightsReader->Next()) {
    ret_dsid = dsid();
    break;
  }
  m_weightsReader->Restart();
  return ret_dsid;
}

bool TL::Algorithm::sampleIsAFII() {
  int branch_value = 0;
  m_weightsReader->Restart();
  while (m_weightsReader->Next()) {
    branch_value = isAFII();
    break;
  }
  m_weightsReader->Restart();
  return (branch_value > 0);
}

void TL::Algorithm::printProgress(int n_prints) const {
  if (m_totalEntries > n_prints) {
    int gap = m_totalEntries / n_prints;
    if (m_eventCounter % gap == 0) {
      auto progress = std::round(100.0 * m_eventCounter / m_totalEntries);
      logger()->info("-- [{:3.0f}%] Event: {}", progress, m_eventCounter);
    }
  }
}
