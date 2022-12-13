#include "TypeVar.h"

void IsNum(std::shared_ptr<TypeVar> var, int lineno)
{
    if (var->type == "INT" || var->type == "BYTE" || var->type == "none"){
        return;
    }
    output::errorMismatch(lineno);
    exit(1);

}

void IsBool(std::shared_ptr<TypeVar> var, int lineno)
{
    if (var->type == "BOOL" || var->type == "none"){
        return;
    }
    output::errorMismatch(lineno);
    exit(1);

}

string GetType(std::shared_ptr<TypeVar> var1, std::shared_ptr<TypeVar> var2)
{
    if (var1->type == "INT" || var2->type == "INT"){
        return "INT";
         
    }
    else{
        return "BYTE";
    }
}

std::vector<string> PushBackVar(std::vector<string> current_list, string param)
{
    current_list.push_back(param);
    return current_list;
}

void check_valid_assign(std::shared_ptr<TypeVar> var1, std::shared_ptr<TypeVar> var2, int lineno)
{
    if(var1->type == var2->type){
        return;
    }
    if(var1->type == "INT" && var2->type == "BYTE"){
        return;
    }
    output::errorMismatch(lineno);
    exit(1);
}

void ByteErr(std::shared_ptr<TypeVar> var, int lineno)
{
    if (var->value > 0xFF){
        output::errorByteTooLarge(lineno, to_string(var->value));
        exit(1);
    }
}