#include "SymbolTable.h"
#include <iostream>
#include <unordered_set>
#include <algorithm>
std::vector<symbol_table_block> symbol_table_block_stack;
std::stack<int> offset_stack;
bool main_exist = false;
int while_scope = 0;

static vector<string> make_reversed_vector(vector<string> v)
{
    vector<string> reverse_vector;
    for (int i = v.size() - 1; i >= 0; i--){
        reverse_vector.push_back(v[i]);
    }
    return reverse_vector;
}

void OpenNewBlock(bool is_from_while_opened, string ret_type)
{
    symbol_table_block curr_block;
    if (is_from_while_opened ) {
        curr_block.is_while = true;
    } else if (!symbol_table_block_stack.empty() && symbol_table_block_stack.back().is_while){
        curr_block.is_while = true;
    } else {
        curr_block.is_while = false;
    }

    if (!ret_type.empty() ) {
        curr_block.ret_type = !ret_type.empty() ? ret_type : symbol_table_block_stack.back().ret_type;
    } else if (!symbol_table_block_stack.empty() && !symbol_table_block_stack.back().ret_type.empty()){
        curr_block.ret_type = !ret_type.empty() ? ret_type : symbol_table_block_stack.back().ret_type;
    } else {
        curr_block.ret_type = "";
    }

    symbol_table_block_stack.push_back(curr_block);

    if (offset_stack.empty()){
        offset_stack.push(0);
    }
    else{
        offset_stack.push(offset_stack.top());
    }
}

void CloseBlock()
{
    offset_stack.pop();
    output::endScope();
    for (auto entry : symbol_table_block_stack.back().entries){
        string type = entry.type;
        if (entry.is_func){
            vector<string> reversed_vector = make_reversed_vector(entry.type_list);
            type = output::makeFunctionType(entry.type, reversed_vector);
        }
        int offset_to_print = entry.is_offset_null ? 0 : entry.offset;
        output::printID(entry.id, offset_to_print, type);
    }
    symbol_table_block_stack.pop_back();
}

void insert_var_to_sym_table(std::shared_ptr<TypeVar> type, std::shared_ptr<TypeVar> id, bool is_func ,int lineno)
{
	if (type->type == "VOID" ) {
		output::errorMismatch(lineno);
		exit(1);
	}

    symTableBlockEntry curr_entry((offset_stack.top())++, false, id->id, type->type,is_func);
    symbol_table_block_stack.back().entries.push_back(curr_entry);
}

void insert_func_to_sym_table(string type, string id, std::vector<string> name_vector, std::vector<string> type_vector)
{
    if (id == "main" && type == "VOID" && type_vector.empty()){
        main_exist = true;
    }
    symTableBlockEntry curr_entry((int)0, true, id, type, true, name_vector, type_vector);
    symbol_table_block_stack.back().entries.push_back(curr_entry);
    curr_entry.is_func = true;
}

void insert_func_params_to_sym_table(std::vector<string> name_vector, std::vector<string> type_vector, int lineno)
{
    int counter = 0;
    std::unordered_set<string> func_params;
    for (int i = name_vector.size() - 1; i >= 0; i--){
        if (func_params.find(name_vector[i]) != func_params.end()){
            output::errorDef(lineno, name_vector[i]);
            exit(1);
        }
        counter--;
        func_params.insert(name_vector[i]);
        symTableBlockEntry var_entry(counter, false, name_vector[i], type_vector[i], false);
        symbol_table_block_stack.back().entries.push_back(var_entry);
    }
}

void check_id_exsist_in_curr_scope(std::shared_ptr<TypeVar> var, int lineno)
{
    for (auto block = symbol_table_block_stack.rbegin(); block != symbol_table_block_stack.rend(); ++block){
        for (auto entry : block->entries){
            if (entry.id == var->id){
                output::errorDef(lineno, var->id);
                exit(1);
            }
        }
    }
}

void is_scope_void(int lineno)
{
    if (symbol_table_block_stack.back().ret_type == "VOID"){
        return;
    }
    output::errorMismatch(lineno);
     exit(1);
}

void is_scope_while(int lineno, bool is_break)
{
    if (!while_scope){
        if (!is_break){
            output::errorUnexpectedContinue(lineno);
            exit(1);
        }
        else{
            output::errorUnexpectedBreak(lineno);
            exit(1);
        }
    }
}

