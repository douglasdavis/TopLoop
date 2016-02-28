// TL
#include <MyTopLoopAna/MyTopLoopAna.h>
#include <TopLoop/Job.h>
#include <TopLoop/FileManager.h>

// C++
#include <iostream>

int main(int argc, char *argv[]) {
  MyTopLoopAna* mdsta = new MyTopLoopAna();

  mdsta->fileManager()->setTreeName("nominal");
  mdsta->fileManager()->feedDir(std::string(argv[1]));
  //mdsta->fileManager()->feedTxt(std::string(argv[1]));
  TL::Job job(mdsta);
  job.run();

  return 0;
}
