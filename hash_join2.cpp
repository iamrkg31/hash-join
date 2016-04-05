#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
using namespace std;

int arr[16];

//used in hash_func_final
int hash_func(int arr[],int k_l)
{
	int temp=0;

	for(int j=0;j<k_l;j++){
		temp=temp+pow(2,j)*arr[j];
	}

	return temp;
}

// calculate binary equivalent of n (only last k_l bits) and convert them in decimal and use in hash function
int hash_func_final(int n,int k_l){
	int i;
	
	for(int j=0;j<k_l;j++){		  
		  arr[j]=n%2;		  
		  n=n/2;
		  if(n==0){
			//eg. in case of 1, 1st bit becomes 1 but rest k_l-1 bits should be 0
                  	for(int i=j+1;i<k_l;i++){ 
				arr[i]=0;
			}	

			break;
		}
		 
	}

	i=hash_func(arr,k_l);     
        for(int j=0;j<16;j++) arr[i]=0;
	return i;
}

int main(void) {
	
	vector<vector<int> > vec_r1,vec_r2; //two 2d vectors whose each row stores no. of records in a buckets (row no.= bucket no)
	vector<int> r1,r2; 
        
	int record_size1,record_size2,page_size,available_pages,max_hash_round;

	record_size1 = 180;  //record size of relation 1
	record_size2 = 130;  //record size of relation 2
	page_size = 400;     //page size for both relations
	available_pages = 5; //available no. of pages = pages in lth bucket of r1 + pages in lth bucket of r2 + 1 page (for output)
	max_hash_round = 3;  //maximum hashing rounds 
        
        
	// open input file    
	ifstream file1("r1.txt");
	ifstream file2("r2.txt");

	// test file open   
	if (file1) {        
	    int val;
	    // read the elements in the file into a vector r1 
	    while ( file1 >> val ) {
		r1.push_back(val);
	    }
	}

	// test file open   
	if (file2) {  
	    int val;
	    // read the elements in the file into a vector r2
	    while ( file2 >> val ) {
		r2.push_back(val);
	    }
	}

	// close the files
	file1.close();
	file2.close();



	int size_r1,size_r2,buckets,buckets1,hash_round=1,flag=0,temp,k_l;
        int records_per_page_r1,records_per_page_r2;

	buckets = (available_pages -1);	  //initially no. of buckets
        buckets1 = (available_pages -1); //initially no. of buckets
	temp=0;
        records_per_page_r1 = page_size/record_size1; //no of records in a page in r1
	records_per_page_r2 = page_size/record_size2; //no of records in a page in r2

      

	// loop unless max hashing round is reached or no more violence of available pages in each bucket
	while(1){
 		k_l = buckets1 + hash_round; // used in determining hash function 
                int page_full_check_r1[buckets],page_full_check_r2[buckets]; //arrays used, when a page is full flush to secondary storage 

		size_r1 = ceil(float(record_size1*r1.size())/page_size); //size of relation 1
		size_r2 = ceil(float(record_size2*r2.size())/page_size); //size of relation 2

                flag =0;
		
		for(int i=0;i<buckets;i++){

			vector<int> row;
			vec_r1.push_back(row);
			vec_r2.push_back(row);

			page_full_check_r1[i]=records_per_page_r1; //in relation 1, no of records in a page
			page_full_check_r2[i]=records_per_page_r2; //in relation 2, no of records in a page

		}     
  
                cout<<"\nhashing round "<<hash_round<<": "<<endl;
		cout<<"size of relation 1: "<<size_r1<<endl; //size of relation 1 in each hashing round
		cout<<"size of relation 2: "<<size_r2<<endl; //size of relation 2 in each hashing round
		cout<<"no. of available pages: "<<available_pages<<endl;
		cout<<"no. of buckets in hash table: "<<buckets<<endl;
                
	          
                // add records in buckets for relation 1
		cout<<"relation 1:\n";
		for(int i=0;i<r1.size();i++){

			int temp1;
			temp1 = hash_func_final(r1[i],k_l)%(buckets1); // hash function used

			if(temp!=0){
				for(int j=1;j<=temp;j++){
				if(temp1==j) temp1=temp1+temp;
				}
			} 
              
                        //add elements of r1 in a particular bucket given by temp1
                        cout<<"adding "<<r1[i]<<" to the bucket "<<temp1<<"\n"; 
			vec_r1[temp1].push_back(r1[i]);

                        //after adding an element in a page in a particular bucket reduce the no of records a page can store by 1
 			page_full_check_r1[temp1]=page_full_check_r1[temp1]-1;

                        //if the page is full, flush to secondary storage and now add in new page 
			if(page_full_check_r1[temp1] == 0) {
				cout<<"page for the bucket "<<temp1<<" full, flushed to secondary storage\n";
				page_full_check_r1[temp1]= records_per_page_r1;
			}
		}
		  
		// add records in buckets for relation 2
		cout<<"relation 2:\n"; 
		for(int i=0;i<r2.size();i++){

			int temp2;
			temp2 = hash_func_final(r2[i],k_l)%(buckets1);// hash function used
			if(temp!=0){
				for(int j=1;j<=temp;j++){
				if(temp2==j) temp2=temp2+temp;
				}
			}
	
                        //add elements of r2 in a particular bucket given by temp2
	 		cout<<"adding "<<r2[i]<<" to the bucket "<<temp2<<"\n";
			vec_r2[temp2].push_back(r2[i]);

    			//after adding an element in a page in a particular bucket reduce the no of records a page can store by 1
			page_full_check_r2[temp2]=page_full_check_r2[temp2]-1;

			//if the page is full, flush to secondary storage and now add in new page 
			if(page_full_check_r2[temp2] == 0) {
				cout<<"page for the bucket "<<temp2<<" full, flushed to secondary storage\n";
				page_full_check_r2[temp2] = records_per_page_r2;
			}
		}


		//print the records in each bucket after each round and also pages
		for(int i=0;i<buckets;i++){
			int pages;

			cout<<"\nbucket "<<i<<":"<<endl;

			cout<<"r1 keys:"<<endl;
			for(int j=0;j<vec_r1[i].size();j++){
					       
				cout<<vec_r1[i][j]<<endl;
			}
			pages = ceil(float(record_size1*vec_r1[i].size())/page_size);
			cout<<"pages: "<<pages<<endl;

			cout<<"r2 keys: "<<endl;
			for(int k=0;k<vec_r2[i].size();k++){
				cout<<vec_r2[i][k]<<endl;
			}
			pages = ceil(float(record_size2*vec_r2[i].size())/page_size);
			cout<<"pages: "<<pages<<endl;
		}





  		//check for violence of available pages in each bucket, if yes increase hashing round by 1 and redistribute records of that bucket between the bucket violating and 3 other newly buckets created  
                for(int i=0;i<buckets;i++){

			int temp1 = ceil(float(record_size1*vec_r1[i].size())/page_size); //size of relation 1 in terms of pages
			int temp2 = ceil(float(record_size2*vec_r2[i].size())/page_size); //size of relation 2 in terms of pages
                       
			// if bucket is violating the available pages in a bucket
			if((temp1 + temp2) > (available_pages-1)){
				
				hash_round++; 
                                
                                //max hashing round is reached
				if(hash_round > max_hash_round){
					cout<<"\nmax hassing round reached\n"; exit(1);
								 
				}

                                temp=temp+3;            
                                buckets=buckets+temp; // increase no of buckets by 3
                                flag=1;               // set flag=1 saying available pages in bucket is violated loop again

                                r1 = vec_r1[i];  //change relation 1 to relation1 of bucket violating and loop again on it
                                r2 = vec_r2[i];	//change relation 2 to relation1 of bucket violating and loop again on it

				//clear the row corresponding to the violating bucket
                                vec_r1[i].clear();  
				vec_r2[i].clear();
                   	
				break;
			}

		}  

               if(flag==0) break;  //break the loop if no violation 

    	}

        //perform in memory join ie, in each bucket matching records of r1 and r2
	cout<<"\nperforming in memory join:\n";
	for(int i=0;i<buckets;i++){

		for(int j=0;j<vec_r1[i].size();j++){
                       
			for(int k=0;k<vec_r2[i].size();k++){
				if(vec_r1[i][j] == vec_r2[i][k]) cout<<"match found in bucket "<<i<<": "<<vec_r1[i][j]<<endl;
			}
		}
	}				


    return 0;
}
