#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <dlfcn.h>

typedef void (*run_all_func)(const char *data, size_t length, unsigned char *hash);

int main() {
	void *handle;
	run_all_func run_all;
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

	char hello[100]="aer39invqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	unsigned char result[32];
	run_all(hello,len,result);
	for(int i=0; i<32; i++) {
		printf("%02x ",result[i]);
	}
	printf("\n");
	dlclose(handle);
}