void is_valid_ret_type(std::shared_ptr<TypeVar> var, int lineno)
{

    string type = var->type;
    if (!var->id.empty()){
        bool found = false;
        symTableBlockEntry found_entry;
        for (auto block = symbol_table_block_stack.rbegin(); block != symbol_table_block_stack.rend(); ++block){
            for (auto entry : block->entries){
                if (entry.id == var->id){
                    found = true;
                    found_entry = entry;
                    break;
                }
            }
            if (found){
                type = found_entry.type;
                break;
            }
        }
    }
    
    if (symbol_table_block_stack.back().ret_type != type && !(symbol_table_block_stack.back().ret_type == "INT" && var->type == "BYTE")){
        output::errorMismatch(lineno);
        exit(1);
    }
}

void check_valid_auto_assign(std::shared_ptr<TypeVar> var, std::shared_ptr<TypeVar> type, int lineno){
	if (type->type == "VOID" || type->type == "STRING") {
		output::errorMismatch(lineno);
		exit(1);
	}
	

}

void check_valid_var_assign(std::shared_ptr<TypeVar> var, std::shared_ptr<TypeVar> type, int lineno)
{
    bool found = false;
    symTableBlockEntry found_entry;
    for (auto block = symbol_table_block_stack.rbegin(); block != symbol_table_block_stack.rend(); ++block){
        for (auto entry : block->entries){
            if (entry.id == var->id){
                found = true;
                found_entry = entry;
                break;
            }
        }
        if (found){
            break;
        }
    }

    if (!found || found_entry.is_func){
        output::errorUndef(lineno, var->id);
        exit(1);
    }


    if (found_entry.type != type->type && !(found_entry.type == "INT" && type->type == "BYTE")){
        output::errorMismatch(lineno);
        exit(1);
    }

}

void CallFunc(std::shared_ptr<TypeVar> id_var, std::shared_ptr<TypeVar> exp_list_var, std::shared_ptr<TypeVar> call_var, int lineno)
{
    symTableBlockEntry found_entry;
    auto main_block = symbol_table_block_stack.front();
    bool found = false;
    for (auto entry : main_block.entries){
        if (entry.is_func && entry.id == id_var->id){
            found = true;
            found_entry = entry;
            break;
        }
    }

    if (!found){
        output::errorUndefFunc(lineno, id_var->id);
        exit(1);
    }

    call_var->type = found_entry.type;


    for (int i = 0; i < exp_list_var->name_list.size(); i++){
        if (exp_list_var->type_list[i] != "none"){
            continue;
        }
        bool found = false;
        symTableBlockEntry found_entry;
        for (auto block = symbol_table_block_stack.rbegin(); block != symbol_table_block_stack.rend(); ++block){
            for (auto entry : block->entries){
                if (entry.id == exp_list_var->name_list[i]){
                    found = true;
                    found_entry = entry;
                    break;
                }
            }
            if (found){
                exp_list_var->type_list[i] = found_entry.type;
                break;
            }
        }

        if (!found || found_entry.is_func){
            output::errorUndef(lineno, exp_list_var->name_list[i]);
            exit(1);
        }
    }

    if (exp_list_var->type_list.size() != found_entry.type_list.size()){

        vector<string> reverse = make_reversed_vector(found_entry.type_list);
        output::errorPrototypeMismatch(lineno,id_var->id,reverse);
        exit(1);
    }
    for (int i = 0; i < found_entry.type_list.size(); i++){
        string call_type = exp_list_var->type_list[i];
        string sym_tab_type = found_entry.type_list[i];
        if (!(sym_tab_type == call_type || (sym_tab_type == "INT" && call_type == "BYTE"))){
            vector<string> reverse = make_reversed_vector(found_entry.type_list);
            output::errorPrototypeMismatch(lineno, id_var->id, reverse);
            exit(1);
        }
    }
}

void main_scope_initialization()
{
    OpenNewBlock(false, "");
    insert_func_to_sym_table("VOID", "print", {"input"}, {"STRING"});
    insert_func_to_sym_table("VOID", "printi", {"input"}, {"INT"});
}

void close_main_scope()
{
    if (main_exist){
        CloseBlock();
        return;
    }
    
    output::errorMainMissing();
    exit(1);
}

void WhileState(bool curr_while_state)
{
    if(curr_while_state){
        while_scope+=1;
    }
    else{
        while_scope-=1;
    }
}

void VarExistsInScope(std::shared_ptr<TypeVar> var, int lineno)
{
    bool found = false;
    symTableBlockEntry found_entry;
    for (auto block = symbol_table_block_stack.rbegin(); block != symbol_table_block_stack.rend(); ++block){
        for (auto entry : block->entries){
            if (entry.id == var->id && !entry.is_func){
                found = true;
                found_entry = entry;
                break;
            }
        }
        if (found){
            var->type = found_entry.type;
            return;
        }
    }
    output::errorUndef(lineno, var->id);
    exit(1);
}