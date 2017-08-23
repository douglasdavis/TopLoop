/** @file Variables.cxx
 *  @brief TL::Variables class implementation
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#include <TopLoop/Core/Variables.h>

TL::Variables::Variables() {}

TL::Variables::~Variables() {}

void TL::Variables::ignoreListFile(const std::string& fileName) {
  std::string line;
  std::ifstream infile(fileName);
  while ( std::getline(infile,line) ) {
    m_ignoreList.emplace_back(line);
  }
}
