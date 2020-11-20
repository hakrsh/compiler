#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include<algorithm>
#include<deque>
using namespace std;

map<string,string> leading_set,trailing_set;
map <string,string> productions;

string getString(char a)
{
    string s(1,a);
    return s;
}

vector<string> tokenize(string rhs)
{
    vector<string> rhs_tokens;
    stringstream check1(rhs); 
    string tok; 
    while(getline(check1, tok, '/')) 
    { 
        rhs_tokens.push_back(tok); 
    }
    return rhs_tokens;
}

vector<string> comma_tokenize(string rhs)
{
    vector<string> rhs_tokens;
    stringstream check1(rhs); 
    string tok; 
    while(getline(check1, tok, ',')) 
    { 
        rhs_tokens.push_back(tok); 
    }
    return rhs_tokens;
}

string leading(string non_ter, string prod)
{
     for(int i=0;i<prod.size();i++)
     {
        if(prod.size()>2)
        {
            if(isupper(prod[i]) && !isupper(prod[i+1]))
            {
                return getString(prod[i+1]);
            }
        }
        if(!isupper(prod[i]))
        {
            return getString(prod[i]);
        }

            string x = getString(prod[i]);
            if(leading_set.find(x) != leading_set.end())
                return leading_set[x];
            else
            {
                leading_set[x] = "";
                vector <string> rhs_tokens = tokenize(productions[x]);
                for(int j=0;j<rhs_tokens.size();j++)
                {
                    if(j<rhs_tokens.size()-1)
                        leading_set[x] += leading(x,rhs_tokens[j])+",";
                    else
                        leading_set[x] += leading(x,rhs_tokens[j]);
                }
                return leading_set[x];
            }
        
     }
     return "INVALID"; 
}

string trailing(string non_ter, string prod)
{
    int n=prod.size();
     for(int i=0;i<n;i++)
     {

        if(n>2)
        {
            if(isupper(prod[n-1]) && !isupper(prod[n-2]))
            {
                return getString(prod[n-2]);
            }
        }
        if(!isupper(prod[n-1]))
        {
            return getString(prod[n-1]);
        }

            string x = getString(prod[i]);
            if(trailing_set.find(x) != trailing_set.end())
                return trailing_set[x];
            else
            {
                trailing_set[x] = "";
                vector <string> rhs_tokens = tokenize(productions[x]);
                for(int j=0;j<rhs_tokens.size();j++)
                {
                    if(j<rhs_tokens.size()-1)
                        trailing_set[x] += trailing(x,rhs_tokens[j])+",";
                    else
                        trailing_set[x] += trailing(x,rhs_tokens[j]);
                }
                return trailing_set[x];
            }
        
     }
     return "INVALID"; 
}

