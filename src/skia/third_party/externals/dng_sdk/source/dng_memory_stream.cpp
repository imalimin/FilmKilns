/*****************************************************************************/
// Copyright 2006-2007 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in
// accordance with the terms of the Adobe license agreement accompanying it.
/*****************************************************************************/

/* $Id: //mondo/dng_sdk_1_4/dng_sdk/source/dng_memory_stream.cpp#1 $ */ 
/* $DateTime: 2012/05/30 13:28:51 $ */
/* $Change: 832332 $ */
/* $Author: tknoll $ */

/*****************************************************************************/

#include "dng_memory_stream.h"

#include "dng_bottlenecks.h"
#include "dng_exceptions.h"
#include "dng_safe_arithmetic.h"
#include "dng_utils.h"

/*****************************************************************************/

dng_memory_stream::dng_memory_stream (dng_memory_allocator &allocator,
									  dng_abort_sniffer *sniffer,
						   			  uint32 pageSize)
						   			  
	:	dng_stream (sniffer, 
					kDefaultBufferSize,
					kDNGStreamInvalidOffset)
	
	,	fAllocator (allocator)
	,	fPageSize  (pageSize )
	
	,	fPageCount      (0)
	,	fPagesAllocated (0)
	,	fPageList       (NULL)
	
	,	fMemoryStreamLength (0)
	
	{
	
	}
		
/*****************************************************************************/

dng_memory_stream::~dng_memory_stream ()
	{
	
	if (fPageList)
		{
		
		for (uint32 index = 0; index < fPageCount; index++)
			{
			
			delete fPageList [index];
			
			}
			
		free (fPageList);
		
		}
	
	}
		
/*****************************************************************************/
		
uint64 dng_memory_stream::DoGetLength ()
	{
	
	return fMemoryStreamLength;
	
	}
		
/*****************************************************************************/
		
void dng_memory_stream::DoRead (void *data,
							    uint32 count,
							    uint64 offset)
	{
	
	if (offset + count > fMemoryStreamLength)
		{
		
		ThrowEndOfFile ();
		
		}
		
	uint64 baseOffset = offset;
	
	while (count)
		{
		
		uint32 pageIndex  = (uint32) (offset / fPageSize);
		uint32 pageOffset = (uint32) (offset % fPageSize);
		
		uint32 blockCount = Min_uint32 (fPageSize - pageOffset, count);
		
		const uint8 *sPtr = fPageList [pageIndex]->Buffer_uint8 () +
						    pageOffset;
		
		uint8 *dPtr = ((uint8 *) data) + (uint32) (offset - baseOffset);
		
		DoCopyBytes (sPtr, dPtr, blockCount);
		
		offset += blockCount;
		count  -= blockCount;
		
		}
	
	}
							 
/*****************************************************************************/

void dng_memory_stream::DoSetLength (uint64 length)
	{
	
	while (length > fPageCount * (uint64) fPageSize)
		{
		
		if (fPageCount == fPagesAllocated)
			{
			
			uint32 newSizeTemp1 = 0, newSizeTemp2 = 0;
			if (!SafeUint32Add (fPagesAllocated, 32u, &newSizeTemp1) ||
				!SafeUint32Mult (fPagesAllocated, 2u, &newSizeTemp2))
				{
				ThrowMemoryFull ("Arithmetic overflow in DoSetLength()");
				}
			uint32 newSize = Max_uint32 (newSizeTemp1, newSizeTemp2);
			uint32 numBytes;
			if (!SafeUint32Mult (newSize, sizeof (dng_memory_block *),
								 &numBytes))
				{
				ThrowMemoryFull ("Arithmetic overflow in DoSetLength()");
				}
			
			dng_memory_block **list = (dng_memory_block **) malloc (numBytes);
			
			if (!list)
				{
				
				ThrowMemoryFull ();
				
				}
			
			if (fPageCount)
				{
				
				// The multiplication here is safe against overflow. fPageCount
				// can never reach a value that is large enough to cause
				// overflow because the computation of numBytes above would fail
				// before a list of that size could be allocated.
				DoCopyBytes (fPageList,
							 list,
							 fPageCount * (uint32) sizeof (dng_memory_block *));
				
				}
				
			if (fPageList)
				{
				
				free (fPageList);
				
				}
				
			fPageList = list;
			
			fPagesAllocated = newSize;
			
			}
			
		fPageList [fPageCount] = fAllocator.Allocate (fPageSize);
		
		fPageCount++;
		
		}
		
	fMemoryStreamLength = length;
	
	}
							 
/*****************************************************************************/

void dng_memory_stream::DoWrite (const void *data,
							     uint32 count,
							     uint64 offset)
	{
	
	DoSetLength (Max_uint64 (fMemoryStreamLength,
							 offset + count));
	
	uint64 baseOffset = offset;
	
	while (count)
		{
		
		uint32 pageIndex  = (uint32) (offset / fPageSize);
		uint32 pageOffset = (uint32) (offset % fPageSize);
		
		uint32 blockCount = Min_uint32 (fPageSize - pageOffset, count);
		
		const uint8 *sPtr = ((const uint8 *) data) + (uint32) (offset - baseOffset);
		
		uint8 *dPtr = fPageList [pageIndex]->Buffer_uint8 () +
					  pageOffset;
		
		DoCopyBytes (sPtr, dPtr, blockCount);
		
		offset += blockCount;
		count  -= blockCount;
		
		}
	
	}
		
/*****************************************************************************/

void dng_memory_stream::CopyToStream (dng_stream &dstStream,
									  uint64 count)
	{
	
	if (count < kBigBufferSize)
		{
	
		dng_stream::CopyToStream (dstStream, count);
		
		}
		
	else
		{
		
		Flush ();
		
		uint64 offset = Position ();
		
		if (offset + count > Length ())
			{
			
			ThrowEndOfFile ();
			
			}
			
		while (count)
			{
			
			uint32 pageIndex  = (uint32) (offset / fPageSize);
			uint32 pageOffset = (uint32) (offset % fPageSize);
			
			uint32 blockCount = (uint32) Min_uint64 (fPageSize - pageOffset, count);
			
			const uint8 *sPtr = fPageList [pageIndex]->Buffer_uint8 () +
								pageOffset;
								
			dstStream.Put (sPtr, blockCount);
			
			offset += blockCount;
			count  -= blockCount;
			
			}
			
		SetReadPosition (offset);
	
		}
	
	}
		
/*****************************************************************************/
