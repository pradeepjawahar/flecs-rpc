FILE(REMOVE_RECURSE
  "CMakeFiles/gen_agent_interface"
  "agent.h"
  "agent.cpp"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/gen_agent_interface.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
