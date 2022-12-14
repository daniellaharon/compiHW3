#include "TypeVar.h"


void ByteErr(std::shared_ptr<TypeVar> var, int lineno)
{
    if (var->value > 0xFF){
        output::errorByteTooLarge(lineno, to_string(var->value));
        exit(1);
    }
}

string GetType(std::shared_ptr<TypeVar> left, std::shared_ptr<TypeVar> right)
{
    if (left->type == "INT" || right->type == "INT"){
        return "INT";

    }
    else{
        return "BYTE";
    }
}

void IsNum(std::shared_ptr<TypeVar> var, int lineno)
{
    if (!(var->type == "INT" || var->type == "BYTE" || var->type == "none")){
        output::errorMismatch(lineno);
        exit(1);
    }
}

void IsBool(std::shared_ptr<TypeVar> var, int lineno)
{
    if (!(var->type == "BOOL" || var->type == "none")){
        output::errorMismatch(lineno);
        exit(1);
    }
}

void CheckAssign(std::shared_ptr<TypeVar> left, std::shared_ptr<TypeVar> right, int lineno)
{
    if(!((left->type == "INT" && right->type == "BYTE")||(left->type == right->type))){
        output::errorMismatch(lineno);
        exit(1);
    }

}


std::vector<string> PushBackVar(std::vector<string> vec, string param)
{
    vec.push_back(param);
    return vec;
}


