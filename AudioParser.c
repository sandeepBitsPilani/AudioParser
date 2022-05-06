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

typedef struct {
	/* header */
   char type[4]; // Always "RIFF"
   int chunksize1; //The size of the rest of the file, so file size minus 8 bytes
   char format[4]; //"WAVE" - always for WAV

    /* fmt - sound data's format */
    char subchuck1ID[4]; //"fmt "
    int subchunk1Size; //Size of the rest of this subchunk - 16 for PCM
    unsigned short int audioFormat; //form of compression, PCM/uncompressed = 1
    unsigned short int numChannels; //Mono = 1, stereo = 2
    int sampleRate; //Number of audio samples per second. 44100 most likely
    int byteRate; // Bytes per second: SampleRate * NumChannels * BitsPerSample/8
    unsigned short int blockAlign; // NumChannels * BitsPerSample/8
    unsigned short int bitsPerSample; //The number should be aligned (multiple of 8)

    /* sound data chunk*/
    char subchuck2ID[4]; // "data"
    int subchunk2Size; // Size of the actual sound data
    unsigned char data[MAX_FILE_SIZE_IN_BYTES]; // Actual sound data

} WAVData;

/* AIFF by default only supports uncompressed PCM data...*/
typedef struct {
	/* FORM */
	char chunkID[4]; //"FORM" - always for audio IFF
	long chunksize; //Total size of the next two portions of the chunk
	char formType[4]; //"AIFF" - always for audio IFF

} AIFFData;

WAVData* makeWAVStructWithFile (FILE *file);
void makeAIFFStructWithFile (FILE *file);
void loadSpecificFile (void);
void loadSample (void);
void exportWAVSoundDataWithFileName(WAVData *wav, char fileName[]);

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

void loadSample(void) {
	char fileName[] = "audio.wav";
	FILE *file = fopen(fileName, "r");
	if (file == NULL)
	{
		printf("%s not found\n", fileName);
		exit(1);
	}
	printf("%s\n----------\n", fileName);

	WAVData *wav = makeWAVStructWithFile(file);
	exportWAVSoundDataWithFileName(wav, fileName);
}
