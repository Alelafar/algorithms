#include<iostream>
#include<vector>
#include<algorithm>
#include<assert.h>
#include<stdio.h>
#include<fstream>
#include<math.h>
#include<string>

const int N_stampa=5;
const float Confronto=0;

using namespace std; 

void print(vector<vector<int> >& v, int n) {

  for(int i=0; i<n; i++){
    cout << i << " -> ";
    for(int j=0; j<v[i].size(); j++) {
      cout << v[i][j] << " ; ";
    }
    cout << endl;
  }
}


void Intersezione(vector<int>& V, vector<vector<int> >& gin, vector<vector<int> >& gex, int l, int nodo){
	
		for(int p=0; p<gex[l].size(); p++){
		if(gex[l][p] != nodo){
			int k=0;
			while(gin[gex[l][p]][k]<nodo)
				k++;
			if(gin[gex[l][p]][k]==nodo)
    	 		V.push_back(gex[l][p]); 
		}
	}
}

float Somma_entrante(vector<vector<int> >& gin, vector<vector<int> >& gex, int l, int n, vector<int>& rank){
	float sum=0;
	vector<int> Vin /*intersezione entrante*/;
	if(gin[l].size() != 0 && gin[n].size() != 0)
		Intersezione(Vin,gex,gin,l,n);
	for(int j=0; j<Vin.size(); j++){
		float m;
		float t=gex[Vin[j]].size() /* è almeno 2 quindi != 0 */ ;
		float f,p= rank[Vin[j]];
		f=1/(1+p);
		m = (1 / (t+f) );
		sum= sum + m;
	}
	Vin.erase(Vin.begin(),Vin.end());
return sum;	
}

float Calcolo(vector<vector<int> >& gin, vector<vector<int> >& gex, vector<int>& V, int l, int n, vector<int>& rank){
	float res /* (similitudine tra il nodo l e il nodo n) */, temp=0; 
	float d,e,f, a=V.size(), b=gex[l].size(), c=gex[n].size(); /* tutti e tre sono sempre diversi da 0 */
	
	d=1/b;
	e=1/c;
	res=( a )*( d  + e ) ;
	for(int i=0; i< V.size(); i++){
		float t;
		float m=gin[V[i]].size(); /* vale almeno 2 quindi sicuramente != 0 */
		t= ( 1/m );
		float u=rank[V[i]];
		float v = ( 1/ (1 + u ) );
		temp= temp + ( t ) * ( v );
	}
	res= res * ( temp  * Somma_entrante(gin,gex,l,n,rank) ) ;
	
return res;		
}

bool myComparison(const pair<float,int> &a, const pair<float,int> &b)
{
      return a.first<b.first;
}

void Similitudine(int l, vector<vector<int> >& gin, vector<vector<int> >& gex, vector<int>& rank){
	vector<pair<float,int> > s(854); /* ( vettore di similitudini) */
	vector <int> V;
	
	for(int i=0; i<gex.size(); i++){
		V.erase(V.begin(),V.end());
		if(i == l){
			s[i]=make_pair(-1,i);
		}
		else{
			/*vector<int> V;*/
			if(gex[i].size() != 0)
				Intersezione(V,gin,gex,l,i);				
			if(V.size()==0)
				s[i]=make_pair(0,i);
			else{
				s[i]=make_pair(Calcolo(gin,gex,V,l,i,rank),i);
			}  
		}
	}
			
	sort(s.begin(), s.end());
	
//	for(int j=0; j<854; j++){
//		cout<< s[j].first << " " << s[j].second <<endl; 
//	}


	ifstream Name ("AlexaRanks.txt");
	
	if(Name.is_open()){
		
		vector<string> nome(854);
		
		for(int j=0; j<854; j++){
			getline(Name, nome[j]);
		}
		
		int cont=1;
		
		for(int j=0; j<N_stampa; j++){
			if(s[853-j].first != Confronto)
				//cout << -s[j].first << " " << " " << nome[s[j].second];
				cout << s[853-j].first << "  " << nome[s[853-j].second];
			else{
				//printf("%s", &nome[s[j].second]);
				cout << s[cont].first << " " << nome[s[cont].second];
				cont++;
			}
			cout << endl;
		}
		cout <<endl;
	}
	
	else	
	   cout << "AlexaRanks could not be opened"	<< endl;
	    
	Name.close();
		
}


int main(){
	FILE *Netinf;
	int nn /*(numero nodi)*/, na /*(numero archi)*/;
	int x,y; /*(ausiliarie)*/
	int l /*(nodo da indagare)*/;
	
	cout << "Il programma prende in input un intero l che individua un nodo nel grafo 'Netinf'. Si restituiscono, quindi, i 5 nodi piu' simili al nodo l." << endl << endl;
	Netinf=fopen("NetInf.txt", "r");
	fscanf(Netinf, "%d %d", &nn,&na);
	
	//vector<float> rank(nn);
	vector<int> rank(nn);
	vector<vector<int> > gin(nn) /*(grafo ingresso)*/;
	vector<vector<int> > gex(nn) /*(grafo uscita)*/;
	
	for(int i=0; i<nn ; i++){
		fscanf(Netinf, "%d", &rank[i]);
	}
		
	// for(int i=0; i<nn ; i++)
	//	cout<<rank[i] << " ";
	
	// cout<< endl;
	
	for( int i=0; i<na; i++){
		fscanf(Netinf, "%d %d", &x,&y);
		gex[x].push_back(y);
		gin[y].push_back(x);
	}
	
	fclose(Netinf);
	
	for(int j=0; j<nn; j++){
		sort(gin[j].begin(), gin[j].end());
		sort(gex[j].begin(), gex[j].end());
	}
	
	cout << "Digitare il nodo l: " ;
	cin >> l;
	cout << endl;
	
//	print(gex,gex.size());
//	print(gin,gin.size());
	
	if(l>=nn)
		cout << "Error: this node doesn't exist" <<endl;
	else
		Similitudine(l,gin, gex, rank);
		
return 0;	
}
