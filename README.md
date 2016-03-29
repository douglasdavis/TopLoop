# TopLoop

`TopLoop` is a small library inspired by the ASG
[`EventLoop`](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/EventLoop)
package. `TopLoop` is built specifically for analyzing Top group
ntuples.

### Authors

* Douglas Davis < [douglas.davis@cern.ch](douglas.davis@cern.ch) >

## Installation

### Requirements

* CMake
* ROOT version 6+
* Boost libraries

### Building

To get starting building and setting up a working environment, it is
suggested to have the following structure:

    .
    └── TLWorkArea
        ├── TopLoop
        ├── TopLoopBuild
        └── TopLoopInstall


In the `TopLoopBuild` directory, run:

    $ cmake -DCMAKE_INSTALL_PREFIX=/path/to/WorkingArea/TopLoopInstall ../TopLoop
    $ make
    $ make install

This will install the library and headers to `TopLoopInstall/include`
and `TopLoopInstall/lib`, respectively.  Other folders from the
package will also be copied over.

If your Boost installation location isn't automatically found by
CMake, then you need to direct CMake to it's location with the flag
`-DBOOST_ROOT=/path/to/boost/installation`.

An optional flag to build the included example exists:
`-DBUILDEX`. Therefore one can build the example with:

    $ cmake -DCMAKE_INSTALL_PREFIX=/path/to/WorkingArea/TopLoopInstall ../TopLoop -DBUILDEX=ON

### The author's setup:

I have a function in my shell's init file (.bashrc) of the form:

```bash
setupTopLoop() {
    export TL_SOURCE=$HOME/Software/TLWorkArea/TopLoop
    export TL_INSTALL=$HOME/Software/TLWorkArea/install
    export TL_BUILD=$HOME/Software/TLWorkArea/build
    export PATH=$PATH:$TL_INSTALL/bin
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TL_INSTALL/lib
    alias cleanTLinstall='rm -rf ${TL_INSTALL}/*'
    cd $TL_SOURCE/..
}
```

Which I can then build with (in my `$TL_BUILD` directory):

    $ cmake -DCMAKE_INSTALL_PREFIX=$TL_INSTALL $TL_SOURCE -DBUILDEX=ON

And now I also have a set of environment variables for navigating the
development environment.

### For `lxplus` like environments

Boost is a small headache on `lxplus` like environments. In order to
link with Boost, we must have a RootCore environment to help us
out. It is suggested that have your RootCore area be above the
`TLWorkArea` quoted above.  The basic setup would be of the form:

    .
    └── AnalysisLand_Top2.3.44
        ├── RootCoreBin
        ├── TLWorkArea
        ├── rcSetup.csh
        └── rcSetup.sh


With a proper `rcSetup`ed environment
(e.g. `rcSetup,Top,2.3.44`), we can point CMake to the Boost
dependencies by running (in `$TL_BUILD`):

    $ cmake -DCMAKE_INSTALL_PREFIX=$TL_INSTALL $TL_SOURCE \
      -DBoost_INCLUDE_DIR=$ROOTCOREBIN/include \
      -DBoost_LIBRARY_DIR=$ROOTCOREBIN/lib/x86_64-slc-gcc49-opt \
      -DCMAKE_CXX_COMPILER=`which g++` \
      $TL_SOURCE -DBUILDEX=ON

If using a different RootCore release, `lib/x86_64-slc-gcc49-opt`
might be different! Note that the `which g++` might be necessary
on some systems to ensure that CMake does not find a locally
installed g++, and instead looks in cvmfs.

## Example

One can find an example `TopLoop` algorithm in the `example`
directory.

## Structure of a `TopLoop` algorithm

Packages will inherit from `TL::AnaBase` (similar to how EventLoop
algorithms inherit from `EL::Algorithm`). `TL::AnaBase` pointers are
fed to a `TL::Job` object, where the analysis algorithm is
executed. At the present time, there are four main functions which a
user will manipulate for their own uses:

* `init()`: run at the beginning of the job
* `setupOutput()`: run immediately following `init()`
* `execute()`: run on each iteration of the event set
* `finish()`: run after the completion of `execute()`

The base class (`TL::AnaBase`) has these functions declared as
`virtual`; therefore the user does not need to build a custom version
in their own analysis. These functions are executed in the above order
by the `TL::Job::run()` function. The function `execute()` is run
inside of a loop over events accessed by ROOT's `TTreeReader`
class. The user's `main` program simply declares a pointer of their
algorithm, and feeds the algorithm to a `TL::Job`. (A few inbetween
steps to properly set up the algorithm will likely be required).

## More Details

See the wiki (under construction).
