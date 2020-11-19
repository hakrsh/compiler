#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include<algorithm>
using namespace std;


map<string,vector<string>> FOLLOW_SET,symbol_to_prod;
map<string,string> prod_to_sym;
map<string,string> FIRST_SET;
string start_sym;
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

string getString(char a)
{
	string s(1,a);
	return s;
}

map <string,string> remove_left_recursion(map<string,string> productions)
{
	map <string,string> no_lr_productions;
	for(auto i=productions.begin();i!=productions.end();i++)
	{
		int flag=0;
		vector <string> alpha,beta;
		vector<string> rhs_tokens = tokenize(i->second);
		for(int j=0;j<rhs_tokens.size();j++)
		{
			if(getString(rhs_tokens[j][0])==i->first)
			{
				flag=1;
				alpha.push_back(rhs_tokens[j].substr(1,rhs_tokens[j].size()-1));
			}
			else{
				beta.push_back(rhs_tokens[j]);
			}
		}
		if(flag==0)
		{
			no_lr_productions[i->first]=productions[i->first];
		}
		else
		{
			string new_nont = i->first + "'";
			string new_prod1="";
			string new_prod2="";
			for(int k=0;k<beta.size();k++)
			{
				if(k<beta.size()-1)
					new_prod1 += beta[k] + new_nont + "/";
				else
					new_prod1 += beta[k] + new_nont;
			}
			for(int k=0;k<alpha.size();k++)
			{
					new_prod2 += alpha[k] + new_nont + "/";
			}
			new_prod2+="#";
			no_lr_productions[i->first]=new_prod1;
			no_lr_productions[new_nont]=new_prod2;

		}

	}
	return no_lr_productions;
}

string merge_strings(string string1,string string2)
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

string remove_epsilon(string s)
{
	if(s=="#")
		return "";
	for(int i=0;i<s.size();i++)
	{
		if(s[i]=='#')
		{
			if(i==s.size()-1)
				s.erase(i-1,2);
			else if(i==0)
				s.erase(i,2);
			else
				s.erase(i,2);
			break;

		}
	}
	// cout<<"OK::"<<s<<endl;
	return s;
}

string FIRST(string non_ter, map<string,string> productions)
{
	vector<string> rhs_tokens = tokenize(productions[non_ter]);
	string f="";
	for(int j=0;j<rhs_tokens.size();j++)
		{
			if(rhs_tokens[j]=="#")
				f+=",#";
			else if(!isupper(rhs_tokens[j][0]))
				f+=","+getString(rhs_tokens[j][0]);
			else
			{

				int k=0;
				string temp_first = FIRST(getString(rhs_tokens[j][k]),productions);
				// cout<<"temp_first "<<rhs_tokens[j][k]<<": "<<temp_first<<endl;
				if((temp_first.find("#") || temp_first=="#") && k<rhs_tokens[j].size())
					f+=","+remove_epsilon(temp_first);
				else
					f+=","+temp_first;
				string tem_first = temp_first;
				while(k+1<rhs_tokens[j].size() && tem_first.find("#") != std::string::npos)
				{
					tem_first="";
					if(!isupper(rhs_tokens[j][k+1])){
						f+=","+getString(rhs_tokens[j][k+1]);
						break;
					}
					else{
						tem_first = FIRST(getString(rhs_tokens[j][k+1]),productions);
						if((tem_first.find("#") || tem_first=="#") && k+1<rhs_tokens[j].size()-1)
							f+=","+remove_epsilon(tem_first);
						else
							f+=","+tem_first;
						k++;
					}
					
				}

			}

		}
	if(f[0]==',')
		f.erase(0,1);
	return f;

}

