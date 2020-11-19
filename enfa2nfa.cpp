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
		cout<<tokens[0]<<endl<<endl;
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

string make_new_state(string string1,string string2)
{
	vector<string> new1,new2,new3;
	stringstream check1(string1); 
    string tok; 
    while(getline(check1, tok, ',')) 
    { 
        new1.push_back(tok); 
    }

    stringstream check2(string2); 
    while(getline(check2, tok, ',')) 
    { 
        new2.push_back(tok); 
    }
 
    std::sort(new1.begin(), new1.end());
    std::sort(new2.begin(), new2.end());

    std::set_union(new1.begin(),new1.end(),
                          new2.begin(),new2.end(),
                          back_inserter(new3));
    string final="";
    for(int i=0;i<new3.size();i++)
    	{
    		if(i==new3.size()-1)
    			final+=new3[i];
    		else
    			final=final+new3[i]+",";
    	}
    return final;
}

int main()
{
	vector<string> states,inputs;
	int num_states, num_inputs,i,j;
	cout<<"Number of states: ";
	cin>>num_states;
	cout<<"Number of inputs: ";
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
			cout<<"current state: "<<states[i]<<" input: "<<inputs[j]<<" next state: ";
			cin>>ch;
			st_table[states[i]][inputs[j]]=ch;
		}
	}
	cout<<"\nNFA state table with 'Epsilon' transitions: \n\n";
	cout<<setw(20)<<left<<"STATE";
	for(i=0;i<inputs.size();i++)
		cout<<setw(20)<<left<<inputs[i];
	cout<<endl;
	for(i=0;i<num_states;i++)
	{
		cout<<setw(20)<<left<<states[i];
		for(j=0;j<num_inputs;j++)
		{
			cout<<setw(20)<<left<<st_table[states[i]][inputs[j]];
		}
		cout<<"\n";
	}

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
	map<string, map<string,string>> nfa_st_table;
	for(i=0;i<states.size();i++)
	{
		if(st_table[states[i]]["Epsilon"]=="#")
		{
			for(j=0;j<num_inputs;j++)
			{
				if (st_table[states[i]][inputs[j]]=="#")
				{
					nfa_st_table[states[i]][inputs[j]]="#";
					continue;	
				}
				nfa_st_table[states[i]][inputs[j]] = closure[st_table[states[i]][inputs[j]]];
			}
			continue;
		}
		vector<string> clos_states = tokenize(closure[states[i]]);
		for(j=0;j<num_inputs-1;j++)
		{	
			vector<string> inter_states;
			for(int k=0;k<clos_states.size();k++)
			{
				if(st_table[clos_states[k]][inputs[j]]!="#")
					inter_states.push_back(st_table[clos_states[k]][inputs[j]]);
			}
			if(inter_states.size()==0)
				inter_states.push_back("#");
			vector<string> final_clos;
			for(int k=0;k<inter_states.size();k++)
			{
				if(inter_states[k]!="#")
					final_clos.push_back(closure[inter_states[k]]);
			}
			if(final_clos.size()==0)
			{
				nfa_st_table[states[i]][inputs[j]]="#";
				continue;
			}
			string new_state=final_clos[0];
			for(int k=1;k<final_clos.size();k++)
			{
				new_state=make_new_state(new_state,final_clos[k]);
			}
			
			nfa_st_table[states[i]][inputs[j]]=new_state;
		}
		
	}

	cout<<"\nNFA state table without 'Epsilon' transitions: \n\n";
	cout<<setw(20)<<left<<"STATE";
	for(i=0;i<inputs.size()-1;i++)
		cout<<setw(20)<<left<<inputs[i];
	cout<<endl;
	for(i=0;i<num_states;i++)
	{
		cout<<setw(20)<<left<<states[i];
		for(j=0;j<num_inputs-1;j++)
		{
			cout<<setw(20)<<left<<nfa_st_table[states[i]][inputs[j]];
		}
		cout<<"\n";
	}
	return 0;
}