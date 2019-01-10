API for EDM
===========

Classes
-------

The EDM contains a number of classes for handling physics objects

TL::EDM::FinalState
^^^^^^^^^^^^^^^^^^^

.. doxygenclass:: TL::EDM::FinalState
   :project: TopLoop
   :members:
   :protected-members:
   :undoc-members:

TL::EDM::PhysicsObject
^^^^^^^^^^^^^^^^^^^^^^

.. doxygenclass:: TL::EDM::PhysicsObject
   :project: TopLoop
   :members:
   :protected-members:
   :undoc-members:

TL::EDM::Lepton
^^^^^^^^^^^^^^^

.. doxygenclass:: TL::EDM::Lepton
   :project: TopLoop
   :members:
   :protected-members:

TL::EDM::Electron
^^^^^^^^^^^^^^^^^

.. doxygenclass:: TL::EDM::Electron
   :project: TopLoop
   :members:
   :protected-members:
   :undoc-members:

TL::EDM::Muon
^^^^^^^^^^^^^

.. doxygenclass:: TL::EDM::Muon
   :project: TopLoop
   :members:
   :protected-members:
   :undoc-members:

TL::EDM::Jet
^^^^^^^^^^^^

.. doxygenclass:: TL::EDM::Jet
   :project: TopLoop
   :members:
   :protected-members:
   :undoc-members:

TL::EDM::MissingET
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: TL::EDM::MissingET
   :project: TopLoop
   :members:
   :protected-members:
   :undoc-members:

TL::EDM::LeptonPair
^^^^^^^^^^^^^^^^^^^

.. doxygenclass:: TL::EDM::LeptonPair
   :project: TopLoop
   :members:
   :protected-members:
   :undoc-members:

Helper Functions
----------------

The helper functions are designed to look at physical systems. A
lightweight ``PhysicsSystem`` class is provided to wrap just four
vector information.

.. doxygenclass:: TL::EDM::PhysicsSystem
   :project: TopLoop
   :members:
   :undoc-members:

.. doxygenfunction:: TL::EDM::pTsys
   :project: TopLoop

.. doxygenfunction:: TL::EDM::sigma_pTsys
   :project: TopLoop

.. doxygenfunction:: TL::EDM::HTsys
   :project: TopLoop

.. doxygenfunction:: TL::EDM::Hsys
   :project: TopLoop

.. doxygenfunction:: TL::EDM::centrality
   :project: TopLoop

.. doxygenfunction:: TL::EDM::deltaR
   :project: TopLoop

.. doxygenfunction:: TL::EDM::deltapT
   :project: TopLoop

.. doxygenfunction:: TL::EDM::deltaphi
   :project: TopLoop

.. doxygenfunction:: TL::EDM::transverseMass
   :project: TopLoop

.. doxygenfunction:: TL::EDM::energyMassRatio
   :project: TopLoop

.. doxygenfunction:: TL::EDM::thrust
   :project: TopLoop
