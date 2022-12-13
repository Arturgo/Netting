#include <bits/stdc++.h>
#include "gurobi_c++.h"
using namespace std;

struct Transaction {
	int sender, receiver;
	double amount;
};

vector<double> ratios;
vector<Transaction> transactions;

vector<vector<int>> neighbours;

const double EPS = 1e-4;

int extremity(int transaction, int bank) {
	if(transactions[transaction].sender == bank)
		return transactions[transaction].receiver;
	else
		return transactions[transaction].sender;
}

bool is_tight(int transaction) {
	return ratios[transaction] < EPS || ratios[transaction] > 1 - EPS;
}

double tighten(int bank, int parent = -1,
double slackUp = 1e9, double slackDown = 1e9, double sum = 0.) {
	for(int neighbour : neighbours[bank]) {
		if(neighbour != parent && !is_tight(neighbour)) {
			double nSlackUp, nSlackDown, nSum;
			
			Transaction t = transactions[neighbour];
			
			if(t.sender == bank) {
				nSlackUp = min(slackUp, (1. - ratios[neighbour]) * t.amount);
				nSlackDown = min(slackDown, ratios[neighbour] * t.amount);
				nSum = sum + 1.;
			}
			else {
				nSlackUp = min(slackUp, ratios[neighbour] * t.amount);
				nSlackDown = min(slackDown, (1. - ratios[neighbour]) * t.amount);
				nSum = sum - 1.;
			}
			
			double delta = tighten(extremity(neighbour, bank), neighbour,
			nSlackUp, nSlackDown, nSum);
			
			if(t.sender == bank) {
				ratios[neighbour] += delta / t.amount;
			}
			else {
				ratios[neighbour] -= delta / t.amount;
			}
			
			return delta;
		}
	}
	
	if(sum >= 0.)
		return slackUp;
	else
		return -slackDown;
}

vector<double> finalLiqs;
vector<double> biggestTransaction;

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
	
	biggestTransaction = vector<double>(nbBanks, 0);
	for(int iBank = 0;iBank < nbBanks;iBank++) {
		double liq;
		cin >> liq;
		finalLiqs.push_back(liq);
	}
	
	double upperThroughput = 0.;
	
	neighbours.resize(nbBanks);
	for(int iTransaction = 0;iTransaction < nbTransactions;iTransaction++) {
		int sender, receiver;
		double amount;
		cin >> sender >> receiver >> amount;
		
		biggestTransaction[sender] = max(amount, biggestTransaction[sender]);
		biggestTransaction[receiver] = max(amount, biggestTransaction[receiver]);
		
		transactions.push_back({sender, receiver, amount});
		
		neighbours[sender].push_back(iTransaction);
		neighbours[receiver].push_back(iTransaction);
		
		upperThroughput += amount;
		
		vars[iTransaction] = model.addVar(
			0.0, 1.0, -amount, GRB_CONTINUOUS, "x_" + to_string(iTransaction)
		);
		
		constrs[sender] += -amount * vars[iTransaction];
		constrs[receiver] += amount * vars[iTransaction];
	}
	
	for(int iBank = 0;iBank < nbBanks;iBank++) {
		constrs[iBank] += finalLiqs[iBank] - biggestTransaction[iBank];
	}
	
	for(int iBank = 0;iBank < nbBanks;iBank++) {
		model.addConstr(0. <= constrs[iBank]);
	}
	
	model.optimize();
	
	ratios.resize(nbTransactions);
	for(int iTransaction = 0;iTransaction < nbTransactions;iTransaction++) {
		ratios[iTransaction] = vars[iTransaction].get(GRB_DoubleAttr_X);
	}
	
	bool isTight = false;
	do {
		isTight = true;
		
		for(int iBank = 0;iBank < nbBanks;iBank++) {
			int nbLoose = 0;
			for(int neighbour : neighbours[iBank]) {
				if(!is_tight(neighbour)) {
					nbLoose++;
				}
			}
			
			if(nbLoose != 0) isTight = false;
			
			if(nbLoose == 1) {
				tighten(iBank);
			}
		}
	} while(!isTight);
	
	double finalThroughput = 0.;
	for(int iTransaction = 0;iTransaction < nbTransactions;iTransaction++) {
		if(ratios[iTransaction] > 1 - EPS) {
			finalLiqs[transactions[iTransaction].sender] -= transactions[iTransaction].amount;
			finalLiqs[transactions[iTransaction].receiver] += transactions[iTransaction].amount;
			finalThroughput += transactions[iTransaction].amount;
		}
	}
	
	/*for(int iBank = 0;iBank < nbBanks;iBank++) {
		assert(finalLiqs[iBank] >= -biggestTransaction[iBank] - EPS);
		cerr << finalLiqs[iBank] << " " << biggestTransaction[iBank] << endl;
	}*/
	
	fout << finalThroughput << endl;
	return 0;
}
