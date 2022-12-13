#ifndef TYPEVAR_H
#define TYPEVAR_H

#include <string>
#include "hw3_output.h"
using std::string;
#include <memory>

#define YYSTYPE std::shared_ptr<TypeVar>

class TypeVar
{
    public:
        int value{};
        string id;
        string type = "none";
        std::vector<string> name_list;
        std::vector<string> type_list;
    TypeVar() = default;
    ~TypeVar() = default;
};

void check_valid_num(std::shared_ptr<TypeVar> var, int lineno);
void check_valid_bool(std::shared_ptr<TypeVar> var, int lineno);
void check_valid_assign(std::shared_ptr<TypeVar> var1, std::shared_ptr<TypeVar> var2, int lineno);
void check_valid_cast(std::shared_ptr<TypeVar> var1, std::shared_ptr<TypeVar> var2, int lineno);
void byte_overflow(std::shared_ptr<TypeVar> var, int lineno);
string get_type(std::shared_ptr<TypeVar> var1, std::shared_ptr<TypeVar> var2);
std::vector<string> insert_to_vars_list(std::vector<string> current_list, string param);


#endif