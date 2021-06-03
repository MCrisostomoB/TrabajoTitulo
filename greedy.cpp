#include "utils.hpp"
#include "greedy.hpp"
using namespace std;

vector<vector<Bloque>> GreedySolution(vector<vector<int>> &horario,vector<vector<int>> &conflicts,vector<vector<int>> preferences  ,vector<vector<Bloque>> dbloques ,int &totalconflictos,vector<Charlas> charlas ,vector<RPAC> rpac ,vector<vector<int>> rtcc,int sessions,int people,int talks,int days){
	vector<vector<int>> swaps;
	swaps.resize(sessions+1);
	int i;
	for(int i=0;i<talks;i++){
	  for(int j= 0;j<talks;j++){
		  for(int k = 0;k<people;k++){
			  if(preferences[k][i]== 1 && preferences[k][j]==1 && i!=j){
				  charlas[i].otrascharlas.push_back(j);
				  charlas[i].lcharlas++;
				  break;
			  }
		  }
	  }
	}


	// for(i=0;i<talks;i++){
	//   std::cerr << "Charla:"<<i<< '\n';
	//   for(int j=0;j<charlas[i].lcharlas;j++){
	// 	  std::cout << charlas[i].otrascharlas[j]<< '\t';
	//   }
	//   std::cout << "\n" << '\n';
	// }

	int n;
	for(i=0;i<talks;i++){
		n=0;
		int actualval = 0;
		if(charlas[i].considerado == 0){
			for(int j=0;j<charlas[i].lcharlas;j++){
				if(n==sessions){
					break;
				}
				if(charlas[i].otrascharlas[j]!= actualval){
					if(charlas[actualval].considerado==0){
						// cout<<"actualval: "<<actualval<<"\n";
						if(checkconst1(actualval,charlas[i].charlas,n,rtcc)){
							int posible = 1;
							if(n>1){
								for(int r = 0; r<n;r++){
									for(int f =0; f < charlas[charlas[i].charlas[r]].lcharlas; f++){
										if(charlas[charlas[i].charlas[r]].otrascharlas[f] == actualval){
											posible = 0;
											break;
										}
									}
								}
							}
							if(posible == 1){
								int found = 0;

								for(int t = 0; t<n;t++){
									if(charlas[i].charlas[t] == i){
										found = 1;
									}
								}
								if(found == 0 && n==sessions-1){
									charlas[i].charlas[n]=i;
									charlas[i].cruce = 0;
									charlas[i].considerado = 1;
								}else{
									// cout << "found: "<< actualval << "\n";
									charlas[i].charlas[n]=actualval;
									charlas[i].cruce = 0;
									charlas[actualval].considerado = 1;
								}

								n++;

								for(int t = 0; t<n;t++){
									charlas[charlas[i].charlas[t]].n= n;
								}

							}
						}
					}
					if(actualval<charlas[i].otrascharlas[j]){
						j--;
					}
				}

				actualval++;
			}

		}
		if(sessions!= 2){
			if(charlas[i].n < sessions-1){
				charlas[i].cruce = 9999999;
				charlas[i].n = 0;
				charlas[i].considerado=0;
				for(int s= 0; s<n; s++){
					charlas[charlas[i].charlas[s]].considerado = 0;
					charlas[i].charlas[s] = 0;
				}
			}
		}else{
			if(charlas[i].n <= sessions-1){
				charlas[i].cruce = 9999999;
				charlas[i].n = 0;
				charlas[i].considerado=0;
				for(int s= 0; s<n; s++){
					charlas[charlas[i].charlas[s]].considerado = 0;
					charlas[i].charlas[s] = 0;
				}
			}
		}
	}
	// std::cout << "resultado 1" << '\n';
	
	// for(i=0;i<talks;i++){
	// 	if(charlas[i].cruce == 0){
	//     	cout << charlas[i].cruce <<"\t"<<charlas[i].charlas[0]+1<<"\t"<<charlas[i].charlas[1]+1<<'\n';
	// 	}
	// }
	vector<int> charlasrestantes;
	int lrestantes=0;
	for(i=0;i<talks+1;i++){
		if(charlas[i].considerado == 0){
			charlasrestantes.push_back(i);
			lrestantes++;
			// cout <<"charla: "<<i+1<<'\n';
		}
	}
	for(i = 0 ; i<talks; i++){
		if(charlas[i].n<sessions && charlas[i].cruce == 0){
			charlas[i].cruce = 5000;
			for(int k = 0 ; k<lrestantes; k++){
				int newsum = 0;
				for(int h = 0 ; h < people; h++){
					int tempsum = 0;
					for(int g = 0 ; g< charlas[i].n-1;g++){
						tempsum = tempsum + preferences[h][charlas[i].charlas[g]];
					}
					tempsum = tempsum + preferences[h][charlasrestantes[k]];
					if(tempsum != 0){
						newsum= newsum + tempsum-1;
					}
				}
				if(charlas[i].cruce>newsum){
					
					if(charlas[charlasrestantes[k]].considerado==0 && checkconst1(charlasrestantes[k],charlas[i].charlas,charlas[i].n,rtcc)){
						charlas[i].cruce = newsum;
						if(charlas[i].charlas[sessions-1] == 0){
							charlas[charlasrestantes[k]].considerado = 1;
							charlas[i].charlas[sessions-1] = charlasrestantes[k];
						}else{
							charlas[charlasrestantes[k]].considerado = 1;
							charlas[charlas[i].charlas[sessions-1]].considerado = 0;
							charlas[i].charlas[sessions-1] = charlasrestantes[k];
						}
					}
				}
			}
		}
	}
	// std::cout << "resultado 2" << '\n';
	// for(i=0;i<talks;i++){
	// 	if(charlas[i].cruce != 9999999){
	// 		cout << charlas[i].cruce <<"\t"<<charlas[i].charlas[0]+1<<"\t"<<charlas[i].charlas[1]+1<<"\t"<<charlas[i].charlas[2]+1<<'\n';
	// 	}
	// }
	charlasrestantes.clear();
	lrestantes = 0;
	// std::cout << "segunda tanda" << '\n';
	for(int h = 0; h<talks; h++){
		if(charlas[h].considerado == 0){
			charlasrestantes.push_back(h);
			lrestantes++;
			// cout <<"charla: "<<h+1<<'\n';
		}
	}
	for(int h = 0 ; h<lrestantes;h++){
		if(charlas[charlasrestantes[h]].considerado == 0){
			charlas[charlasrestantes[h]].considerado = 1;
			charlas[charlasrestantes[h]].charlas[0] = charlasrestantes[h];
			charlas[charlasrestantes[h]].n=1;
		}
		int changed = 0;
		for(int j = 0 ; j <lrestantes;j++){
			if(h!=j&& charlas[charlasrestantes[j]].considerado!=1 && checkconst1(charlasrestantes[j],charlas[charlasrestantes[h]].charlas,charlas[charlasrestantes[h]].n,rtcc)){
				int newsum = 0;
				for(int p = 0 ; p<people;p++){
					int tempsum = 0;
					for(int g = 0 ; g< charlas[charlasrestantes[h]].n;g++){
						tempsum = tempsum + preferences[p][charlas[charlasrestantes[h]].charlas[g]];
					}

					tempsum = tempsum + preferences[p][charlasrestantes[j]];
					if(tempsum != 0){
						newsum= newsum + tempsum-1;
					}
				}
				// std::cout << "nueva suma : "<<newsum <<" Cruce : " <<charlas[charlasrestantes[h]].cruce<< '\n';
				if(charlas[charlasrestantes[h]].cruce>newsum&&charlas[charlasrestantes[h]].n != sessions){
					changed = 1;
					charlas[charlasrestantes[h]].charlas[charlas[charlasrestantes[h]].n] =charlasrestantes[j];
					charlas[charlasrestantes[h]].cruce = newsum;
				}
			}
		}
		if(changed == 1){
			charlas[charlas[charlasrestantes[h]].charlas[charlas[charlasrestantes[h]].n]].considerado = 1;
			charlas[charlasrestantes[h]].n++;
			for(int p = 1 ; p<charlas[charlasrestantes[h]].n;p++){
				charlas[charlas[charlasrestantes[h]].charlas[p]].n = charlas[charlasrestantes[h]].n;
			}
		}
		// std::cout << "charla restante: "<<charlas[charlasrestantes[h]].n <<"En h:"<<h<< '\n';
		if(charlas[charlasrestantes[h]].n != sessions){
			charlas[charlasrestantes[h]].cruce = 9999999;
			h--;
		}
		// std::cout << "H:"<<h << '\n';

	}
	int conflicto = 0,thorario = 0;
	// string nfile="Resultados_"+(string)argv[1];
	// ofstream out(nfile);
	// streambuf *coutbuf = cout.rdbuf();
	// cout.rdbuf(out.rdbuf());
	// cout <<"Conflictos"<<"\t";
	// for(i = 0;i<sessions; i++){
	// 	cout <<"Ch"<<i+1<< "\t";
	// }
	// std::cout <<'\n';
	for(i=0;i<talks;i++){
		if(charlas[i].cruce != 9999999){
			// cout << charlas[i].cruce <<"\t\t";
			for(int j=0; j<sessions; j++){
				horario[thorario].push_back((int)charlas[i].charlas[j]);
				// cout <<charlas[i].charlas[j]+1<<"\t";
			}
			// std::cout  << '\n';
			totalconflictos = totalconflictos+ charlas[i].cruce;
			thorario++;
		}
	}
	// cout<< "adentro de greedy solution total cruce : "<< totalconflictos <<endl;
	// std::cout <<"\n"<< "Conflicto total: "<< totalconflictos<< '\n';
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle (horario.begin(),horario.end(),std::default_random_engine(seed));
	// for(i = 0; (unsigned)i<horario.size();i++){
	// 	for(j = 0 ;(unsigned)j<horario[i].size();j++){
	// 		cout<< horario[i][j]<<"\t";
	// 	}
	// 	cout <<"\n";
	// }
	
	// cout << "Asignacion \n";
	int pos = 0;
	vector<vector<int>> unused;
	// vector<int> bestperm;
	for(i = 0; i<days;i++){
		for(int j = 0 ;(unsigned)j<dbloques[i].size();j++){
			// int jumps = 999999;
			for(int p = 0 ; p<dbloques[i][j].bsize;p++){
				if(checkconst2v(i,dbloques[i][j].start+pos,horario[pos],rpac)){
					// cout<<"adentro de check const 2v"<<endl;
					dbloques[i][j].charlas.push_back(horario[pos]);
					// for(int h = 0 ; (unsigned)h<dbloques[i][j].charlas[p].size();h++){
					// 	cout<< dbloques[i][j].charlas[p][h]<<" " ;
					// }
					// cout<<endl;
				}else{
					// cout<<"Falso"<<"\n";
					unused.push_back(horario[pos]);
					// break;
				}
				pos++;
				// cout<<"pos: "<<pos<<"\n";
				// if((unsigned)pos ==horario.size()){
				// 	break;
				// }
			}
			dbloques[i][j].totaljumps = 0;
			dbloques[i][j].totalconfs = 0;

		}
	}
	// cout<< "chequeo "<<endl;
	pos = 0;
	for(int i = 0; i<days;i++){
		for(int j = 0 ;(unsigned)j<dbloques[i].size();j++){
			if(dbloques[i][j].bsize != dbloques[i][j].charlas.size()){
				for(int p = dbloques[i][j].charlas.size() ; p<dbloques[i][j].bsize;p++){
					dbloques[i][j].charlas.push_back(unused[pos]);
					pos++;
				}
			}
		}
	}

	// cout<<"Jump Count"<<"\n";
	// dbloques[0][0].charlas[0][0] = 4;
	// dbloques[0][0].charlas[0][1] = 0;
	// dbloques[0][0].charlas[1][0] = 5;
	// dbloques[0][0].charlas[1][1] = 1;

	int total_jumps = JumpCount(dbloques,preferences,sessions,people,days);

	// std::cout << "Horario Formado" << '\n';
	// for (i= 0; i<days;i++){
	// 	std::cout << "Dia: "<<i << '\n';
	// 	for(int j = 0 ; (unsigned)j<dbloques[i].size();j++){
	// 		std::cout << "Bloque: "<<dbloques[i][j].start <<"-"<<dbloques[i][j].finish <<'\n';
	// 		for(int k = 0; k<dbloques[i][j].bsize;k++){
	// 			for(int s = 0; s<sessions ; s++){
	// 				std::cout << dbloques[i][j].charlas[k][s]+1<<'\t';
	// 			}
	// 				std::cout << "|\n";
	// 		}
	// 		std::cout  << "------------------------------\n";
	// 	}
	// }
	// std::cout << "Total de saltos: "<< total_jumps << '\n';
  return dbloques;
}

