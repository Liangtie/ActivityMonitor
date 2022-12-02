target_sources(BrixActivityWatch PRIVATE
helper_ui.cpp
sqlite3.c
)

if(${MACOSX})
target_sources(BrixActivityWatch PRIVATE
macutilities.mm
)
endif()
