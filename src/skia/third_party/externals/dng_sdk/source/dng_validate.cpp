/*****************************************************************************/
// Copyright 2006-2012 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in
// accordance with the terms of the Adobe license agreement accompanying it.
/*****************************************************************************/

/* $Id: //mondo/dng_sdk_1_4/dng_sdk/source/dng_validate.cpp#2 $ */ 
/* $DateTime: 2012/06/14 20:24:41 $ */
/* $Change: 835078 $ */
/* $Author: tknoll $ */

// Process exit codes
// ------------------
//
// As usual, 0 indicates success.
//
// If an exception occurs, the exit code will be equal to:
//
//    DNG SDK error code - 100000 + 100
//
// For example, the error dng_error_memory, which has a DNG SDK error code of
// 100005, is returned as an exit code of 105.
//
// This convention accounts for the fact that the shell truncates process exit
// codes to 8 bits and that the exit code 1 is used by ASAN to signal that a
// memory error occurred (so mapping the first DNG SDK error code to an exit
// code of 1 would not be a good idea).

/*****************************************************************************/

#include "dng_color_space.h"
#include "dng_date_time.h"
#include "dng_exceptions.h"
#include "dng_file_stream.h"
#include "dng_globals.h"
#include "dng_host.h"
#include "dng_ifd.h"
#include "dng_image_writer.h"
#include "dng_info.h"
#include "dng_linearization_info.h"
#include "dng_mosaic_info.h"
#include "dng_negative.h"
#include "dng_preview.h"
#include "dng_render.h"
#include "dng_simple_image.h"
#include "dng_tag_codes.h"
#include "dng_tag_types.h"
#include "dng_tag_values.h"

#if qDNGUseXMP
#include "dng_xmp.h"
#include "dng_xmp_sdk.h"
#endif

/*****************************************************************************/

#if qDNGValidateTarget
		
/*****************************************************************************/

#define kDNGValidateVersion "1.4"
		
/*****************************************************************************/

static bool gFourColorBayer = false;
		
static int32 gMosaicPlane = -1;

static uint32 gPreferredSize = 0;
static uint32 gMinimumSize   = 0;
static uint32 gMaximumSize   = 0;

static uint32 gProxyDNGSize = 0;

static const dng_color_space *gFinalSpace = &dng_space_sRGB::Get ();

static uint32 gFinalPixelType = ttByte;

static dng_string gDumpStage1;
static dng_string gDumpStage2;
static dng_string gDumpStage3;
static dng_string gDumpTIF;
static dng_string gDumpDNG;

/*****************************************************************************/

