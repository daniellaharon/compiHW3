#ifndef TYPEVAR_H
#define TYPEVAR_H

#include <string>
#include "hw3_output.h"
using std::string;
#include <memory>

#define YYSTYPE std::shared_ptr<TypeVar>

enum ExpType{
    NONE,
    INT_EXP,
    BYTE_EXP,
    BOOL_EXP,
    STRING_EXP,
    VOID_EXP
};

class TypeVar
{
public:
    int value{};
    string id;
    ExpType type = NONE;
    std::vector<string> name_list;
    std::vector<ExpType> type_list;
    TypeVar() = default;
    ~TypeVar() = default;
};

void IsNum(std::shared_ptr<TypeVar> var, int lineno);
void IsBool(std::shared_ptr<TypeVar> var, int lineno);
void CheckAssign(std::shared_ptr<TypeVar> var1, std::shared_ptr<TypeVar> var2, int lineno);
void IsDiffAndBadAssign(std::shared_ptr<TypeVar> var1, std::shared_ptr<TypeVar> var2, int lineno);
void ByteErr(std::shared_ptr<TypeVar> var, int lineno);
ExpType GetType(std::shared_ptr<TypeVar> var1, std::shared_ptr<TypeVar> var2);
ExpType GetType2(std::shared_ptr<TypeVar> var1, std::shared_ptr<TypeVar> var2);
std::vector<string> PushBackVar(std::vector<string> current_list, string param);
std::vector<ExpType> PushBackVarType(std::vector<ExpType> current_list, ExpType param);

#endif