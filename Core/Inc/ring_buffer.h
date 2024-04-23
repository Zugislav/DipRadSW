#ifndef __RING_BUFFER_H_
#define __RING_BUFFER_H_

#include <stdint.h>

#define STATIC static
#define INLINE
#define BUFFER_SIZE 1024

/** @defgroup Ring_Buffer CHIP: Simple ring buffer implementation
 * @ingroup CHIP_Common
 * @{
 */

/**
 * @brief Ring buffer structure
 */
typedef struct {
	void *data;
	int count;
	int itemSz;
	uint32_t head;
	uint32_t tail;
} RINGBUFF_T;

/**
 * @def		RB_VHEAD(rb)
 * volatile typecasted head index
 */
#define RB_VHEAD(rb)              (*(volatile uint32_t *) &(rb)->head)

/**
 * @def		RB_VTAIL(rb)
 * volatile typecasted tail index
 */
#define RB_VTAIL(rb)              (*(volatile uint32_t *) &(rb)->tail)

/**
 * @brief	Initialize ring buffer
 * @param	RingBuff	: Pointer to ring buffer to initialize
 * @param	buffer		: Pointer to buffer to associate with RingBuff
 * @param	itemSize	: Size of each buffer item size
 * @param	count		: Size of ring buffer
 * @note	Memory pointed by @a buffer must have correct alignment of
 * 			@a itemSize, and @a count must be a power of 2 and must at
 * 			least be 2 or greater.
 * @return	Nothing
 */
int RingBuffer_Init(RINGBUFF_T *RingBuff, void *buffer, int itemSize, int count);

void RingBuffer_Flush(RINGBUFF_T *RingBuff);
int RingBuffer_GetSize(RINGBUFF_T *RingBuff);
int RingBuffer_GetCount(RINGBUFF_T *RingBuff);
int RingBuffer_GetFree(RINGBUFF_T *RingBuff);
int RingBuffer_IsFull(RINGBUFF_T *RingBuff);
int RingBuffer_IsEmpty(RINGBUFF_T *RingBuff);

int RingBuffer_InsertOne(RINGBUFF_T *RingBuff, const uint8_t *data);

/**
 * @brief	Insert a single item into ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	data		: pointer to item
 * @return	1 when successfully inserted,
 *			0 on error (Buffer not initialized using
 *			RingBuffer_Init() or attempted to insert
 *			when buffer is full)
 */
int RingBuffer_Insert(RINGBUFF_T *RingBuff, const uint8_t *data);

/**
 * @brief	Insert an array of items into ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	data		: Pointer to first element of the item array
 * @param	num			: Number of items in the array
 * @return	number of items successfully inserted,
 *			0 on error (Buffer not initialized using
 *			RingBuffer_Init() or attempted to insert
 *			when buffer is full)
 */
int RingBuffer_InsertMult(RINGBUFF_T *RingBuff, const uint8_t *data, int num);

/**
 * @brief	Pop an item from the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	data		: Pointer to memory where popped item be stored
 * @return	1 when item popped successfuly onto @a data,
 * 			0 When error (Buffer not initialized using
 * 			RingBuffer_Init() or attempted to pop item when
 * 			the buffer is empty)
 */
int RingBuffer_Pop(RINGBUFF_T *RingBuff, uint8_t *data);

/**
 * @brief	Pop an array of items from the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	data		: Pointer to memory where popped items be stored
 * @param	num			: Max number of items array @a data can hold
 * @return	Number of items popped onto @a data,
 * 			0 on error (Buffer not initialized using RingBuffer_Init()
 * 			or attempted to pop when the buffer is empty)
 */
int RingBuffer_PopMult(RINGBUFF_T *RingBuff, void *data, int num);


/**
 * @}
 */

#endif /* __RING_BUFFER_H_ */
