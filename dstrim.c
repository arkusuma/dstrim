/*
 * Nintendo DS ROM trimmer
 *
 * Author: Anugrah Redja Kusuma <anugrah.redja@gmail.com>
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define BUF_SIZE 4096
#define DS_HEADER_SIZE 512

int dstrim(const char *fname)
{
	FILE *fp;
	long file_size, rom_size, total;
	unsigned char len[4];

	fprintf(stderr, "%s: ", fname);

	if ((fp = fopen(fname, "rb")) == 0) {
		perror("");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);

	fseek(fp, 0x80, SEEK_SET);
	fread(len, 1, 4, fp);

	fclose(fp);

	rom_size = len[0] | len[1] << 8 | len[2] << 16 | len[3] << 24;
	if (rom_size <= DS_HEADER_SIZE || 
			rom_size <= file_size / 2 || 
			rom_size > file_size) {
		fprintf(stderr, "invalid ROM file\n");
		return -1;
	}

	if (rom_size == file_size) {
		fprintf(stderr, "already trimmed\n");
		return 0;
	}

	if (truncate(fname, rom_size) != 0) {
		perror("");
		return -1;
	}

	fprintf(stderr, "done, %ld bytes removed\n", file_size - rom_size);
	return 0;
}

int main(int argc, char **argv)
{
	int i;

	if (argc <= 1) {
		printf("DS Trimmer - Remove unused trailing bytes from a DS ROM\n");
		printf("By: Anugrah Redja Kusuma (anugrah.redja@gmail.com)\n\n");
		printf("Usage: dstrim [ROM]...\n");
		return 1;
	}

	for (i = 1; i < argc; i++)
		dstrim(argv[i]);

	return 0;
}
