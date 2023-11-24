#include <iostream>
#include <fstream>
#include <sstream>
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
        if (parentScope != nullptr)
        {
            parentScope->increaseChild();
            id = parentScope->getId() + "." + to_string(parentScope->getChildCount());
        }
        else
            id = "1";
        cout << "\tScopeTable# " << id << " created\n";
    }
    ~ScopeTable()
    {
        for (int i = 0; i < total_buckets; i++)
            if (hashTable[i])
                delete hashTable[i];
        delete hashTable;
        cout << "\tScopeTable# " << id << " deleted\n";
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
            cout << "\tInserted  at position <" << hash + 1 << ", 1> of ScopeTable# " << id << endl;
            return true;
        }
        if (Lookup(name, false) != nullptr)
        {
            cout << "\t\'" << name << "\' already exists in the current ScopeTable# " << id << endl;
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
        cout << "\tInserted  at position <" << hash + 1 << ", " << pos << "> of ScopeTable# " << id << endl;
        return true;
    }
    SymbolInfo *Lookup(string name, bool show = true)
    {
        int hash = hashSdbm(name) % total_buckets;
        SymbolInfo *iter = hashTable[hash];
        int pos = 1;
        while (iter)
        {
            if (iter->getName() == name)
            {
                if (show)
                    cout << "\t\'" << name << "\' found at position <" << hash + 1 << ", " << pos << "> of ScopeTable# " << id << endl;
                return iter;
            }
            pos++;
            iter = iter->getNext();
        }
        if (show)
            cout << "\t\'" << name << "\' not found in any of the ScopeTables" << endl;
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
            cout << "\tDeleted \'"<<name<<"\' from position <" << hash + 1 << ", " << pos << "> of ScopeTable# " << id << endl;
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
                cout << "\tDeleted \'i\' from position <" << hash + 1 << ", " << pos << "> of ScopeTable# " << id << endl;
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
    SymbolTable(int n)
    {
        this->n = n;
        current = new ScopeTable(n);
    }
    ~SymbolTable()
    {
        ScopeTable *iter = current, *temp;
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
        while (iter != nullptr)
        {
            if (iter->Lookup(name, false) != nullptr)
                return iter->Lookup(name);
            iter = iter->getParentScope();
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

int main()
{
    freopen("out.txt", "w", stdout);
    ifstream file("input.txt"); // Replace "example.txt" with your file name
    if (!file.is_open())
    {
        cerr << "Unable to open the file!" << endl;
        return 1;
    }
    string line;
     getline(file,line);
    int n = stoi(line);
    int j = 0;
    bool trash;
    SymbolInfo *symboltrash;
    SymbolTable *symboltable = new SymbolTable(n);
    while (getline(file, line))
    {
        istringstream iss(line);
        string token;
        string array[100];
        int i = 0;
        cout << "Cmd " << ++j << ":";
        while (iss >> token)
        {
            array[i++] = token;
            cout<< " " << token ;
        }
        cout << endl;
        switch (array[0][0])
        {
        case 'I':
            if (i != 3)
            {
                cout << "\tWrong number of arugments for the command I" << endl;
            }
            else
            {
                trash = symboltable->Insert(array[1], array[2]);
            }
            break;
        case 'L':
            if (i != 2)
                cout << "\tWrong number of arugments for the command L" << endl;
            else
                symboltrash = symboltable->Lookup(array[1]);
            break;
        case 'D':
            if (i != 2)
                cout << "\tWrong number of arugments for the command D" << endl;
            else
                trash = symboltable->Remove(array[1]);
            break;
        case 'P':
            if (i != 2)
                cout << "\tWrong number of arugments for the command P" << endl;
            else
            {
                if (array[1] == "A")
                    symboltable->printAll();
                else if (array[1] == "C")
                    symboltable->printCurrent();
                else
                    cout << "\tInvalid argument for the command P"<<endl;
            }
            break;
        case 'S':
            if (i != 1)
                cout << "\tWrong number of arugments for the command S" << endl;
            else
                symboltable->EnterScope();
            /* code */
            break;
        case 'E':
            if (i != 1)
                cout << "\tWrong number of arugments for the command E" << endl;
            else
                symboltable->ExitScope();
            /* code */
            break;
        case 'Q':
            break;
        default:
            cout << "\tInvalid Input\n";
            break;
        }
    }
    file.close();
    delete symboltable;
    return 0;
    // ScopeTable *scopeTable = new ScopeTable(7);
    // scopeTable->Insert("foo","FUNCTION");
    // // cout<<"dsd\n";
    // scopeTable->Insert("i","VAR");
    // // cout<<"dsd\n";
    // // cout<<scopeTable->Lookup("i")->getName();
    // // cout<<"dsd\n";
    // cout<<(scopeTable->Lookup("j") == nullptr)<<endl;
    // // cout<<"dsd\n";
    // scopeTable->Insert("23","NUMBER");
    // scopeTable->print();
    // scopeTable->Delete("i");
    // scopeTable->print();

    // SymbolTable *symboltable = new SymbolTable(7);
    // symboltable->Insert("foo","FUNCTION");
    // symboltable->Insert("i","VAR");
    // symboltable->Lookup("i");
    // symboltable->Lookup("j");
    // symboltable->Insert("23","NUMBER");
    // symboltable->printCurrent();
    // symboltable->Remove("i");
    // symboltable->Remove("j");
    // symboltable->printAll();
    // symboltable->EnterScope();
    // symboltable->Insert("<=","RELOP");
    // symboltable->Insert("==","RELOP");
    // symboltable->printAll();
    // symboltable->Insert(">=","RELOP");
    // symboltable->printCurrent();
    // symboltable->Insert("<=","RELOP");
    // symboltable->Insert("foo","FUNCTION");
    // symboltable->Remove("==");
    // symboltable->Remove("23");
    // symboltable->printAll();
    // symboltable->EnterScope();
    // symboltable->Insert("x","VAR");
    // symboltable->Insert("y","VAR");
    // symboltable->printAll();
    // symboltable->ExitScope();
    // symboltable->Insert("num","VAR");
    // symboltable->EnterScope();
    // symboltable->Insert("true", "BOOL");
    // symboltable->printAll();
    // symboltable->ExitScope();
    // symboltable->ExitScope();
    // symboltable->ExitScope();
    // symboltable->printCurrent();
    // symboltable->EnterScope();
    // symboltable->printAll();
    // delete symboltable;
}