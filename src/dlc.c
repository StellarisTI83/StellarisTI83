#include <fileioc.h>
#include <debug.h>
#include <string.h>

#include "main.h"

#include "dlc.h"
#include "ai.h"
#include "flottes.h"
#include "systemes.h"

#define MAX_VALUE_LENGTH 255

/* types =============================================================== */

enum _keys {
	ky_none, ky_struct, ky_name, ky_ship_size
};

typedef enum _keys keys;

enum _value_type {
	type_error, type_struct, type_struct_end, type_string, type_int
};

typedef enum _value_type value_type;

/* private functions =================================================== */

static void dlc_Read_Chunk(void *data, size_t size, const char *slot, int* cursorIndex) {
	memcpy(data, &slot[*cursorIndex], size);
	*cursorIndex += size/sizeof(data[*cursorIndex]);
}

static int dlc_Parse(const char* dlc_var) {
	uint16_t numberOfChunk;
	int index = 0, cursorIndex = 0;
	uint16_t sizeOfChunk;
	uint16_t keyRaw;
	uint16_t intValue;
	keys key;
	char stringBuffer[MAX_VALUE_LENGTH];
	
	dlc_Read_Chunk(&numberOfChunk, sizeof(uint16_t), dlc_var, &cursorIndex);
	
	#ifdef DEBUG_VERSION
		dbg_sprintf(dbgout, "number of chunk: %d\n", numberOfChunk);
	#endif
	while(index < numberOfChunk){
		memset(stringBuffer, 0, MAX_VALUE_LENGTH * sizeof(char));
		dlc_Read_Chunk(&sizeOfChunk, sizeof(uint16_t), dlc_var, &cursorIndex);
		dlc_Read_Chunk(&keyRaw, sizeof(uint16_t), dlc_var, &cursorIndex);
		key = (keys)keyRaw;
		switch(key) {
			case ky_name:
				dlc_Read_Chunk(&stringBuffer, sizeOfChunk, dlc_var, &cursorIndex);
				#ifdef DEBUG_VERSION
				dbg_sprintf(dbgout, "size of chunk: %d, key: %d value(string): '%s'\n", sizeOfChunk, key, stringBuffer);
				#endif
				break;
			case ky_ship_size:
				dlc_Read_Chunk(&intValue, sizeOfChunk, dlc_var, &cursorIndex);
				#ifdef DEBUG_VERSION
				dbg_sprintf(dbgout, "size of chunk: %d, key: %d value(short): %d\n", sizeOfChunk, key, intValue);
				#endif
				break;
			case ky_struct:
				#ifdef DEBUG_VERSION
				dbg_sprintf(dbgout, "size of chunk: %d, key: %d structure\n", sizeOfChunk, key);
				#endif
				cursorIndex = dlc_Parse(&dlc_var[cursorIndex]);
				break;
			default:
				#ifdef DEBUG_VERSION
				dbg_sprintf(dbgout, "Unknow key '%d' at index %d\n", keyRaw, index);
				#endif
				break;
		}
		index++;
	}
	return cursorIndex;
}


/* entry points ======================================================== */
/**
 * Load a dlc/data set with his name
 */
void dlc_Load(const char *name) {
	ti_var_t dlc_var;
	char *buffer;
	uint16_t sizeOfChunk;

	ti_CloseAll();

	dlc_var = ti_Open(name, "r");

	#ifdef DEBUG_VERSION
		dbg_sprintf(dbgout, "Open %s\n", name);
	#endif

	ti_Read(&sizeOfChunk, sizeof(uint16_t), 1, dlc_var);
	buffer = malloc(sizeOfChunk + 2);	
    if(!buffer){
		#ifdef DEBUG_VERSION
		dbg_sprintf(dbgerr, "Malloc returned NULL when creating dlc buffer");
		#endif
        exit(EXIT_FAILURE);
    }
	ti_Read(buffer, sizeOfChunk + 2, 1, dlc_var);
	dlc_Parse(buffer);

	free(buffer);
	ti_Close(dlc_var);
}
