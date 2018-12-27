//
// Created by nikolaspatrik on 8.12.2018.
//

#include <string>
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Constants.h"



using namespace llvm;

static cl::opt<std::string> OutputGlobalVariable("var", cl::desc("Specifies name of the global variable"), cl::value_desc("global_variable_name"));


namespace {
    struct GlobalVariablePass: public ModulePass {
        static char ID;
        GlobalVariablePass() : ModulePass(ID) {}

        bool runOnModule(Module &M) override {
            Constant *value;
            if(OutputGlobalVariable.empty()) { // If option -var not used write all global variables.
                for (auto &i: M.getGlobalList()) {
                    if (i.hasInitializer())
                        value = i.getInitializer();
                    errs() << i.getName() << " = " << *value << "\n";
                }
                return false;
            }

            auto variable = M.getGlobalVariable(OutputGlobalVariable, false);
            if(variable->hasInitializer()){
                value = variable->getInitializer();
            errs() << variable->getName() << " = " << *value << "\n";
            }

            return false;
        }
    };
}

char GlobalVariablePass::ID = 0;
static RegisterPass<GlobalVariablePass> X("globvar", "Global Variable Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
/**
 * Pass for testing script not important !!! Only writes all global variables except internals and starting with str.
 */
namespace {
    struct GlobalVariablePass2: public ModulePass {
        static char ID;
        GlobalVariablePass2() : ModulePass(ID) {}

        bool runOnModule(Module &M) override {
            for (auto &i: M.getGlobalList()) {
                if(!i.getName().startswith(".") && !i.getName().startswith("str"))
                    errs() << i.getName() << " " ;
            }
            return false;
        }
    };
}


char GlobalVariablePass2::ID = 0;
static RegisterPass<GlobalVariablePass2> Y("globvarlist", "Global Variable Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
