#include <iostream>
using namespace std;

class SymbolInfo
{
    string name;
    string type;
    SymbolInfo *next;

public:
    SymbolInfo(string name, string type, SymbolInfo *next = nullptr) : name(name), type(type), next(next) {}
    ~SymbolInfo() { next = nullptr; }
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
    int suffix;
public:
    ScopeTable(int n, ScopeTable *parent = nullptr) : total_buckets(n), parentScope(parent)
    {
        hashTable = new SymbolInfo *[total_buckets];
        for (int i = 0; i < total_buckets; i++)
            hashTable[i] = nullptr;
        childCount = 0;
        if (parentScope != nullptr)
        {
            parentScope->increaseChild();
            suffix = parentScope->getChildCount();
            id = parentScope->getId() + "." + to_string(suffix);
        }
        else{
            suffix = 1;
            id = "1";
        }
        cout << "\tScopeTable# " << id << " created\n";
    }
    ~ScopeTable()
    {
        for (int i = 0; i < total_buckets; i++)
        // if (hashTable[i])
        {
            SymbolInfo *iter = hashTable[i], *temp = nullptr;
            while (iter != nullptr)
            {
                temp = iter;
                iter = iter->getNext();
                delete temp;
                temp = nullptr; //
            }
        }
        delete []hashTable;
        cout << "\tScopeTable# " << id << " deleted\n";
    }
    int getSuffix(){
        return suffix;
    }
    unsigned long long hashSdbm(string str)
    {
        unsigned long long hash = 0;
        // int i = 0;
        // while (i < str.size())
        for (int i = 0; i < str.size(); i++)
            hash = str[i] + (hash << 6) + (hash << 16) - hash;
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
            cout << "\tInserted  at position <" << hash + 1 << ", 1> of ScopeTable# " << id << endl;
            return true;
        }
        SymbolInfo *iter = hashTable[hash];
        while (iter != nullptr)
        {
            if (iter->getName() == name)
            {
                cout << "\t\'" << name << "\' already exists in the current ScopeTable# " << id << endl;
                return false;
            }
            iter = iter->getNext();
        }
        // if (Lookup(name, false) != nullptr)
        // {
        //     cout << "\t\'" << name << "\' already exists in the current ScopeTable# " << id << endl;
        //     return false;
        // }
        iter = hashTable[hash];
        int pos = 2;
        while (iter->getNext())
        {
            iter = iter->getNext();
            pos++;
        }
        iter->setNext(new SymbolInfo(name, type));
        cout << "\tInserted  at position <" << hash + 1 << ", " << pos << "> of ScopeTable# " << id << endl;
        return true;
    }
    SymbolInfo *Lookup(string name)
    {
        int hash = hashSdbm(name) % total_buckets;
        SymbolInfo *iter = hashTable[hash];
        int pos = 1;
        while (iter)
        {
            if (iter->getName() == name)
            {
                cout << "\t\'" << name << "\' found at position <" << hash + 1 << ", " << pos << "> of ScopeTable# " << id << endl;
                return iter;
            }
            pos++;
            iter = iter->getNext();
        }
        return iter;
    }
    bool Delete(string name)
    {
        int hash = hashSdbm(name) % total_buckets;
        SymbolInfo *iter = hashTable[hash];
        int pos = 1;
        if (iter == nullptr)
        {
            cout << "\tNot found in the current ScopeTable# " << id << endl;
            return false;
        }
        if (iter->getName() == name)
        {
            hashTable[hash] = iter->getNext();
            delete iter;
            cout << "\tDeleted '" << name << "' from position <" << hash + 1 << ", " << pos << "> of ScopeTable# " << id << endl;
            return true;
        }
        while (iter->getNext() != nullptr)
        {
            pos++;
            if (iter->getNext()->getName() == name)
            {
                SymbolInfo *temp = iter->getNext();
                iter->setNext(temp->getNext());
                delete temp;
                cout << "\tDeleted '" << name << "' from position <" << hash + 1 << ", " << pos << "> of ScopeTable# " << id << endl;
                return true;
            }
            iter = iter->getNext();
        }
        cout << "\tNot found in the current ScopeTable# " << id << endl;
        return false;
    }
    void print()
    {
        cout << "\tScopeTable# " << id << "\n";
        for (int i = 1; i <= total_buckets; i++)
        {
            cout << "\t" << i;
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
    SymbolTable(int n) : n(n) { current = new ScopeTable(n); }
    ~SymbolTable()
    {
        ScopeTable *iter = current, *temp = nullptr;
        while (iter != nullptr)
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
        if (current->getId() == "1")
        {
            cout << "\tScopeTable# 1 cannot be deleted\n";
            return;
        }
        ScopeTable *temp = current;
        current = current->getParentScope();
        delete temp;
    }
    bool Insert(string name, string type)
    {
        return current->Insert(name, type);
    }
    bool Remove(string name)
    {
        return current->Delete(name);
    }
    SymbolInfo *Lookup(string name)
    {
        ScopeTable *iter = current;
        SymbolInfo *temp = nullptr;
        while (iter != nullptr)
        {
            temp =  iter->Lookup(name);
            if(temp!= nullptr)
                return temp;
            iter = iter->getParentScope();
            temp = nullptr;
        }
        cout << "\t\'" << name << "\' not found in any of the ScopeTables" << endl;
        return nullptr;
    }
    void printCurrent()
    {
        current->print();
    }
    void printAll()
    {
        ScopeTable *iter = current;
        while (iter != nullptr)
        {
            iter->print();
            iter = iter->getParentScope();
        }
    }
};