add_lld_library(lldCex
  CexDriver.cpp

  LINK_COMPONENTS
  Support
  TargetParser

  LINK_LIBS
  lldCommon
  )