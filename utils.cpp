#include "utils.hpp"

int JumpCount(vector<vector<Bloque>> &dbloques, vector<vector<int>> preferences,int sessions,int people,int days){
	int total = 0 ;
	int mayor = 0 ;
	vector<int> estado;
	for(int d = 0 ; d<days; d++){
		for(int b = 0 ; (unsigned)b<dbloques[d].size();b++){
			if(dbloques[d][b].bsize == 1){
				dbloques[d][b].totaljumps = 0 ;
			}
			else if(dbloques[d][b].bsize!=0){
				mayor = 0 ;
				estado.resize(dbloques[d][b].bsize);
				std::fill(estado.begin(),estado.end(),1);
				int profundidad = 1;
				while(profundidad <dbloques[d][b].bsize){
					// cout <<"inicio while "<< " profundidad: "<< profundidad<<" estado 0: "<< estado[0] <<" el medio: "<<estado[1]<< " ultimo estado: "<<estado[dbloques[d][b].bsize-1]<<"\n";
					// cout<<"antes de conteo "<<"\n";
					int cont = 0;
					for(int i = 0 ; i<sessions;i++){
						for(int j = 0 ;j<sessions; j++){
							if(i!=j){
								for(int p = 0 ; p<people;p++){
								// cout <<"persona: "<< p <<" ch1: " <<preferences[p][dbloques[d][b].charlas[j][estado[j]-1]]<<" ch2 : "<<preferences[p][dbloques[d][b].charlas[j-1][estado[j-1]-1]]<<"\n";
									if (preferences[p][dbloques[d][b].charlas[profundidad][j]] == 1 && preferences[p][dbloques[d][b].charlas[profundidad-1][i]] == 1){
										if(preferences[p][dbloques[d][b].charlas[profundidad][i]] != 1){
											cont++;
										}
										// cout<<"contando: "<<cont<<"\n";
									}
								}
							}
						}
					}
					profundidad++;	
					// cout<<"conteo: "<<cont<<"\n";
					mayor+=cont;
					total+=cont;

				}
				dbloques[d][b].totaljumps = mayor;
			}
		}
	}
	return total;
}

void BlockCount(Bloque &bloque , int people, int sessions, vector<vector<int>> preferences){
	int mayor = 0 ;
	vector<int> estado;
	estado.resize(bloque.bsize);
	std::fill(estado.begin(),estado.end(),1);
	int profundidad = 1;
	while(profundidad <bloque.bsize){

		int cont = 0;
		for(int i = 0 ; i<sessions;i++){
			for(int j = 0 ;j<sessions; j++){
				if(i!=j){
					for(int p = 0 ; p<people;p++){
						if (preferences[p][bloque.charlas[profundidad][j]] == 1 && preferences[p][bloque.charlas[profundidad-1][i]] == 1){
								if(preferences[p][bloque.charlas[profundidad][i]] != 1){
									cont++;
								}
							}
					}
				}
			}
		}
		profundidad++;	
		mayor+=cont;

	}
	bloque.totaljumps = mayor;
}


bool areDistinct(vector<int> arr)
{
    int n = arr.size();
    unordered_set<int> s;
    for (int i = 0; i < n; i++) {
        s.insert(arr[i]);
    }
    return (s.size() == arr.size());
}

