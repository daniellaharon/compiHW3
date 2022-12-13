#include "TypeVar.h"

void check_valid_num(std::shared_ptr<TypeVar> var, int lineno)
{
    if (var->type == "INT" || var->type == "BYTE" || var->type == "none"){
        return;
    }
    output::errorMismatch(lineno);
    exit(1);

}

void check_valid_bool(std::shared_ptr<TypeVar> var, int lineno)
{
    if (var->type == "BOOL" || var->type == "none"){
        return;
    }
    output::errorMismatch(lineno);
    exit(1);

}

string get_type(std::shared_ptr<TypeVar> var1, std::shared_ptr<TypeVar> var2)
{
    if (var1->type == "INT" || var2->type == "INT"){
        return "INT";
         
    }
    else{
        return "BYTE";
    }
}

void check_valid_cast(std::shared_ptr<TypeVar> var1, std::shared_ptr<TypeVar> var2, int lineno)
{
    check_valid_num(var1, lineno);
    check_valid_num(var2, lineno);
}

std::vector<string> insert_to_vars_list(std::vector<string> current_list, string param)
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

void byte_overflow(std::shared_ptr<TypeVar> var, int lineno)
{
    if (var->value > 0xFF){
        output::errorByteTooLarge(lineno, to_string(var->value));
        exit(1);
    }
}