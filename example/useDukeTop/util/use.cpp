// DT
#include <MyDukeTopAna/MyDukeTopAna.h>
#include <DukeTop/Job.h>
#include <DukeTop/FileManager.h>

// C++
#include <iostream>

int main(int argc, char *argv[]) {
  MyDukeTopAna* mdsta = new MyDukeTopAna();

  mdsta->fileManager()->setTreeName("nominal");
  //  mdsta->fileManager()->feedDir(std::string(argv[1]));
  mdsta->fileManager()->feedTxt(std::string(argv[1]));
  DT::Job job(mdsta);
  job.run();

  return 0;
}
