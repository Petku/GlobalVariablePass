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
#include "llvm/Support/Format.h"
#include "llvm/ADT/APInt.h"
#include "llvm/IR/Instructions.h"


using namespace llvm;

static cl::opt<std::string> OutputGlobalVariable("var", cl::desc("Specifies name of the global variable"), cl::value_desc("global_variable_name"));

namespace {
    void writeConstant(raw_ostream &Out, const Constant *CV);

    struct GlobalVariablePass: public ModulePass {
        static char ID;
        GlobalVariablePass() : ModulePass(ID) {}

        bool runOnModule(Module &M) override {
            Constant *value;
            if(OutputGlobalVariable.empty()) { // If option -var not used write all global variables.
                for (auto &i: M.getGlobalList()) {
                    if (i.hasInitializer()) {
                        value = i.getInitializer();
                        writeConstant(errs(), value);
                        errs() << "\n";
                    }
                }
                return false;
            }

            auto variable = M.getGlobalVariable(OutputGlobalVariable, false);
            if(variable && variable->hasInitializer()){
                value = variable->getInitializer();
                writeConstant(errs(), value);
                errs() << "\n";
            }

            return false;
        }
    };

    void writeConstant(raw_ostream &Out, const Constant *CV)
    {
        if (const ConstantInt *CI = dyn_cast<ConstantInt>(CV)) {
            if (CI->getType()->isIntegerTy(1)) {
                 Out << (CI->getZExtValue() ? "true" : "false");
                return;
            }
            APInt AI = CI->getValue();
            if( CI->getBitWidth() == 8) { // if sizeof constant == sizeof char
                const uint64_t *letter = AI.getRawData();
                if(char letter2 = (char) (*letter)) {
                    Out << letter2;
                    return;
                }
            }
            Out << CI->getValue();
            return;
        }

        if (const ConstantFP *CFP = dyn_cast<ConstantFP>(CV)) {
            const APFloat &APF = CFP->getValueAPF();
            if (&APF.getSemantics() == &APFloat::IEEEsingle() ||
                &APF.getSemantics() == &APFloat::IEEEdouble()) {
                // We would like to output the FP constant value in exponential notation,
                // but we cannot do this if doing so will lose precision.  Check here to
                // make sure that we only output it in exponential format if we can parse
                // the value back and get the same value.
                //
                bool ignored;
                bool isDouble = &APF.getSemantics() == &APFloat::IEEEdouble();
                bool isInf = APF.isInfinity();
                bool isNaN = APF.isNaN();
                if (!isInf && !isNaN) {
                    double Val = isDouble ? APF.convertToDouble() : APF.convertToFloat();
                    Out << Val ;
                }
                else if(isInf)
                    Out << "inf";
                else if(isNaN)
                    Out << "NaN";
                return;
            }
        }


        if (auto *CA = dyn_cast<ConstantArray>(CV)) {
            Type *ETy = CA->getType()->getElementType();
            Out << '[';
            Out << ' ';
            writeConstant(Out, CA->getOperand(0));
            for (unsigned i = 1, e = CA->getNumOperands(); i != e; ++i) {
                Out << ", ";
                Out << ' ';
                writeConstant(Out, CA->getOperand(i));
            }
            Out << ']' ;
            return;
        }

        if (auto *CA = dyn_cast<ConstantDataArray>(CV)) {
            // As a special case, print the array as a string if it is an array of
            // i8 with ConstantInt values.
            if (CA->isString()) {
                Out << CA->getAsString() ;
                return;
            }

            Out << '[';
            Out << ' ';
            writeConstant(Out, CA->getElementAsConstant(0));
            for (unsigned i = 1, e = CA->getNumElements(); i != e; ++i) {
                Out << ", ";
                Out << ' ';
                writeConstant(Out, CA->getElementAsConstant(i));
            }
            Out << ']';
            return;
        }

        if (const ConstantStruct *CS = dyn_cast<ConstantStruct>(CV)) {
            if (CS->getType()->isPacked())
                Out << '<';
            Out << '{';
            unsigned N = CS->getNumOperands();
            if (N) {
                Out << ' ';

                writeConstant(Out, CS->getOperand(0));

                for (unsigned i = 1; i < N; i++) {
                    Out << ", ";

                    writeConstant(Out, CS->getOperand(i));
                }
                Out << ' ';
            }

            Out << '}';
            if (CS->getType()->isPacked())
                Out << '>';
            return;
        }

        if (isa<ConstantVector>(CV) || isa<ConstantDataVector>(CV)) {
            Out << '<';
            Out << ' ';
            writeConstant(Out, CV->getAggregateElement(0U));
            for (unsigned i = 1, e = CV->getType()->getVectorNumElements(); i != e;++i){
                Out << ", ";
                writeConstant(Out, CV->getAggregateElement(i));
            }
            Out << '>' ;
            return;
        }

        if (isa<ConstantAggregateZero>(CV)) {
            Out << "zeroinitializer";
            return;
        }

        if (isa<ConstantPointerNull>(CV)) {
            Out << "null";
            return;
        }

        if (isa<ConstantTokenNone>(CV)) {
            Out << "none";
            return;
        }

        if (isa<UndefValue>(CV)) {
            Out << "undef";
            return;
        }

        if (const ConstantExpr *CE = dyn_cast<ConstantExpr>(CV)) {
            Value *firstop = CE->getOperand(0);
            if (GlobalVariable *GV = dyn_cast<GlobalVariable>(firstop)) {
                Constant *v = GV->getInitializer();
                writeConstant(Out, v);
            }
            return;
        }

        Out << "None of searched constant types";
    }
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
