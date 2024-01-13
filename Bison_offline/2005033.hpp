#include <iostream>
#include <stdio.h>
#include <cstring>

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
    }
    ~ScopeTable()
    {
        for (int i = 0; i < total_buckets; i++)
        {
            SymbolInfo *iter = hashTable[i], *temp = nullptr;
            while (iter != nullptr)
            {
                temp = iter;
                iter = iter->getNext();
                delete temp;
                temp = nullptr; 
            }
        }
        delete []hashTable;
    }
    int getSuffix(){
        return suffix;
    }
    unsigned long long hashSdbm(string str)
    {
        unsigned long long hash = 0;
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
            return true;
        }
        SymbolInfo *iter = hashTable[hash];
        while (iter != nullptr)
        {
            if (iter->getName() == name)
            {
                return false;
            }
            iter = iter->getNext();
        }
        iter = hashTable[hash];
        int pos = 2;
        while (iter->getNext())
        {
            iter = iter->getNext();
            pos++;
        }
        iter->setNext(new SymbolInfo(name, type));
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
            return false;
        }
        if (iter->getName() == name)
        {
            hashTable[hash] = iter->getNext();
            delete iter;
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
                return true;
            }
            iter = iter->getNext();
        }
        return false;
    }
    void print(FILE *out )
    {
        fprintf(out,"\tScopeTable# %s\n",id.c_str());
        for (int i = 1; i <= total_buckets; i++)
        {
            fprintf(out, "\t%d", i);
            SymbolInfo *iter = hashTable[i - 1];
            while (iter != nullptr)
            {
                fprintf(out," --> (%s,%s)", iter->getName().c_str(), iter->getType().c_str());
                iter = iter->getNext();
            }
            fprintf(out, "\n");
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
        return nullptr;
    }
    void printCurrent(FILE *out)
    {
        current->print(out);
    }
    void printAll(FILE *out)
    {
        ScopeTable *iter = current;
        while (iter != nullptr)
        {
            iter->print(out);
            iter = iter->getParentScope();
        }
    }
};