/*
	***********************************************************
  * @brief   ringbuff.c
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ���ļ����ڴ����λ���������
  ***********************************************************
	*/

#include "include.h"
	
#define kmalloc(size, mask) 		malloc(size)
#define kfree(ptr) 							free(ptr)
#define BUG_ON(cond) 						ASSERT(!(cond))


#define min(a,b)  ((a)>(b) ? (b) : (a)) 

/* ��ͬ������ѡ�� */
#if defined(__GNUC__) || defined(__x86_64__)
#define TPOOL_COMPILER_BARRIER() __asm__ __volatile("" : : : "memory")

static inline void FullMemoryBarrier()
{
    __asm__ __volatile__("mfence": : : "memory");
}
#define smp_mb() FullMemoryBarrier()
#define smp_rmb() TPOOL_COMPILER_BARRIER()
#define smp_wmb() TPOOL_COMPILER_BARRIER()	
#endif

/*********************** �ڲ����ú��� ******************************/
static int fls(int x)
{
  int r = 32;

  if (!x)
          return 0;
  if (!(x & 0xffff0000u)) {
          x <<= 16;
          r -= 16;
  }
  if (!(x & 0xff000000u)) {
          x <<= 8;
          r -= 8;
  }
  if (!(x & 0xf0000000u)) {
          x <<= 4;
          r -= 4;
  }
  if (!(x & 0xc0000000u)) {
          x <<= 2;
          r -= 2;
  }
  if (!(x & 0x80000000u)) {
          x <<= 1;
          r -= 1;
  }
  return r;
}

#ifdef __GNUC__
#define __attribute_const__     __attribute__((__const__))
#endif

/************************************************************
  * @brief   roundup_pow_of_two
  * @param   size�����ݽ��������ݳ���
  * @return  size�����ش���֮������ݳ���
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ���ڴ������ݣ�ʹ���ݳ��ȱ���Ϊ 2^n
	*					 ������ǣ���ת�����������ಿ�֣���
	*					 roundup_pow_of_two(66) -> ���� 64
  ***********************************************************/
static unsigned long roundup_pow_of_two(unsigned long x)
{
	return (1 << (fls(x-1)-1));				//���¶���
  //return (1UL << fls(x - 1));			//���϶��룬�ö�̬�ڴ����ʹ��
}

/*********************** �ڲ����ú��� ******************************/

/************************************************************
  * @brief   Create_Fifo
  * @param   fifo�����λ��������
  *          buffer�����λ���������������
  *          size�����λ������Ĵ�С����������СҪΪ2^n
  * @return  err_t��ERR_OK��ʾ�����ɹ���������ʾʧ��
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ���ڴ���һ�����λ�����
  ***********************************************************/
err_t Create_RingBuff(RingBuff_t* rb, 
                      uint8_t *buffer,
                      uint32_t size
#if USE_MUTEX
                      ,spinlock_t *lock
#endif
								)
{
	if((rb == NULL)||(buffer == NULL)||(size == 0))
	{
		PRINT_ERR("data is null!");
		return ERR_NULL;
	}
	
	PRINT_DEBUG("ringbuff size is %d!",size);
	/* ��������С����Ϊ2^n�ֽ�,ϵͳ��ǿ��ת��,
		 ������ܻᵼ��ָ����ʷǷ���ַ��
		 �ռ��СԽ��,ǿתʱ��ʧ�ڴ�Խ�� */
	if(size&(size - 1))
	{
		size = roundup_pow_of_two(size);
		PRINT_DEBUG("change ringbuff size is %d!",size);
	}

	rb->buffer = buffer;
	rb->size = size;
	rb->in = rb->out = 0;
#if USE_MUTEX	
	rb->lock = lock;
#endif
	PRINT_DEBUG("create ringBuff ok!");
	return ERR_OK;
}

/************************************************************
  * @brief   Delete_RingBuff
  * @param   rb�����λ��������
  * @return  err_t��ERR_OK��ʾ�ɹ���������ʾʧ��
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ɾ��һ�����λ�����
  ***********************************************************/
err_t Delete_RingBuff(RingBuff_t *rb)
{
	if(rb == NULL)
	{
		PRINT_ERR("ringbuff is null!");
		return ERR_NULL;
	}
	
	rb->buffer = NULL;
	rb->size = 0;
	rb->in = rb->out = 0;
#if USE_MUTEX	
	rb->lock = NULL;
#endif
	return ERR_OK;
}

