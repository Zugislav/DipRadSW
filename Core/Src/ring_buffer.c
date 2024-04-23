#include <string.h>
#include "ring_buffer.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define RB_INDH(rb)                ((rb)->head & ((rb)->count - 1))
#define RB_INDT(rb)                ((rb)->tail & ((rb)->count - 1))

#if !defined(MAX)
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#if !defined(MIN)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif


/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initialize ring buffer */
int RingBuffer_Init(RINGBUFF_T *RingBuff, void *buffer, int itemSize, int count)
{
	RingBuff->data = buffer;
	RingBuff->count = count;
	RingBuff->itemSz = itemSize;
	RingBuff->head = RingBuff->tail = 0;

	return 1;
}

/**
 * @brief   Resets the ring buffer to empty
 * @param   RingBuff    : Pointer to ring buffer
 * @return  Nothing
 */
void RingBuffer_Flush(RINGBUFF_T *RingBuff)
{
    RingBuff->head = RingBuff->tail = 0;
}

/**
 * @brief   Return size the ring buffer
 * @param   RingBuff    : Pointer to ring buffer
 * @return  Size of the ring buffer in bytes
 */
int RingBuffer_GetSize(RINGBUFF_T *RingBuff)
{
    return RingBuff->count;
}

/**
 * @brief   Return number of items in the ring buffer
 * @param   RingBuff    : Pointer to ring buffer
 * @return  Number of items in the ring buffer
 */
int RingBuffer_GetCount(RINGBUFF_T *RingBuff)
{
    return RB_VHEAD(RingBuff) - RB_VTAIL(RingBuff);
}

/**
 * @brief   Return number of free items in the ring buffer
 * @param   RingBuff    : Pointer to ring buffer
 * @return  Number of free items in the ring buffer
 */
int RingBuffer_GetFree(RINGBUFF_T *RingBuff)
{
    return RingBuff->count - RingBuffer_GetCount(RingBuff);
}

/**
 * @brief   Return number of items in the ring buffer
 * @param   RingBuff    : Pointer to ring buffer
 * @return  1 if the ring buffer is full, otherwise 0
 */
int RingBuffer_IsFull(RINGBUFF_T *RingBuff)
{
    return (RingBuffer_GetCount(RingBuff) >= RingBuff->count);
}

/**
 * @brief   Return empty status of ring buffer
 * @param   RingBuff    : Pointer to ring buffer
 * @return  1 if the ring buffer is empty, otherwise 0
 */
int RingBuffer_IsEmpty(RINGBUFF_T *RingBuff)
{
    return RB_VHEAD(RingBuff) == RB_VTAIL(RingBuff);
}

/* Insert a single item into Ring Buffer */
int RingBuffer_Insert(RINGBUFF_T *RingBuff, const uint8_t *data)
{
	uint8_t *ptr = RingBuff->data;

	/* We cannot insert when queue is full */
	if (RingBuffer_IsFull(RingBuff))
		return 0;

	ptr += RB_INDH(RingBuff) * RingBuff->itemSz;
	memcpy(ptr, data, RingBuff->itemSz);
	RingBuff->head++;

	return 1;
}

/* Insert a single item into Ring Buffer */
int RingBuffer_InsertOne(RINGBUFF_T *RingBuff, const uint8_t *data)
{
    uint8_t *ptr = RingBuff->data;
    ptr += RB_INDH(RingBuff);
    *ptr = *data;
    RingBuff->head++;
    return 1;
}

/* Insert multiple items into Ring Buffer */
int RingBuffer_InsertMult(RINGBUFF_T *RingBuff, const uint8_t *data, int num)
{
	uint8_t *ptr = RingBuff->data;
	int cnt1, cnt2;

	/* We cannot insert when queue is full */
	//if (RingBuffer_IsFull(RingBuff))
	//	return 0;

	/* Calculate the segment lengths */
	cnt1 = cnt2 = RingBuffer_GetFree(RingBuff);
	if ((int)RB_INDH(RingBuff) + cnt1 >= RingBuff->count)
		cnt1 = RingBuff->count - RB_INDH(RingBuff);
	cnt2 -= cnt1;

	cnt1 = MIN(cnt1, num);
	num -= cnt1;

	cnt2 = MIN(cnt2, num);
	num -= cnt2;

	/* Write segment 1 */
	ptr += RB_INDH(RingBuff) * RingBuff->itemSz;
	*ptr = *data;
	//memcpy(ptr, data, cnt1 * RingBuff->itemSz);
	RingBuff->head += cnt1;

	/* Write segment 2 */
	ptr = (uint8_t *) RingBuff->data + RB_INDH(RingBuff) * RingBuff->itemSz;
	data = (const uint8_t *) data + cnt1 * RingBuff->itemSz;
	*ptr = *data;
	//memcpy(ptr, data, cnt2 * RingBuff->itemSz);
	RingBuff->head += cnt2;

	return cnt1 + cnt2;
}

/* Pop single item from Ring Buffer */
int RingBuffer_Pop(RINGBUFF_T *RingBuff, uint8_t *data)
{
	uint8_t *ptr = RingBuff->data;

	/* We cannot pop when queue is empty */
	if (RingBuffer_IsEmpty(RingBuff))
		return 0;

	ptr += RB_INDT(RingBuff) * RingBuff->itemSz;
	*data = *ptr;
	//memcpy(data, ptr, RingBuff->itemSz);
	RingBuff->tail++;

	return 1;
}

/* Pop multiple items from Ring buffer */
int RingBuffer_PopMult(RINGBUFF_T *RingBuff, void *data, int num)
{
	uint8_t *ptr = RingBuff->data;
	int cnt1, cnt2;

	/* We cannot insert when queue is empty */
	if (RingBuffer_IsEmpty(RingBuff))
		return 0;

	/* Calculate the segment lengths */
	cnt1 = cnt2 = RingBuffer_GetCount(RingBuff);
	if ((int)RB_INDT(RingBuff) + cnt1 >= RingBuff->count)
		cnt1 = RingBuff->count - RB_INDT(RingBuff);
	cnt2 -= cnt1;

	cnt1 = MIN(cnt1, num);
	num -= cnt1;

	cnt2 = MIN(cnt2, num);
	num -= cnt2;

	/* Write segment 1 */
	ptr += RB_INDT(RingBuff) * RingBuff->itemSz;
	memcpy(data, ptr, cnt1 * RingBuff->itemSz);
	RingBuff->tail += cnt1;

	/* Write segment 2 */
	ptr = (uint8_t *) RingBuff->data + RB_INDT(RingBuff) * RingBuff->itemSz;
	data = (uint8_t *) data + cnt1 * RingBuff->itemSz;
	memcpy(data, ptr, cnt2 * RingBuff->itemSz);
	RingBuff->tail += cnt2;

	return cnt1 + cnt2;
}
