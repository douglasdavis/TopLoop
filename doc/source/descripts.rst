Class Descriptions
==================

The Core Classes
----------------

The core classes have their headers in ``TopLoop/Core``

The Algorithm and Variables Classes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The introduction already mentioned the canonical ``TL::Algorithm``
functions that must be defined for every TopLoop function. Every
TopLoop algorithm must implement this functions, but the Algorithm
class comes with a bit more. It "hosts" all of the branches that you
want to access during your ntuple processing. It does this by
inheriting from the ``TL::Variables`` class. It also is injected with
the ``TL::WeightTool`` (described below) which gives you convenient
ways to retrieve all of the crazy mess of weights that are required
for doing an ATLAS analysis.

The FileManager Class
^^^^^^^^^^^^^^^^^^^^^

The ``TL::FileManager`` class is pretty self explanatory. It manages
the files that are part of a single top ntuple sample. It is "fed" by
two functions: either ``feedDir`` or ``feedTxt``. ``feedDir`` is the
most convenient - you just point it to the path on disk where the
sample exists. ``feedTxt`` requires that the file name be the same as
the single top ntuple sample name, so it should be used for more
specialized cases. The ``FileManager`` class then takes advantage of
the ``SampleMetaSvc`` class (described below) to extract information
-- see the API for more.

The Job Class
^^^^^^^^^^^^^

The ``TL::Job`` class does the event loop execution. You have a few
choices of how you'd like to run the loop. You can run a
``RecoStandard`` loop - and it will process just the tree name that
you provided to the ``FileManager`` (e.g. "nominal" or
"EG_SCALE_ALL__1up"). If you choose to execute a loop that is
``RecoWithParticle`` then you get accesss to all of the standard
reconstruction information, while in parallel getting the particle and
truth (hard scatter particles) level information from the
``particleLevel`` and ``truth`` SgTop ntuple trees.

The SampleMetaSvc Class
^^^^^^^^^^^^^^^^^^^^^^^

This is a Singleton class that is available at _all_ points in your
code. It reads in a JSON file that lives in the ``TopLoop``
repoository and constructs a whole bunch of meta data about all of the
samples we use (it's very easy to add new samples to it). It also has
other convenient functions for determining if a sample is Full or Fast
Sim, or what campaign it's from, or its SgTop ntuple version based on
the SgTop ntuple sample name. See the API for more.

The WeightTool Class
^^^^^^^^^^^^^^^^^^^^

The ``TL::WeightTool`` class is injected into the Algorithm class and
allows you to extract a lot of useful information. First of all, it
gets the sum of weights for the "nominal" setup, and can also grab
generator variation normalizations and individual weights for you
(from the ``totalEventsWeighted_mc_generator_weights`` and
``names_mc_generator_weights`` branches in the ``sumWeights``
tree. Secondly, it also handles cross section/k-factor
information. See the API!

The Data Model Classes
----------------------

The data model classes are summarized in the introduction section. See
API documentation for more.
