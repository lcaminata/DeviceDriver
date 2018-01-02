#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

static char output[256];

int main() {
	int toOpen;
	int toWriteRead;
	char input[256];
	printf("Starting program on user space\n\n");
	toOpen = open("/dev/driver", O_RDWR);
	printf("Hello, this program is set up to: return the age of the user which is stored on the kernel.\n\n");
	printf("There are four possible users: Luciano, Daniel, Ivo, Maria.\nWhich user will be selected?\n\n");
	scanf("%[^\n]%*c", input);
	printf("\n");
	int i = 0;
	while(i < strlen(input)) {
		input[i] = tolower(input[i]);
		i++;
	}
	toWriteRead = write(toOpen, input, strlen(input));
	printf("User input set, getting results from kernel, press enter to receive result!\n\n");
	getchar();
	toWriteRead = read(toOpen, output, 256);
	printf("%s\n", output);
	return 0;		
}
