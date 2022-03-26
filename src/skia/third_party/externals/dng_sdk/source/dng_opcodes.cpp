/*****************************************************************************/
// Copyright 2008 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in
// accordance with the terms of the Adobe license agreement accompanying it.
/*****************************************************************************/

/* $Id: //mondo/dng_sdk_1_4/dng_sdk/source/dng_opcodes.cpp#1 $ */ 
/* $DateTime: 2012/05/30 13:28:51 $ */
/* $Change: 832332 $ */
/* $Author: tknoll $ */

/*****************************************************************************/

#include "dng_opcodes.h"

#include "dng_bottlenecks.h"
#include "dng_exceptions.h"
#include "dng_filter_task.h"
#include "dng_globals.h"
#include "dng_host.h"
#include "dng_image.h"
#include "dng_negative.h"
#include "dng_parse_utils.h"
#include "dng_stream.h"
#include "dng_tag_values.h"

/*****************************************************************************/

dng_opcode::dng_opcode (uint32 opcodeID,
						uint32 minVersion,
						uint32 flags)
						
	:	fOpcodeID          (opcodeID)
	,	fMinVersion        (minVersion)
	,	fFlags             (flags)
	,	fWasReadFromStream (false)
	,	fStage             (0)
	
	{
	
	}
					
/*****************************************************************************/

dng_opcode::dng_opcode (uint32 opcodeID,
					    dng_stream &stream,
						const char *name)
						
	:	fOpcodeID          (opcodeID)
	,	fMinVersion        (0)
	,	fFlags             (0)
	,	fWasReadFromStream (true)
	,	fStage             (0)
	
	{
	
	fMinVersion = stream.Get_uint32 ();
	fFlags      = stream.Get_uint32 ();
	
	#if qDNGValidate
	
	if (gVerbose)
		{
		
		printf ("\nOpcode: ");
		
		if (name)
			{
			printf ("%s", name);
			}
		else
			{
			printf ("Unknown (%u)", (unsigned) opcodeID);
			}
		
		printf (", minVersion = %u.%u.%u.%u",
				(unsigned) ((fMinVersion >> 24) & 0x0FF),
				(unsigned) ((fMinVersion >> 16) & 0x0FF),
				(unsigned) ((fMinVersion >>  8) & 0x0FF),
				(unsigned) ((fMinVersion      ) & 0x0FF));
				
		printf (", flags = %u\n", (unsigned) fFlags);
		
		}
	
	#else
	
	(void) name;
	
	#endif
	
	}
					
/*****************************************************************************/

dng_opcode::~dng_opcode ()
	{
	
	}

/*****************************************************************************/

void dng_opcode::PutData (dng_stream &stream) const
	{
	
	// No data by default
	
	stream.Put_uint32 (0);
	
	}

/*****************************************************************************/

bool dng_opcode::AboutToApply (dng_host &host,
							   dng_negative &negative)
	{
	
	if (SkipIfPreview () && host.ForPreview ())
		{
		
		negative.SetIsPreview (true);
		
		}
		
	else if (MinVersion () > dngVersion_Current &&
			 WasReadFromStream ())
		{
		
		if (!Optional ())
			{
			
			// Somebody screwed up computing the DNGBackwardVersion...
			
			ThrowBadFormat ();
			
			}
		
		}

	else if (!IsValidForNegative (negative))
		{
		
		ThrowBadFormat ();
		
		}
		
	else if (!IsNOP ())
		{
		
		return true;
		
		}
		
	return false;
	
	}

/*****************************************************************************/

dng_opcode_Unknown::dng_opcode_Unknown (dng_host &host,
										uint32 opcodeID,
										dng_stream &stream)
										
	:	dng_opcode (opcodeID,
					stream,
					NULL)
										
	,	fData ()
										
	{
	
	uint32 size = stream.Get_uint32 ();
	
	if (size)
		{
		
		fData.Reset (host.Allocate (size));
		
		stream.Get (fData->Buffer      (),
					fData->LogicalSize ());
					
		#if qDNGValidate
		
		if (gVerbose)
			{
			
			DumpHexAscii (fData->Buffer_uint8 (),
						  fData->LogicalSize  ());
			
			}
			
		#endif
					
		}
	
	}

