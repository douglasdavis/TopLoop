TopLoop Analysis Package
========================

TopLoop is a small library inspired by the ASG EventLoop package.
TopLoop is built specifically for analyzing Single Top ntuples output
by the TopAnalysis ``top-xaod`` program with the ``SingleTopAnalysis``
software loaded. Technically, this software can analyze *any* ntuple,
but the structure and API are tailored for ntuples output by
``top-xaod`` steered by ``SingleTopAnalysis``.

TopLoop delivers a base algorithm class, a job runner, a file manager,
and a small event data model. The algorithm has the following structure:

Components of an Algorithm
--------------------------

-  ``init()``: run at the beginning of the job
-  ``setupOutput()``: run immediately following init()
-  ``execute()``: run on each iteration of the event set
-  ``finish()``: run after the completion of all execute() calls

Running an Algorithm
--------------------

To run an algorithm we give it to the ``TL::Job`` class. We also have
to give the job a ``TL::FileManager`` class. A short example:

.. code-block:: cpp

   int main() {
     // we make a smart pointer to your algorithm which inherits from TL::Algorithm
     auto alg = std::make_unique<YourAlgoritmClass>();
     // a smart pointer to a file manager
     auto fm = std::make_unique<TL::FileManager>();
     // if you want access to particle level and truth information -- must be called before feeding
     fm->enableParticleLevel();
     fm->feedDir("/path/to/sgtop/ntuple/dataset");

     TL::Job job;
     // See API for other loop types; RecoStandard is the default
     // here we'll enable particle level information
     job.setLoopType(TL::LoopType::RecoWithParticle);

     // give the file manager and algorithm to the job
     TL_CHECK(job.setFileManager(std::move(fm)));
     TL_CHECK(job.setAlgorithm(std::move(alg)));
     return job.run();
   }

TopLoop makes use of status code returns (similar to other ATLAS
code). The ``TL_CHECK`` macro is necessary if you don't want to use the
``TL::StatusCode::isSuccess()`` function.


TopLoop Variable Access
-----------------------

All tree variables (branches) are stored as smart pointers to
``TTreeReaderValue<T>`` objects. The pointer object names correspond to
a built-in prefix (``bv__``) plus the name of the branch
(e.g. ``bv__el_pt`` for the branch ``el_pt``). The variables are updated
on each iteration behind the scenes (in the ``TL::Job`` implementation).
The user does not need to worry about the variables updating! Two macros
handle everything related to setting up variables and access to them.
The user just “calls” the variable like one calls a function (more
details below).

A list of available variables are found in
`TopLoop/Core/Variables.h <https://gitlab.cern.ch/atlasphys-top/singletop/tW_13TeV_Rel21/TopLoop/blob/master/TopLoop/Core/Variables.h>`__

An example code block (using the built-in EDM):

.. code-block:: cpp

   TL::StatusCode MyAlgorithm::execute() {
     //...
     TL::EDM::FinalState finalState;
     auto averageMu = mu();
     for ( std::size_t i = 0; i < el_pt().size(); ++i ) {
       TL::EDM::Electron elec;
       auto pt  = el_pt().at(i);
       auto eta = el_eta().at(i);
       auto phi = el_phi().at(i);
       elec.p4().SetPtEtaPhiM(pt,eta,phi,0.511)
       finalState.addElectron(elec);
     }
     // ... use the final state
   }

Particle level and truth variables
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If particle level access has been enabled in your ``TL::FileManager``
and ``TL::Job``, then the algorithm will have access to branches in
the ``particleLevel`` and ``truth`` trees. Particle level variables
are prefixed with ``PL_`` and truth variables are prefixed with
``truth_``. For example, accessing the ``el_pt`` branch from the
particle level tree, and the ``MC_t_beforeFSR_pt`` branch from the
truth tree:

.. code:: cpp

   std::size_t n_partLevel_electrons = PL_el_pt().size();
   if ( n_partLevel_electrons > 1 ) {
     float second_electron_pt_partLevel = PL_el_pt().at(1) * toGeV;
   }
   float top_pt = truth_MC_t_beforeFSR_pt() * toGeV;

Adding custom variable access
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

It’s very easy to add additional variable access in a TopLoop based
algorithm. If your analysis adds more output on top of the standard
AnalysisTop variables, you’ll need this.

There are two macros defined: ``DECLARE_BRANCH`` and ``CONNECT_BRANCH``.
In the header, we use the former, it just takes the name of the branch
and the type. Then in the implementation of your algorithm’s ``init()``
function, we use the latter and say which TTreeReader pointer to connect
to.

An example where the additional variable of interest is called
``el_foo`` exists:

.. code-block:: cpp

   // in the header class definition.
   DECLARE_BRANCH(el_foo, std::vector<float>)

.. code-block:: cpp

   // in your init() implementation
   CONNECT_BRANCH(el_foo, std::vector<float>, reader());

.. code-block:: cpp

   // in your execute() implementation
   for ( const auto& el_foo_itr : el_foo() ) {
     auto el_foo_squared = el_foo_itr*el_foo_itr;
   }

Under the hood, the macros create a
``std::unique_ptr<TTreeReaderValue<T>>`` and a public function to access
the value via a double derefencing. If you imagine the top ntuple as a
class itself (or perhaps a pandas.DataFrame) then the function call
structure is intuitive.

For particle level variables, the macros are ``DECLARE_PL_BRANCH`` and
``CONNECT_PL_BRANCH``.

Example Algorithm
-----------------

A small example project: `ExampleProject
<https://gitlab.cern.ch/TopLoop/ExampleProject>`_.


We have a mature TopLoop based algorithm for the tW dilepton analysis:
`WtLoop
<http://gitlab.cern.ch/atlasphys-top/singletop/tW_13TeV_Rel21/WtLoop>`_.
