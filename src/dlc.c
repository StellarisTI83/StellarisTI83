#include <fileioc.h>
#include <debug.h>
#include <string.h>

#include "main.h"

#include "ai.h"
#include "fleet.h"
#include "dlc.h"
#include "systemes.h"

#define MAX_VALUE_LENGTH 255

/* types =============================================================== */

enum _keys {
    ky_none, ky_struct, ky_name, ky_power, ky_ship_type, ky_hull_life, ky_armor_life, ky_shield_life
};

typedef enum _keys keys;

enum _value_type {
	type_error, type_struct, type_struct_end, type_string, type_int
};

typedef enum _value_type value_type;


/* private functions =================================================== */

static void dlc_ReadChunk(void *data, size_t size, const char *slot, int* cursorIndex) {
	memcpy(data, &slot[*cursorIndex], size);
	*cursorIndex += size/sizeof(data[*cursorIndex]);
}

static int dlc_Parse(const char* dlc_var, void *liste, keys mother_key) {
	uint16_t numberOfChunk;
	int index = 0, cursorIndex = 0;
	uint16_t sizeOfChunk;
	uint16_t keyRaw;
	uint16_t intValue;
	keys key;
	char stringBuffer[MAX_VALUE_LENGTH];
	FleetTemplate *fleetTemplate = NULL;
	
	dlc_ReadChunk(&numberOfChunk, sizeof(uint16_t), dlc_var, &cursorIndex);
	

	#ifdef DEBUG_VERSION
		dbg_sprintf(dbgout, "number of chunk: %d\n", numberOfChunk);
	#endif
	while(index < numberOfChunk){
		memset(stringBuffer, 0, MAX_VALUE_LENGTH * sizeof(char));
		dlc_ReadChunk(&sizeOfChunk, sizeof(uint16_t), dlc_var, &cursorIndex);
		dlc_ReadChunk(&keyRaw, sizeof(uint16_t), dlc_var, &cursorIndex);
		key = (keys)keyRaw;
		switch(key) {
			case ky_name:
				dlc_ReadChunk(&stringBuffer, sizeOfChunk, dlc_var, &cursorIndex);
				#ifdef DEBUG_VERSION
				dbg_sprintf(dbgout, "size of chunk: %d, key: ky_name value(string): '%s'\n", sizeOfChunk, stringBuffer);
				#endif
				break;
			case ky_ship_type:
				dlc_ReadChunk(&intValue, sizeOfChunk, dlc_var, &cursorIndex);
				#ifdef DEBUG_VERSION
				dbg_sprintf(dbgout, "size of chunk: %d, key: ky_ship_type value(short): %d\n", sizeOfChunk, intValue);
				#endif
				fleet_TemplateSetType(liste, intValue);
				break;
			case ky_power:
				dlc_ReadChunk(&intValue, sizeOfChunk, dlc_var, &cursorIndex);
				#ifdef DEBUG_VERSION
				dbg_sprintf(dbgout, "size of chunk: %d, key: ky_power value(short): %d\n", sizeOfChunk, intValue);
				#endif
				fleet_TemplateSetPower(liste, intValue);
				break;
			case ky_hull_life:
				dlc_ReadChunk(&intValue, sizeOfChunk, dlc_var, &cursorIndex);
				#ifdef DEBUG_VERSION
				dbg_sprintf(dbgout, "size of chunk: %d, key: ky_hull_life value(short): %d\n", sizeOfChunk, intValue);
				#endif
				fleet_TemplateSetHull(liste, intValue);
				break;
			case ky_armor_life:
				dlc_ReadChunk(&intValue, sizeOfChunk, dlc_var, &cursorIndex);
				#ifdef DEBUG_VERSION
				dbg_sprintf(dbgout, "size of chunk: %d, key: ky_armor_life value(short): %d\n", sizeOfChunk, intValue);
				#endif
				fleet_TemplateSetArmor(liste, intValue);
				break;
			case ky_shield_life:
				dlc_ReadChunk(&intValue, sizeOfChunk, dlc_var, &cursorIndex);
				#ifdef DEBUG_VERSION
				dbg_sprintf(dbgout, "size of chunk: %d, key: ky_shield_life value(short): %d\n", sizeOfChunk, intValue);
				#endif
				fleet_TemplateSetShield(liste, intValue);
				break;
			case ky_struct:
				#ifdef DEBUG_VERSION
				dbg_sprintf(dbgout, "size of chunk: %d, key: ky_struct structure\n", sizeOfChunk);
				#endif
				if(mother_key == ky_none){
					fleetTemplate = fleet_TemplateAdd(liste);
					cursorIndex = dlc_Parse(&dlc_var[cursorIndex], fleetTemplate, key);
				}
				break;
			default:
				#ifdef DEBUG_VERSION
				dbg_sprintf(dbgout, "Unknow key '%d' at index %d\n", keyRaw, index);
				#endif
				break;
		}
		index++;
	}
	#ifdef DEBUG_VERSION
	if(mother_key == ky_struct) {
		dbg_printf("Type: %d, power: %d, hull: %d, armor: %d, shield: %d\n", fleet_TemplateGetType(liste), fleet_TemplateGetPower(liste), fleet_TemplateGetHull(liste), fleet_TemplateGetArmor(liste), fleet_TemplateGetShield(liste));
	}
	#endif
	return cursorIndex;
}


/* entry points ======================================================== */
/**
 * Load a dlc/data set with his name
 */
FleetTemplateListe *dlc_Load(const char *name) {
	ti_var_t dlc_var;
	char *buffer;
	uint16_t sizeOfChunk;
	FleetTemplateListe *fleetTemplateList = fleet_TemplateListCreate();

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
	dlc_Parse(buffer, fleetTemplateList, ky_none);

	free(buffer);
	ti_Close(dlc_var);
	return fleetTemplateList;
}

void dlc_Unload(FleetTemplateListe *fleetTemplateList) {
	int index = 0;
	FleetTemplate *fleetTemplate = (FleetTemplate*)GenericCell_Get((GenericList*)fleetTemplateList, index);
	while (fleetTemplate) {
		free(fleetTemplate);
		index++;
		fleetTemplate = (FleetTemplate*)GenericCell_Get((GenericList*)fleetTemplateList, index);
	}
}