#include "ast.hpp"

#include <string>
#include <regex>

static int makeNameUnq=0;

static std::string makeName(std::string base)
{
    return "_"+base+"_"+std::to_string(makeNameUnq++);
}

void CompileRec(
    std::string destReg,    // The name of the register to put the result in
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match( program->type, reNum ) ){
        std::cout<<"const "<<destReg<<" "<<program->type<<std::endl;
        
    }else if( regex_match( program->type, reId ) ){
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout<<"add "<<destReg<<" "<<program->type<<" "<<zero<<std::endl;

    }else if(program->type=="Param"){
        std::cout<<"param "<<destReg<<" "<<program->value<<std::endl;

    }else if(program->type=="Seq"){
        for(unsigned i=0; i<program->branches.size(); i++){
            CompileRec(destReg, program->branches[i]);
        }
       
    }else if(program->type=="Input"){
        std::cout<<"input "<<destReg<<std::endl;

    }else if(program->type=="Output"){
        CompileRec(destReg, program->branches.at(0));
        std::cout<<"output "<<destReg<<std::endl;

    }else if(program->type=="Add"){
        std::string srcRegA = makeName("srcRegA");
        std::string srcRegB = makeName("srcRegB");
        CompileRec(srcRegA, program->branches.at(0));
        CompileRec(srcRegB, program->branches.at(1));
        std::cout<<"add "<<destReg<<" "<<srcRegA<<" "<<srcRegB<<std::endl;

    }else if(program->type=="Sub"){
        std::string srcRegA = makeName("srcRegA");
        std::string srcRegB = makeName("srcRegB");
        CompileRec(srcRegA, program->branches.at(0));
        CompileRec(srcRegB, program->branches.at(1));
        std::cout<<"sub "<<destReg<<" "<<srcRegA<<" "<<srcRegB<<std::endl;

    }else if(program->type=="LessThan"){
        std::string srcRegA = makeName("srcRegA");
        std::string srcRegB = makeName("srcRegB");
        CompileRec(srcRegA, program->branches.at(0));
        CompileRec(srcRegB, program->branches.at(1));
        std::cout<<"lt "<<destReg<<" "<<srcRegA<<" "<<srcRegB<<std::endl;

    }else if(program->type=="Assign"){
        std::string srcRegA = makeName("srcRegA");
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        CompileRec(srcRegA, program->branches.at(0));
        std::cout<<"add "<<program->value<<" "<<srcRegA<<" "<<zero<<std::endl;
        std::cout<<"add "<<destReg<<" "<<srcRegA<<" "<<zero<<std::endl;

    }else if(program->type=="If"){
        std::string srcReg = makeName("srcReg");
        std::string zero = makeName("zero");
        std::string label1 = makeName("label1");
        std::string label2 = makeName("label2");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        CompileRec(srcReg, program->branches.at(0));
        std::cout<<"beq "<<srcReg<<" "<<zero<<" "<<label1<<std::endl;
        CompileRec(destReg, program->branches.at(1));
        std::cout<<"bne "<<srcReg<<" "<<zero<<" "<<label2<<std::endl;
        std::cout<<":"<<label1<<std::endl;
        CompileRec(destReg, program->branches.at(2));
        std::cout<<":"<<label2<<std::endl;

    }else if(program->type=="While"){
        std::string srcReg = makeName("srcReg");
        std::string zero = makeName("zero");
        std::string label1 = makeName("label1");
        std::string label2 = makeName("label2");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout<<":"<<label1<<std::endl;
        CompileRec(srcReg, program->branches.at(0));
        std::cout<<"beq "<<srcReg<<" "<<zero<<" "<<label2<<std::endl;
        CompileRec(destReg, program->branches.at(1));
        std::cout<<"bne "<<srcReg<<" "<<zero<<" "<<label1<<std::endl;
        std::cout<<":"<<label2<<std::endl;
        std::cout << "add "<<destReg<<" "<<srcReg<<" "<<zero<<std::endl;

    }else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}

void Compile(
    TreePtr program
){
    // Create a register to hold the final result
    std::string res=makeName("res");
    
    // Compile the whole thing
    CompileRec(res, program);
    
    // Put the result out
    std::cout<<"halt "<<res<<"\n";
}
