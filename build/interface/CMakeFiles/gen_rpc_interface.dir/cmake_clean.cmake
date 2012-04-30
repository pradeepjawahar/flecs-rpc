FILE(REMOVE_RECURSE
  "CMakeFiles/gen_rpc_interface"
  "flecs.h"
  "flecs.cpp"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/gen_rpc_interface.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
