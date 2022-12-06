#include <bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	
	int nbBanks, nbTransactions;
	cin >> nbBanks >> nbTransactions;
	
	double globalLiquidity;
	cin >> globalLiquidity;
	
	cout << nbBanks << " " << nbTransactions << endl;
	
	// liquidities
	for(int iBank = 0;iBank < nbBanks;iBank++) {
		cout << globalLiquidity << " ";
	}
	cout << endl;
	
	for(int iTransaction = 0;iTransaction < nbTransactions;iTransaction++) {
		int sender, receiver;
		
		do {
			sender = rand() % nbBanks;
			receiver = rand() % nbBanks;
		} while(sender == receiver);
		
		double amount = rand() / (double)RAND_MAX;
		
		cout << sender << " " << receiver << " " << amount << endl;
	}
	
	return 0;
}
