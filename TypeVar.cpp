#include "TypeVar.h"

void IsNum(std::shared_ptr<TypeVar> var, int lineno)
{
    if (var->type == INT_EXP || var->type == BYTE_EXP || var->type == NONE)
    {
        return;
    }
    output::errorMismatch(lineno);
    exit(1);

}

void IsBool(std::shared_ptr<TypeVar> var, int lineno)
{
    if (var->type == BOOL_EXP || var->type == NONE)
    {
        return;
    }
    output::errorMismatch(lineno);
    exit(1);

}

ExpType GetType(std::shared_ptr<TypeVar> var1, std::shared_ptr<TypeVar> var2)
{
    if (var1->type == INT_EXP || var2->type == INT_EXP)
    {
        return INT_EXP;

    }
    else{
        return BYTE_EXP;
    }
}

std::vector<string> PushBackVar(std::vector<string> current_list, string param)
{
    current_list.push_back(param);
    return current_list;
}

std::vector<ExpType> PushBackVarType(std::vector<ExpType> current_list, ExpType param)
{
    current_list.push_back(param);
    return current_list;
}

void CheckAssign(std::shared_ptr<TypeVar> var1, std::shared_ptr<TypeVar> var2, int lineno)
{
    if(var1->type == var2->type){
        return;
    }
    if(var1->type == INT_EXP && var2->type == BYTE_EXP)
    {
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