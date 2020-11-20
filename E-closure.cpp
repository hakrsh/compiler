#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include<algorithm>
using namespace std;
vector<string> visited;
vector<string> tokenize(string cur_st)
{
	vector <string> tokens;
	stringstream check1(cur_st); 
    string tok; 
    while(getline(check1, tok, ',')) 
    { 
        tokens.push_back(tok); 
    }
    return tokens;
}

string epsilon(string cur_st, vector<string> states, map<string, map<string,string>> st_table)
{
	string e_clos = "";
	string e_output = st_table[cur_st]["Epsilon"];
	vector<string> tokens = tokenize(e_output);
	if(tokens.size()==1)
	{
		if(tokens[0]=="#")
			return "";
		if(find(visited.begin(),visited.end(),tokens[0])!=visited.end())
			return "";
		visited.push_back(tokens[0]);
		// cout<<tokens[0]<<endl<<endl;
		return tokens[0]+","+epsilon(tokens[0],states,st_table);
	}
	else
	{
		e_clos=e_clos+e_output+",";
		for(int i=0;i<tokens.size();i++)
		{
			e_clos=e_clos+epsilon(tokens[i],states,st_table);
		}
	}
	
	return e_clos;
}

int main()
{
	vector<string> states,inputs;
	int num_states, num_inputs,i,j;
	cout<<"#states: ";
	cin>>num_states;
	cout<<"#inputs: ";
	cin>>num_inputs;
	for(i=0;i<num_states;i++)
	{
		string ch;
		cout<<"Enter state "<<i+1<<" : ";
		cin>>ch;
		states.push_back(ch);
	}
	for(i=0;i<num_inputs;i++)
	{
		string ch;
		cout<<"Enter input "<<i+1<<" : ";
		cin>>ch;
		inputs.push_back(ch);
	}
	inputs.push_back("Epsilon");
	num_inputs++;
	map<string, map<string,string>> st_table;
	cout<<"(Enter '#' for null state)\n";
	for(i=0;i<num_states;i++)
	{
		for(j=0;j<num_inputs;j++)
		{
			string ch;
			// cout<<"current state: "<<states[i]<<" input: "<<inputs[j]<<" next state: ";
			cout<<states[i]<<" on: "<<inputs[j]<<" : ";
			cin>>ch;
			st_table[states[i]][inputs[j]]=ch;
		}
	}
	// cout<<"\nNFA state table with 'Epsilon' transitions: \n\n";
	// cout<<setw(20)<<left<<"STATE";
	// for(i=0;i<inputs.size();i++)
	// 	cout<<setw(20)<<left<<inputs[i];
	// cout<<endl;
	// for(i=0;i<num_states;i++)
	// {
	// 	cout<<setw(20)<<left<<states[i];
	// 	for(j=0;j<num_inputs;j++)
	// 	{
	// 		cout<<setw(20)<<left<<st_table[states[i]][inputs[j]];
	// 	}
	// 	cout<<"\n";
	// }

	map<string,string> closure;
	for(i=0;i<num_states;i++)
	{
		string e_clos="";
		e_clos=e_clos+states[i];
		visited.push_back(states[i]);
		if(st_table[states[i]]["Epsilon"]!="#")
			e_clos=e_clos+","+epsilon(states[i],states,st_table);
		if(e_clos[e_clos.size()-1]==',')
			e_clos.resize(e_clos.size()-1);
		closure[states[i]]=e_clos;
		visited.clear();
	}
	for(i=0;i<num_states;i++)
	{
		cout<<"Epsilon closure of "<<states[i]<<" : "<<closure[states[i]]<<endl;
	}
	return 0;
}