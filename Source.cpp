#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <algorithm>
#define min 0
#define max 1
#define alpha 0.1
using namespace std;
template<typename t>
vector<vector<t>> transp(vector<vector<t>>& a)
{
	vector<vector<t>> res(a[0].size(), vector<t>(a.size()));
	for (int i = 0; i < a.size(); i++)
	{
		for (int j = 0; j < a[0].size(); j++)
		{
			res[j][i] = a[i][j];
		}
	}
	return res;
};
template<typename t>
vector<vector<t>> transp(vector<t>& a)
{
	vector<vector<t>> res(a.size(),vector<t>(1));
	for (int i = 0; i < a.size(); i++)
	{
		res[i][0] = a[i];
	};
	return res;
};
template<typename t>
vector<vector<t>> matrmultiply(vector<vector<t>> a, vector<vector<t>> b)
{
	auto f = [](vector<t> a, vector<t> b)->t
	{
		t ans = a[0] * b[0];
		for (int i = 1; i < a.size(); i++)
		{
			ans = ans + (a[i] * b[i]);
		};
		return ans;
	};
	if (a[0].size() != b.size())
	{
		cout << "error";
		return vector<vector<t>>();
	};
	vector<vector<t>> resu(a.size(), vector<t>(b[0].size()));
	b=transp(b);
	for (int i = 0; i < a.size(); i++)
	{
		for (int j = 0; j < b.size(); j++)
		{
			resu[i][j] = f(a[i], b[j]);
		};
	};
	return resu;
};
template<typename t>
vector<t> matrmultiply(vector<t> a, vector<vector<t>> b)
{
	if (a.size() != b.size()) cout << "error";
	vector<t> res(b[0].size());
	for (int i = 0; i < b[0].size(); i++)
	{
		for (int j = 0; j < b.size(); j++)
		{
			res[i] = res[i] + (a[j] * b[j][i]);
		};
	};
	return res;
};
template<typename t>
vector<vector<t>> matrmultiply(vector<vector<t>> a, vector<t> b)
{
	vector<vector<t>> res(a.size(), vector<t>(b.size(),0));
	for (int i = 0; i < a.size(); i++)
	{
		for (int j = 0; j < b.size(); j++)
		{
			res[i][j] = a[i][0] * b[j];
		};
	};
	return res;
};
template<typename t>
vector<t> matsum(vector<t> a, vector<t> b)
{
	vector<t> res(a.size());
	for (int i = 0; i < a.size(); i++)
	{
		res[i] = a[i] + b[i];
	};
	return res;
};
template<typename t>
void vin(vector<vector<vector<t>>>& vec, int a = 0, int b = RAND_MAX)
{
	for (int i = 0; i < vec.size(); i++)
	{
		for (int j = 0; j < vec[i].size(); j++)
		{
			for (int k = 0; k < vec[i][j].size(); k++)
			{
				vec[i][j][k] = (a + rand() % (b - a) + t(rand() - 1) / RAND_MAX);
			};
		};
	};
} 
template<typename t>
void vin(vector<vector<t>>& vec, int a = 0, int b = RAND_MAX)
{
	for (int j = 0; j < vec.size(); j++)
	{
		for (int k = 0; k < vec[j].size(); k++)
		{
			vec[j][k] = (a + rand() % (b - a) + t(rand() - 1) / RAND_MAX);
		};
	};
};
template<typename t>
void vin(vector<t>& vec, int a = 0, int b = RAND_MAX)
{
	for (int k = 0; k < vec.size(); k++)
	{
		vec[k] = (a + rand() % (b - a) + t(rand() - 1) / RAND_MAX);
	};
};
template <typename t2>
struct network
{
	template<typename t>
	t relu(t a)
	{
		return 1 * (a >= 0.5);
	};
	template<typename t>
	t proizv(t a)
	{
		return a>0;
	};
	int lays, count;
	vector<vector<long double>> B;
	vector<vector<long double>> dedb,dedt,dedh;
	vector<vector<vector<long double>>> W;
	vector<vector<vector<long double>>> dedw;
	long double E;
	t2 prediction;
	float prediction2;
	vector<vector<long double>> T, H;
	template<typename t>
	vector<t> activ(vector<t> inp)
	{
		vector<t> res(inp.size());
		for (int i=0;i<inp.size();i++)
		{
			res[i]=relu(inp[i]);
		};
		return res;
	};
	network(int l, int c, int in, int out)
	{
		lays = l;
		count = c;
		vector<vector<long double>> temp(lays, vector<long double>(count));
		temp.push_back(vector<long double>(out));
		dedh = dedt = temp;
		vector<vector<vector<long double>>> temp1(lays - 1, vector<vector<long double>>(count, vector<long double>(count)));
		temp1.insert(temp1.begin(), vector<vector<long double>>(in, vector<long
			double>(count)));
		temp1.push_back(vector<vector<long double>>(count, vector<long double>(out)));
		dedb = temp;
		vin(temp,min, max);
		B = temp;
		dedw = temp1;
		vin(temp1, min, max);
		W = temp1;
		vector<vector<long double>> temp3(lays, vector<long double>(count));
		temp3.push_back(vector<long double>(count));
		temp3.insert(temp3.begin(), vector<long double>(in));
		T = H = temp3;
	};
	void forward(vector<long double> input)
	{
		H[0] = input;
		for (int i = 0; i < lays + 1; i++)
		{
			T[i + 1] = matrmultiply(H[i], W[i]);
			T[i + 1] = matsum(T[i + 1], B[i]);
			H[i + 1] = activ(T[i + 1]);
		};
		prediction = t2(T[T.size() - 1][0]);
		prediction2 = H[H.size() - 1][0];
	};
	long answer;
	template<typename t>
	void gradient(t ans)
	{
		answer = ans;
		E = (prediction-ans);
		dedh[lays][0] = E;
		for (int i = lays; i >= 0; i--)
		{
			for (int j = 0; j < dedt[i].size(); j++)
			{
				dedt[i][j] = (dedh[i][j] * proizv(T[i + 1][j]));
			};
			dedw[i] = matrmultiply(transp(H[i]),dedt[i]);
			dedb = dedt;
			if (i != 0)
			{
				dedh[i - 1] = matrmultiply(dedt[i], transp(W[i]));
			};
		};
	};
	void step()
	{
		if (prediction != answer)
		{
			for (int i = 0; i < W.size(); i++)
			{
				for (int j = 0; j < W[i].size(); j++)
				{
					for (int k = 0; k < W[i][j].size(); k++)
					{
						W[i][j][k] = W[i][j][k] - alpha * dedw[i][j][k];
					};
				};
			};
		for (int i = 0; i < B.size(); i++)
		{
			for (int j = 0; j < B[i].size(); j++)
			{
				B[i][j] = B[i][j] - alpha * dedb[i][j];
			};
		};
		};
	};
};
int main()
{
	network<float> first(1, 3, 2, 1);
	float a, b;
	for (int i = 0; i < 1000; i++)
	{
		first.forward({ 0, 0 });
		first.gradient(0);
		first.step();
		first.forward({ 0, 1 });
		first.gradient(1);
		first.step();
		first.forward({ 1, 0 });
		first.gradient(1);
		first.step();
		first.forward({ 1, 1 });
		first.gradient(0);
		first.step();
	};
	while (true)
	{
		cin >> a;
		cin >> b;
		first.forward({ a,b });
		cout << first.prediction2<<endl;
	}
};