/*****************************************************************************/

void dng_opcode_Unknown::PutData (dng_stream &stream) const
	{
	
	if (fData.Get ())
		{
		
		stream.Put_uint32 (fData->LogicalSize ());
		
		stream.Put (fData->Buffer      (),
					fData->LogicalSize ());
					 
		}
		
	else
		{
		
		stream.Put_uint32 (0);
		
		}
	
	}

/*****************************************************************************/

void dng_opcode_Unknown::Apply (dng_host & /* host */,
							    dng_negative & /* negative */,
							    AutoPtr<dng_image> & /* image */)
	{
	
	// We should never need to apply an unknown opcode.
	
	if (!Optional ())
		{
	
		ThrowBadFormat ();
		
		}
	
	}

/*****************************************************************************/

class dng_filter_opcode_task: public dng_filter_task
	{
	
	private:
	
		dng_filter_opcode &fOpcode;
		
		dng_negative &fNegative;
		
	public:
	
		dng_filter_opcode_task (dng_filter_opcode &opcode,
								dng_negative &negative,
								const dng_image &srcImage,
						 		dng_image &dstImage)
												
			:	dng_filter_task (srcImage,
								 dstImage)
								 
			,	fOpcode   (opcode)
			,	fNegative (negative)
			
			{
			
			fSrcPixelType = fOpcode.BufferPixelType (srcImage.PixelType ());
			
			fDstPixelType = fSrcPixelType;
			
			fSrcRepeat = opcode.SrcRepeat ();
			
			}
	
		virtual dng_rect SrcArea (const dng_rect &dstArea)
			{
			
			return fOpcode.SrcArea (dstArea,
									fDstImage.Bounds ());
			
			}

		virtual dng_point SrcTileSize (const dng_point &dstTileSize)
			{
			
			return fOpcode.SrcTileSize (dstTileSize,
										fDstImage.Bounds ());
			
			}

		virtual void ProcessArea (uint32 threadIndex,
								  dng_pixel_buffer &srcBuffer,
								  dng_pixel_buffer &dstBuffer)
			{
			
			fOpcode.ProcessArea (fNegative,
								 threadIndex,
								 srcBuffer,
								 dstBuffer,
								 dstBuffer.Area (),
								 fDstImage.Bounds ());
								 
			}

		virtual void Start (uint32 threadCount,
							const dng_point &tileSize,
							dng_memory_allocator *allocator,
							dng_abort_sniffer *sniffer)
			{
			
			dng_filter_task::Start (threadCount,
									tileSize,
									allocator,
									sniffer);
									
			fOpcode.Prepare (fNegative,
							 threadCount,
						     tileSize,
							 fDstImage.Bounds (),
							 fDstImage.Planes (),
							 fDstPixelType,
						     *allocator);
						   
			}
							
	};

/*****************************************************************************/

dng_filter_opcode::dng_filter_opcode (uint32 opcodeID,
									  uint32 minVersion,
									  uint32 flags)
									  
	:	dng_opcode (opcodeID,
					minVersion,
					flags)
					
	{
	
	}

/*****************************************************************************/

dng_filter_opcode::dng_filter_opcode (uint32 opcodeID,
									  dng_stream &stream,
									  const char *name)
									  
	:	dng_opcode (opcodeID,
					stream,
					name)
					
	{
	
	}

/*****************************************************************************/

