#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <utility>
#include <curl/curl.h>

#include "hisse.h"

using namespace std;

struct MemoryStruct {
  char *memory;
  size_t size;
}chunk;
 
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp){
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}


int take_data(void)
{
  CURL *curl;
  CURLcode res;
  
  int i;
  
  chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */ 
  chunk.size = 0;    /* no data at this point */   


  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL,"https://borsa.doviz.com/hisseler"); //"http://bigpara.hurriyet.com.tr/borsa/canli-borsa/");
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);  

    res = curl_easy_perform(curl);
        
    if(res != CURLE_OK){
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }
	
    curl_easy_cleanup(curl);
  }
	return 0;
}

int take_data(char* cp)
{
  CURL *curl;
  CURLcode res;
  
  int i;
  
  chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */ 
  chunk.size = 0;    /* no data at this point */   

  cout<<cp<<endl;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL,cp); //"http://bigpara.hurriyet.com.tr/borsa/canli-borsa/");
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);  

    res = curl_easy_perform(curl);
        
    if(res != CURLE_OK){
      fprintf(stderr, "curl_easy_perform(char *) failed: %s\n",
              curl_easy_strerror(res));
    }
	
    curl_easy_cleanup(curl);
  }
	return 0;
}

double get_dval(string &str){
  string s{"class=\"borsa-row1\" style=\"text-align:center;\"><span class=\"color-"}; //elde edilen html stringte hisse degerleri icin ayrac
  string subs;
  size_t found_begin=str.find(s);
  while(found_begin!=string::npos){
    found_begin+=s.size();
    found_begin=str.find(">",found_begin+1);
    size_t found_end=str.find('<',found_begin+1);
    subs=str.substr(found_begin+1,found_end-found_begin-1);
    
    found_begin=str.find(s,found_end);
  }

  for(size_t i=0; i<subs.size();++i){
    if(subs.at(i)==',')
      subs.at(i)= '.';
    else if (subs.at(i)=='.')
      subs.erase(subs.find("."),1); 
  }

  return stod(subs);
}

int main(int argc, char * argv[])
{
	vector<string> svec;
  vector<hisse> hvec;
  if (argc==1)
    take_data();
  else{
    cout<<argv[1]<<endl;
    take_data(argv[1]);
  }
	string str{chunk.memory};
  free(chunk.memory);
  string s{"<tr class=\"hisse-tablo\">"};
  
  size_t found_begin=str.find(s);
  while(found_begin!=string::npos){
    size_t found_end=str.find("</tr>",found_begin+1);
    svec.push_back(str.substr(found_begin,found_end-found_begin));
    found_begin=str.find(s,found_end);
  }

  s="'Hisse Senedi', '";
  for(auto iter : svec){
    size_t found_begin=iter.find(s);
    while(found_begin!=string::npos){
      found_begin+=s.size();
      size_t found_end=iter.find('\'',found_begin+1);
      hvec.push_back(hisse{iter.substr(found_begin,found_end-found_begin),get_dval(iter)});
      found_begin=iter.find(s,found_end);
    }
  }
      
  fstream fs;
  fs.open("deneme.txt", std::fstream::in | std::fstream::out | std::fstream::trunc );
  
  if(fs.is_open()){
    cout<<"true\n";
    for(auto iter: hvec)
      fs<<iter<<endl;
    fs.close();
  }else{
    cout<<"false\n";
  }
}