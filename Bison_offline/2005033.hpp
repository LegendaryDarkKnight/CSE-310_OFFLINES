#ifndef HELLO_SOMIK
#define HELLO_SOMIK

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <vector>
extern int line_count;
using namespace std;

class Pair{
    string name;
    string type;
public:
    Pair(string name, string type):name(name),type(type){}
    string getName(){return name;}
    string getType(){return type;}
};

class FuncExtras
{
    string returnType;
    vector<Pair *> parameterList;

    bool defined;

public:
    FuncExtras(string returnType,vector<Pair *> parameterList, bool defined):returnType(returnType), parameterList(parameterList), defined(defined)
    {}
    string getReturnType(){
        return returnType;
    }
    vector<Pair *> getParameterList(){
        return parameterList;
    }
    int getNumber(){
        return parameterList.size();
    }
    bool getDefinition(){
        return defined;
    }
};

class ParseTree{
    string type;
    string line; // for parse tree
    int startLine;
    int endLine;
    vector<ParseTree *> child;
public:
    ParseTree(string type, string line, int startLine = line_count, int endLine=line_count):type(type),line(line),startLine(startLine),endLine(endLine){

    }
    string getLine(){
        return line;
    }
    string getType(){
        return type;
    }
    void setType(string type){
        this->type = type;
    }
    void setLine(string line){
        this->line = line;
    }
    int getStartLine(){
        return startLine;
    }
    int getEndLine(){
        return endLine;
    }
    void setStartLine(int line){
        startLine = line;
    }
    void setEndLine(int line){
        endLine = line;
    }
    void addChild(ParseTree *s){
        child.push_back(s);
    }
    void printTree(ParseTree *s, int level, FILE *out){
        if(s==nullptr) return;
        for(int i=0; i<level; i++)
            fprintf(out, " ");
        fprintf(out, "%s \t<Line: %d", s->line.c_str(), s->startLine);
        if(s->type==""){
            fprintf(out, ">\n");
        }
        else{
            fprintf(out,"-%d>\n",s->endLine);
        }
        for(auto v: s->child){
            printTree(v,level+1,out);
        }

    }
    int getChildCount(){
        return child.size();
    }
};

class SymbolInfo
{
    string name;
    string type;
    SymbolInfo *next;
    SymbolInfo *next1;
    FuncExtras *extra;
    int line;
    bool array1 = false;    
public:
    SymbolInfo(string name, string type,FuncExtras *extra = nullptr, int line=line_count, SymbolInfo *next = nullptr) : name(name), type(type), line(line),next(next), extra(extra) {
    }
    ~SymbolInfo() { next = nullptr; if(extra!=nullptr) delete extra;}
    void setName(string name) { this->name = name; }
    string getName() { return name; }
    void setType(string type) { this->type = type; }
    string getType() { return type; }
    void setNext(SymbolInfo *ptr) { next = ptr; }
    SymbolInfo *getNext() { return next; }
    void setNext1(SymbolInfo *ptr) { next1 = ptr; }
    SymbolInfo *getNext1() { return next1; }
    bool isFunc(){
        return extra!=nullptr;
    }
    FuncExtras* getExtra(){
        return extra;
    }
    void setLine(int l){line = l;}
    int getLine(){return line;}   
    int getStartLine(){
        return line;
    }
    int getEndLine(){
        return line;
    }
    void setArray(){
        array1 = true;
    }
    bool isArray(){
        return array1;
    }
};


