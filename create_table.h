#ifndef CREATETABLE_H
#define CREATETABLE_H
#pragma once

#include <iostream>
#include <string>
#include "Command.h"
#include "Utils.h"

using namespace std;

class CreateTable
{
private:
    string tableName; 
    string* columnNames;
    string* columnTypes;
    int* columnSizes;
    string* defaultValues;
    int noColumns;

public:
    CreateTable() : tableName(""), columnNames(nullptr), columnTypes(nullptr), columnSizes(nullptr), defaultValues(nullptr), noColumns(0)
{}
    CreateTable(Command& cmd) : tableName(""), columnNames(nullptr), columnTypes(nullptr), columnSizes(nullptr), defaultValues(nullptr), noColumns(0) {
        string* tokens = cmd.getTokenizedInput();
        int noTokens = cmd.getNoTokens();
        if (noTokens < 4) 
        {
            cout << "Error: Incomplete CREATE TABLE command!" << endl;
            delete[] tokens;
            return;
        }
        tableName = tokens[2];
        ParenthesisTokenizer pTokenizer(tokens[3]);
        string* columnTokens = pTokenizer.getTokenizedParenthesis();
        int noTokensInParenthesis = pTokenizer.getNoValuesInParenthesis();
        int i = 0;
        int colCount = 0;
        while (i < noTokensInParenthesis) {
            // Skip name
            i++;
            if (i >= noTokensInParenthesis) break;
            i++;
            if (i >= noTokensInParenthesis) break;
            colCount++;
            if (i < noTokensInParenthesis)
            {
                string upperToken = columnTokens[i];
                for (size_t j = 0; j < upperToken.length(); j++) {
                    upperToken[j] = toupper(upperToken[j]);
                }
                
                if (upperToken == "DEFAULT")
                {
                    i++; // skip DEFAULT
                    i++; // skip default value
                }
            }
        }
        
        noColumns = colCount;
        
        if (noColumns == 0) 
        {
            delete[] columnTokens;
            delete[] tokens;
            return;
        }
        columnNames = new string[noColumns];
        columnTypes = new string[noColumns];
        columnSizes = new int[noColumns];
        defaultValues = new string[noColumns];
        
        for (int j = 0; j < noColumns; j++)
            {
            columnNames[j] = "";
            columnTypes[j] = "";
            columnSizes[j] = -1;
            defaultValues[j] = "";
        }
        i = 0;
        int colIndex = 0;
        
        while (i < noTokensInParenthesis && colIndex < noColumns) 
        {
            columnNames[colIndex] = columnTokens[i];
            i++;
            if (i >= noTokensInParenthesis) 
                break;
            
            string typeToken = columnTokens[i];
            i++;
            int openParen = typeToken.find('(');
            if (openParen != string::npos) {
                int closeParen = typeToken.find(')');
                if (closeParen != string::npos)
                {
                    columnTypes[colIndex] = typeToken.substr(0, openParen);
                    string sizeStr = typeToken.substr(openParen + 1, closeParen - openParen - 1);
                    try 
                        { columnSizes[colIndex] = stoi(sizeStr) } 
                    catch (...)
                        {columnSizes[colIndex] = -1;} } 
                else
                {columnTypes[colIndex] = typeToken;} } 
            else {columnTypes[colIndex] = typeToken;}
            
            if (i < noTokensInParenthesis)
            { string upperToken = columnTokens[i];
                for (size_t j = 0; j < upperToken.length(); j++) 
                { upperToken[j] = toupper(upperToken[j]);}
                
                if (upperToken == "DEFAULT")
                {i++; 
                    if (i < noTokensInParenthesis)
                    {defaultValues[colIndex] = columnTokens[i];
                        i++; }}}
            
            colIndex++;
        }
        delete[] columnTokens;
        delete[] tokens;
    }
    
    
    ~CreateTable() 
{
        delete[] columnNames;
        delete[] columnTypes;
        delete[] columnSizes;
        delete[] defaultValues;
    }
    
