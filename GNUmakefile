# G4CMP/charge/GNUmakefile -- for building charge-carrier propagation demo
# $Id: a55c84d6afb4ae3bb2f66e369e4018cb5cc2ea6e $
#
# 20140509  Support different executables by looking at target name

ifndef G4CMP_NAME		# Two-pass method
  .PHONY : g4cmpCharge

  g4cmpCharge :
	@$(MAKE) G4CMP_NAME=$@ bin

  G4CMP_NAME := g4cmpCharge
endif

include ../g4cmp.gmk
