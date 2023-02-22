#ifndef PLUGIN_EVENTS_TYPES_H
#define PLUGIN_EVENTS_TYPES_H


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum GNFPLUGIN_EVENT_FLAG
	{
		GNFPLUGIN_ONCHANGESCOPE_EVENT = 1 << 0
	}	GNFPLUGIN_EVENT_FLAG;


#ifdef __cplusplus
}
#endif
#endif //PLUGIN_NATIVE_EVENT_TYPES_H