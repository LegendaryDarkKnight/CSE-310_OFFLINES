#include <iostream>
using namespace std;

class SymbolInfo
{
    string name;
    string type;
    SymbolInfo *next;

public:
    SymbolInfo(string name, string type, SymbolInfo *next = nullptr)
    {
        this->name = name;
        this->type = type;
        this->next = next;
    }
    ~SymbolInfo()
    {
    }
    void setName(string name) { this->name = name; }
    string getName() { return name; }
    void setType(string type) { this->type = type; }
    string getType() { return type; }
    void setNext(SymbolInfo *ptr) { next = ptr; }
    SymbolInfo *getNext() { return next; }
};

class ScopeTable
{
    SymbolInfo **hashTable;
    int total_buckets;
    ScopeTable *parentScope;
    string id;
    int childCount;

public:
    ScopeTable(int n, ScopeTable *parent = nullptr)
    {
        total_buckets = n;
        hashTable = new SymbolInfo *[total_buckets];
        for (int i = 0; i < total_buckets; i++)
            hashTable[i] = nullptr;
        childCount = 0;
        parentScope = parent;
        if (parentScope!=nullptr)
        {
            parentScope->increaseChild();
            id = parentScope->getId() + "." + to_string(parentScope->getChildCount());
        }
        else
            id = "1";
        cout << "\tScopetable# " << id << " created\n";
    }
    ~ScopeTable()
    {
        for (int i = 0; i < total_buckets; i++)
            if (hashTable[i])
                delete hashTable[i];
        delete hashTable;
        cout << "\tScopetable# " << id << " deleted\n";
    }
    unsigned long long hashSdbm(string str)
    {
        unsigned long long hash = 0;
        int i = 0;
        while (i < str.size())
            hash = (unsigned long long)str[i++] + (hash << 6) + (hash << 16) - hash;
        return hash;
    }
    void increaseChild() { childCount++; }
    int getChildCount() { return childCount; }
    string getId() { return id; }
    void setParentScope(ScopeTable *s)
    {
        parentScope = s;
    }
    ScopeTable *getParentScope()
    {
        return parentScope;
    }
    bool Insert(string name, string type)
    {
        int hash = hashSdbm(name) % total_buckets;
        if (hashTable[hash] == nullptr)
        {
            hashTable[hash] = new SymbolInfo(name, type);
            cout << "\tInserted  at position <" << hash + 1 << ",1> of ScopeTable# " << id << endl;
            return true;
        }
        if (Lookup(name)){
            cout<<"\t\'"<<name<<"\' already exists in the current ScopeTable# "<<id<<endl;
            return false;
        }
        SymbolInfo *iter = hashTable[hash];
        int pos = 2;
        while (iter->getNext())
        {
            iter = iter->getNext();
            pos++;
        }
        iter->setNext(new SymbolInfo(name, type));
        cout << "\tInserted  at position <" << hash + 1 << "," << pos << "> of ScopeTable# " << id << endl;
        return true;
    }
    SymbolInfo *Lookup(string name)
    {
        int hash = hashSdbm(name) % total_buckets;
        SymbolInfo *iter = hashTable[hash];
        while (iter)
        {
            if (iter->getName() == name)
                break;
            iter = iter->getNext();
        }
        return iter;
    }
    bool Delete(string name)
    {
        int hash = hashSdbm(name) % total_buckets;
        SymbolInfo *iter = hashTable[hash];
        int pos = 1;
        if(iter == nullptr){
            cout<<"\tNot found in the current ScopeTable# "<<id<<endl;
            return false;
        }
        if (iter->getName() == name)
        {
            hashTable[hash] = iter->getNext();
            delete iter;
            cout<<"\tDeleted \'i\' from position <"<<hash+1<<","<<pos<<"> of ScopeTable# "<<id<<endl;
            return true;
        }
        while (iter->getNext()!=nullptr)
        {
            pos++;
            if (iter->getNext()->getName() == name)
            {
                SymbolInfo *temp = iter->getNext();
                iter->setNext(temp->getNext());
                delete temp;
                cout<<"\tDeleted \'i\' from position <"<<hash+1<<","<<pos<<"> of ScopeTable# "<<id<<endl;
                return true;
            }
            iter = iter->getNext();
        }
        cout<<"\tNot found in the current ScopeTable# "<<id<<endl;
        return false;
    }
    void print()
    {
        cout << "\tScopetable# " << id << "\n";
        for (int i = 1; i <= total_buckets; i++)
        {
            cout <<"\t"<< i;
            SymbolInfo *iter = hashTable[i - 1];
            while (iter != nullptr)
            {
                cout << " --> (" << iter->getName() << "," << iter->getType() << ")";
                iter = iter->getNext();
            }
            cout << endl;
        }
    }
};