bool checkconst1(int newv, vector<int> check, int n, vector<vector<int>> tcc){
	// cout << "in check const\n";
	for(int i = 0 ; i<n ; i++){
		// cout << "charlas : "<<check[i]<<"\t"<<newv<<"\n";
		// cout <<"\t"<<tcc[newv][check[i]] << "\t"<< tcc[check[i]][newv] <<"\n";
		if(tcc[newv][check[i]] == 1 && tcc[check[i]][newv] == 1){
			// cout<< "returning false\n";
			return false;
		}
	}
	return true;
}
bool checkconst2(int day,int bloque,int charla, vector<RPAC> rpac){
	for(int i = 0 ; (unsigned)i<rpac.size();i++){
		if(rpac[i].day == day && rpac[i].talk == charla){
			if(rpac[i].type == 3){
				return false;
			}else if (rpac[i].type == 1){
				if(rpac[i].timeslot[0] == bloque){
					return false;
				}
			}else{
				// cout<<"bloque: "<<bloque<<"\n";
				if(rpac[i].timeslot[0]<= bloque && rpac[i].timeslot[1]>= bloque){
					return false;
				}
			}
		}
	}
	return true;

}
bool checkconst2v(int day,int bloque,vector<int> charla, vector<RPAC> rpac){
	for(int i = 0 ; (unsigned)i<rpac.size();i++){
		bool cfound = false;
		for(int j = 0 ;(unsigned)j<charla.size() ;j++){
			// cout<< "charla rpac: " <<rpac[i].talk << "charla vector: "<< charla[j]<<endl;
			if (rpac[i].talk == charla[j]){
				cfound = true;
			}
		}
		// cout << "encontrado: "<< cfound <<endl;
		if(rpac[i].day == day && cfound){
			if(rpac[i].type == 3){
				// cout << "type 3"<<endl;
				return false;
			}else if (rpac[i].type == 1){
				// cout << "type 1 bloque : "<< rpac[i].timeslot[0] << " bloque de entrada : "<< bloque<<" igualdad: "<< (rpac[i].timeslot[0] == bloque)<<endl;
				if(rpac[i].timeslot[0] == bloque){
					// cout << "Retornando Falso "<< endl;
					return false;
				}
			}else{
				// cout << "type 2"<<endl;
				// cout<<"bloque: "<<bloque<<"\n";
				if(rpac[i].timeslot[0]<= bloque && rpac[i].timeslot[1]>= bloque){
					return false;
				}
			}
		}
	}
	// cout<<"afuera del for"<<endl;
	return true;

}

