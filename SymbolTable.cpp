#include "SymbolTable.h"
#include <iostream>
#include <unordered_set>

std::vector<symbol_table_block> symbol_table_block_stack;
std::stack<int> offset_stack;
bool main_exist = false;
int while_scope = 0;

static string ExpTypeToString(ExpType type)
{
    switch (type)
    {
        case INT_EXP:
            return "INT";
        case BYTE_EXP:
            return "BYTE";
        case BOOL_EXP:
            return "BOOL";
        case STRING_EXP:
            return "STRING";
        case VOID_EXP:
            return "VOID";
        default:
            return "None";
    }
}

static ExpType StringToExpType(string type)
{
    if(type == "INT")
    {
        return INT_EXP;
    }
    if(type == "BYTE")
    {
        return BYTE_EXP;
    }
    if(type == "BOOL")
    {
        return BOOL_EXP;
    }
    if(type == "STRING")
    {
        return STRING_EXP;
    }
    if(type == "VOID")
    {
        return VOID_EXP;
    }
    return NONE;
}

static vector<string> ReverseVec(vector<string> vec)
{
    vector<string> reverse_vec;
    for (int i = vec.size() - 1; i >= 0; i--)
    {
        reverse_vec.push_back(vec[i]);
    }
    return reverse_vec;
}
static vector<string> ReverseVecTypes(vector<ExpType> vec)
{
    vector<string> reverse_vec;
    for (int i = vec.size() - 1; i >= 0; i--)
    {
        string x = ExpTypeToString(vec[i]);
        reverse_vec.push_back(x);
    }
    return reverse_vec;
}

static std::vector<ExpType> StringToExpTypeVec(std::vector<string> types_vec)
{
    std::vector<ExpType> res;
    for (auto & i : types_vec)
    {
        res.push_back(StringToExpType(i));
    }
    return res;
}

