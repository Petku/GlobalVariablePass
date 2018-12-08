add_llvm_loadable_module(LLVMGlobVars
     globvars.cpp
 )

set_target_properties(LLVMGlobVars PROPERTIES
        COMPILE_FLAGS "-fno-rtti"
        )