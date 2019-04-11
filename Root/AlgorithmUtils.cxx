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
  return fileManager()->dsid();
}

bool TL::Algorithm::sampleIsAFII() {
  return fileManager()->isAFII();
}

void TL::Algorithm::printProgress(const unsigned int n_prints) const {
  if (m_totalEntries > n_prints) {
    int gap = m_totalEntries / n_prints;
    if (m_eventCounter % gap == 0) {
      auto progress = std::round(100.0 * m_eventCounter / m_totalEntries);
      logger()->info("-- [{:3.0f}%] Event: {}", progress, m_eventCounter);
    }
  }
}