vector<vector<Bloque>> GreedySchedule(vector<vector<int>> &horario,vector<vector<int>> preferences,vector<vector<Bloque>> dbloques,int sessions,int people,int days,vector<RPAC> rpac,int pseed, vector<vector<int>> conflicts){
	for (int i = 0; i <days;i++){
		for(int j = 0 ; (unsigned)j<dbloques[i].size();j++){
			dbloques[i][j].totaljumps = 0;
			dbloques[i][j].totalconfs = 0;
			for(int k = 0 ; k<dbloques[i][j].bsize;k++){
				dbloques[i][j].charlas[k].clear();
				dbloques[i][j].charlas[k].resize(0);
				dbloques[i][j].charlas.resize(0);
			}
		}
	}
	// cout << "dbloques horario size1 "<< dbloques[0][0].charlas.size()<< endl;
	shuffle (horario.begin(),horario.end(),std::default_random_engine(pseed));
	// for(int i = 0 ; i <horario.size();i++){
	// 	for(int j = 0 ; (unsigned)j<horario[i].size();j++){
	// 		cout<< horario[i][j]<<" " ;
	// 	}
	// 	cout<<"|"<<endl;
	// }
	int pos = 0;
	vector<vector<int>> unused;
	for(int i = 0; i<days;i++){
		for(int j = 0 ;(unsigned)j<dbloques[i].size();j++){
			for(int p = 0 ; p<dbloques[i][j].bsize;p++){
				// cout<< "dia: " << i << "ts : "<< dbloques[i][j].start+pos<<endl;
				if(checkconst2v(i,dbloques[i][j].start+pos,horario[pos],rpac)){
					dbloques[i][j].charlas.push_back(horario[pos]);
					// cout<<"adentro de check const 2v2"<<endl;
					// for(int h = 0 ; (unsigned)h<dbloques[i][j].charlas[p].size();h++){
					// 	cout<< dbloques[i][j].charlas[p][h]<<" " ;
					// }
					// cout<<endl;
				}else{
					// cout<<pos<<"\n";
					// cout<<"Falso2"<<"\n";
					unused.push_back(horario[pos]);
					// break;
				}
				pos++;
				// cout<<"pos: "<<pos<<"\n";
				// if((unsigned)pos ==horario.size()){
				// 	break;
				// }
			}
			dbloques[i][j].totaljumps = 0;
			dbloques[i][j].totalconfs = 0;		}
	}
	// cout << "unused size: "<< unused.size()<<endl;
	// cout << "dbloques horario size "<< dbloques[0][0].charlas.size()<< endl;
	// std::cout << "Horario Formado" << '\n';

	// for (int i= 0; i<days;i++){
	// 	std::cout << "Dia: "<<i << '\n';
	// 	for(int j = 0 ; (unsigned)j<dbloques[i].size();j++){
	// 		std::cout << "Bloque: "<<dbloques[i][j].start <<"-"<<dbloques[i][j].finish <<'\n';
	// 		for(int k = 0; k<dbloques[i][j].bsize;k++){
	// 			for(int s = 0; s<sessions ; s++){
	// 				std::cout << dbloques[i][j].charlas[k][s]<<'\t';
	// 			}
	// 				std::cout << "|\n";
	// 		}
	// 		std::cout  << "------------------------------\n";
	// 	}
	// }
	pos = 0;
	for(int i = 0; i<days;i++){
		for(int j = 0 ;(unsigned)j<dbloques[i].size();j++){
			if(dbloques[i][j].bsize != dbloques[i][j].charlas.size()){
				for(int p = dbloques[i][j].charlas.size() ; p<dbloques[i][j].bsize;p++){
					dbloques[i][j].charlas.push_back(unused[pos]);
					pos++;
				}
			}
		}
	}
	for(int i = 0; i<days;i++){
		for(int j = 0 ;(unsigned)j<dbloques[i].size();j++){
			dbloques[i][j].totalconfs = ConflictCountBlock(conflicts, dbloques[i][j].charlas);
		}
	}
 



	// std::cout << "Horario Formado" << '\n';
	// for (int i= 0; i<days;i++){
	// 	std::cout << "Dia: "<<i << '\n';
	// 	for(int j = 0 ; (unsigned)j<dbloques[i].size();j++){
	// 		std::cout << "Bloque: "<<dbloques[i][j].start <<"-"<<dbloques[i][j].finish <<'\n';
	// 		for(int k = 0; k<dbloques[i][j].bsize;k++){
	// 			for(int s = 0; s<sessions ; s++){
	// 				std::cout << dbloques[i][j].charlas[k][s]+1<<'\t';
	// 			}
	// 				std::cout << "|\n";
	// 		}
	// 		std::cout  << "------------------------------\n";
	// 	}
	// }
	// std::cout << "Total de saltos: "<< total_jumps << '\n';
  return dbloques;

}