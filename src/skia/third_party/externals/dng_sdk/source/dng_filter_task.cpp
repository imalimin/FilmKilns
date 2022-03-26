/*****************************************************************************/
// Copyright 2006 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in
// accordance with the terms of the Adobe license agreement accompanying it.
/*****************************************************************************/

/* $Id: //mondo/dng_sdk_1_4/dng_sdk/source/dng_filter_task.cpp#1 $ */ 
/* $DateTime: 2012/05/30 13:28:51 $ */
/* $Change: 832332 $ */
/* $Author: tknoll $ */

/*****************************************************************************/

#include "dng_filter_task.h"

#include "dng_bottlenecks.h"
#include "dng_exceptions.h"
#include "dng_image.h"
#include "dng_memory.h"
#include "dng_tag_types.h"
#include "dng_tag_values.h"
#include "dng_utils.h"

/*****************************************************************************/

dng_filter_task::dng_filter_task (const dng_image &srcImage,
						 		  dng_image &dstImage)
	
	:	fSrcImage     (srcImage)
	,	fDstImage     (dstImage)
	
	,	fSrcPlane     (0                    )
	,	fSrcPlanes    (srcImage.Planes    ())
	,	fSrcPixelType (srcImage.PixelType ())
	
	,	fDstPlane     (0                    )
	,	fDstPlanes    (dstImage.Planes    ())
	,	fDstPixelType (dstImage.PixelType ())
	
	,	fSrcRepeat    (1, 1)
	,	fSrcTileSize  (0, 0)
	
	{

	}
							  
/*****************************************************************************/

dng_filter_task::~dng_filter_task ()
	{
	
	}
		
/*****************************************************************************/

void dng_filter_task::Start (uint32 threadCount,
							 const dng_point &tileSize,
							 dng_memory_allocator *allocator,
							 dng_abort_sniffer * /* sniffer */)
	{
	
	fSrcTileSize = SrcTileSize (tileSize);
		
		uint32 srcBufferSize = ComputeBufferSize(fSrcPixelType, fSrcTileSize,
												 fSrcPlanes, pad16Bytes);
		uint32 dstBufferSize = ComputeBufferSize(fDstPixelType, tileSize,
												 fDstPlanes, pad16Bytes);
		
		for (uint32 threadIndex = 0; threadIndex < threadCount; threadIndex++)
		{
		
		fSrcBuffer [threadIndex] . Reset (allocator->Allocate (srcBufferSize));
		
		fDstBuffer [threadIndex] . Reset (allocator->Allocate (dstBufferSize));
		
		// Zero buffers so any pad bytes have defined values.
		
		DoZeroBytes (fSrcBuffer [threadIndex]->Buffer      (),
					 fSrcBuffer [threadIndex]->LogicalSize ());
		
		DoZeroBytes (fDstBuffer [threadIndex]->Buffer      (),
					 fDstBuffer [threadIndex]->LogicalSize ());
		
		}
		
	}

/*****************************************************************************/

void dng_filter_task::Process (uint32 threadIndex,
							   const dng_rect &area,
							   dng_abort_sniffer * /* sniffer */)
	{
	
	// Find source area for this destination area.
	
	dng_rect srcArea = SrcArea (area);
	
	int32 src_area_w;
	int32 src_area_h;
	if (!ConvertUint32ToInt32 (srcArea.W (), &src_area_w) || !ConvertUint32ToInt32 (srcArea.H (), &src_area_h) || src_area_w > fSrcTileSize.h || src_area_h > fSrcTileSize.v)
		{
		
		ThrowMemoryFull("Area exceeds tile size.");
		
		}
					  
	// Setup srcBuffer.
	
	dng_pixel_buffer srcBuffer(srcArea, fSrcPlane, fSrcPlanes, fSrcPixelType,
							   pcRowInterleavedAlign16,
							   fSrcBuffer [threadIndex]->Buffer ());
	
	// Setup dstBuffer.
	
	dng_pixel_buffer dstBuffer(area, fDstPlane, fDstPlanes, fDstPixelType,
							   pcRowInterleavedAlign16,
							   fDstBuffer [threadIndex]->Buffer ());
	
	// Get source pixels.
	
	fSrcImage.Get (srcBuffer,
				   dng_image::edge_repeat,
				   fSrcRepeat.v,
				   fSrcRepeat.h);
				   
	// Process area.
	
	ProcessArea (threadIndex,
				 srcBuffer,
				 dstBuffer);

	// Save result pixels.
	
	fDstImage.Put (dstBuffer);
	
	}

/*****************************************************************************/