int constcounter(vector<int> check, int n,int day,int bloque,int charla, vector<RPAC> rpac,vector<vector<int>> tcc){
	int cont = 0;
	for(int i = 0 ; i<n ; i++){
		if(tcc[charla][check[i]] == 1 && tcc[check[i]][charla] == 1){
			cont++;
		}
	}
	for(int i = 0 ; (unsigned)i<rpac.size();i++){
		if(rpac[i].day == day && rpac[i].talk == charla){
			if(rpac[i].type == 3){
				cont++;
			}else if (rpac[i].type == 1){
				if(rpac[i].timeslot[0] == bloque){
					cont++;
				}
			}else{
				// cout<<"bloque: "<<bloque<<"\n";
				if(rpac[i].timeslot[0]<= bloque && rpac[i].timeslot[1]>= bloque){
					cont++;
				}
			}
		}
	}
	// cout<< "cont " <<cont<<endl;
	return cont;
}
vector<string> split(const std::string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);

   while (getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

int ConflictCount(vector<vector<int>> conflicts, vector<int> talks){
	int total = 0 ;
	for(int i = 0 ; (unsigned)i<talks.size();i++){
		for(int j = i ; (unsigned)j<talks.size();j++){
			if(talks[i] != talks[j]){
				// cout<<"COMB 1 : "<< conflicts[talks[i]][talks[j]] << " comb 2: "<<conflicts[talks[j]][talks[i]]<<endl;
				total += conflicts[talks[i]][talks[j]];
			}
		}
	}
	return total;
}
int ConflictCountBlock(vector<vector<int>> conflicts, vector<vector<int>> talks){
	int total = 0 ;
	for(int k = 0 ; k<talks.size();k++){
		for(int i = 0 ; (unsigned)i<talks[k].size();i++){
			for(int j = i ; (unsigned)j<talks[k].size();j++){
				if(talks[k][i] != talks[k][j]){
					// cout<<"COMB 1 : "<< conflicts[talks[i]][talks[j]] << " comb 2: "<<conflicts[talks[j]][talks[i]]<<endl;
					total += conflicts[talks[k][i]][talks[k][j]];
				}
			}
		}
	}
	return total;
}

int ConflictCounttotal(vector<vector<int>> conflicts, vector<vector<Bloque>> talks){
	int total = 0 ;
	for(int k = 0 ; (unsigned)k<talks.size();k++){ //dia
		for(int i = 0 ; (unsigned)i<talks[k].size();i++){// bloque
			for(int f = 0; f < talks[k][i].bsize;f++){
				for(int j = 0 ; (unsigned)j<talks[k][i].charlas[f].size();j++){
					// cout<<"charla 1 : "<< talks[i] << " charla 2: "<<talks[j]<<endl;
					for(int l = j ;  (unsigned)l<talks[k][i].charlas[f].size();l++){
						total += conflicts[talks[k][i].charlas[f][j]][talks[k][i].charlas[f][l]];

					}
				}
			}
		}
	}
	return total;
}

int CompleteConstCount(Gen gen ,int sessions, vector<RPAC> rpac,vector<vector<int>> tcc ){
	int viol = 0;
	for(int i = 0 ; (unsigned)i<rpac.size();i++){
		// cout<<"rpac : "<< i <<endl;
		int found = 0 ;
		if(rpac[i].type == 3){
			for(int j = 0 ; (unsigned)j < gen.horario[rpac[i].day].size();j++){
				for(int k = 0; k< gen.horario[rpac[i].day][j].bsize;k++){
					for (int h = 0 ; h<sessions;h++){
						if( gen.horario[rpac[i].day][h].charlas[k][h] == rpac[i].talk){
							viol++;
							found = 1;
							break;
						}
					}
					if(found == 1){
						break;
					}
				}
				if(found == 1){
					break;
				}
			}
		}else if (rpac[i].type == 1){
			// cout << "tipo 1 "<< endl;
			for(int j = 0 ; (unsigned)j < gen.horario[rpac[i].day].size();j++){
				// cout << "start : "<< gen.horario[rpac[i].day][j].start << " end: "<< gen.horario[rpac[i].day][j].finish << endl;
				if(gen.horario[rpac[i].day][j].start <= rpac[i].timeslot[0] &&gen.horario[rpac[i].day][j].finish >= rpac[i].timeslot[0]){
					// cout << "entro al primer if "<<endl;
					for(int k = 0; k< gen.horario[rpac[i].day][j].bsize;k++){
						for (int h = 0 ; h<sessions;h++){
							// cout <<" k : " <<k << " h : "<< h<<endl;
							// cout << "charla : "<< gen.horario[rpac[i].day][j].charlas[k][h]<< endl;
							// cout << "size : "<< gen.horario[rpac[i].day][h].charlas[k].size() << endl;
							// cout << "2 compare "<< rpac[i].talk<< endl;
							// cout<< "found : "<< found<< endl;
							if(gen.horario[rpac[i].day][j].charlas[k][h] == rpac[i].talk){
								// cout << "encontrado"<< endl;
								viol++;
								found = 1;
								break;
							}
						}
						if(found == 1){
							break;
						}
					}
					if(found == 1){
						break;
					}
				}
			}
		}else{
			for(int j = 0 ; (unsigned)j < gen.horario[rpac[i].day].size();j++){
				if( (gen.horario[rpac[i].day][j].start >= rpac[i].timeslot[0] &&gen.horario[rpac[i].day][j].finish <= rpac[i].timeslot[1]) || (rpac[i].timeslot[0]>gen.horario[rpac[i].day][j].start && rpac[i].timeslot[1]>= gen.horario[rpac[i].day][j].finish) || (rpac[i].timeslot[0]<=gen.horario[rpac[i].day][j].start && rpac[i].timeslot[1]< gen.horario[rpac[i].day][j].finish)){
					for(int k = 0; k< gen.horario[rpac[i].day][j].bsize;k++){
						for (int h = 0 ; h<sessions;h++){
							if( gen.horario[rpac[i].day][j].charlas[k][h] == rpac[i].talk){
								viol++;
								found = 1;
								break;
							}
						}
						if(found == 1){
							break;
						}
					}
					if(found == 1){
						break;
					}
				}
			}
		}
	}
	for(int i = 0; (unsigned)i<gen.horario.size();i++){
		for(int j = 0 ; (unsigned)j<gen.horario[i].size();j++){
			for(int k = 0 ; k < gen.horario[i][j].bsize;k++){
				for (int h = 0 ; h <sessions; h++){
					for(int l = h ; l<sessions;l++){
						if ( tcc[gen.horario[i][j].charlas[k][h]][gen.horario[i][j].charlas[k][l]]== 1){
							viol++;
						}
					}
				}
			}
		}
	}
	return viol;
}