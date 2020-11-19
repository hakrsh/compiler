#include<iostream>
#include<bits/stdc++.h>
#include<fstream>
#include<string>
#include<sstream>
#include<algorithm>
#include<regex> 
using namespace std;

int notDelimiter(char ch)
{
	if(ch != ' ' && ch != '+' && ch != '-' && ch != '*' &&  
        ch != '/' && ch != ',' && ch != ';' && ch != '>' &&  
        ch != '<' && ch != '=' && ch != '(' && ch != ')' &&  
        ch != '[' && ch != ']' && ch != '{' && ch != '}' && 
        ch != '%' && ch != '!')
        return 1;
    return 0;

}
int isOperator(char ch)
{
	if(ch == '+' || ch == '-' || ch == '*' ||  
        ch == '/' || ch == '>' ||  
        ch == '<' || ch == '=' || ch == '%' || ch == '!')
        return 1;
    return 0;
}
int isKeyword(string str)
{ 
    if (str=="if" || str=="else" || 
        str=="while" || str=="do" ||  
        str=="break" ||  
         str=="continue" || str=="int" 
        || str=="double" || str=="float" 
        || str=="return" || str=="char" 
        || str=="case" || str=="char" 
        || str=="sizeof" || str=="long" 
        || str=="short" || str=="typedef" 
        || str=="switch" || str=="unsigned" 
        || str=="void" || str=="static" 
        || str=="struct" || str=="goto" || str=="main") 
        return 1; 
    return 0; 
}

int isValidIdentifier(string str)
{
	if (str[0] == '0' || str[0] == '1' || str[0] == '2' || 
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||  
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||  
        str[0] == '9' || !notDelimiter(str[0])) 
        return 1; 
    return 0; 
}

int isNumber(string s){

    regex e ("\\d+\\.?\\d*");

    if (regex_match (s,e))
        return 1;
	return 0;
}

int main()
{
	fstream f;
	string line;
	vector<string> keywords,operators,identifiers,decimals;
	f.open("input3.txt",ios::in);
	while(getline(f,line))
	{
		int r=0,l=0,len=line.length();
		while(r<len)
		{
			if(line[l]=='"')
			{
				l++;
				r++;
				while(line[l]!='"')
				{
					l++;
					r++;
				}
				l++;
				r++;
				continue;
			}
			if(notDelimiter(line[r])==1)
			{
				r++;
				continue;
			}
			if(notDelimiter(line[r])==0 && l==r)
			{
				// cout<<l<<" "<<r<<"\n";
				if(isOperator(line[r])==1)
				{
					string s="";
					if(r+1 < len && isOperator(line[r+1])==1)
					{
						if(line[r]=='/' && line[r+1]=='/')
							break;
						s+=line[r];
						s+=line[r+1];
						operators.push_back(s);
						// cout<<s<<"\n";
						r+=2;
						l=r;
					}
					else
					{
						s+=line[r];
						operators.push_back(s);
						// cout<<s<<"\n";
						r++;
						l=r;
					}
				}
				else
					r++;
					l=r;
					continue;
			}
				
			else if((notDelimiter(line[r])==0 && l!=r) || (r==len && l!=r))
			{
				string s = line.substr(l,r-l);
				if (isKeyword(s)==1)
				{
					// cout<<s<<"\n";
					keywords.push_back(s);
					if(isOperator(line[r])!=1)
					{
						r++;
						l=r;
					}
					else
					{
						l=r;
					}
				}
				else if(isValidIdentifier(s)==0)
				{
					// cout<<s<<"\n";
					identifiers.push_back(s);
					if(isOperator(line[r])!=1)
					{
						r++;
						l=r;
					}
					else
					{
						l=r;
					}
				}
				else if(isNumber(s)==1)
				{
					// cout<<s<<"\n";
					decimals.push_back(s);
					if(isOperator(line[r])!=1)
					{
						r++;
						l=r;
					}
					else
					{
						l=r;
					}
				}

			}
		}
	}
	cout<<"Identifiers: ";
	for(int i=0;i<(identifiers.size());i++)
	{
		cout<<identifiers[i];
		if(i==identifiers.size()-1)
		{
			cout<<"\n";
			break;
		}
		cout<<",";
	}
	cout<<"Keywords: ";
	for(int i=0;i<keywords.size();i++)
	{
		cout<<keywords[i];
		if(i==keywords.size()-1)
		{
			cout<<"\n";
			break;
		}
		cout<<",";
	}
	cout<<"Operators: ";
	for(int i=0;i<operators.size();i++)
	{
		cout<<operators[i];
		if(i==operators.size()-1)
		{
			cout<<"\n";
			break;
		}
		cout<<",";
	}
	cout<<"Decimals: ";
	for(int i=0;i<decimals.size();i++)
	{
		cout<<decimals[i];
		if(i==decimals.size()-1)
		{
			cout<<"\n";
			break;
		}
		cout<<",";
	}
	f.close();
	return 0;
}