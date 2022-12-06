#include <bits/stdc++.h>
using namespace std;

struct Transaction {
	int sender, receiver;
	double amount;
};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	
	int nbBanks, nbTransactions;
	cin >> nbBanks >> nbTransactions;
	
	vector<double> liquidities;
	
	for(int iBank = 0;iBank < nbBanks;iBank++) {
		double liq;
		cin >> liq;
		liquidities.push_back(liq);
	}
	
	vector<vector<Transaction>> transactions(nbTransactions);
	
	double throughput = 0.;
	
	for(int iTransaction = 0;iTransaction < nbTransactions;iTransaction++) {
		int sender, receiver;
		double amount;
		cin >> sender >> receiver >> amount;
		transactions[sender].push_back({sender, receiver, amount});
		
		liquidities[sender] -= amount;
		liquidities[receiver] += amount;
		throughput += amount;
	}
	
	double upperThroughput = throughput;
	
	
	/*************************
	/        TWEAK HERE
	**************************
	
	srand(time(NULL));
	
	//Order transactions
	for(int iBank = 0;iBank < nbBanks;iBank++) {
		random_shuffle(transactions[iBank].begin(), transactions[iBank].end());
		sort(transactions[iBank].begin(), transactions[iBank].end(), [&](const Transaction& a, const Transaction& b) {
			return a.amount < b.amount;
		});
	}
	
	//*/
	
	vector<int> negativeBanks;
	
	for(int iBank = 0;iBank < nbBanks;iBank++) {
		if(liquidities[iBank] < 0) {
			negativeBanks.push_back(iBank);
		}
	}
	
	while(!negativeBanks.empty()) {
		int bank = negativeBanks.back();
		negativeBanks.pop_back();
		
		while(liquidities[bank] < 0) {
			Transaction t = transactions[bank].back();
			transactions[bank].pop_back();
			
			liquidities[t.sender] += t.amount;
			liquidities[t.receiver] -= t.amount;
			throughput -= t.amount;
			
			if(liquidities[t.receiver] < 0) {
				negativeBanks.push_back(t.receiver);
			}
		}
	}
	
	cout << fixed << setprecision(3) << throughput << " " << upperThroughput << endl;
	
	return 0;
}
