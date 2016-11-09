#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ enum TL::STATUS+;

#pragma link C++ class TL::FileManager+;
#pragma link C++ class TL::AnaBase+;
#pragma link C++ class TL::Job+;

#pragma link C++ enum TL::kInitialState+;
#pragma link C++ enum TL::kGenerator+;
#pragma link C++ enum TL::kSampleType+;
#pragma link C++ class std::map<int,std::tuple<TL::kInitialState,TL::kGenerator,TL::kSampleType>+;
#pragma link C++ class TL::SampleMetaSvc+;

#endif