int main()
{
    string start_sym;
    int n_prod,i,j,n_ter;
    vector<string> terminals;
    cout<<"Enter the number of terminals: ";
    cin>>n_ter;
    cout<<"Enter each terminal\n";
    for(int i=0;i<n_ter;i++)
    {
        string s;
        cout<<"Terminal "<<i+1<<": ";
        cin>>s;
        terminals.push_back(s);
    }
    cout<<"#productions: ";
    cin>>n_prod;
    cout<<"Start symbol: ";
    cin>>start_sym;
    cout<<"Enter each production in the form (S->AB/S->a) and '#' for epsilon\n\n";
    for(i=0;i<n_prod;i++)
    {
        string prod;
        cout<<"Production "<<i+1<<": ";
        cin>>prod;
        string lhs(1,prod[0]);
        string rhs="";
        for(j=3;j<prod.size();j++)
            rhs+=prod[j];
        productions[lhs]=rhs;
    }
    
    for(auto i=productions.begin();i!=productions.end();i++)
    {
        cout<<i->first<<"->"<<i->second<<endl;
    }

    //Calculate leading and trailing

    for(auto i=productions.begin();i!=productions.end();i++)
    {
        if(leading_set.find(i->first)!=leading_set.end())
            continue;
        leading_set[i->first] = "";
        vector <string> rhs_tokens = tokenize(i->second);
        for(int j=0;j<rhs_tokens.size();j++)
        {
            if(j<rhs_tokens.size()-1)
                leading_set[i->first] += leading(i->first,rhs_tokens[j])+",";
            else
                leading_set[i->first] += leading(i->first,rhs_tokens[j]);
        }
    }

    for(auto i=productions.begin();i!=productions.end();i++)
    {
        if(trailing_set.find(i->first)!=trailing_set.end())
            continue;
        trailing_set[i->first] = "";
        vector <string> rhs_tokens = tokenize(i->second);
        for(int j=0;j<rhs_tokens.size();j++)
        {
            if(j<rhs_tokens.size()-1)
                trailing_set[i->first] += trailing(i->first,rhs_tokens[j])+",";
            else
                trailing_set[i->first] += trailing(i->first,rhs_tokens[j]);
        }
    }

    for(auto i=leading_set.begin();i!=leading_set.end();i++)
    {
        cout<<"Leading("<<i->first<<") = "<<i->second<<endl;
    }
    cout<<endl;
    for(auto i=trailing_set.begin();i!=trailing_set.end();i++)
    {
        cout<<"Trailing("<<i->first<<") = "<<i->second<<endl;
    }

    //construct operator precedence table
    map <string, map<string,string>> op_table;

    for(int i=0;i<n_ter;i++)
    {
        for(int j=0;j<n_ter;j++)
        {
            op_table[terminals[i]][terminals[j]] = "";
        }
    }

    for(auto i=productions.begin();i!=productions.end();i++)
    {
        std::vector<string> rhs_tokens = tokenize(i->second);
        
        for(int j=0;j<rhs_tokens.size();j++)
        {
            string prod = rhs_tokens[j];
            for(int k=0;k<prod.size()-1;k++)
            {
                if(!isupper(prod[k]) && !isupper(prod[k+1]))
                    op_table[getString(prod[k])][getString(prod[k+1])] = "=";
                if(k<=prod.size()-3 && !isupper(prod[k])
                    && !isupper(prod[k+2]) && isupper(prod[k+1]))
                    op_table[getString(prod[k])][getString(prod[k+2])] = "=";
                if(!isupper(prod[k]) && isupper(prod[k+1]))
                {
                    string ter = getString(prod[k]);
                    std::vector<string> lead = comma_tokenize(leading_set[getString(prod[k+1])]);
                    for(int l=0;l<lead.size();l++)
                    {
                        string s = lead[l];
                        op_table[ter][s] = "<";
                    }
                }
                if(isupper(prod[k]) && !isupper(prod[k+1]))
                {
                    string ter = getString(prod[k+1]);
                    std::vector<string> trail = comma_tokenize(trailing_set[getString(prod[k])]);
                    for(int l=0;l<trail.size();l++)
                    {
                        string s = trail[l];
                        op_table[s][ter] = ">";
                    }
                }

            }
        }
    }
    std::vector<string> lead_start = comma_tokenize(leading_set[start_sym]);
    for(int i=0;i<lead_start.size();i++)
    {
        op_table["$"][lead_start[i]] = "<";
    }

    std::vector<string> trail_start = comma_tokenize(trailing_set[start_sym]);
    for(int i=0;i<lead_start.size();i++)
    {
        op_table[trail_start[i]]["$"] = ">";
    }

    cout<<endl<<"   ";
    for(int i=0;i<terminals.size();i++)
        cout<<setw(5)<<left<<terminals[i];
    cout<<endl;
    for(int i=0;i<n_ter;i++)
    {
        cout<<terminals[i]<<"  ";
        for(int j=0;j<n_ter;j++)
        {
            cout<<setw(5)<<left<<op_table[terminals[i]][terminals[j]];
        }
        cout<<endl;
    }
    string input_string;
    while(1)
    {
        cout<<endl<<"Enter the string to be parsed (Enter 1 to exit):";
        cin>>input_string;
        if(input_string=="1")
            return 0;
        input_string+="$";
        deque <string> parse_stack;
        parse_stack.push_back("$");
        int ip=0;
        cout<<setw(10)<<left<<"Stack"<<setw(10)<<right<<"Input"<<endl;
        while(1)
        {

            for(int m=0;m<parse_stack.size();m++)
            {
                cout<<parse_stack[m];
            }
            cout<<setw(10)<<right<<"";
            for(int i=ip;i<input_string.size();i++)
            {
                cout<<input_string[i];
            }
          
            
            string a = parse_stack.back();
            string b = getString(input_string[ip]);
            if(op_table[a][b] == "<")
            {
                parse_stack.push_back("<");
                parse_stack.push_back(b);
                ip++;
                cout<<setw(10)<<right<<"PUSH"<<endl;
            }
            else if(op_table[a][b] == "=")
            {
                parse_stack.push_back("=");
                parse_stack.push_back(b);
                ip++;
                cout<<setw(10)<<right<<"PUSH"<<endl;
            }
            else if(op_table[a][b] == ">")
            {
                string last;
            
                do{
                    parse_stack.pop_back();
                    last = parse_stack.back();
                    parse_stack.pop_back();
                        
                }while(last!="<");
                cout<<setw(10)<<right<<"POP"<<endl;
            }
            else
            {
               if(input_string[ip]=='$' && parse_stack.back()=="$")
               {
                cout<<endl<<"ACCEPTED"<<endl;
                break;
               }
               else
               {
                cout<<endl<<"NOT ACCEPTED"<<endl;
                break;
               }
            }
            
        }
    }
    
    return 0;
}
