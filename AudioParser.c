#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 
 * Size Reference:
 *
 * int = 32bit = 4 bytes 
 * char = 8bit = 1 byte 
 */

 #define MAX_FILE_SIZE_IN_BYTES 500000 

/* 
 * This struct will hold all WAV data. Only supports PCM uncompressed WAV. 
 * WAV follows a RIFF data structure where there are 3 chunks.
 */

int main() {
	printf("Load the sample WAV file (audio.wav) or load your own?\n('1' for sample, '2' for other)\n");
	int fileSourceDecision;
	scanf("%d", &fileSourceDecision);
	if (!fileSourceDecision || !((fileSourceDecision == 1) || (fileSourceDecision == 2))) {
		printf("Invalid\n");
	} else if (fileSourceDecision == 1) {
		loadSample();
	}else if (fileSourceDecision == 2) {
		loadSpecificFile();
	}
	printf("This is test comment 1");
}