static dng_error_code dng_validate (const char *filename)
	{
	
	printf ("Validating \"%s\"...\n", filename);
	
	try
		{
	
		dng_file_stream stream (filename);
		
		dng_host host;
		
		host.SetPreferredSize (gPreferredSize);
		host.SetMinimumSize   (gMinimumSize  );
		host.SetMaximumSize   (gMaximumSize  );
		
		host.ValidateSizes ();
		
		if (host.MinimumSize ())
			{
			
			host.SetForPreview (true);
			
			gDumpDNG.Clear ();
			
			}
			
		if (gDumpDNG.NotEmpty ())
			{
			
			host.SetSaveDNGVersion (dngVersion_SaveDefault);
			
			host.SetSaveLinearDNG (false);
			
			host.SetKeepOriginalFile (false);
			
			}
			
		// Read into the negative.
		
		AutoPtr<dng_negative> negative;
		
			{
			
			dng_info info;
			
			info.Parse (host, stream);
			
			info.PostParse (host);
			
			if (!info.IsValidDNG ())
				{
				return dng_error_bad_format;
				}
				
			negative.Reset (host.Make_dng_negative ());
			
			negative->Parse (host, stream, info);
			
			negative->PostParse (host, stream, info);
			
				{
				
				dng_timer timer ("Raw image read time");

				negative->ReadStage1Image (host, stream, info);
				
				}
				
			if (info.fMaskIndex != -1)
				{
				
				dng_timer timer ("Transparency mask read time");

				negative->ReadTransparencyMask (host, stream, info);
				
				}
				
			negative->ValidateRawImageDigest (host);
				
			}
					 
		// Option to write stage 1 image.
			
		if (gDumpStage1.NotEmpty ())
			{
			
			dng_file_stream stream2 (gDumpStage1.Get (), true);
			
			const dng_image &stage1 = *negative->Stage1Image ();
			
			dng_image_writer writer;
			
			writer.WriteTIFF (host,
							  stream2,
							  stage1,
							  stage1.Planes () >= 3 ? piRGB 
												    : piBlackIsZero);

			gDumpStage1.Clear ();
			
			}
			
		// Metadata.
			
		negative->SynchronizeMetadata ();
		
		// Four color Bayer option.
		
		if (gFourColorBayer)
			{
			negative->SetFourColorBayer ();
			}
			
		// Build stage 2 image.
		
			{
			
			dng_timer timer ("Linearization time");
			
			negative->BuildStage2Image (host);
						         
			}
					 
		if (gDumpStage2.NotEmpty ())
			{
			
			dng_file_stream stream2 (gDumpStage2.Get (), true);
			
			const dng_image &stage2 = *negative->Stage2Image ();
						
			dng_image_writer writer;
			
			writer.WriteTIFF (host,
							  stream2,
							  stage2,
							  stage2.Planes () >= 3 ? piRGB 
												    : piBlackIsZero);
			
			gDumpStage2.Clear ();
			
			}
			
		// Build stage 3 image.
			
			{
			
			dng_timer timer ("Interpolate time");
		
			negative->BuildStage3Image (host,
									    gMosaicPlane);
							
			}
			
		// Convert to proxy, if requested.
		
		if (gProxyDNGSize)
			{
			
			dng_timer timer ("ConvertToProxy time");
			
			dng_image_writer writer;
			
			negative->ConvertToProxy (host,
									  writer,
									  gProxyDNGSize);
		
			}
			
		// Flatten transparency, if required.
		
		if (negative->NeedFlattenTransparency (host))
			{
			
			dng_timer timer ("FlattenTransparency time");
		
			negative->FlattenTransparency (host);
			
			}
			
		if (gDumpStage3.NotEmpty ())
			{
			
			dng_file_stream stream2 (gDumpStage3.Get (), true);
			
			const dng_image &stage3 = *negative->Stage3Image ();
			
			dng_image_writer writer;
			
			writer.WriteTIFF (host,
							  stream2,
							  stage3,
							  stage3.Planes () >= 3 ? piRGB 
												    : piBlackIsZero);
			
			gDumpStage3.Clear ();
			
			}
			
		// Output DNG file if requested.
			
		if (gDumpDNG.NotEmpty ())
			{
			
			// Build the preview list.
			
			dng_preview_list previewList;
			
			dng_date_time_info dateTimeInfo;
			
			CurrentDateTimeAndZone (dateTimeInfo);
											  
			for (uint32 previewIndex = 0; previewIndex < 2; previewIndex++)
				{
				
				// Skip preview if writing a compresssed main image to save space
				// in this example code.
				
				if (negative->RawJPEGImage () != NULL && previewIndex > 0)
					{
					break;
					}
					
				// Report timing.
			
				dng_timer timer (previewIndex == 0 ? "Build thumbnail time"
												   : "Build preview time");
				
				// Render a preview sized image.
				
				AutoPtr<dng_image> previewImage;
				
					{
					
					dng_render render (host, *negative);
					
					render.SetFinalSpace (negative->IsMonochrome () ? dng_space_GrayGamma22::Get ()
																	: dng_space_sRGB       ::Get ());
					
					render.SetFinalPixelType (ttByte);
					
					render.SetMaximumSize (previewIndex == 0 ? 256 : 1024);
				
					previewImage.Reset (render.Render ());
				
					}
					
				// Don't write the preview if it is same size as thumbnail.
				
				if (previewIndex > 0 &&
					Max_uint32 (previewImage->Bounds ().W (),
								previewImage->Bounds ().H ()) <= 256)
					{
					break;
					}
				
				// If we have compressed JPEG data, create a compressed thumbnail.  Otherwise
				// save a uncompressed thumbnail.
				
				bool useCompressedPreview = (negative->RawJPEGImage () != NULL) ||
											(previewIndex > 0);
				
				AutoPtr<dng_preview> preview (useCompressedPreview ?
											  (dng_preview *) new dng_jpeg_preview :
											  (dng_preview *) new dng_image_preview);
											  
				// Setup up preview info.
									
				preview->fInfo.fApplicationName   .Set ("dng_validate");
				preview->fInfo.fApplicationVersion.Set (kDNGValidateVersion);
				
				preview->fInfo.fSettingsName.Set ("Default");

				preview->fInfo.fColorSpace = previewImage->Planes () == 1 ?
											 previewColorSpace_GrayGamma22 :
											 previewColorSpace_sRGB;
											
				preview->fInfo.fDateTime = dateTimeInfo.Encode_ISO_8601 ();
				
				if (!useCompressedPreview)
					{
					
					dng_image_preview *imagePreview = dynamic_cast<dng_image_preview *> (preview.Get ());
				
					imagePreview->fImage.Reset (previewImage.Release ());
					
					}
					
				else
					{

					dng_jpeg_preview *jpegPreview = dynamic_cast<dng_jpeg_preview *> (preview.Get ());
					
					int32 quality = (previewIndex == 0 ? 8 : 5);

					dng_image_writer writer;
					
					writer.EncodeJPEGPreview (host,
										      *previewImage,
										      *jpegPreview,
											  quality);
										  
					}
		
				previewList.Append (preview);
				
				}
				
			// Write DNG file.
			
			dng_file_stream stream2 (gDumpDNG.Get (), true);
			
				{
				
				dng_timer timer ("Write DNG time");
			
				dng_image_writer writer;
			
				writer.WriteDNG (host,
								 stream2,
								 *negative.Get (),
								 &previewList,
								 dngVersion_Current,
								 false);

				}
				
			gDumpDNG.Clear ();
			
			}
					
		// Output TIF file if requested.
			
		if (gDumpTIF.NotEmpty ())
			{
			
			// Render final image.
				
			dng_render render (host, *negative);
			
			render.SetFinalSpace     (*gFinalSpace   );
			render.SetFinalPixelType (gFinalPixelType);
			
			if (host.MinimumSize ())
				{
				
				dng_point stage3Size = negative->Stage3Image ()->Size ();
				
				render.SetMaximumSize (Max_uint32 (stage3Size.v,
												   stage3Size.h));

				}
			
			AutoPtr<dng_image> finalImage;
			
				{
				
				dng_timer timer ("Render time");
			
				finalImage.Reset (render.Render ());
				
				}
				
			finalImage->Rotate (negative->Orientation ());
			
			// Now that Camera Raw supports non-raw formats, we should
			// not keep any Camera Raw settings in the XMP around when
			// writing rendered files.
			
			#if qDNGUseXMP
			
			if (negative->GetXMP ())
				{

				negative->GetXMP ()->RemoveProperties (XMP_NS_CRS);
				negative->GetXMP ()->RemoveProperties (XMP_NS_CRSS);
				
				}
			
			#endif
			
			// Write TIF file.
			
			dng_file_stream stream2 (gDumpTIF.Get (), true);
			
				{
				
				dng_timer timer ("Write TIFF time");
			
				dng_image_writer writer;
			
				writer.WriteTIFF (host,
								  stream2,
								  *finalImage.Get (),
								  finalImage->Planes () >= 3 ? piRGB 
															 : piBlackIsZero,
								  ccUncompressed,
								  negative.Get (),
								  &render.FinalSpace ());
								  
				}
				
			gDumpTIF.Clear ();
			
			}
					
		}
	
	catch (const dng_exception &except)
		{
		
		return except.ErrorCode ();
		
		}
		
	catch (...)
		{
		
		return dng_error_unknown;
		
		}
		
	printf ("Validation complete\n");
	
	return dng_error_none;

	}