    friend ostream& operator<<(ostream& os, const CreateTable& ct) {
        os << "╔════════════════════════════════════════════════════════════╗" << endl;
        os << "║                    CREATE TABLE PARSED                     ║" << endl;
        os << "╚════════════════════════════════════════════════════════════╝" << endl;
        os << endl;
        
        os << "Table name: " << ct.tableName << endl;
        os << "Number of columns: " << ct.noColumns << endl;
        os << endl;
        
        if (ct.noColumns > 0) {
            os << "┌─────────────────────────────────────────────────────────┐" << endl;
            os << "│                    COLUMN DEFINITIONS                   │" << endl;
            os << "├─────────────────────────────────────────────────────────┤" << endl;
            
            for (int i = 0; i < ct.noColumns; i++) {
                os << "│ Column " << (i + 1) << ":";
                os << endl;
                
                os << "│   Name:    " << ct.columnNames[i] << endl;
                os << "│   Type:    " << ct.columnTypes[i];
                
                if (ct.columnSizes[i] != -1) {
                    os << "(" << ct.columnSizes[i] << ")";
                }
                os << endl;
                
                if (!ct.defaultValues[i].empty()) {
                    os << "│   Default: " << ct.defaultValues[i] << endl;
                }
                
                if (i < ct.noColumns - 1) {
                    os << "│" << endl;
                }
            }
            
            os << "└─────────────────────────────────────────────────────────┘" << endl;
        } else {
            os << "No columns defined!" << endl;
        }
        
        return os;
    }
    string getTableName()
{  return tableName; }
    
    int getNoColumns()
{ return noColumns;  }
    
    string* getColumnNames()
{ if (noColumns == 0)
    return nullptr;
        string* copy = new string[noColumns];
        for (int i = 0; i < noColumns; i++) 
        {copy[i] = columnNames[i];}
        return copy;
    }
    
    string* getColumnTypes()
{
        if (noColumns == 0)
            return nullptr;
        string* copy = new string[noColumns];
        for (int i = 0; i < noColumns; i++)
            { copy[i] = columnTypes[i];  }
        return copy;
    }
    
    int* getColumnSizes() {
        if (noColumns == 0) 
            return nullptr;
        int* copy = new int[noColumns];
        for (int i = 0; i < noColumns; i++)
            { copy[i] = columnSizes[i];}
        return copy;
    }
    
    string* getDefaultValues()
{ if (noColumns == 0) 
            return nullptr;
        string* copy = new string[noColumns];
        for (int i = 0; i < noColumns; i++)
            { copy[i] = defaultValues[i]; }
        return copy;
    }
    
    string getColumnName(int index) {
        if (index >= 0 && index < noColumns)
            return columnNames[index];
        return ""; }
    
    string getColumnType(int index) 
{if (index >= 0 && index < noColumns)
            return columnTypes[index];
        return "";}
    
    int getColumnSize(int index)
{ if (index >= 0 && index < noColumns)
            return columnSizes[index];
        return -1;
    }
    
    string getDefaultValue(int index)
{ if (index >= 0 && index < noColumns)
        return defaultValues[index];
        return "";
    }
    
    bool isValid() 
{
        return !tableName.empty() && noColumns > 0;
    }
    
    void printDetailed()
{
        cout << "=== DEBUG INFO ===" << endl;
        cout << "Table Name: '" << tableName << "'" << endl;
        cout << "Number of Columns: " << noColumns << endl;
        
        for (int i = 0; i < noColumns; i++) {
            cout << "\nColumn " << i << ":" << endl;
            cout << "  Name: '" << columnNames[i] << "'" << endl;
            cout << "  Type: '" << columnTypes[i] << "'" << endl;
            cout << "  Size: " << columnSizes[i] << endl;
            cout << "  Default: '" << defaultValues[i] << "'" << endl;
        }
    }
};

#endif
