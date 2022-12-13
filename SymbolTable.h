#ifndef SYMTABLE
#define SYMTABLE

#include "TypeVar.h"
#include <stack>
class symbol_table_block_entry
{
    public:
        int offset;
        bool is_offset_null;
        string id;
        string type;
        bool is_func = false;
        std::vector<string> name_list;
        std::vector<string> type_list;
        symbol_table_block_entry() = default;
        symbol_table_block_entry(int offset, bool is_offset_null, string id, string type, bool is_func) :
            offset(offset), is_offset_null(is_offset_null), id(id), type(type), is_func(is_func){}
        symbol_table_block_entry(int offset, bool is_offset_null, string id, string type, bool is_func, std::vector<string> name_list, std::vector<string> type_list) :
            offset(offset), is_offset_null(is_offset_null), id(id), type(type), is_func(is_func),
            name_list(name_list), type_list(type_list){}
};

typedef struct{
    vector<symbol_table_block_entry> entries;
    bool is_while;
    string ret_type;
} symbol_table_block;


// Global variables

void insert_func_params_to_sym_table(std::vector<string> name_vector, std::vector<string> type_vector, int lineno);
void assign_var_type(std::shared_ptr<Var> var, int lineno);
void insert_var_to_sym_table(std::shared_ptr<Var> type, std::shared_ptr<Var> id, bool is_func, int lineno);
void check_id_exsist_in_curr_scope(std::shared_ptr<Var> var, int lineno);
void check_valid_var_assign(std::shared_ptr<Var> var, std::shared_ptr<Var> type, int lineno);
void check_valid_auto_assign(std::shared_ptr<Var> var, std::shared_ptr<Var> type, int lineno);
void open_block(bool is_from_while_opened, string ret_type);
void close_block();
void inc_while();
void dec_while();
void call_function(std::shared_ptr<Var> id_var, std::shared_ptr<Var> exp_list_var, std::shared_ptr<Var> call_var, int lineno);
void is_scope_void(int lineno);
void is_scope_while(int lineno, bool is_break);
void is_valid_ret_type(std::shared_ptr<Var> var, int lineno);
void insert_func_to_sym_table(string type, string id, std::vector<string> name_vector, std::vector<string> type_vector);
void main_scope_initialization();
void close_main_scope();





#endif