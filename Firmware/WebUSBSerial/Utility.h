#ifndef UTILITY_H_
#define UTILITY_H_
#include <stdint.h>

#define MAX(a, b) (((a) > (b)) ? (a):(b))
#define MIN(a, b) (((a) < (b)) ? (a):(b))

#define TU_U16_HIGH(u16)      ((uint8_t) (((u16) >> 8) & 0x00ff))
#define TU_U16_LOW(u16)       ((uint8_t) ((u16)       & 0x00ff))
#define U16_TO_U8S_BE(u16)    TU_U16_HIGH(u16), TU_U16_LOW(u16)
#define U16_TO_U8S_LE(u16)    TU_U16_LOW(u16), TU_U16_HIGH(u16)

#define U32_B1_U8(u32)        ((uint8_t) ((((uint32_t) u32) >> 24) & 0x000000ff))
#define U32_B2_U8(u32)        ((uint8_t) ((((uint32_t) u32) >> 16) & 0x000000ff))
#define U32_B3_U8(u32)        ((uint8_t) ((((uint32_t) u32) >>  8) & 0x000000ff))
#define U32_B4_U8(u32)        ((uint8_t) (((uint32_t)  u32)        & 0x000000ff))

#define U32_TO_U8S_BE(u32)    U32_B1_U8(u32), U32_B2_U8(u32), U32_B3_U8(u32), U32_B4_U8(u32)
#define U32_TO_U8S_LE(u32)    U32_B4_U8(u32), U32_B3_U8(u32), U32_B2_U8(u32), U32_B1_U8(u32)



#endif