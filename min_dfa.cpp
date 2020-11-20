#include<iostream>
#include<string>
#include<bits/stdc++.h>
using namespace std;

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
	vector<string> states,inputs,final_states;
	map<string,int> states_to_index;
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
		states_to_index[ch]=i;
		string in;
		cout<<"Is "<<ch<<" a final state(yes/no): ";
		cin>>in;
		if(in=="yes" || in=="Yes" || in=="YES")
			final_states.push_back(ch);
	}
	for(i=0;i<num_inputs;i++)
	{
		string ch;
		cout<<"Enter input "<<i+1<<" : ";
		cin>>ch;
		inputs.push_back(ch);
	}
	map<string, map<string,string> > st_table;
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
	cout<<"\nDFA STATE TABLE: \n\n";
	cout<<setw(20)<<left<<"STATE";
	for(i=0;i<inputs.size();i++)
		cout<<setw(20)<<left<<inputs[i];
	cout << endl
          << "--------------------------------------------------------------------\n";
	for(i=0;i<num_states;i++)
	{
		cout<<setw(20)<<left<<states[i];
		for(j=0;j<num_inputs;j++)
		{
			cout<<setw(20)<<left<<st_table[states[i]][inputs[j]];
		}
		cout << endl
          << "--------------------------------------------------------------------\n";
	}

	int pair_table[num_states][num_states];
	int it =0;
	for(i=0;i<num_states;i++)
	{
		for(j=0;j<num_states;j++)
		{
			pair_table[i][j]=0;
		}
	}
	for(i=0;i<num_states;i++)
	{
		for(j=0;j<it;j++)
		{
			if((find(final_states.begin(),final_states.end(),states[i])!=final_states.end() && 
			   find(final_states.begin(),final_states.end(),states[j])==final_states.end()) ||
				(find(final_states.begin(),final_states.end(),states[i])==final_states.end() && 
			   find(final_states.begin(),final_states.end(),states[j])!=final_states.end()))
				pair_table[i][j]=1;
		}
		it++;
	}
	it=0;
	for(i=0;i<num_states;i++)
	{
		for(j=0;j<it;j++)
		{
			cout<<pair_table[i][j]<<" ";
		}
		cout<<endl;
		it++;
	}
	cout<<endl;
	while(1)
	{
		int count=0;
		it=0;
		for(i=0;i<num_states;i++)
		{
			for(j=0;j<it;j++)
			{
				for(int k=0;k<num_inputs;k++)
				{
					if(pair_table[i][j]==1)
						break;
					string m = st_table[states[i]][inputs[k]];
					string n = st_table[states[j]][inputs[k]];
					int x = states_to_index[m];
					int y = states_to_index[n];
					if(pair_table[x][y]==1 || pair_table[y][x]==1)
					{
						pair_table[i][j]=1;
						count++;
					}
				}
			}
			it++;
		}
		if(count==0)
			break;
	}
	it=0;
	for(i=0;i<num_states;i++)
	{
		for(j=0;j<it;j++)
		{
			cout<<pair_table[i][j]<<" ";
		}
		cout<<endl;
		it++;
	}
	cout<<endl;
	it=0;
	vector <pair<string,string> > dfa_states;
	for(i=0;i<num_states;i++)
	{
		for(j=0;j<it;j++)
		{
			if(pair_table[i][j]==0)
				dfa_states.push_back(make_pair(states[i],states[j]));
		}
		it++;
	}
	for(i=0;i<dfa_states.size();i++)
	{
		cout<<dfa_states[i].first<<" "<<dfa_states[i].second<<endl;
	}
	map<string, map<string,string> > min_st_table;
	vector <string> min_dfa_states;
	char sta = 'A'; 
	set<string> set_states;
	for(i=0;i<dfa_states.size();i++)
	{
		int flag=0;
		if(set_states.find(dfa_states[i].first)!=set_states.end() ||
			set_states.find(dfa_states[i].second)!=set_states.end())
		{
			continue;
		}
		set_states.insert(dfa_states[i].first);
		set_states.insert(dfa_states[i].second);
		for(j=i+1;j<dfa_states.size();j++)
		{
			if(dfa_states[i].first==dfa_states[j].first || dfa_states[i].first==dfa_states[j].second
				|| dfa_states[i].second==dfa_states[j].first || dfa_states[i].second==dfa_states[j].second)
			{
				string a = dfa_states[i].first + "," + dfa_states[i].second;
				string b = dfa_states[j].first + "," + dfa_states[j].second;
				string min_state = make_new_state(a,b);
				if(min_dfa_states.size()==0)
				{
					min_dfa_states.push_back(min_state);
				}
				else
					if(find(min_dfa_states.begin(),min_dfa_states.end(),min_state)==min_dfa_states.end())
						min_dfa_states.push_back(min_state);
				set_states.insert(dfa_states[j].first);
				set_states.insert(dfa_states[j].second);
				flag=1;
			}

		}
		if(flag==0)
		{
			string a = dfa_states[i].first + "," + dfa_states[i].second;
			min_dfa_states.push_back(a);
		}
	}
	cout<<endl;
	for(i=0;i<states.size();i++)
	{
		if(set_states.find(states[i])==set_states.end())
			min_dfa_states.push_back(states[i]);
	}
	for(i=0;i<min_dfa_states.size();i++)
	{
		cout<<min_dfa_states[i]<<endl;
	}
	return 0;
}