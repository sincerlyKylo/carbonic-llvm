add_lld_library(lldCex
  Driver.cpp
  ArgumentParser.cpp
  CEXLinkerContext.cpp
  CEXLinkerServer.cpp

  LINK_COMPONENTS
  Support
  TargetParser

  LINK_LIBS
  lldCommon
  )