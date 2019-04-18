Why Something Crashed
=====================

Hopefully when TopLoop crashes you get a satisfactory error
message. If not, hopefully his page is a bit more helpful.

Use debugging mode
------------------

The first thing to do if you don't understand the crash is to set the
logger output to ``spdlog::level::debug``. Wherever you're steering
your algorithm (in some ``main`` function). Add the following:

.. code-block:: cpp

   spdlog::set_level(spdlog::level::debug)

Common crash details
--------------------

Branch doesn't exist
^^^^^^^^^^^^^^^^^^^^

You might be asking for a branch that is not available in a particular
tree. For example, all of the weight systematics are usually not
available in a systematic tree. Running in debug mode will tell you
which branches are not available to you before the event loop starts.

You can find the list of available branches in the
`TopLoop/Core/Variables.h
<https://gitlab.cern.ch/TopLoop/TopLoop/blob/master/TopLoop/Core/Variables.h>`_
file. If the branch you need isn't there, you can add it in your
algorithm class by using the ``DECLARE_BRANCH`` and ``CONNECT_BRANCH``
preprocessor macros (as described in the documentation intro section),
or we can just add it to the main list.

Sample DSID is not in meta data
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TopLoop uses the ``SampleMetaSvc`` class for handling a lot of meta
data. We keep a JSON file mapping DSIDs to metadata `here
<https://gitlab.cern.ch/TopLoop/TopLoop/blob/master/data/samplemeta.json>`_. If
the DSID you are processing is not in the list, we need to add
it. This might also require a patch to the class - but it's very easy
to make these additions.
