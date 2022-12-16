#ifndef SYMTABLE
#define SYMTABLE

#include "TypeVar.h"
#include <stack>
class symTableBlockEntry
{
public:
    int offset;
    bool is_offset_null;
    string id;
    ExpType type;
    bool is_func = false;
    std::vector<string> name_list;
    std::vector<ExpType> type_list;

    symTableBlockEntry() = default;

    symTableBlockEntry(int offset, bool is_offset_null, string id, ExpType type, bool is_func) :
            offset(offset), is_offset_null(is_offset_null), id(id), type(type), is_func(is_func) {}

    symTableBlockEntry(int offset, bool is_offset_null, string id, ExpType type, bool is_func,
                       std::vector<string> name_list, std::vector<ExpType> type_list) :
            offset(offset), is_offset_null(is_offset_null), id(id), type(type), is_func(is_func),
            name_list(name_list), type_list(type_list) {}
};

typedef struct{
    vector<symTableBlockEntry> sym_tab;
    bool is_while;
    string ret_type;
} symbol_table_block;


// Global variables

void InsertParamsToSymTab(std::vector<string> names_vec, std::vector<ExpType> types_vec, int lineno);
void VarExistsInScope(std::shared_ptr<TypeVar> var, int lineno);
void InsertToSymTable(std::shared_ptr<TypeVar> type, std::shared_ptr<TypeVar> id, bool is_func, int lineno);
void CheckPrevDeclID(std::shared_ptr<TypeVar> var, int lineno);
void ValidateAssign(std::shared_ptr<TypeVar> var, std::shared_ptr<TypeVar> type, int lineno);
void check_valid_auto_assign(std::shared_ptr<TypeVar> var, std::shared_ptr<TypeVar> type, int lineno);
void OpenNewBlock(bool is_while_block, ExpType ret_type);
void CloseBlock();

void WhileState(bool curr_while_state);
void CallFunc(std::shared_ptr<TypeVar> id_var, std::shared_ptr<TypeVar> exp_list_var, std::shared_ptr<TypeVar> call_var, int lineno);
void CheckVoidScope(int lineno);
void CheckWhileScope(int lineno, bool is_break);
void ValidateRetType(std::shared_ptr<TypeVar> var, int lineno);
void InsertFuncSymTab(ExpType type, string id, std::vector<string> names_vec, std::vector<ExpType> types_vec);

void InitializeMainScope();
void CloseMainScope();

#endif