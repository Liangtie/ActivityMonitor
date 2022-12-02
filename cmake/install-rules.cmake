install(
    TARGETS BrixActivityWatch
    RUNTIME COMPONENT BrixActivityWatch_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
