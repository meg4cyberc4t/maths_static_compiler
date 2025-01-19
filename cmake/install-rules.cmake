install(
    TARGETS maths_static_compiler_exe
    RUNTIME COMPONENT maths_static_compiler_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
