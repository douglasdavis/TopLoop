TopLoop Analysis Package
========================

TopLoop is a small library inspired by the ASG EventLoop
package. TopLoop is built specifically for analyzing Single Top
ntuples output by the TopAnalysis `top-xaod` program with the
`SingleTopAnalysis` software loaded. Technically, this software can
analyze _any_ ntuple, but the structure and API are tailored for
ntuples output by `top-xaod`. Any user can implement the ability to
access any branch - see the [Adding new
variables](#adding-new-variables) section.

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
a built-in prefix (`bv__`) plus the name of the branch
(e.g. `bv__el_pt` for the branch `el_pt`). The variables are updated
on each iteration behind the scenes (in the `TL::Job`
implementation). The user does not need to worry about the variables
updating! Two macros handle everything related to setting up variables
and access to them. The user just "calls" the variable like one calls
a function (more details below).

A list of available variables are found in
[TopLoop/Core/Variables.h](https://gitlab.cern.ch/atlas-aida/TopLoop/blob/master/TopLoop/Core/Variables.h)

An example code block:

```cpp
TL::StatusCode MyAlgorithm::execute() {
  //...
  TL::EDM::FinalState finalState;
  auto averageMu = mu();
  for ( std::size_t i = 0; i < el_pt().size(); ++i ) {
    TL::EDM::Electron elec;
    auto pt  = el_pt().at(i);
    auto eta = el_eta().at(i);
    auto phi = el_phi().at(i);
    elec.p().SetPtEtaPhiM(pt,eta,phi,0.511)
    finalState.addLepton(elec);
  }
  // ...
}
```

### Adding new variables

It's very easy to add additional variable access in a TopLoop based
algorithm. If your analysis adds more output on top of the standard
AnalysisTop variables, you'll need this.

There are two macros defined: `DECLARE_BRANCH` and
`CONNECT_BRANCH`. In the header, we use the former, it just takes the
name of the branch and the type. Then in the implementation of your
algorithm's `init()` function, we use the latter and say which
TTreeReader pointer to connect to.

An example where the additional variable of interest is called
`el_foo` exists:

```cpp
// in the header class definition.
DECLARE_BRANCH(el_foo, std::vector<float>)
```

```cpp
// in your init() implementation
CONNECT_BRANCH(el_foo, std::vector<float>, reader());
```


```cpp
// in your execute() implementation
for ( const auto& el_foo_itr : el_foo() ) {
  auto el_foo_squared = el_foo_itr*el_foo_itr;
}
```

Under the hood, the macros create a
`std::unique_ptr<TTreeReaderValue<T>>` and a public function to access
the value via a double derefencing. If you imagine the top ntuple as a
class itself (or perhaps a pandas.DataFrame) then the function call
structure is intuitive.

## API Documentation

Doxygen documentation can be found here: http://ddavis.web.cern.ch/ddavis/TopLoopDoc/

## Example Algorithm

We're starting to ramp up an algorithm for the Wt dilepton analysis:
[WtLoop](http://gitlab.cern.ch/atlas-aida/WtLoop)
