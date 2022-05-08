#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Reference for the wav file format: https://ccrma.stanford.edu/courses/422/projects/WaveFormat/ */
/* Reference for aiff file format: http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/AIFF/Docs/AIFF-1.3.pdf */

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

void loadSpecificFile(void) {
	printf("What's the WAV file name? (80 charcters max, include)\n");
	char fileName[80];
	scanf("%s", fileName);

	FILE *file = fopen(fileName, "r");
	if (file == NULL) {
		printf("%s could not be found\n", fileName);
		exit(1);
	}

	WAVData *wav = makeWAVStructWithFile(file);
	exportWAVSoundDataWithFileName(wav, fileName);
}

WAVData* makeWAVStructWithFile(FILE *file) {
	WAVData *wav = malloc(sizeof(WAVData));
	fread(wav, sizeof(WAVData), 1, file);
	//printf("%s\n",wav.type);
	printf("Size: %d bytes\n", wav->chunksize1 + 8);
	printf("Type: %s\n", wav->format);
	// //printf("%s\n",wav.subchuck1ID);
	printf("Data Info Size: %d bytes\n", wav->subchunk1Size);
	printf("Compression code: %d", wav->audioFormat);
	if (wav->audioFormat == 1) {
		printf(" (PCM / uncompressed)\n");
	} else {
		printf("\n");
	}
	printf("Number of Channels: %d\n", wav->numChannels);
	printf("Sample Rate: %d Hz\n", wav->sampleRate);
	printf("Bytes Per Second: %d bytes\n", wav->byteRate);
	//printf("%d\n",wav.blockAlign);
	//printf("%d\n\n",wav.bitsPerSample);

	//printf("data\n");
	//printf("%s\n",wav.subchuck2ID);
	printf("Data Size: %d bytes\n", wav->subchunk2Size);

	return wav;
}

void makeAIFFStructWithFile(FILE *file) {
	AIFFData aiff;
	fread(&aiff, sizeof(AIFFData),1, file);
	printf("%s\n", aiff.chunkID);
	printf("%ld", aiff.chunksize);
}

/* Exports the audio sample data to a txt file */
void exportWAVSoundDataWithFileName(WAVData *wav, char fileName[]) {
	char fileNameWithoutExtension[80];
	memcpy(fileNameWithoutExtension, &fileName[0], (strlen(fileName)-4));
	char textFileNameBuffer[80];
	sprintf(textFileNameBuffer, "%s.txt",fileName);
	FILE *f = fopen(textFileNameBuffer, "w");
	if (f == NULL) {
    	printf("Error creating file!\n");
    	exit(1);
	}
	int i;
	/* Each data value will be separated by a \n */
	for (i = 0; i < wav->subchunk2Size; i++){
		fprintf(f, "%d\n", wav->data[i]);
	}
	fclose(f);
	printf("----------\n");
	printf("The digital audio sample data has been exported to %s\n", textFileNameBuffer);
} 