void FOLLOW(string sym,string non_ter,vector<string> prods)
{

	    if(non_ter==start_sym)
			FOLLOW_SET[non_ter].push_back("$");
		for(int j=0;j<prods.size();j++)
		{
			string pr = prods[j];
			for(int k=0;k<pr.size();k++)
			{
				if(getString(pr[k])==non_ter)
				{
					if(k==pr.size()-1)
					{
						if(FOLLOW_SET.find(prod_to_sym[pr])!=FOLLOW_SET.end())
							FOLLOW_SET[non_ter] = FOLLOW_SET[prod_to_sym[pr]];
						else
							{
									if(sym==prod_to_sym[pr])
										continue;
									FOLLOW(getString(pr[k]),prod_to_sym[pr],symbol_to_prod[prod_to_sym[pr]]);
									FOLLOW_SET[non_ter] = FOLLOW_SET[prod_to_sym[pr]];
							}
						break;
					}
					else if(!isupper(pr[k+1]))
					{
						FOLLOW_SET[non_ter].push_back(getString(pr[k+1]));
						break;
					}
					else
					{
						for(int l=k+1;l<pr.size();l++)
						{
							if(!isupper(pr[l]))
							{
								FOLLOW_SET[non_ter].push_back(getString(pr[l]));
								break;
							}
							if(FIRST_SET[getString(pr[l])].find("#")!= std::string::npos)
							{
								FOLLOW_SET[non_ter].push_back(remove_epsilon(FIRST_SET[getString(pr[l])]));
								if(l==pr.size()-1)
								{
									if(FOLLOW_SET.find(prod_to_sym[pr])!=FOLLOW_SET.end())
										FOLLOW_SET[non_ter] = FOLLOW_SET[prod_to_sym[pr]];
									else
									{
									   FOLLOW(getString(pr[l]),prod_to_sym[pr],symbol_to_prod[prod_to_sym[pr]]);
										FOLLOW_SET[non_ter] = FOLLOW_SET[prod_to_sym[pr]];
									}
								}
							}
							else
							{
								FOLLOW_SET[non_ter].push_back(FIRST_SET[getString(pr[l])]);
								break;
							}
						}
						break;
					}
				}
			}
		}
}