void dng_filter_opcode::Apply (dng_host &host,
							   dng_negative &negative,
							   AutoPtr<dng_image> &image)
	{

	dng_rect modifiedBounds = ModifiedBounds (image->Bounds ());
	
	if (modifiedBounds.NotEmpty ())
		{

		// Allocate destination image.
		
		AutoPtr<dng_image> dstImage;
		
		// If we are processing the entire image, allocate an
		// undefined image.
		
		if (modifiedBounds == image->Bounds ())
			{
			
			dstImage.Reset (host.Make_dng_image (image->Bounds	  (),
												 image->Planes	  (),
												 image->PixelType ()));
												 
			}
			
		// Else start with a clone of the existing image.
			
		else
			{
			
			dstImage.Reset (image->Clone ());
			
			}
																  
		// Filter the image.
		
		dng_filter_opcode_task task (*this,
									 negative,
									 *image,
									 *dstImage);

		host.PerformAreaTask (task,
							  modifiedBounds);
							
		// Return the new image.
		
		image.Reset (dstImage.Release ());
		
		}
	
	}
		
/*****************************************************************************/

class dng_inplace_opcode_task: public dng_area_task
	{
	
	private:
	
		dng_inplace_opcode &fOpcode;
		
		dng_negative &fNegative;
		
		dng_image &fImage;
		
		uint32 fPixelType;
		
		AutoPtr<dng_memory_block> fBuffer [kMaxMPThreads];

	public:
	
		dng_inplace_opcode_task (dng_inplace_opcode &opcode,
								 dng_negative &negative,
						 		 dng_image &image)
												
			:	dng_area_task ()
								 
			,	fOpcode    (opcode)
			,	fNegative  (negative)
			,	fImage     (image)
			,	fPixelType (opcode.BufferPixelType (image.PixelType ()))
			
			{
			
			}
			
		virtual void Start (uint32 threadCount,
							const dng_point &tileSize,
							dng_memory_allocator *allocator,
							dng_abort_sniffer * /* sniffer */)
			{
			
			uint32 bufferSize = ComputeBufferSize(fPixelType, tileSize,
												  fImage.Planes(), pad16Bytes);
								   
			for (uint32 threadIndex = 0; threadIndex < threadCount; threadIndex++)
				{
				
				fBuffer [threadIndex] . Reset (allocator->Allocate (bufferSize));
				
				}
				
			fOpcode.Prepare (fNegative,
							 threadCount,
						     tileSize,
							 fImage.Bounds (),
							 fImage.Planes (),
							 fPixelType,
						     *allocator);
		
			}
							
		virtual void Process (uint32 threadIndex,
							  const dng_rect &tile,
							  dng_abort_sniffer * /* sniffer */)
			{
			
			// Setup buffer.
			
			dng_pixel_buffer buffer(tile, 0, fImage.Planes (), fPixelType,
									pcRowInterleavedAlign16,
									fBuffer [threadIndex]->Buffer ());
			
			// Get source pixels.
			
			fImage.Get (buffer);
						   
			// Process area.
			
			fOpcode.ProcessArea (fNegative,
								 threadIndex,
								 buffer,
								 tile,
								 fImage.Bounds ());

			// Save result pixels.
			
			fImage.Put (buffer);
	
			}
		
	};
	
/*****************************************************************************/

dng_inplace_opcode::dng_inplace_opcode (uint32 opcodeID,
									    uint32 minVersion,
									    uint32 flags)
									  
	:	dng_opcode (opcodeID,
					minVersion,
					flags)
					
	{
	
	}

/*****************************************************************************/

dng_inplace_opcode::dng_inplace_opcode (uint32 opcodeID,
									    dng_stream &stream,
									    const char *name)
									  
	:	dng_opcode (opcodeID,
					stream,
					name)
					
	{
	
	}

/*****************************************************************************/

void dng_inplace_opcode::Apply (dng_host &host,
							    dng_negative &negative,
							    AutoPtr<dng_image> &image)
	{
	
	dng_rect modifiedBounds = ModifiedBounds (image->Bounds ());
	
	if (modifiedBounds.NotEmpty ())
		{

		dng_inplace_opcode_task task (*this,
									  negative,
									  *image);

		host.PerformAreaTask (task,
							  modifiedBounds);
							  
		}

	}
		
/*****************************************************************************/