void OpenNewBlock(bool is_while_block, ExpType ret_type)
{
    symbol_table_block curr_block;
    if (is_while_block)
    {
        curr_block.is_while = true;
    } else if (!symbol_table_block_stack.empty() && symbol_table_block_stack.back().is_while)
    {
        curr_block.is_while = true;
    } else {
        curr_block.is_while = false;
    }
    string ret_str = ExpTypeToString(ret_type);
    if (!ret_str.empty() )
    {
        curr_block.ret_type = !ret_str.empty() ? ret_str : symbol_table_block_stack.back().ret_type;
    } else if (!symbol_table_block_stack.empty() && !symbol_table_block_stack.back().ret_type.empty()){
        curr_block.ret_type = !ret_str.empty() ? ret_str : symbol_table_block_stack.back().ret_type;
    } else {
        curr_block.ret_type = NONE;
    }

    symbol_table_block_stack.push_back(curr_block);

    if (offset_stack.empty())
    {
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
    for (auto entry : symbol_table_block_stack.back().sym_tab)
    {
        string type = ExpTypeToString(entry.type);
        if (entry.is_func)
        {
            vector<string> reversed_vec = ReverseVecTypes(entry.type_list);
            type = output::makeFunctionType(type, reversed_vec);
        }
        int offset_to_print = entry.is_offset_null ? 0 : entry.offset;
        output::printID(entry.id, offset_to_print, type);
    }
    symbol_table_block_stack.pop_back();
}

void InsertToSymTable(std::shared_ptr<TypeVar> type, std::shared_ptr<TypeVar> id, bool is_func ,int lineno)
{
    if (type->type == VOID_EXP)
    {
        output::errorMismatch(lineno);
        exit(1);
    }

    symTableBlockEntry entry((offset_stack.top())++, false, id->id, type->type,is_func);
    symbol_table_block_stack.back().sym_tab.push_back(entry);
}

void InsertFuncSymTab(ExpType type, string id, std::vector<string> names_vec, std::vector<ExpType> types_vec)
{
    if (id == "main" && type == VOID_EXP && types_vec.empty())
    {
        main_exist = true;
    }
    symTableBlockEntry entry((int)0, true, id, type, true, names_vec, types_vec);
    symbol_table_block_stack.back().sym_tab.push_back(entry);
    entry.is_func = true;
}

void InsertParamsToSymTab(std::vector<string> names_vec, std::vector<ExpType> types_vec, int lineno)
{
    int counter = 0;
    std::unordered_set<string> func_params;
    for (int i = names_vec.size() - 1; i >= 0; i--)
    {
        if (func_params.find(names_vec[i]) != func_params.end())
        {
            output::errorDef(lineno, names_vec[i]);
            exit(1);
        }
        counter--;
        func_params.insert(names_vec[i]);
        symTableBlockEntry entry(counter, false, names_vec[i], types_vec[i], false);
        symbol_table_block_stack.back().sym_tab.push_back(entry);
    }
}

void CheckPrevDeclID(std::shared_ptr<TypeVar> var, int lineno)
{
    for (auto blk = symbol_table_block_stack.rbegin(); blk != symbol_table_block_stack.rend(); blk++)
    {
        for (auto entry : blk->sym_tab)
        {
            if (entry.id == var->id)
            {
                output::errorDef(lineno, var->id);
                exit(1);
            }
        }
    }
}

void CheckVoidScope(int lineno)
{
    if (symbol_table_block_stack.back().ret_type != "VOID")
    {
        output::errorMismatch(lineno);
        exit(1);
    }
}

void CheckWhileScope(int lineno, bool is_break)
{
    if (!while_scope)
    {
        if (is_break){
            output::errorUnexpectedBreak(lineno);
            exit(1);
        }
        else{
            output::errorUnexpectedContinue(lineno);
            exit(1);
        }
    }
}

void ValidateRetType(std::shared_ptr<TypeVar> var, int lineno)
{

    string type = ExpTypeToString(var->type);
    if (!var->id.empty())
    {
        bool found = false;
        symTableBlockEntry entry_found;
        for (auto blk = symbol_table_block_stack.rbegin(); blk != symbol_table_block_stack.rend(); blk++)
        {
            for (auto entry : blk->sym_tab)
            {
                if (entry.id == var->id)
                {
                    found = true;
                    entry_found = entry;
                    break;
                }
            }
            if (found)
            {
                type = ExpTypeToString(entry_found.type);
                break;
            }
        }
    }

    if (symbol_table_block_stack.back().ret_type != type &&
        !(symbol_table_block_stack.back().ret_type == "INT" && ExpTypeToString(var->type) == "BYTE"))
    {
        output::errorMismatch(lineno);
        exit(1);
    }
}

void check_valid_auto_assign(std::shared_ptr<TypeVar> var, std::shared_ptr<TypeVar> type, int lineno)
{
    if (type->type == VOID_EXP || type->type == STRING_EXP)
    {
        output::errorMismatch(lineno);
        exit(1);
    }
}

void ValidateAssign(std::shared_ptr<TypeVar> var, std::shared_ptr<TypeVar> type, int lineno)
{
    bool found = false;
    symTableBlockEntry entry_found;
    for (auto blk = symbol_table_block_stack.rbegin(); blk != symbol_table_block_stack.rend(); blk++)
    {
        for (auto entry : blk->sym_tab)
        {
            if (entry.id == var->id)
            {
                found = true;
                entry_found = entry;
                break;
            }
        }
        if (found){
            break;
        }
    }

    if (!found || entry_found.is_func)
    {
        output::errorUndef(lineno, var->id);
        exit(1);
    }
    if (entry_found.type != type->type && !(entry_found.type == INT_EXP && type->type == BYTE_EXP))
    {
        output::errorMismatch(lineno);
        exit(1);
    }

}

void CallFunc(std::shared_ptr<TypeVar> id_var, std::shared_ptr<TypeVar> exp_list_var, std::shared_ptr<TypeVar> call_var, int lineno)
{
    symTableBlockEntry entry_found;
    auto main_block = symbol_table_block_stack.front();
    bool found = false;
    for (auto entry : main_block.sym_tab)
    {
        if (entry.is_func && entry.id == id_var->id)
        {
            found = true;
            entry_found = entry;
            break;
        }
    }
    if (!found)
    {
        output::errorUndefFunc(lineno, id_var->id);
        exit(1);
    }

    call_var->type = entry_found.type;

    for (int i = 0; i < exp_list_var->name_list.size(); i++)
    {
        if (exp_list_var->type_list[i] != NONE)
        {
            continue;
        }
        bool found = false;
        symTableBlockEntry entry_found;
        for (auto blk = symbol_table_block_stack.rbegin(); blk != symbol_table_block_stack.rend(); blk++)
        {
            for (auto entry : blk->sym_tab)
            {
                if (entry.id == exp_list_var->name_list[i]){
                    found = true;
                    entry_found = entry;
                    break;
                }
            }
            if (found){
                exp_list_var->type_list[i] = entry_found.type;
                break;
            }
        }

        if (!found || entry_found.is_func)
        {
            output::errorUndef(lineno, exp_list_var->name_list[i]);
            exit(1);
        }
    }

    if (exp_list_var->type_list.size() != entry_found.type_list.size())
    {

        vector<string> reverse = ReverseVecTypes(entry_found.type_list);
        output::errorPrototypeMismatch(lineno,id_var->id,reverse);
        exit(1);
    }
    for (int i = 0; i < entry_found.type_list.size(); i++)
    {
        string call_type = ExpTypeToString(exp_list_var->type_list[i]);
        string sym_tab_type =  ExpTypeToString(entry_found.type_list[i]);
        if (!(sym_tab_type == call_type || (sym_tab_type == "INT" && call_type == "BYTE")))
        {
            vector<string> reverse = ReverseVecTypes(entry_found.type_list);
            output::errorPrototypeMismatch(lineno, id_var->id, reverse);
            exit(1);
        }
    }
}

void InitializeMainScope()
{
    OpenNewBlock(false, NONE);
    InsertFuncSymTab(VOID_EXP, "print", {"input"}, {STRING_EXP});
    InsertFuncSymTab(VOID_EXP, "printi", {"input"}, {INT_EXP});
}

void CloseMainScope()
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
    symTableBlockEntry entry_found;
    for (auto blk = symbol_table_block_stack.rbegin(); blk != symbol_table_block_stack.rend(); blk++)
    {
        for (auto entry : blk->sym_tab)
        {
            if (entry.id == var->id && !entry.is_func)
            {
                found = true;
                entry_found = entry;
                break;
            }
        }
        if (found){
            var->type = entry_found.type;
            return;
        }
    }
    output::errorUndef(lineno, var->id);
    exit(1);
}