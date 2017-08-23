TopLoop Analysis Package      {#mainpage}
========================

TopLoop is a small library inspired by the ASG EventLoop
package. TopLoop is built specifically for analyzing Top group ntuples
output by the TopAnalysis top-xaod program.

TopLoop delivers a base algorithm class, a job runner, a file manager,
and a small event data model. The algorithm has the following
structure:

## Components

- `init()`: run at the beginning of the job
- `setupOutput()`: run immediately following init()
- `execute()`: run on each iteration of the event set
- `finish()`: run after the completion of all execute() calls

## TopLoop Variables.

All tree variables (branches) are stored as smart pointers to
`TTreeReaderValue<T>` objects. The pointer object names correspond to
the name of the branch. The variables are updated on each iteration
behind the scenes (in the `TL::Job` implementation). The user does not
need to worry about the variables updating! We dereference the pointer
and TTreeReader to access the value of a variable, e.g.:

```cpp
StatusCode MyAlgorithm::execute() {
  //...
  auto averageMu = *(*mu);
  TL::EDM::Lepton aLepton;
  for ( std::size_t i = 0; i < (*el_pt)->size(); ++i ) {
    auto pt  = (*el_pt)->at(i);
    auto eta = (*el_eta)->at(i);
    auto phi = (*el_phi)->at(i);
    aLepton.p().SetPtEtaPhiM(pt,eta,phi,0.511)
  }
  // ...
}
```

A list of available variables are found in
[TopLoop/Core/Variables.h](https://gitlab.cern.ch/atlas-aida/TopLoop/blob/master/TopLoop/Core/Variables.h)

### Adding new variables

It's very easy to add additional variable access in a TopLoop based
algorithm. If your analysis adds more output on top of the standard
AnalysisTop variables, you'll need this.

In your algorithm's header you declare the
`std::shared_ptr<TTreeReaderValue<T>>` object. Then in your
algorithm's `init()` function you use TopLoop function
`setupTreeVar<T>`. An example where the additional variable
`el_true_pt` exists:

```cpp
// in the header
std::shared_ptr<TTRV_vec_float> el_true_pt;
```
```cpp
// in init() source
el_true_pt = setupTreeVar<TTRV_vec_float>(m_reader,"el_true_pt");
```
```cpp
// in execute() source
for ( aut const& truepT : *(*el_true_pt) ) {
  auto true_pt_squqred = truepT*truepT;
}
```

## Example Algorithms

Within the AIDA group, we have a mature algorithm built on top of the
TopLoop framework called
[AIDALoop](https://gitlab.cern.ch/atlas-aida/AIDALoop/tree/rel21Wt_test)

We're also starting to ramp up an algorithm for the Wt dilepton analysis:
[WtLoop](http://gitlab.cern.ch/atlas-aida/WtLoop)

## API Documentation

Doxygen documentation can be found here: cern.ch/ddavis/TopLoopDoc
