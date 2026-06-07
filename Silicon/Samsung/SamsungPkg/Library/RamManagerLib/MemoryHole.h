#ifndef _MEMORY_HOLE_H_
#define _MEMORY_HOLE_H_

//
// SMC Functions
//
#define SMC_DRM_GET_SOC_INFO (0x82002060)
#define SMC_CMD_GET_SOC_INFO (-302)

//
// HVC Functions
//
#define HVC_GET_DRM_PLUGIN_INFO (0x82002060)
#define HVC_GET_HARX_INFO       (0xC6000020)

//
// DRM SoC Info Arguments
//
#define SOC_INFO_SEC_PGTBL_BASE   1
#define SOC_INFO_SEC_PGTBL_LENGTH 2

//
// CMD SoC Info Arguments
//
#define SOC_INFO_TYPE_SEC_DRAM_LENGTH 1
#define SOC_INFO_TYPE_SEC_DRAM_BASE   2

//
// H-Arx Info Arguments
//
#define HARX_INFO_HARX_BASE   1
#define HARX_INFO_HARX_LENGTH 2

//
// DRM Plug-In 
//
#define DRM_PLUGIN_INFO_DRM_PLUGIN_BASE   1
#define DRM_PLUGIN_INFO_DRM_PLUGIN_LENGTH 2

#endif /* _MEMORY_HOLE_H_ */
