# DukeTop

`DukeTop` is a small library inspired by the ASG
[`EventLoop`](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/EventLoop)
package. `DukeTop` is built specifically for analyzing Top group
ntuples.

### Authors

* Douglas Davis < [douglas.davis@cern.ch](douglas.davis@cern.ch) >

## Structure of a `DukeTop` algorithm

Packages will inherit from `DT::AnaBase` (similar to how EventLoop
algorithms inherit from `EL::Algorithm`). `DT::AnaBase` pointers are
fed to a `DT::Job` object, where the analysis algorithm is
executed. At the present time, there are four main functions which a
user will manipulate for their own uses:

* `init()`: run at the beginning of the job
* `setupOutput()`: run immediately following `init()`
* `execute()`: run on each iteration of the event set
* `finish()`: run after the completion of `execute()`

The base class (`DT::AnaBase`) has these functions declared as
`virtual`; therefore the user does not need to build a custom version
in their own analysis. These functions are executed in the above order
by the `DT::Job::run()` function. The function `execute()` is run
inside of a loop over events accessed by ROOT's `TTreeReader`
class. The user's `main` program simply declares a pointer of their
algorithm, and feeds the algorithm to a `DT::Job`. (A few inbetween
steps to properly set up the algorithm will likely be required).

## Example

One can find an example `DukeTop` algorithm in the `example`
directory.

## More Detailed Information

Coming soon.