/*****************************************************************************/

int main (int argc, char *argv [])
	{
	
	try
		{

		if (argc == 1)
			{

			fprintf (stderr,
					 "\n"
					 "dng_validate, version " kDNGValidateVersion " "
					 #if qDNG64Bit
					 "(64-bit)"
					 #else
					 "(32-bit)"
					 #endif
					 "\n"
					 "Copyright 2005-2012 Adobe Systems, Inc.\n"
					 "\n"
					 "Usage:  %s [options] file1 file2 ...\n"
					 "\n"
					 "Valid options:\n"
					 "-v            Verbose mode\n"
					 "-d <num>      Dump line limit (implies -v)\n"
					 "-b4           Use four-color Bayer interpolation\n"
					 "-s <num>      Use this sample of multi-sample CFAs\n"
					 "-size <num>   Preferred preview image size\n"
					 "-min <num>    Minimum preview image size\n"
					 "-max <num>    Maximum preview image size\n" 
					 "-proxy <num>  Target size for proxy DNG\n"
					 "-cs1          Color space: \"sRGB\" (default)\n"
					 "-cs2          Color space: \"Adobe RGB\"\n"
					 "-cs3          Color space: \"ProPhoto RGB\"\n"
					 "-cs4          Color space: \"ColorMatch RGB\"\n"
					 "-cs5          Color space: \"Gray Gamma 1.8\"\n"
					 "-cs6          Color space: \"Gray Gamma 2.2\"\n"
					 "-16           16-bits/channel output\n"
					 "-1 <file>     Write stage 1 image to \"<file>.tif\"\n"
					 "-2 <file>     Write stage 2 image to \"<file>.tif\"\n"
					 "-3 <file>     Write stage 3 image to \"<file>.tif\"\n"
					 "-tif <file>   Write TIF image to \"<file>.tif\"\n"
					 "-dng <file>   Write DNG image to \"<file>.dng\"\n"
					 "\n",
					 argv [0]);
					 
			return 1;
			
			}
			
		int index;
		
		for (index = 1; index < argc && argv [index] [0] == '-'; index++)
			{
			
			dng_string option;
			
			option.Set (&argv [index] [1]);
			
			if (option.Matches ("v", true))
				{
				gVerbose = true;
				}
				
			else if (option.Matches ("d", true))
				{
					
				gVerbose = true;
				
				gDumpLineLimit = 0;
				
				if (index + 1 < argc)
					{
					gDumpLineLimit = atoi (argv [++index]);
					}
					
				if (!gDumpLineLimit)
					{
					fprintf (stderr, "*** Invalid number after -d\n");
					return 1;
					}
					
				}
				
			else if (option.Matches ("s", true))
				{
				
				if (index + 1 < argc)
					{
					gMosaicPlane = atoi (argv [++index]);
					}
					
				else
					{
					fprintf (stderr, "*** Missing number after -s\n");
					return 1;
					}
					
				}
					
			else if (option.Matches ("b4", true))
				{
				gFourColorBayer = true;
				}
					
			else if (option.Matches ("size", true))
				{
				
				if (index + 1 < argc)
					{
					gPreferredSize = (uint32) atoi (argv [++index]);
					}
					
				else
					{
					fprintf (stderr, "*** Missing number after -size\n");
					return 1;
					}
					
				}
					
			else if (option.Matches ("min", true))
				{
				
				if (index + 1 < argc)
					{
					gMinimumSize = (uint32) atoi (argv [++index]);
					}
					
				else
					{
					fprintf (stderr, "*** Missing number after -min\n");
					return 1;
					}
					
				}
					
			else if (option.Matches ("max", true))
				{
				
				if (index + 1 < argc)
					{
					gMaximumSize = (uint32) atoi (argv [++index]);
					}
					
				else
					{
					fprintf (stderr, "*** Missing number after -max\n");
					return 1;
					}
					
				}
					
			else if (option.Matches ("proxy", true))
				{
				
				if (index + 1 < argc)
					{
					gProxyDNGSize = (uint32) atoi (argv [++index]);
					}
					
				else
					{
					fprintf (stderr, "*** Missing number after -proxy\n");
					return 1;
					}

				}
					
			else if (option.Matches ("cs1", true))
				{
				
				gFinalSpace = &dng_space_sRGB::Get ();
				
				}
					
			else if (option.Matches ("cs2", true))
				{
				
				gFinalSpace = &dng_space_AdobeRGB::Get ();
				
				}
					
			else if (option.Matches ("cs3", true))
				{
				
				gFinalSpace = &dng_space_ProPhoto::Get ();
				
				}
					
			else if (option.Matches ("cs4", true))
				{
				
				gFinalSpace = &dng_space_ColorMatch::Get ();
				
				}
					
			else if (option.Matches ("cs5", true))
				{
				
				gFinalSpace = &dng_space_GrayGamma18::Get ();
				
				}
					
			else if (option.Matches ("cs6", true))
				{
				
				gFinalSpace = &dng_space_GrayGamma22::Get ();
				
				}
					
			else if (option.Matches ("16"))
				{
				
				gFinalPixelType = ttShort;
				
				}
					
			else if (option.Matches ("1"))
				{
				
				gDumpStage1.Clear ();
				
				if (index + 1 < argc)
					{
					gDumpStage1.Set (argv [++index]);
					}
					
				if (gDumpStage1.IsEmpty () || gDumpStage1.StartsWith ("-"))
					{
					fprintf (stderr, "*** Missing file name after -1\n");
					return 1;
					}
				
				if (!gDumpStage1.EndsWith (".tif"))
					{
					gDumpStage1.Append (".tif");
					}
				
				}
					
			else if (option.Matches ("2"))
				{
				
				gDumpStage2.Clear ();
				
				if (index + 1 < argc)
					{
					gDumpStage2.Set (argv [++index]);
					}
					
				if (gDumpStage2.IsEmpty () || gDumpStage2.StartsWith ("-"))
					{
					fprintf (stderr, "*** Missing file name after -2\n");
					return 1;
					}
				
				if (!gDumpStage2.EndsWith (".tif"))
					{
					gDumpStage2.Append (".tif");
					}
				
				}
					
			else if (option.Matches ("3"))
				{
				
				gDumpStage3.Clear ();
				
				if (index + 1 < argc)
					{
					gDumpStage3.Set (argv [++index]);
					}
					
				if (gDumpStage3.IsEmpty () || gDumpStage3.StartsWith ("-"))
					{
					fprintf (stderr, "*** Missing file name after -3\n");
					return 1;
					}
				
				if (!gDumpStage3.EndsWith (".tif"))
					{
					gDumpStage3.Append (".tif");
					}
				
				}
				
			else if (option.Matches ("tif", true))
				{
				
				gDumpTIF.Clear ();
				
				if (index + 1 < argc)
					{
					gDumpTIF.Set (argv [++index]);
					}
					
				if (gDumpTIF.IsEmpty () || gDumpTIF.StartsWith ("-"))
					{
					fprintf (stderr, "*** Missing file name after -tif\n");
					return 1;
					}
				
				if (!gDumpTIF.EndsWith (".tif"))
					{
					gDumpTIF.Append (".tif");
					}
				
				}
				
			else if (option.Matches ("dng", true))
				{
				
				gDumpDNG.Clear ();
				
				if (index + 1 < argc)
					{
					gDumpDNG.Set (argv [++index]);
					}
					
				if (gDumpDNG.IsEmpty () || gDumpDNG.StartsWith ("-"))
					{
					fprintf (stderr, "*** Missing file name after -dng\n");
					return 1;
					}
				
				if (!gDumpDNG.EndsWith (".dng"))
					{
					gDumpDNG.Append (".dng");
					}
				
				}
				
			else
				{
				fprintf (stderr, "*** Unknown option \"-%s\"\n", option.Get ());
				return 1;
				}
				
			}
					
		if (index == argc)
			{
			fprintf (stderr, "*** No file specified\n");
			return 1;
			}
		
		#if qDNGUseXMP
			
		dng_xmp_sdk::InitializeSDK ();
		
		#endif
			
		int result = 0;
		
		while (index < argc)
			{
			
			dng_error_code error_code = dng_validate (argv [index++]);
			if (error_code != dng_error_none)
				{
				
				result = error_code - dng_error_unknown + 100;
				
				}
			
			}
		
		#if qDNGUseXMP
		
		dng_xmp_sdk::TerminateSDK ();
		
		#endif
			
		return result;
		
		}
		
	catch (...)
		{
		
		}
		
	fprintf (stderr, "*** Exception thrown in main routine\n");
	
	return 1;
	
	}
	
/*****************************************************************************/

#endif
	
/*****************************************************************************/
