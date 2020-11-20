#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include<algorithm>
using namespace std;

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
	map<string, map<string,string> > st_table;
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
	cout<<"\nNFA STATE TABLE: \n\n";
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
	int num_dfa_states=0;
	vector <string> dfa_states, vis_states,unvis_states;
	string cur_st = states[0];
	map <string,map<string,string> > dfa_st_table;
	dfa_states.push_back(cur_st);
	// unvis_states=states;
	while(1)
	{
		if(cur_st==states[0])
		{
			num_dfa_states++;
			vis_states.push_back(cur_st);
			// if(find(unvis_states.begin(),unvis_states.end(),cur_st)!=unvis_states.end())
			// 	unvis_states.erase(remove(unvis_states.begin(),unvis_states.end(),cur_st),unvis_states.end());
			for(i=0;i<num_inputs;i++)
			{
				dfa_st_table[cur_st][inputs[i]]=st_table[cur_st][inputs[i]];
				if(find(dfa_states.begin(),dfa_states.end(),st_table[cur_st][inputs[i]])==dfa_states.end())
					dfa_states.push_back(st_table[cur_st][inputs[i]]);
			}
				
			int flag=0;
			while(!dfa_states.empty())
			{
				string s;
				s = dfa_states[0];
				dfa_states.erase(dfa_states.begin());
				if(find(vis_states.begin(),vis_states.end(),s)==vis_states.end())
					{
						cur_st = s;
						flag=1;
						break;
					}
			}
			num_dfa_states++;
			if(flag==0)
				break;
			else
				continue;
		}
			
		else
		{	
			vis_states.push_back(cur_st);
			vector<string> tokens = tokenize(cur_st);
      		for(i=0;i<num_inputs;i++)
      		{
      			vector<string> tok_dup=tokens;
      			vector<string> next;
      			while(!tok_dup.empty())
      			{
      				string s = tok_dup[0];
      				if(st_table[s][inputs[i]]!="#")
      					next.push_back(st_table[s][inputs[i]]);
      				tok_dup.erase(tok_dup.begin());
      			}
      			string new_st;
      			if(next.empty())
      				new_st="#";
      			else
      			{
      				new_st = next[0];
      				next.erase(next.begin());

      			}
      			while(!next.empty())
      			{
      				new_st = make_new_state(next[0],new_st);
      				next.erase(next.begin());

      			}
      			if(find(dfa_states.begin(),dfa_states.end(),new_st)==dfa_states.end() && find(vis_states.begin(),vis_states.end(),new_st)==vis_states.end() && new_st!="#")
      			{
      				dfa_states.push_back(new_st);
      			}
      			dfa_st_table[cur_st][inputs[i]]=new_st;

      		}
      		num_dfa_states++;
      		if(dfa_states.empty())
      			break;
      		cur_st=dfa_states[0];
      		dfa_states.erase(dfa_states.begin());
		}
	 }
	cout<<"\nDFA STATE TABLE:\n\n";
	cout<<setw(20)<<left<<"STATE";
	for(i=0;i<inputs.size();i++)
		cout<<setw(20)<<left<<inputs[i];
	cout<<endl;
	for(i=0;i<vis_states.size();i++)
	{
		cout<<setw(20)<<left<<vis_states[i];
		for(j=0;j<num_inputs;j++)
		{
	
			cout<<setw(20)<<left<<dfa_st_table[vis_states[i]][inputs[j]];
		}
		cout<<'\n';
	}
	return 0;

}