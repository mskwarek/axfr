#include <iostream>

extern "C"{
#include<dns.h>
}

int main(){
	for(int i = 0; i<100; ++i){
		ngethostbyname("onet.pl", "8.8.8.8", 252);
	}
	return 0;
}
