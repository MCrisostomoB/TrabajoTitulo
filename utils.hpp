#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include <bits/stdc++.h>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <random>  
#include <chrono>

using namespace std;

class Bloque{
	public:
		vector<vector<int>> charlas;
		int bsize;
		int totaljumps;
		int totalconfs;
		int start;
		int finish;

};
class Missing{
	public:
		int charla;
		int day;
		int block;
		int ts;
		int s; 
};

class Gen{
	public:
		vector<vector<Bloque>> horario;
		int costo;
		int hops;
		int conflictos;
		int violaciones;
};
class Charlas{
  public:
    int cruce;
    vector<int> charlas;
    int considerado;
	vector<int> otrascharlas;
	int lcharlas;
	int n;
};

class RPAC {
	public:
		int type;
		int talk;
		int day;
		vector<int> timeslot;
};

int ConflictCount(vector<vector<int>> conflicts, vector<int> talks);
int JumpCount(vector<vector<Bloque>> &dbloques, vector<vector<int>> preferences,int sessions,int people,int days);
bool areDistinct(vector<int> arr);
bool checkconst1(int newv, vector<int> check, int n, vector<vector<int>> tcc);
bool checkconst2(int day,int bloque,int charla, vector<RPAC> rpac);
vector<string> split(const std::string& s, char delimiter);
void BlockCount(Bloque &bloque , int people, int sessions, vector<vector<int>> preferences);
int constcounter(vector<int> check, int n,int day,int bloque,int charla, vector<RPAC> rpac,vector<vector<int>> tcc);
bool checkconst2v(int day,int bloque,vector<int> charla, vector<RPAC> rpac);
int ConflictCounttotal(vector<vector<int>> conflicts, vector<vector<Bloque>> talks);
int CompleteConstCount(Gen gen ,int sessions, vector<RPAC> rpac,vector<vector<int>> tcc );
