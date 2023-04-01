#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");

    if( regex_match(program->type, reNum) ){
        return std::atol(program->type.c_str());
        
    // TODO : Check for things matching reId
    }else if(regex_match(program->type, reId)){
        return context.bindings.at(program->type);       
        
    }else if(program->type=="Input"){
        int32_t in;  
        std::cin>> in;
        return in;

    }else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;
        
    }else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;

    }else if(program->type=="LessThan"){
        int32_t A = Interpret(context, program->branches.at(0));
        int32_t B = Interpret(context, program->branches.at(1));
        if(A < B){
            return 1;
        }
        else{
            return 0;
        }
    }else if(program->type=="Add"){
        int32_t A = Interpret(context, program->branches.at(0));
        int32_t B = Interpret(context, program->branches.at(1));
        return A + B;
    }else if(program->type=="Sub"){
        int32_t A = Interpret(context, program->branches.at(0));
        int32_t B = Interpret(context, program->branches.at(1));
        return A - B;
    }else if(program->type=="Assign"){
        int32_t val = Interpret(context, program->branches.at(0));
        context.bindings[program->value] = val;
        return val;   
    }else if(program->type=="Seq"){
        int32_t n = program->branches.size();
        for(int32_t i=0; i < n-1 ; i++){
            Interpret(context, program->branches.at(i));
        }
        return Interpret(context, program->branches.at(n-1));
    }else if(program->type=="If"){
        int32_t C = Interpret(context, program->branches.at(0));
        if(C != 0){
            return Interpret(context, program->branches.at(1));
        }
        else{
            return Interpret(context, program->branches.at(2));
        } 
    }else if(program->type=="While"){
        int32_t C = Interpret(context, program->branches.at(0));
        while(C != 0){
            Interpret(context, program->branches.at(1));
            C = Interpret(context, program->branches.at(0));
        } 
        return 0;
    }else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
