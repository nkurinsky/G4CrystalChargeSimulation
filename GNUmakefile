# G4CMP/charge/GNUmakefile -- for building charge-carrier propagation demo
# $Id: a55c84d6afb4ae3bb2f66e369e4018cb5cc2ea6e $
#
# 20140509  Support different executables by looking at target name
# 20180509  Updated to work with G4CMP wherever it is


ifndef G4CMP_NAME		# Two-pass method
  .PHONY : g4TestXtal

  g4TestXtal :
	$(MAKE) G4CMP_NAME=$@ bin

  G4CMP_NAME := g4TestXtal
endif

include $(G4CMPINSTALL)/g4cmp.gmk