/************************************************************
  * @brief   Write_RingBuff
  * @param   rb:���λ��������
  * @param   wbuff:д���������ʼ��ַ
  * @param   len:д�����ݵĳ���(�ֽ�)
  * @return  len:ʵ��д�����ݵĳ���(�ֽ�)
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ����������buff�ռ俽��len�ֽڳ��ȵ����ݵ�
             rb���λ������еĿ��пռ䡣
  ***********************************************************/
uint32_t Write_RingBuff(RingBuff_t *rb,
                        uint8_t *wbuff, 
                        uint32_t len)
{
  uint32_t l;

  len = min(len, rb->size - rb->in + rb->out);

  /* ��һ���ֵĿ���:�ӻ��λ�����д������ֱ�����������һ����ַ */
  l = min(len, rb->size - (rb->in & (rb->size - 1)));
  memcpy(rb->buffer + (rb->in & (rb->size - 1)), wbuff, l);

  /* ���������ڻ�����ͷд��ʣ��Ĳ���
     ���û����������൱����Ч */
  memcpy(rb->buffer, wbuff + l, len - l);

  rb->in += len;
	
	PRINT_DEBUG("write ringBuff len is %d!",len);
  return len;
}

/************************************************************
  * @brief   Read_RingBuff
  * @param   rb:���λ��������
  * @param   wbuff:��ȡ���ݱ������ʼ��ַ
  * @param   len:��Ҫ��ȡ���ݵĳ���(�ֽ�)
  * @return  len:ʵ�ʶ�ȡ���ݵĳ���(�ֽ�)
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ����������rb���λ������е��������򿽱�len�ֽ�
             ���ȵ����ݵ�rbuff�ռ䡣
  ***********************************************************/
uint32_t Read_RingBuff(RingBuff_t *rb,
                       uint8_t *rbuff, 
                       uint32_t len)
{
  uint32_t l;

  len = min(len, rb->in - rb->out);

  /* ��һ���ֵĿ���:�ӻ��λ�������ȡ����ֱ�����������һ�� */
  l = min(len, rb->size - (rb->out & (rb->size - 1)));
  memcpy(rbuff, rb->buffer + (rb->out & (rb->size - 1)), l);

  /* ���������ڻ�����ͷ��ȡʣ��Ĳ���
     ���û����������൱����Ч */
  memcpy(rbuff + l, rb->buffer, len - l);

  rb->out += len;
	
	PRINT_DEBUG("read ringBuff len is %d!",len);
  return len;
}

/************************************************************
  * @brief   CanRead_RingBuff
	* @param   rb:���λ��������
	* @return  uint32:�ɶ����ݳ��� 0 / len
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    �ɶ����ݳ���
  ***********************************************************/
uint32_t CanRead_RingBuff(RingBuff_t *rb)
{
	if(NULL == rb)
	{
		PRINT_ERR("ringbuff is null!");
		return 0;
	}
	if(rb->in == rb->out)
		return 0;
	
	if(rb->in > rb->out)
		return (rb->in - rb->out);
	
	return (rb->size - (rb->out - rb->in));
}

/************************************************************
  * @brief   CanRead_RingBuff
	* @param   rb:���λ��������
	* @return  uint32:��д���ݳ��� 0 / len
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ��д���ݳ���
  ***********************************************************/
uint32_t CanWrite_RingBuff(RingBuff_t *rb)
{
	if(NULL == rb)
	{
		PRINT_ERR("ringbuff is null!");
		return 0;
	}

	return (rb->size - CanRead_RingBuff(rb));
}



/******************************** DEMO *****************************************************/
//	RingBuff_t ringbuff_handle;
//	
//	uint8_t rb[64];
//	
//	Create_RingBuff(&ringbuff_handle, 
//								rb,
//								sizeof(rb));
//			Write_RingBuff(&ringbuff_handle,
//                     res, 
//                     datapack.data_length);
//			
//			PRINT_DEBUG("CanRead_RingBuff = %d!",CanRead_RingBuff(&ringbuff_handle));
//			PRINT_DEBUG("CanWrite_RingBuff = %d!",CanWrite_RingBuff(&ringbuff_handle));
//			
//			Read_RingBuff(&ringbuff_handle,
//                     res, 
//                     datapack.data_length);
/******************************** DEMO *****************************************************/