class SymbolTable
{
    ScopeTable *current;
    int n;

public:
    SymbolTable(int n)
    {
        this->n = n;
        current = new ScopeTable(n);
    }
    ~SymbolTable(){
        ScopeTable *iter = current, *temp;
        while (iter!=nullptr)
        {
            temp = iter;
            iter = iter->getParentScope();
            delete temp;
        } 
    }
    void EnterScope()
    {
        ScopeTable *temp = new ScopeTable(n, current);
        current = temp;
    }
    void ExitScope()
    {
        if (current->getId() == "1"){
            cout<<"\tScopeTable# 1 cannot be deleted\n";
            return;
        }
        ScopeTable *temp = current;
        current = current->getParentScope();
        delete temp;
    }
    bool Insert(string name, string type){
        return current->Insert(name,type);
    }
    bool Remove(string name){
        return current->Delete(name);
    }
    SymbolInfo* Lookup(string name){
        ScopeTable *iter = current;
        while (iter!=nullptr)
        {
            if(iter->Lookup(name)!=nullptr)
                return iter->Lookup(name);
            iter = iter->getParentScope();
        }       
    }
    void printCurrent(){
        current->print();
    }
    void printAll(){
        ScopeTable *iter = current;
        while (iter!=nullptr)
        {
            iter->print();
            iter = iter->getParentScope();
        }  
    }
};
int main()
{
    // ScopeTable *scopetable = new ScopeTable(7);
    // scopetable->Insert("foo","FUNCTION");
    // // cout<<"dsd\n";
    // scopetable->Insert("i","VAR");
    // // cout<<"dsd\n";
    // // cout<<scopetable->Lookup("i")->getName();
    // // cout<<"dsd\n";
    // cout<<(scopetable->Lookup("j") == nullptr)<<endl;
    // // cout<<"dsd\n";
    // scopetable->Insert("23","NUMBER");
    // scopetable->print();
    // scopetable->Delete("i");
    // scopetable->print();
    freopen("out.txt","w",stdout);
    SymbolTable *symboltable = new SymbolTable(7);
    symboltable->Insert("foo","FUNCTION");
    symboltable->Insert("i","VAR");
    symboltable->Insert("23","NUMBER");
    symboltable->printCurrent();
    symboltable->Remove("i");
    symboltable->Remove("j");
    symboltable->printAll();
    symboltable->EnterScope();
    symboltable->Insert("<=","RELOP");
    symboltable->Insert("==","RELOP");
    symboltable->printAll();
    symboltable->Insert(">=","RELOP");
    symboltable->printCurrent();
    symboltable->Insert("<=","RELOP");
    symboltable->Insert("foo","FUNCTION");
    symboltable->Remove("==");
    symboltable->Remove("23");
    symboltable->printAll();
    symboltable->EnterScope();
    symboltable->Insert("x","VAR");
    symboltable->Insert("y","VAR");
    symboltable->printAll();
    symboltable->ExitScope();
    symboltable->Insert("num","VAR");
    symboltable->EnterScope();
    symboltable->Insert("true", "BOOL");
    symboltable->printAll();
    symboltable->ExitScope();
    symboltable->ExitScope();
    symboltable->ExitScope();
    symboltable->printCurrent();
    symboltable->EnterScope();
    symboltable->printAll();
    delete symboltable;
}