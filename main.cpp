#include<iostream>
#include"alloc.h"
#include"allocator.h"
#include<vector>
using namespace std;
int main()
{
	
	int ia[5] = { 1, 2, 3, 4, 5 };
	int i = 0;
	vector<int,simple_alloc<int,zsco::alloc> >test(ia,ia+5);
	for (i = 0; i < test.size(); i++)
		cout << test[i] << '\t';
	return 0;
}