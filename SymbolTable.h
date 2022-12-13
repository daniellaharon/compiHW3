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
        string type;
        bool is_func = false;
        std::vector<string> name_list;
        std::vector<string> type_list;
    symTableBlockEntry() = default;
    symTableBlockEntry(int offset, bool is_offset_null, string id, string type, bool is_func) :
            offset(offset), is_offset_null(is_offset_null), id(id), type(type), is_func(is_func){}
    symTableBlockEntry(int offset, bool is_offset_null, string id, string type, bool is_func, std::vector<string> name_list, std::vector<string> type_list) :
            offset(offset), is_offset_null(is_offset_null), id(id), type(type), is_func(is_func),
            name_list(name_list), type_list(type_list){}
};

typedef struct{
    vector<symTableBlockEntry> entries;
    bool is_while;
    string ret_type;
} symbol_table_block;


// Global variables

void insert_func_params_to_sym_table(std::vector<string> name_vector, std::vector<string> type_vector, int lineno);
void VarExistsInScope(std::shared_ptr<TypeVar> var, int lineno);
void insert_var_to_sym_table(std::shared_ptr<TypeVar> type, std::shared_ptr<TypeVar> id, bool is_func, int lineno);
void check_id_exsist_in_curr_scope(std::shared_ptr<TypeVar> var, int lineno);
void check_valid_var_assign(std::shared_ptr<TypeVar> var, std::shared_ptr<TypeVar> type, int lineno);
void check_valid_auto_assign(std::shared_ptr<TypeVar> var, std::shared_ptr<TypeVar> type, int lineno);
void OpenNewBlock(bool is_from_while_opened, string ret_type);
void CloseBlock();
void WhileState(bool curr_while_state);
void CallFunc(std::shared_ptr<TypeVar> id_var, std::shared_ptr<TypeVar> exp_list_var, std::shared_ptr<TypeVar> call_var, int lineno);
void is_scope_void(int lineno);
void is_scope_while(int lineno, bool is_break);
void is_valid_ret_type(std::shared_ptr<TypeVar> var, int lineno);
void insert_func_to_sym_table(string type, string id, std::vector<string> name_vector, std::vector<string> type_vector);
void main_scope_initialization();
void close_main_scope();





#endif