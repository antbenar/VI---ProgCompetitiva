#include <iostream>
#include <map>
#include <stdio.h>

using namespace std;

int N,ret;
struct Trie {
	int v[2];
	map<char, Trie> h;
	Trie() : v{0,0} {}
	
	void solve() {
		for(auto &it : h)
			it.second.solve();
		ret += min(v[0], v[1]);
	}
};

int main() {
	string name;
	
	while ( cin >> N && (N != -1)) {
		
		Trie root;
		
		for (int k =0; k < 2; ++k ){
			for ( int i =0; i < N; ++i ){
				
				Trie *t = &root;
				cin >> name;
				
				for( int p = 0; p < name.size() ; ++p ) {
					t = &t->h[name[p]];
					t->v[k]++;
				}
			}
		}
		
		ret = 0;
		root.solve();
		cout << ret << endl;
	}
	
	return 0;
}