/** @file Job.cxx
 *  @brief TL::Job class implementation
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

// TL
#include <TopLoop/Core/Job.h>
#include <TopLoop/Core/Algorithm.h>
#include <TopLoop/Core/Utils.h>
#include <TopLoop/Core/FileManager.h>
#include <TopLoop/tqdm/tqdm.h>

TL::Job::Job() : TL::Loggable("TL::Job") {}

TL::Job::~Job() {}

TL::StatusCode TL::Job::setAlgorithm(std::unique_ptr<TL::Algorithm> alg) {
  if ( alg == nullptr ) {
    return TL::StatusCode::FAILURE;
  }
  m_algorithm = std::move(alg);
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Job::setFileManager(std::unique_ptr<TL::FileManager> fm) {
  if ( fm == nullptr ) {
    return TL::StatusCode::FAILURE;
  }
  m_fm = std::move(fm);
  return TL::StatusCode::SUCCESS;
}

TL::StatusCode TL::Job::run() {
  TL_CHECK(m_algorithm->setFileManager(std::move(m_fm)));
  TL_CHECK(m_algorithm->init());
  if ( not m_algorithm->initCalled() ) {
    logger()->error("You didn't call TL::Algorithm::init()");
    logger()->error("in your algorithm's init() function");
    logger()->error("This is a required line!");
    return TL::StatusCode::FAILURE;
  }
  TL_CHECK(m_algorithm->setupOutput());
  m_algorithm->reader()->Restart();
  tqdm bar;
  bar.set_theme_braille_spin();
  while ( m_algorithm->reader()->Next() ) {
    bar.progress(m_algorithm->m_eventCounter,m_algorithm->m_totalEntries);
    TL_CHECK(m_algorithm->execute());
  }
  TL_CHECK(m_algorithm->finish());
  return TL::StatusCode::SUCCESS;
}
