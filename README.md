# TopLoop

`TopLoop` is a small library inspired by the ASG
[`EventLoop`](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/EventLoop)
package. `TopLoop` is built specifically for analyzing Top group
ntuples.

### Authors

* Douglas Davis < [douglas.davis@cern.ch](douglas.davis@cern.ch) >

## Structure of a `TopLoop` algorithm

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


## Installation

### Requirements

* ROOT version 6+
* Boost libraries

### Building

`TopLoop` currently supports two build methods:

1. CMake
2. Old school (`./configure; make`)

#### CMake

To build with cmake, it is suggested to have a working environment
with the following structure:

    .
    └── WorkingArea
        ├── TopLoop
        ├── TopLoopBuild
        └── TopLoopInstall

In the `TopLoopBuild` directory, run:

    $ cmake -DCMAKE_INSTALL_PREFIX=/path/to/WorkingArea/TopLoopInstall ../TopLoop
    $ make
    $ make install

This will install the library and headers to `TopLoopInstall/include`
and `Duke/TopInstall/lib`, respectively.  Other folders from the
package will also be copied over.

If your Boost installation location isn't automatically found by
CMake, then you need to direct CMake to it's location with the flag
`-DBOOST_ROOT=/path/to/boost/installation`.

#### Old school

To build with `./configure; make`, in the `TopLoop` directory simply
run

    $ ./configure
    $ make

This will install the library to `TopLoop/lib`.  If your Boost
installation is not handled by a package manager (i.e. it's not in
`/usr/local` or `/usr`, you likely need to direct the configure script
to the include and library directories for your Boost installation:

    $ ./configure --boost-inc /path/to/boost/include --boost-lib /path/to/boost/lib
    $ make

A few more details: The configure script automatically generates two
files, Makefile and Makefile.common.  Makefile.common is useful to
include in a Makefile written for compiling your own algorithm (see
the example).

## Example

One can find an example `TopLoop` algorithm in the `example`
directory.

## More Details

See the wiki (under construction).