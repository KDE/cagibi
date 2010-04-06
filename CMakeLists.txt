project( Cagibi )

find_package( KDE4 REQUIRED )

include( KDE4Defaults )
include( MacroLibrary )

add_definitions( ${KDE4_DEFINITIONS} )
include_directories(
  ${KDE4_INCLUDES}
  ${KDEPIMLIBS_INCLUDE_DIR}
)

macro_optional_add_subdirectory( daemon )
