#!/usr/bin/env python2

import sys
if len(sys.argv) < 2:
    sys.exit('give dir')
else:
    read_dir = sys.argv[1]

import ROOT

ROOT.gSystem.Load("$TL_INSTALL/lib/libTopLoop")
ROOT.gSystem.Load("$TL_INSTALL/lib/libTopLoopEDM")
ROOT.gSystem.Load("$TL_INSTALL/lib/libMyTopLoopAna")
ROOT.gSystem.Load("$TL_INSTALL/lib/libttZloop")

from ROOT import ttZloop
from ROOT import TL

ana = ttZloop()
ana.setOutfileName('ttZout.root')
job = TL.Job(ana)
ana.fileManager().feedDir(read_dir)
job.run()
