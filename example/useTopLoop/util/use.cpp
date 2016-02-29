// TL
#include <MyTopLoopAna/MyTopLoopAna.h>
#include <TopLoop/Job.h>
#include <TopLoop/FileManager.h>

// C++
#include <iostream>

// Boost
#include <boost/program_options.hpp>

int main(int argc, char *argv[]) {

  namespace bpo = boost::program_options;
  bpo::options_description opts("Top Loop Algorithm steering program");
  opts.add_options()
    ("help,h","Display help message")
    ("feed-dir",bpo::value<std::string>(),
     "flag to point the algorithm to a directory containing ROOT files")
    ("feed-txt",bpo::value<std::string>(),
     "flag to point the algorithm to a text file containing a list of ROOT files");

  bpo::variables_map vm;
  bpo::store(bpo::parse_command_line(argc,argv,opts),vm);
  bpo::notify(vm);

  if ( vm.count("help") ) {
    std::cout << opts << std::endl;
    return 0;
  }

  if ( !vm.count("feed-dir") && !vm.count("feed-txt") ) {
    std::cout << "You must either use --feed-dir or --feed-txt!" << std::endl;
    std::cout << "Full help:" << std::endl;
    std::cout << opts << std::endl;
    return 0;
  }

  MyTopLoopAna* mdsta = new MyTopLoopAna();

  if ( vm.count("feed-dir") ) {
    mdsta->fileManager()->feedDir(vm["feed-dir"].as<std::string>());
  }

  if ( vm.count("feed-txt") ) {
    mdsta->fileManager()->feedTxt(vm["feed-txt"].as<std::string>());
  }
  
  TL::Job job(mdsta);
  job.run();

  return 0;
}
