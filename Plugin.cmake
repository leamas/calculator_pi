# ~~~
# Summary:      Local, non-generic plugin setup
# Copyright (c) 2020-2021 Mike Rossiter
# License:      GPLv3+
# ~~~

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.


# -------- Options ----------

set(OCPN_TEST_REPO
    "opencpn/calculator-alpha"
    CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
    "opencpn/calculator-beta"
    CACHE STRING
    "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
    "opencpn/calculator-prod"
    CACHE STRING
    "Default repository for tagged builds not matching 'beta'"
)

option(CALCULATOR_USE_SVG "Use SVG graphics" ON)

#
#
# -------  Plugin setup --------
#
set(PKG_VERSION  3.0.0)
set(PKG_PRERELEASE "")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME Calculator)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME Calculator) # As of GetCommonName() in plugin API
set(PKG_SUMMARY "Calculator Functions")
set(PKG_DESCRIPTION [=[
Many nautical calculations available using this plugin
]=])

set(PKG_AUTHOR "Mike Rossiter")
set(PKG_IS_OPEN_SOURCE "yes")
set(PKG_HOMEPAGE https://github.com/Rasbats/calculator_pi)
set(PKG_INFO_URL https://opencpn.org/OpenCPN/plugins/calculator.html)

SET(SRC
	src/calculator_pi.h
	src/calculator_pi.cpp
	src/icons.h
	src/icons.cpp
	src/calculatorgui.h
	src/calculatorgui.cpp
	src/calculatorgui_impl.cpp
	src/calculatorgui_impl.h
	src/functions.h
	src/functions.cpp
	src/units_conversion.h
	src/units_conversion.cpp
	src/VariableFactory.h
	src/VariableFactory.cpp
	)

SET(MUPARS
	src/muparser/muParserBase.cpp
	src/muparser/muParserBase.h
	src/muparser/muParserBytecode.cpp
	src/muparser/muParserBytecode.h
	src/muparser/muParserCallback.cpp
	src/muparser/muParserCallback.h
	src/muparser/muParser.cpp
	src/muparser/muParserDef.h
	src/muparser/muParserError.cpp
	src/muparser/muParserError.h
	src/muparser/muParserFixes.h
	src/muparser/muParser.h
	src/muparser/muParserStack.h
	src/muparser/muParserToken.h
	src/muparser/muParserTokenReader.cpp
	src/muparser/muParserTokenReader.h
	src/muparser/muParserTemplateMagic.h
)

set(PKG_API_LIB api-16)  #  A directory in libs/ e. g., api-17 or api-16

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers
  # and ocpn::api is available.
  if (CALCULATOR_USE_SVG)
    target_compile_definitions(${PACKAGE_NAME} PUBLIC CALCULATOR_USE_SVG)
  endif ()
endmacro ()

macro(add_plugin_libraries)
  # Add libraries required by this plugin
  add_subdirectory("libs/tinyxml")
  target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

  add_subdirectory("libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)

  add_subdirectory("libs/plugingl")
  target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)
endmacro ()

