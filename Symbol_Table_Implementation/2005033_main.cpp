#include "2005033_SymbolTable.hpp"

int main()
{
    ifstream file("input1.txt");
    FILE *fpt = freopen("output1.txt", "w", stdout);
    if (!file.is_open())
    {
        cerr << "Unable to open the file!" << endl;
        return 1;
    }
    string line;
    getline(file, line);
    int n = stoi(line);
    int j = 0, i;
    bool trash;
    SymbolInfo *symboltrash;
    SymbolTable *symboltable = new SymbolTable(n);
    while (getline(file, line))
    {
        istringstream iss(line);
        string token;
        string command, arg1, arg2;
        i = 0;

        if (iss >> token)
        {
            command = token;
            i++;
        }
        if (iss >> token)
        {
            arg1 = token;
            i++;
        }
        if (iss >> token)
        {
            arg2 = token;
            i++;
        }
        while (iss >> token)
        {
            i++;
        }
        cout << "Cmd " << ++j << ": ";
        cout << line << endl;
        switch (command[0])
        {
        case 'I':
            if (i != 3)
            {
                cout << "\tWrong number of arguments for the command I" << endl;
            }
            else
            {
                trash = symboltable->Insert(arg1, arg2);
            }
            break;
        case 'L':
            if (i != 2)
                cout << "\tWrong number of arguments for the command L" << endl;
            else
                symboltrash = symboltable->Lookup(arg1);
            break;
        case 'D':
            if (i != 2)
                cout << "\tWrong number of arguments for the command D" << endl;
            else
                trash = symboltable->Remove(arg1);
            break;
        case 'P':
            if (i != 2)
                cout << "\tWrong number of arguments for the command P" << endl;
            else
            {
                if (arg1 == "A")
                    symboltable->printAll();
                else if (arg1 == "C")
                    symboltable->printCurrent();
                else
                    cout << "\tInvalid argument for the command P" << endl;
            }
            break;
        case 'S':
            if (i != 1)
                cout << "\tWrong number of arguments for the command S" << endl;
            else
                symboltable->EnterScope();
            /* code */
            break;
        case 'E':
            if (i != 1)
                cout << "\tWrong number of arguments for the command E" << endl;
            else
                symboltable->ExitScope();
            /* code */
            break;
        case 'Q':
            if (i != 1)
                cout << "\tWrong number of arguments for the command Q" << endl;
            else
                delete symboltable;
            break;
        default:
            cout << "\tInvalid Input\n";
            break;
        }
    }
    fclose(fpt);
    file.close();
    return 0;
}
