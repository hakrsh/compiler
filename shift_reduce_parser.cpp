#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include<algorithm>
#include<deque>
using namespace std;

map<string,string> prod_to_sym = {{"i","F"},{"(E)","F"},{"F","T"},{"T*F","T"},{"T","E"},{"E+T","E"}};


int main()
{
	string input_string;
	string start_sym = "E";
	cout<<"Shift Reduce parser Grammer"<<endl<<"E->E+T/T\nT->T*F/F\nF->(E)/i\n\n";
	cout<<"Enter the input string: ";
	cin>>input_string;
	input_string+="$";
	deque<string> stack;
	stack.push_back("$");
	int ip=0;

	cout<<setw(10)<<left<<"STACK";
	cout<<setw(15)<<left<<"INPUT STRING"<<"\t"<<"ACTION"<<endl;
	string action="";
	while(ip<input_string.size())
	{
		for(int i=0;i<stack.size();i++)
			cout<<stack[i];
		cout<<"\t";
		for(int i=ip;i<input_string.size();i++)
			cout<<input_string[i];
		cout<<"\t"<<action<<endl;
		if(input_string[ip]=='$')
		{
			string expr="";
			for(int i=1;i<stack.size();i++)
				expr+=stack[i];
			if(expr=="E" || expr=="E+T" || expr=="E+F" || expr=="T*F" || expr=="T" || expr=="F")
			{	
				cout<<endl<<"ACCEPT"<<endl;
				break;
			}
			else
			{
				cout<<"Not accepted"<<endl;
				break;
			}	
		}
		else if(input_string[ip]=='i')
		{
			if(stack.back()=="$" || stack.back()=="(")
			{
				stack.push_back("i");
				ip++;
			}
			else if(stack.back()=="+")
			{
				stack.pop_back();
				stack.pop_back();
				stack.push_back("E");
				ip++;
			}
			else if(stack.back()=="*")
			{
				stack.pop_back();
				stack.pop_back();
				stack.push_back("T");
				ip++;
			}
			else
			{
				cout<<"Not accepted"<<endl;
				break;
			}
		}
		else if(input_string[ip]=='+')
		{
			if(stack.back()=="i" || stack.back()=="T" || stack.back()=="F")
			{
				stack.pop_back();
				stack.push_back("E");
				stack.push_back("+");
				ip++;
			}
			else if(stack.back()=="E")
			{
				stack.push_back("+");
				ip++;
			}
			else
			{
				cout<<"Not accepted"<<endl;
				break;
			}
		}
		else if(input_string[ip]=='*')
		{
			if(stack.back()=="i" || stack.back()=="F" || stack.back()=="E")
			{
				stack.pop_back();
				stack.push_back("T");
				stack.push_back("*");
				ip++;
			}
			else if(stack.back()=="T")
			{
				stack.push_back("*");
				ip++;
			}
			else
			{
				cout<<"Not accepted"<<endl;
				break;
			}
		}
		else if(input_string[ip]=='(')
		{
			if(stack.back()=="*" || stack.back()=="+" || stack.back()=="$")
			{
				stack.push_back("(");
				ip++;
			}
			else
			{
				cout<<"Not accepted"<<endl;
				break;
			}
		}
		else if(input_string[ip]==')')
		{
			if(stack.back()=="T" || stack.back()=="F" || stack.back()=="E")
			{
				stack.pop_back();
				if(stack.back()=="(")
				{
					stack.pop_back();
					stack.push_back("F");
					ip++;
				}
				else
				{
					cout<<"Not accepted"<<endl;
					break;
				}
			}
			else
			{
				cout<<"Not accepted"<<endl;
				break;
			}
			// string expr="";
			// int j=0;
			// for(j=0;j<stack.size();j++)
			// {
			// 	if(stack[j]=="(")
			// 		break;
			// }
			// if(j==stack.size())
			// {
			// 	cout<<"Not accepted"<<endl;
			// 	break;
			// }
			// else
			// {
			// 	for(int k=j+1;k<stack.size();k++)
			// 	{
			// 		if(stack[k]==")")
			// 			break;
			// 		expr+=stack[k];
			// 	}
			// 	if(expr=="E+T" || expr=="T*F")
			// 	{
			// 		while(stack.back()!="(")
			// 		{
			// 			stack.pop_back();
			// 		}
			// 		stack.pop_back();
			// 		stack.push_back("E");
			// 	}
			// 	else
			// 		{
			// 			cout<<"Not accepted"<<endl;
			// 			break;	
			// 		}
			// }
		}

	}
	cout<<endl;
}