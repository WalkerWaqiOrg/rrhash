#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <dlfcn.h>
#include <thread>

typedef void (*run_all_func)(const char *data, size_t length, unsigned char *hash);
run_all_func run_all;

void test_a() {
	char hello[100]="aer39invqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	unsigned char result[32];
	for(int i=0; i<256; i++) {
		for(int j=0; j<len; j++) hello[j]+=i;
		run_all(hello,len,result);
		fprintf(stderr,"a ");
		for(int i=0; i<32; i++) {
			fprintf(stderr,"%02x",result[i]);
		}
		fprintf(stderr,"\n");
	}
}

void test_b() {
	char hello[100]="aer39invqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	unsigned char result[32];
	for(int i=255; i>=0; i--) {
		for(int j=0; j<len; j++) hello[j]+=i;
		run_all(hello,len,result);
		fprintf(stderr,"b ");
		for(int i=0; i<32; i++) {
			fprintf(stderr,"%02x",result[i]);
		}
		fprintf(stderr,"\n");
	}
}

int main() {
	void *handle;
	char *error;

#ifdef WIN32
	handle = dlopen("./librrhash.dll", RTLD_LAZY);
#else
	handle = dlopen("./librrhash.so", RTLD_LAZY);
#endif
	if( !handle ) {
		printf("failed to open:\n");
		fputs( dlerror(), stderr);
		exit(1);
	}

	run_all = (run_all_func)dlsym(handle, "run_all");
	if(( error=dlerror())!=NULL) {
		printf("failed to get function:\n");
		fputs(error, stderr);
		exit(1);
	}

	std::thread thread_a(test_a);
	std::thread thread_b(test_b);
	thread_a.join();
	thread_b.join();

	dlclose(handle);
}