class ScopeTable
{
    SymbolInfo **hashTable;
    int total_buckets;
    ScopeTable *parentScope;
    string id;
    int childCount;
    int suffix;
    static int counter1;

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
            id = to_string(++counter1);
        }
        else
        {
            counter1 = 1;
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
        delete[] hashTable;
    }
    int getSuffix()
    {
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
    bool Insert(string name, string type, FuncExtras *extras)
    {
        int hash = hashSdbm(name) % total_buckets;
        if (hashTable[hash] == nullptr)
        {
            hashTable[hash] = new SymbolInfo(name, type, extras);
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
        iter->setNext(new SymbolInfo(name, type,extras));
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
    void print(FILE *out)
    {
        fprintf(out, "\tScopeTable# %s\n", id.c_str());
        for (int i = 1; i <= total_buckets; i++)
        {
            SymbolInfo *iter = hashTable[i - 1];
            if (iter == nullptr)
                continue;
            fprintf(out, "\t%d", i);
            if(iter->isFunc())
                fprintf(out, " --> <%s,%s,%s>", iter->getName().c_str(), iter->getType().c_str(), iter->getExtra()->getReturnType().c_str());
            else if(iter->isArray())
                fprintf(out, " --> <%s,ARRAY>", iter->getName().c_str());
            else
                fprintf(out, " --> <%s,%s>", iter->getName().c_str(), iter->getType().c_str());
            iter = iter->getNext();
            while (iter != nullptr)
            {
                if(iter->isFunc())
                    fprintf(out, " <%s,%s,%s>", iter->getName().c_str(), iter->getType().c_str(), iter->getExtra()->getReturnType().c_str());
                else if(iter->isArray())
                    fprintf(out, " <%s,ARRAY>", iter->getName().c_str());
                else
                    fprintf(out, " <%s,%s>", iter->getName().c_str(), iter->getType().c_str());
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
    bool Insert(string name, string type, FuncExtras* extras)
    {
        return current->Insert(name, type,extras);
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
            temp = iter->Lookup(name);
            if (temp != nullptr)
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

class MyStack
{
    SymbolInfo *head;
    int count;

public:
    MyStack()
    {
        head = nullptr;
        count = 0;
    }
    ~MyStack()
    {
        cleanUp();
    }
    void push(SymbolInfo *s)
    {
        if (head == nullptr)
        {
            head = s;
            count++;
            return;
        }
        s->setNext1(head);
        count++;
        head = s;
    }
    SymbolInfo *pop()
    {
        if (head == nullptr)
            return nullptr;
        SymbolInfo *temp = head;
        head = head->getNext1();
        count--;
        return temp;
    }
    void cleanUp()
    {
        SymbolInfo *iter = head, *temp = nullptr;
        while (iter != nullptr)
        {
            temp = iter;
            iter = iter->getNext1();
            delete temp;
            temp = nullptr;
        }
        head = nullptr;
        count = 0;
    }
    bool isEmpty()
    {
        return head == nullptr;
    }
    int size(){
        return count;
    }
};

class MyStack1
{
    SymbolInfo *head, *tail;
    int count;
    vector<string> vect;
public:
    MyStack1()
    {
        head = tail = nullptr;
        count = 0;
    }
    ~MyStack1()
    {
        cleanUp();
    }
    void push(SymbolInfo *s)
    {
        if (head == nullptr)
        {
            head = tail = s;
            count++;
            return;
        }
        tail->setNext1(s);
        tail = tail->getNext1();
        count++;
    }
    SymbolInfo *pop()
    {
        if (head == nullptr)
            return nullptr;
        SymbolInfo *temp = head;
        head = head->getNext1();
        count--;
        return temp;
    }
    void cleanUp()
    {
        SymbolInfo *iter = head, *temp = nullptr;
        while (iter != nullptr)
        {
            temp = iter;
            iter = iter->getNext1();
            delete temp;
            temp = nullptr;
        }
        head = tail = nullptr;
        count = 0;
    }
    bool isEmpty()
    {
        return head == nullptr;
    }
    int size(){
        return count;
    }
    vector<string> getList(){
        vector<string> v;
        SymbolInfo *iter = head;
        while (iter != nullptr)
        {
            v.push_back(iter->getType());
            iter = iter->getNext1();
        }
        return v;
    }
};
#endif