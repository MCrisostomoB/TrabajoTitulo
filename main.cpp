using namespace std;
#include "utils.hpp"
#include "greedy.hpp"

bool compareInterval(Gen g1, Gen g2){

	return 	(g1.violaciones < g2.violaciones) ||((g1.violaciones == g2.violaciones) && (g1.conflictos < g2.conflictos)) || ((g1.violaciones == g2.violaciones) && (g1.conflictos == g2.conflictos)&& (g1.hops < g2.hops));
}


int main(int argc, char** argv){
	int sessions,people,talks,i,j,days,PAC,pob,elitism,totaltime,peso,mode,pseed,cmode;
	float chancemut;
	float chancecruce;
	Gen bestgen;
	vector<string> values;
	vector<string> times;
	string line,valor;
	ifstream file;
	vector<vector<int>> preferences;
	vector<vector<int>> conflicts;
	vector<vector<Bloque>> dbloques;
	vector<Charlas> charlas;
	vector<RPAC> rpac;
	vector<vector<int>> rtcc;
	file.open(argv[1]);
	string instancia = argv[1];
	chancemut = stof(argv[2]);
	chancecruce = stof(argv[4]);
	pob = stoi(argv[3]);
	elitism = stoi(argv[5]);
	totaltime = stoi(argv[6]);
	peso = stoi(argv[7]);
	mode = stoi(argv[8]);
	pseed = stoi(argv[9]);
	instancia = instancia.substr(0,instancia.size()-3);
	string nfile="Resultados-"+instancia+"_"+to_string(pseed)+"_"+to_string(mode)+".txt";
	ofstream out(nfile);
	streambuf *coutbuf = cout.rdbuf();
	cout.rdbuf(out.rdbuf());
	getline(file,line);
	values = split(line,' ');
	sessions =stoi(values[2]);
	talks = stoi(values[1]);
	charlas.resize(talks);
	days = stoi(values[4]);
	people = stoi(values[0]);
	preferences.resize(people);
	dbloques.resize(days);
	vector<vector<int>> horario;
	horario.resize(talks/sessions);
	srand(pseed);
	std::cout << "inicio" << '\n';
    j = 0;
	while(j!= people){
		getline(file,line);
		values = split(line,' ');
		for(int i = 0; (unsigned)i<values.size() ; i++){
			preferences[j].push_back(stoi(values[i]));
		}
		j++;
	}
	// cout<< "tamaño preference 1 : "<< preferences.size() << " tamaño preference 2 : "<< preferences[0].size()<<endl;
	j = 0;
	while(j!=days){
		getline(file,line);
		values = split(line,' ');
		dbloques[j].resize(values.size());
		for(int i = 0; (unsigned)i<values.size() ; i++){
			times = split(values[i],'-');
			dbloques[j][i].start= stoi(times[0])-1;
			dbloques[j][i].finish= stoi(times[1])-1;
			dbloques[j][i].bsize= stoi(times[1])-stoi(times[0])+1;
			// dbloques[j][i].charlas.resize(dbloques[j][i].bsize);
		}
		j++;
	}
	getline(file,line);
	PAC = stoi(line);
	rpac.resize(PAC);
	j = 0;
	while(j!= PAC){
		getline(file,line);
		values = split(line,' ');
		rpac[j].type = stoi(values[0]);
		values.erase(values.begin());
		rpac[j].talk = stoi(values[0]);
		values.erase(values.begin());
		rpac[j].day = stoi(values[0]);
		values.erase(values.begin());
		for(i = 0 ; (unsigned)i<values.size();i++){
			rpac[j].timeslot.push_back(stoi(values[i]));
		}
		j++;
	}
	rtcc.resize(talks);
	for(i = 0;i<talks;i++ ){
		rtcc[i].resize(talks);
		fill(rtcc[i].begin(),rtcc[i].end(),0);
	}

	getline(file,line);
	while(getline(file,line)){
		values = split(line, ' ');
		int pos = stoi(values[0]);
		values.erase(values.begin());
		for(i = 0; (unsigned)i<values.size(); i++){
			rtcc[pos][stoi(values[i])] = 1;
			rtcc[stoi(values[i])][pos] = 1;
		}
	}
	conflicts.resize(talks);
	for(i = 0; i<talks;i++){
		conflicts[i].resize(talks);
		charlas[i].cruce = 9999999;
		charlas[i].charlas.resize(sessions);
		charlas[i].considerado = 0;
		charlas[i].n = 0;

	}

	for(i = 0; i<talks;i++){
		for(int j = 0; j<talks;j++){
			if(i!=j){
				int count = 0;
				for(int p =0 ; p<people;p++){
					if(preferences[p][i]==1 && preferences[p][j]== 1){
						count++;
					}
				}
				conflicts[i][j]= count;
				conflicts[j][i]= count;
				// cout<< "contados : "<< count << "entre : "<< j << " "<< i <<endl;
			}
		}
	}
	// for(i = 0 ; i<talks ; i++){
	// 	for(int j = 0; j<talks; j++){
	// 		cout << "Conflictos entre "<< i << " y " << j << " es: "<< conflicts[j][i] << " y " << conflicts[i][j]<<endl;
	// 	}
	// }
	int totalconflictos = 0 ;
	auto start = chrono::high_resolution_clock::now();
    dbloques = GreedySolution(horario,conflicts,preferences,dbloques ,totalconflictos,charlas ,rpac ,rtcc,sessions,people,talks,days);
	// cout<< "total conflictos contados : "<< totalconflictos<<endl;
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	// cout <<"Duracion en ms: "<< duration.count() << endl;
	// cout << totalconflictos <<endl;
	cout << "poblacion inicial" << endl;
	start = chrono::high_resolution_clock::now();
	vector<Gen> poblacion;
	int bestchild = 99999;
	for(int i = 0; i<pob ; i++){
		Gen ngen;
		// cout<< i <<endl;
		ngen.horario= GreedySchedule(horario,preferences,dbloques, sessions, people, days,rpac,pseed,conflicts);
		// cout<< "gen : "<< i<<endl;
		// for (i= 0; i<days;i++){
		// 	cout << "Dia: "<<i << endl;
		// 	for(int j = 0 ; (unsigned)j<ngen.horario[i].size();j++){
		// 		cout << "Bloque: "<<ngen.horario[i][j].start+1 <<"-"<<ngen.horario[i][j].finish+1 <<endl;
		// 		for(int k = 0; k<ngen.horario[i][j].bsize;k++){
		// 			// cout << "charlas size: "<< ngen.horario[i][j].charlas[k].size()<< endl;
		// 			for(int s = 0; s<sessions ; s++){
		// 				cout << ngen.horario[i][j].charlas[k][s]+1<<'\t';
		// 			}
		// 				cout << "|"<<endl;
		// 		}
		// 		cout  << "------------------------------\n";
		// 	}
		// }
		// cout << "conflictos contados generando : " << ConflictCounttotal(conflicts,ngen.horario)<< endl;
		int total_jumps = JumpCount(ngen.horario,preferences,sessions,people,days);
		ngen.hops = total_jumps;
		ngen.conflictos = totalconflictos;
		ngen.violaciones = 0;
		ngen.costo = ngen.conflictos + ngen.hops;
		// cout<<"costo: "<<ngen.costo<<endl;
		poblacion.push_back(ngen);
		// if(bestchild > ngen.costo){
		// 	bestgen = ngen;
		// 	bestchild = ngen.costo;
		// }
	}
	int infactibles = 0 ;
	vector<int> lbloques;
	for(int i = 0 ; i<days; i++){
		lbloques.push_back(poblacion[0].horario[i].size());
	}

	// for(int i = 0 ;(unsigned)i<horario.size();i++){
	// 	for(int j = 0 ; (unsigned)j<horario[i].size();j++){
	// 		cout<<horario[i][j]<<"|";
	// 	}
	// 	cout<<endl;
	// }

	// cout<< "conflictos originales"<<endl;
	// for(int i = 0 ; i<pob ; i++){
	// 	cout << poblacion[i].conflictos << endl;
	// }
	cout << "inicio Genetico"<< endl;
	int gen = 1;
	sort(poblacion.begin(),poblacion.end(),compareInterval);
	bestgen = poblacion[0];
	bestchild = poblacion[0].costo;
	// cout<< "Viol elit: "<< poblacion[0].violaciones <<endl;
	while(true){
		vector<Gen> temppob;
		vector<double> probs;
		probs.resize(pob);
		float totalprob = 0 ;
		sort(poblacion.begin(),poblacion.end(),compareInterval);
		for (int i = 0 ; i < pob ; i++){
			totalprob += (float)1/poblacion[i].costo;
		}
		// cout << totalprob << endl;
		double acprob = 0 ;
		for (int i = 0 ; i < pob ; i++){
			probs[i] = acprob + (float)(1.0/poblacion[i].costo)/totalprob;
			acprob+= (float)(1.0/poblacion[i].costo)/totalprob;
			// cout<<"acprob: "<< acprob <<"prob i: "<<probs[i]<< endl;
		}
		for(int i = 0 ; i < elitism ; i++){
			temppob.push_back(poblacion[i]);

		}
		// cout << "Elitismo hecho"<< endl;
		// cout << "Horario Formado" << endl;
		// for (i= 0; i<days;i++){
		// 	cout << "Dia: "<<i << endl;
		// 	for(int j = 0 ; (unsigned)j<temppob[0].horario[i].size();j++){
		// 		cout << "Bloque: "<<temppob[0].horario[i][j].start+1 <<"-"<<temppob[0].horario[i][j].finish+1 <<endl;
		// 		for(int k = 0; k<temppob[0].horario[i][j].bsize;k++){
		// 			// cout << "charlas size: "<< temppob[0].horario[i][j].charlas[k].size()<< endl;
		// 			for(int s = 0; s<sessions ; s++){
		// 				cout << temppob[0].horario[i][j].charlas[k][s]+1<<'\t';
		// 			}
		// 				cout << "|"<<endl;
		// 		}
		// 		cout  << "------------------------------\n";
		// 	}
		// }
		// cout << "Total de saltos: "<< poblacion[0].costo << endl;
		// cout<< "creando poblacion temporal "<< endl;
		while(temppob.size()!=pob){
			int ichild1, ichild2;
			float rcruce = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float pcruce = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			for (int i = 0 ; i < pob-1 ; i++){
				// cout<<"prob2: "<< probs[i]<< endl;
				if(probs[i]<=rcruce && probs[i+1]>=rcruce){
					ichild1 = i+1;
					break;
				}else if(probs[i]>=rcruce){
					ichild1 = i;
					break;
				}
			}
			rcruce = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			for (int i = 0 ; i < pob-1 ; i++){
				if(probs[i]<=rcruce && probs[i+1]>=rcruce){
					ichild2 = i+1;
					break;
				}else if(probs[i]>=rcruce){
					ichild2 = i;
					break;
				}
			}
			if(pcruce < chancecruce){
				Gen child1, child2;
				vector<int> contcharlas;
				vector<Missing> repeated;
				vector<int> faltantes;
				int totalblockhops1 = 0,totalblockhops2=0;
				contcharlas.resize(talks);
				for(int i = 0 ; i <talks; i++){
					contcharlas[i] = 0;
				}
				child1.horario.resize(days);
				child2.horario.resize(days);
				// cout << "rcruce "<< rcruce<< endl;
				// if(temppob.size() == 0){
					// cout<< "indices "<<ichild1 << " " << ichild2<<endl;
				// }
				int day = 0;
				for(int j = 0; (unsigned)j<lbloques.size();j++){
					if(lbloques[j]>0){
						int lcruce = rand()%lbloques[j];
						// cout<<"lcruce: "<<lcruce<<endl;
						for ( int k = 0; k< lcruce; k++){
							child1.horario[day].push_back(poblacion[ichild1].horario[day][k]);
							totalblockhops1+= poblacion[ichild1].horario[day][k].totaljumps;
							child2.horario[day].push_back(poblacion[ichild2].horario[day][k]);
							totalblockhops2+= poblacion[ichild2].horario[day][k].totaljumps;

						}
						for ( int k = lcruce; k< lbloques[j]; k++){
							child1.horario[day].push_back(poblacion[ichild2].horario[day][k]);
							totalblockhops1+= poblacion[ichild2].horario[day][k].totaljumps;
							child2.horario[day].push_back(poblacion[ichild1].horario[day][k]);
							totalblockhops2+= poblacion[ichild1].horario[day][k].totaljumps;

						}
						day++;
					}
				}
				for( int i = 0 ; i<days;i++){
					for ( int j = 0 ; (unsigned)j<child1.horario[i].size();j++){
						for(int k = 0; k <child1.horario[i][j].bsize;k++){
							for(int z = 0 ; z<sessions;z++){
								contcharlas[child1.horario[i][j].charlas[k][z]]++;
								if(contcharlas[child1.horario[i][j].charlas[k][z]]>1){
									Missing rp;
									rp.charla = child1.horario[i][j].charlas[k][z];
									rp.block = j;
									rp.day = i;
									rp.ts = k;
									rp.s = z;
									repeated.push_back(rp);
								}
							}
						}
					}
				}
				auto it = find_if(contcharlas.begin(), contcharlas.end(), [](int i){return i == 0;});
				while(it != contcharlas.end()){
					int ind = distance(contcharlas.begin(),it);
					it = find_if(next(it), contcharlas.end(), [](int i){return i == 0;});
					faltantes.push_back(ind);
				}
				child1.hops = totalblockhops1 ;
				child1.conflictos = 0;
				for(int i = 0 ; i<days;i++){
					for(int j = 0; (unsigned)j<child1.horario[i].size();j++){
						// cout<< "bloque conf : "<< child1.horario[i][j].totalconfs <<endl;
						child1.conflictos+= child1.horario[i][j].totalconfs;
					}
				}
				// cout<<"cantidad de conflictos sumados 1 " << child1.conflictos<< endl;
				// child1.violaciones = 0;
				int viol = 0;
				set<DBCheck> dbcheck;
				set<DBCheck>::iterator eit;
				// cout<<"Antes de repeated 1: " <<faltantes.size()<<endl;
				while(repeated.size()!= 0){                           
					dbcheck.insert(DBCheck(repeated[0].day,repeated[0].block));
					int fcount = ConflictCount(conflicts,child1.horario[repeated[0].day][repeated[0].block].charlas[repeated[0].ts]);
					child1.horario[repeated[0].day][repeated[0].block].totalconfs -=fcount;
					child1.conflictos -= fcount;
					child1.horario[repeated[0].day][repeated[0].block].charlas[repeated[0].ts][repeated[0].s] =faltantes[0];
					// viol += constcounter(child1.horario[repeated[0].day][repeated[0].block].charlas[repeated[0].ts],sessions,repeated[0].day,repeated[0].block,faltantes[0],rpac, rtcc);
					fcount = ConflictCount(conflicts,child1.horario[repeated[0].day][repeated[0].block].charlas[repeated[0].ts]);
					child1.conflictos += fcount;
					child1.horario[repeated[0].day][repeated[0].block].totalconfs +=fcount;
					faltantes.erase(faltantes.begin());
					repeated.erase(repeated.begin());
				}
				for(eit = dbcheck.begin(); eit!= dbcheck.end(); eit++){
					// cout <<"dia: "<< (*eit).d << "bloque :" << (*eit).b<<endl;
					child1.hops -=  child1.horario[(*eit).d][(*eit).b].totaljumps;
					BlockCount(child1.horario[(*eit).d][(*eit).b],people,sessions,preferences);
					child1.hops +=  child1.horario[(*eit).d][(*eit).b].totaljumps;

				}
				dbcheck.clear();
				// cout<<"cantidad de conflictos despues de repeated 1 " << child1.conflictos<< endl;
				// child1.violaciones = viol*peso;
				// cout << "child viol : "<< child1.violaciones << endl;
				child1.costo = child1.conflictos + child1.hops;
				// viol = 0;
				// cout<<"hijo 1 listo"<< endl;
				for(int i = 0 ; i< talks;i++){
					if(contcharlas[i]>1){
						infactibles++;
						break;
					}
				}
				fill(contcharlas.begin(),contcharlas.end(),0);
				for( int i = 0 ; i<days;i++){
					for ( int j = 0 ; (unsigned)j<child2.horario[i].size();j++){
						for(int k = 0; k <child2.horario[i][j].bsize;k++){
							for(int z = 0 ; z<sessions;z++){
								contcharlas[child2.horario[i][j].charlas[k][z]]++;
								if(contcharlas[child2.horario[i][j].charlas[k][z]]>1){
									Missing rp;
									rp.charla = child1.horario[i][j].charlas[k][z];
									rp.block = j;
									rp.day = i;
									rp.ts = k;
									rp.s = z;
									repeated.push_back(rp);
								}
							}
						}
					}
				}
				it = find_if(contcharlas.begin(), contcharlas.end(), [](int i){return i == 0;});
				while(it != contcharlas.end()){
					int ind = distance(contcharlas.begin(),it);
					it = find_if(next(it), contcharlas.end(), [](int i){return i == 0;});
					faltantes.push_back(ind);
				}
				child2.conflictos = 0 ;
				child2.hops = totalblockhops2 ;
				for(int i = 0 ; i<days;i++){
					for(int j = 0; (unsigned)j<child2.horario[i].size();j++){
						child2.conflictos+= child2.horario[i][j].totalconfs;
					}
				}
				// cout<<"cantidad de hops sumados 2 " << child2.hops<< endl;
				// cout<<"cantidad de hops sumados 1 " << child1.hops<< endl;
				// child2.conflictos = totalconflictos;
				// child2.violaciones = 0;
				// cout << "antes de repeated 2 "<<repeated.size()<< endl;
				while(repeated.size()!= 0){
					// cout<<"conflic count "<<repeated[0].ts<<endl;
					dbcheck.insert(DBCheck(repeated[0].day,repeated[0].block));
					int fcount = ConflictCount(conflicts,child2.horario[repeated[0].day][repeated[0].block].charlas[repeated[0].ts]);
					child2.horario[repeated[0].day][repeated[0].block].totalconfs -= fcount;
					child2.conflictos -= fcount;
					// cout<< "contado"<<endl;
					child2.horario[repeated[0].day][repeated[0].block].charlas[repeated[0].ts][repeated[0].s] =faltantes[0];
					// cout<< "contacion de violaciones"<<endl;
					// viol += constcounter(child2.horario[repeated[0].day][repeated[0].block].charlas[repeated[0].ts],sessions,repeated[0].day,repeated[0].block,faltantes[0],rpac, rtcc);
					fcount = ConflictCount(conflicts,child2.horario[repeated[0].day][repeated[0].block].charlas[repeated[0].ts]);
					child2.horario[repeated[0].day][repeated[0].block].totalconfs += fcount;
					child2.conflictos += fcount;
					faltantes.erase(faltantes.begin());
					repeated.erase(repeated.begin());
					// cout<< "violaciones: "<< viol<<endl;
					// cout<<"adentro"<<endl;
				}
				for(eit = dbcheck.begin(); eit!= dbcheck.end(); eit++){
					// cout <<"dia: "<< (*eit).d << "bloque :" << (*eit).b<<endl;
					child2.hops -=  child2.horario[(*eit).d][(*eit).b].totaljumps;
					BlockCount(child2.horario[(*eit).d][(*eit).b],people,sessions,preferences);
					child2.hops +=  child2.horario[(*eit).d][(*eit).b].totaljumps;
				}
				dbcheck.clear();
				// cout<<"cantidad de conflictos despues de repeated 2 " << child2.conflictos<< endl;
				// cout << "conflictos child 2 : " << child2.conflictos << endl;
				// cout<<viol<<endl;
				// child2.violaciones = viol*peso;
				// cout << "child viol2 : "<< child2.violaciones << endl;
				child2.costo = child2.conflictos + child2.hops;
				// cout<<"hijo 2 listo"<<endl;
				for(int i = 0 ; i< talks;i++){
					if(contcharlas[i]>1){
						infactibles++;
						break;
					}
				}
				temppob.push_back(child1);
				if(temppob.size() == pob){
					break;
				}
				temppob.push_back(child2);
			}else{
				temppob.push_back(poblacion[ichild1]);
				if(temppob.size() == pob){
					break;
				}
				temppob.push_back(poblacion[ichild2]);
				if(temppob.size() == pob){
					break;
				}
			}
		}
		// cout<< "conflictos poblacion generada"<< endl;
		// for( int i = 0; i<pob; i++){
		// 	cout << temppob[i].conflictos<<endl;
		// }
		// cout<< "Cantidad de hijos infactibles: "<< infactibles << endl;
		// cout << "Mutacion"<<endl;
		if (mode != 3){
			cmode = mode;
		}
		for(int i = 0 ; i<pob ; i++){
			// cout<< "Conteo de conflictos de un horario "<< ConflictCounttotal(conflicts, temppob[i].horario)<<endl;
			// cout << "Horario Formado" << endl;
			// for (i= 0; i<days;i++){
			// 	cout << "Dia: "<<i << endl;
			// 	for(int j = 0 ; (unsigned)j<temppob[i].horario[i].size();j++){
			// 		cout << "Bloque: "<<temppob[i].horario[i][j].start+1 <<"-"<<temppob[i].horario[i][j].finish+1 <<endl;
			// 		for(int k = 0; k<temppob[i].horario[i][j].bsize;k++){
			// 			// cout << "charlas size: "<< temppob[0].horario[i][j].charlas[k].size()<< endl;
			// 			for(int s = 0; s<sessions ; s++){
			// 				cout << temppob[i].horario[i][j].charlas[k][s]+1<<'\t';
			// 			}
			// 				cout << "|"<<endl;
			// 		}
			// 		cout  << "------------------------------\n";
			// 	}
			// }
			// cout << "Total de saltos: "<< poblacion[0].costo << endl;
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			if( r < chancemut){
				float rmode = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				if(mode == 3){
					if(rmode >= 0 &&rmode<=0.4 ){
						cmode = 2;
					}else if (rmode >= 0.4 &&rmode<=0.8 ){
						cmode = 1;
					}else{
						cmode = 0;
					}
				}
				// cout<< "mutando" << endl;
				int rday = rand()%days;
				// cout <<"rday: "<<rday<<endl;
				// cout <<"size: "<<rand()%temppob[i].horario[rday].size()<<endl;
				int rb = rand()%temppob[i].horario[rday].size();
				int rts = rand()%temppob[i].horario[rday][rb].bsize;
				// cout<<"rday: " <<rday<< endl;
				// for(int j = 0 ;(unsigned)j<temppob[i].horario[0].size();j++){
				// 	cout<<"hops en el bloque: "<< temppob[i].horario[0][j].totaljumps<<endl;
				// }
				// cout << "hops totales contados: "<< temppob[i].hops<<endl;
				temppob[i].hops -= temppob[i].horario[rday][rb].totaljumps;
				int rday2 = rand()%days;
				int rb2 = rand()%temppob[i].horario[rday2].size();
				int rts2 = rand()%temppob[i].horario[rday2][rb2].bsize;
				if(cmode == 1){
					int rs1 = rand()%sessions;
					int rs2 = rand()%sessions;
					int tempch = temppob[i].horario[rday][rb].charlas[rts][rs1];
					// cout<<"dia1: "<<rday<< " dia2: "<<rday2 <<" bloque1: "<<rb<<" bloque2: "<<rb2 <<" timeslot1 : "<< rts<<" timeslot2 : "<< rts2 <<" session1: "<<rs1 <<" session2: "<<rs2 << endl;
					// cout<< "conflictos antes : "<< temppob[i].conflictos << endl;
					// cout << "conflictos contados1 : " << ConflictCount(conflicts,temppob[i].horario[rday][rb].charlas[rts])<< endl;
					// cout << "contando conflictos"<< endl;
					int fcount = ConflictCount(conflicts,temppob[i].horario[rday][rb].charlas[rts]);
					// cout << "antes de los fcounts "<< temppob[i].conflictos <<endl;
					// if(temppob[i].conflictos == 3 ){
					// 	for (int l= 0; l<days;l++){
					// 		std::cout << "Dia: "<<i << '\n';
					// 		for(int j = 0 ; (unsigned)j<temppob[i].horario[l].size();j++){
					// 			std::cout << "Bloque: "<<temppob[i].horario[l][j].start+1 <<"-"<<temppob[i].horario[l][j].finish+1 <<'\n';
					// 			for(int k = 0; k<temppob[i].horario[l][j].bsize;k++){
					// 				for(int s = 0; s<sessions ; s++){
					// 					std::cout << temppob[i].horario[l][j].charlas[k][s]<<'\t';
					// 				}
					// 					std::cout << "|\n";
					// 			}
					// 			std::cout  << "------------------------------\n";
					// 		}
					// 	}
					// }
					temppob[i].horario[rday][rb].totalconfs -= fcount;
					temppob[i].conflictos -= fcount;
					// cout << "conflictos restados"<< endl;
					// cout << "fcount 1 "<< fcount << endl;
					if(!(rday == rday2 && rb2 == rb && rts == rts2)){
						// cout<< "dia: "<<rday2 <<" bloque: "<<rb2 <<" timeslot : "<< rts2 << endl;
						// cout << " chequeo: "<< temppob[i].horario[rday2][rb2].charlas[1][0] <<endl;
						fcount = ConflictCount(conflicts,temppob[i].horario[rday2][rb2].charlas[rts2]);
						// cout << "segundos conflictos contados"<<endl;
						// cout << "conflictos contados2 : " << ConflictCount(conflicts,temppob[i].horario[rday2][rb2].charlas[rts2])<< endl;
						temppob[i].conflictos -= fcount;
						temppob[i].horario[rday2][rb2].totalconfs-= fcount;
						// cout << "fcount 2 "<< fcount << endl;
					}
					// cout << "pasado if "<< endl;
					// cout << " antes : " << temppob[i].horario[rday][rb].charlas[rts][rs1] <<" "<< temppob[i].horario[rday2][rb2].charlas[rts2][rs2]<<" "<<tempch<< endl;
					temppob[i].horario[rday][rb].charlas[rts][rs1] = temppob[i].horario[rday2][rb2].charlas[rts2][rs2];
					temppob[i].horario[rday2][rb2].charlas[rts2][rs2] = tempch;
					// cout << " despues : " << temppob[i].horario[rday][rb].charlas[rts][rs1] <<" "<< temppob[i].horario[rday2][rb2].charlas[rts2][rs2]<<" "<<tempch<< endl;
					// cout<<"antes:"<< temppob[i].hops<< endl;
					// cout<< "dia: "<<rday2 <<" bloque: "<<rb2 <<endl;
					// cout<< "bloque: "<< temppob[i].horario[rday2][rb2].totaljumps <<endl;
					if(!(rday == rday2 && rb2 == rb)){
						temppob[i].hops -= temppob[i].horario[rday2][rb2].totaljumps;
						BlockCount(temppob[i].horario[rday2][rb2],people,sessions,preferences);
						temppob[i].hops += temppob[i].horario[rday2][rb2].totaljumps;
					}


					// cout<<"resta:"<< temppob[i].hops<< endl;
					// cout<<"suma:"<< temppob[i].hops<< endl;
					// cout << "conflictos contados final 1 : " << ConflictCount(conflicts,temppob[i].horario[rday][rb].charlas[rts])<< endl;
					// temppob[i].violaciones = (constcounter(temppob[i].horario[rday2][rb2].charlas[rts2],sessions,rday2,rb2,temppob[i].horario[rday2][rb2].charlas[rts2][rs2],rpac, rtcc)+constcounter(temppob[i].horario[rday][rb].charlas[rts],sessions,rday,rb,temppob[i].horario[rday][rb].charlas[rts][rs1],rpac, rtcc))*peso;
					fcount = ConflictCount(conflicts,temppob[i].horario[rday][rb].charlas[rts]);
					temppob[i].conflictos += fcount;
					temppob[i].horario[rday][rb].totalconfs += fcount;
					// cout << "fcount 3 "<< fcount << endl;
					if(!(rday == rday2 && rb2 == rb && rts == rts2)){
						fcount = ConflictCount(conflicts,temppob[i].horario[rday2][rb2].charlas[rts2]);
						// cout<< "indices " << rday << " "<< rday2 << " " << rb << " "<< rb2 << " "<<rts <<" "<<rts2 <<endl;
						// cout << "conflictos contados final 2 : " << ConflictCount(conflicts,temppob[i].horario[rday2][rb2].charlas[rts2])<< endl;
						temppob[i].conflictos += fcount;
						temppob[i].horario[rday2][rb2].totalconfs+= fcount;
					}
					// cout << "fcount 4 "<< fcount << endl;
					// cout<< "conflictos final : "<< temppob[i].conflictos << endl;
					// if(temppob[i].conflictos <0){
					// 	for (int l= 0; l<days;l++){
					// 		std::cout << "Dia: "<<l << '\n';
					// 		for(int j = 0 ; (unsigned)j<temppob[i].horario[l].size();j++){
					// 			std::cout << "Bloque: "<<temppob[i].horario[l][j].start+1 <<"-"<<temppob[i].horario[l][j].finish+1 <<'\n';
					// 			for(int k = 0; k<temppob[i].horario[l][j].bsize;k++){
					// 				for(int s = 0; s<sessions ; s++){
					// 					std::cout << temppob[i].horario[l][j].charlas[k][s]<<'\t';
					// 				}
					// 					std::cout << "|\n";
					// 			}
					// 			std::cout  << "------------------------------\n";
					// 		}
					// 	}
					// }
				}else if(cmode == 2){
					vector<int> temp ;
					temp = temppob[i].horario[rday][rb].charlas[rts];
					temppob[i].horario[rday][rb].totalconfs -= ConflictCount(conflicts,temppob[i].horario[rday][rb].charlas[rts]);
					if(!(rday == rday2 && rb2 == rb && rts == rts2 )){
						temppob[i].horario[rday2][rb2].totalconfs -= ConflictCount(conflicts,temppob[i].horario[rday2][rb2].charlas[rts2]);
					}
					temppob[i].horario[rday][rb].charlas[rts] = temppob[i].horario[rday2][rb2].charlas[rts2];
					temppob[i].horario[rday2][rb2].charlas[rts2] = temp;
					temppob[i].horario[rday][rb].totalconfs +=ConflictCount(conflicts,temppob[i].horario[rday][rb].charlas[rts]);
					if(!(rday == rday2 && rb2 == rb && rts == rts2 )){
						temppob[i].horario[rday2][rb2].totalconfs +=ConflictCount(conflicts,temppob[i].horario[rday2][rb2].charlas[rts2]);
					}
					if(!(rday == rday2 && rb2 == rb)){
						temppob[i].hops -= temppob[i].horario[rday2][rb2].totaljumps;
						BlockCount(temppob[i].horario[rday2][rb2],people,sessions,preferences);
						temppob[i].hops += temppob[i].horario[rday2][rb2].totaljumps;
					}
				}else{
					next_permutation(temppob[i].horario[rday][rb].charlas[rts].begin(),temppob[i].horario[rday][rb].charlas[rts].end());
				}
				// cout<<"antes:"<< poblacion[i].hops<< endl;
				// cout<<"resta:"<< poblacion[i].hops<< endl;
				BlockCount(temppob[i].horario[rday][rb],people,sessions,preferences);
				temppob[i].hops += temppob[i].horario[rday][rb].totaljumps;
				// cout<<"despues:"<< poblacion[i].hops<< endl;
				temppob[i].costo = temppob[i].conflictos + temppob[i].hops;
				// cout<< "violaciones: "<< temppob[i].violaciones <<endl;
				// cout <<"costos: "<< poblacion[i].costo<< endl;
				// for(int j = 0 ; j<days; j++){
				// 	for(int k = 0 ;(unsigned)k<temppob[i].horario[j].size();k++){
				// 		cout<<"Hops en el bloque: "<< temppob[i].horario[j][k].totaljumps<<endl;
				// 	}
				// }
				// cout << "Hops sumados: " <<temppob[i].hops<<endl;
				// cout<<"----"<<endl;

			}
			// cout<< "check violaciones "<<endl;
			temppob[i].violaciones = CompleteConstCount(temppob[i],sessions, rpac,rtcc);
			// cout<< "violaciones chequeadas "<< endl;
			temppob[i].costo += temppob[i].violaciones * peso;
			if(compareInterval(temppob[i],bestgen)){
				bestgen = temppob[i];
				bestchild = temppob[i].costo;
				// cout<<"mejoro"<<endl;
			}
			// cout << i << endl;
		}
		poblacion = temppob;
		sort(poblacion.begin(),poblacion.end(),compareInterval);
		// cout<< "orden"<<endl;
		// for( int i = 0;i<pob;i++){
		// 	cout<<poblacion[i].conflictos << " " <<poblacion[i].hops<<" "<<poblacion[i].violaciones<< endl;
		// }
		stop = chrono::high_resolution_clock::now();
		duration = chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		// cout <<"El mejo resultado saltos + conflictos: " << bestchild <<" El mejor de la generacion actual: " << poblacion[0].costo<<" Tiempo[ms]: "<< duration.count()<<" promedio de infactibles: "<< infactibles/gen<<endl;
		cout << bestchild <<";" << poblacion[0].hops<<";" << poblacion[0].conflictos<<";" << poblacion[0].violaciones<< ";"<<duration.count()<<endl;
		gen++;
		if(std::chrono::milliseconds(totaltime*1000).count()<duration.count()){
			break;
		}
	}
	std::cout << "Horario Formado" << '\n';
	for (i= 0; i<days;i++){
		std::cout << "Dia: "<<i << '\n';
		for(int j = 0 ; (unsigned)j<bestgen.horario[i].size();j++){
			std::cout << "Bloque: "<<bestgen.horario[i][j].start+1 <<"-"<<bestgen.horario[i][j].finish+1 <<'\n';
			for(int k = 0; k<bestgen.horario[i][j].bsize;k++){
				for(int s = 0; s<sessions ; s++){
					std::cout << bestgen.horario[i][j].charlas[k][s]+1<<'\t';
				}
					std::cout << "|\n";
			}
			std::cout  << "------------------------------\n";
		}
	}
	std::cout << "Costo Total: "<< bestgen.costo << '\n' << "Saltos totales: "<< bestgen.hops << "\n Conflictos totales: "<< bestgen.conflictos<< "\n Violaciones totales: "<< bestgen.violaciones<< endl;
	cout.rdbuf(coutbuf);
	return 0;
}
