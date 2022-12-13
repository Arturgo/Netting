#include <bits/stdc++.h>
#include "gurobi_c++.h"
using namespace std;

struct Transaction {
	int sender, receiver;
	double amount;
};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	
	ofstream fout;
	fout.open("out.txt");
	
	GRBEnv env;
	env.set(GRB_IntParam_LogToConsole, 0);
	GRBModel model = GRBModel(env);
	
	int nbBanks, nbTransactions;
	cin >> nbBanks >> nbTransactions;
	
	vector<GRBVar> vars(nbTransactions);
	vector<GRBLinExpr> constrs(nbBanks);
	
	for(int iBank = 0;iBank < nbBanks;iBank++) {
		double liq;
		cin >> liq;
		constrs[iBank] += liq;
	}
	
	double upperThroughput = 0.;
	
	for(int iTransaction = 0;iTransaction < nbTransactions;iTransaction++) {
		int sender, receiver;
		double amount;
		cin >> sender >> receiver >> amount;
		
		upperThroughput += amount;
		
		vars[iTransaction] = model.addVar(
			0.0, 1.0, -amount, GRB_BINARY, "x_" + to_string(iTransaction)
		);
		
		constrs[sender] += -amount * vars[iTransaction];
		constrs[receiver] += amount * vars[iTransaction];
	}
	
	for(int iBank = 0;iBank < nbBanks;iBank++) {
		model.addConstr(0. <= constrs[iBank]);
	}
	
	model.optimize();
	
	//cout << -model.get(GRB_DoubleAttr_ObjVal) << endl;/*<< " " << upperThroughput << endl;*/
	fout << -model.get(GRB_DoubleAttr_ObjVal) << endl;
	
	return 0;
}