int main()
{
	map <string,string> productions;
	int n_prod,i,j;
	cout<<"Enter the number of productions: ";
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
	map <string,string> no_lr_productions = remove_left_recursion(productions);
	cout<<endl;
	vector<string> rhs_prod;

	for(auto i=no_lr_productions.begin();i!=no_lr_productions.end();i++)
	{

		vector <string> first;
		vector<string> rhs_tokens = tokenize(no_lr_productions[i->first]);
		
		for(int j=0;j<rhs_tokens.size();j++)
		{
			rhs_prod.push_back(rhs_tokens[j]);
			if(rhs_tokens[j]=="#")
				first.push_back("#");
			else if(!isupper(rhs_tokens[j][0]))
				first.push_back(getString(rhs_tokens[j][0]));
			else
			{
				int k=0;
				string temp_first = FIRST(getString(rhs_tokens[j][k]),no_lr_productions);
				// cout<<"temp_first "<<rhs_tokens[j][k]<<": "<<temp_first<<endl;
				if((temp_first.find("#") || temp_first=="#") && k<rhs_tokens[j].size())
					first.push_back(remove_epsilon(temp_first));
				else
					first.push_back(temp_first);
				string tem_first = temp_first;
				while(k+1<rhs_tokens[j].size() && tem_first.find("#") != std::string::npos)
				{
					// cout<<j<<endl;
					tem_first="";
					if(!isupper(rhs_tokens[j][k+1])){
						first.push_back(getString(rhs_tokens[j][k+1]));
						break;
					}
					else{
						tem_first = FIRST(getString(rhs_tokens[j][k+1]),no_lr_productions);
						// cout<<"tem_first "<<rhs_tokens[j][k+1]<<": "<<tem_first<<endl;
						if((tem_first.find("#") || tem_first=="#") && k+1<rhs_tokens[j].size()-1)
							first.push_back(remove_epsilon(tem_first));
						else
							first.push_back(tem_first);
						k++;
					}
					
				}

			}

		}
		string s1 = first[0];
		for(int j=1;j<first.size();j++)
		{
			s1 = merge_strings(first[j],s1);
		}
		cout<<"FIRST("<<i->first<<") = "<<"{"<<s1<<"}"<<endl;
		FIRST_SET[i->first] = s1;
	}

	for(auto i=no_lr_productions.begin();i!=no_lr_productions.end();i++)
	{
		for(int j=0;j<rhs_prod.size();j++)
		{
			if(rhs_prod[j].find(i->first) != std::string::npos)
				symbol_to_prod[i->first].push_back(rhs_prod[j]);
		}
		std::vector<string> tokens = tokenize(i->second);

		for(int j=0;j<tokens.size();j++)
		{
			prod_to_sym[tokens[j]]=i->first;
		}

	}

	for(auto i=no_lr_productions.begin();i!=no_lr_productions.end();i++)
	{
	 	for(int j=0;j<symbol_to_prod[i->first].size();j++)
	 	{
	 		cout<<symbol_to_prod[i->first][j]<<" ";
	 	}
	 	cout<<"\n";
	}

	for(auto i=symbol_to_prod.begin();i!=symbol_to_prod.end();i++)
	{
		vector<string> prods = i->second;
		if(i->first==start_sym)
			FOLLOW_SET[i->first].push_back("$");
		for(int j=0;j<prods.size();j++)
		{
			string pr = prods[j];
			for(int k=0;k<pr.size();k++)
			{
				if(getString(pr[k])==i->first)
				{
					if(k==pr.size()-1)
					{
			
						if(FOLLOW_SET.find(prod_to_sym[pr])!=FOLLOW_SET.end())
						{
							FOLLOW_SET[i->first] = FOLLOW_SET[prod_to_sym[pr]];
						}
						else
							{
								FOLLOW(getString(pr[k]),prod_to_sym[pr],symbol_to_prod[prod_to_sym[pr]]);
								FOLLOW_SET[i->first] = FOLLOW_SET[prod_to_sym[pr]];
							}
						break;
					}
					else if(!isupper(pr[k+1]))
					{
						FOLLOW_SET[i->first].push_back(getString(pr[k+1]));
						break;
					}
					else
					{
						for(int l=k+1;l<pr.size();l++)
						{
							if(!isupper(pr[l]))
							{
								FOLLOW_SET[i->first].push_back(getString(pr[l]));
								break;
							}
							if(FIRST_SET[getString(pr[l])].find("#")!= std::string::npos)
							{
								FOLLOW_SET[i->first].push_back(remove_epsilon(FIRST_SET[getString(pr[l])]));
								if(l==pr.size()-1)
								{
									if(FOLLOW_SET.find(prod_to_sym[pr])!=FOLLOW_SET.end())
										FOLLOW_SET[i->first] = FOLLOW_SET[prod_to_sym[pr]];
									else
									{
										FOLLOW(getString(pr[l]),prod_to_sym[pr],symbol_to_prod[prod_to_sym[pr]]);
										FOLLOW_SET[i->first] = FOLLOW_SET[prod_to_sym[pr]];
									}
								}
							}
							else
							{
								FOLLOW_SET[i->first].push_back(FIRST_SET[getString(pr[l])]);
								break;
							}
						}
						break;
					}
				}
			}
		}

	}
	map<string,string> final_follow_set;
	for(auto i=FOLLOW_SET.begin();i!=FOLLOW_SET.end();i++)
	{
		string s1 = i->second[0];
		for(int j=1;j<(i->second).size();j++)
		{
			s1 = merge_strings(s1,i->second[j]);
		}		
		final_follow_set[i->first]=s1;
	}

	for(auto i=final_follow_set.begin();i!=final_follow_set.end();i++)
	{
		cout<<"FOLLOW("<<i->first<<")"<<"="<<" "<<i->second;
		cout<<endl;
	}
	return 0;
}