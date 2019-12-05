/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "JpegDecoder.h"
#include "jpeglib.h"
#include "AlMath.h"
#include "Logcat.h"
#include <csetjmp>

#define JPEG_EXIF_MARKER JPEG_APP0 + 1
#define JPEG_ICC_MARKER JPEG_APP0 + 2
#define G_LITTLE_ENDIAN     1234
#define G_BIG_ENDIAN        4321

LOCAL(unsigned short) de_get16(void *ptr, uint endian) {
    unsigned short val;

    memcpy(&val, ptr, sizeof(val));
    if (endian == G_BIG_ENDIAN) {
#ifndef WORDS_BIGENDIAN
        val = AlMath::swap16(val);
#endif
    } else {
#ifdef WORDS_BIGENDIAN
        val = vlc_bswap16( val );
#endif
    }
    return val;
}

LOCAL(unsigned int) de_get32(void *ptr, uint endian) {
    unsigned int val;

    memcpy(&val, ptr, sizeof(val));
    if (endian == G_BIG_ENDIAN) {
#ifndef WORDS_BIGENDIAN
        val = AlMath::swap32(val);
#endif
    } else {
#ifdef WORDS_BIGENDIAN
        val = vlc_bswap32( val );
#endif
    }
    return val;
}

void error_exit(j_common_ptr cinfo) {
/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    error_mgr *errorMgr = (error_mgr *) cinfo->err;

/* Always display the message. */
/* We could postpone this until after returning, if we chose. */
    (*cinfo->err->output_message)(cinfo);

/* Return control to the setjmp point */
    longjmp(errorMgr->jmpBuf, 1);
}

JpegDecoder::JpegDecoder(std::string path) : AlAbsDecoder(), path(path) {
    handle = tjInitDecompress();
}

JpegDecoder::~JpegDecoder() {
    tjDestroy(handle);
}

AlBitmapInfo JpegDecoder::getInfo() {
    AlBitmapInfo info;
    FILE *file = fopen(path.c_str(), "rb");
    if (nullptr == file) {
        return info;
    }
    jpeg_decompress_struct cinfo;
    error_mgr errorMgr;
    cinfo.err = jpeg_std_error(&errorMgr.jpegErrorMgr);
    cinfo.image_width = 0;
    cinfo.image_height = 0;
    errorMgr.jpegErrorMgr.error_exit = error_exit;
    if (setjmp(errorMgr.jmpBuf)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(file);
        return info;
    }

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);
    jpeg_save_markers(&cinfo, JPEG_EXIF_MARKER, 0xFFFF);
    // Decode icc profile
    jpeg_save_markers(&cinfo, JPEG_ICC_MARKER, 0xFFFF);
    jpeg_read_header(&cinfo, true);

    int orientation = _getOrientation(cinfo.marker_list);
    switch (orientation) {
        case 6: {
            info.rotation = AlRational(1, 2);
            break;
        }
        case 8: {
            info.rotation = AlRational(3, 2);
            break;
        }
        default:
            break;
    }
    info.width = cinfo.image_width;
    info.height = cinfo.image_height;
    info.depth = 8;
    info.colorSpace = AlColorSpace::RGBA;
    jpeg_destroy_decompress(&cinfo);
    fclose(file);
    return info;
}

HwResult JpegDecoder::process(AlBuffer **buf, AlBitmapInfo *info) {
    uint8_t *buffer = nullptr;
    unsigned long length = readFile(path, &buffer);
    if (0 == length) {
        return 0;
    }

    int subsample, colorspace;
    int flags = 0;
    int fmt = TJPF_RGBA;
    int channels = 4;
    tjDecompressHeader3(handle, buffer, length, &info->width, &info->height, &subsample,
                        &colorspace);

    flags |= 0;
    *buf = AlBuffer::alloc(info->width * info->height * channels);
    tjDecompress2(handle, buffer, length, (*buf)->data(), info->width, 0, info->height, fmt, flags);
    delete[]buffer;
    info->depth = 8;
    info->colorSpace = AlColorSpace::RGBA;
    return Hw::SUCCESS;
}

/*
 * Look through the meta data in the libjpeg decompress structure to determine
 * if an EXIF Orientation tag is present. If so return its value (1-8),
 * otherwise return 0
 *
 * This function is based on the function get_orientation in io-jpeg.c, part of
 * the GdkPixbuf library, licensed under LGPLv2+.
 *   Copyright (C) 1999 Michael Zucchi, The Free Software Foundation
*/
int JpegDecoder::_getOrientation(jpeg_saved_marker_ptr make_list) {
    uint i;                    /* index into working buffer */
    uint16_t tag_type;           /* endianed tag type extracted from tiff header */
    uint ret;                  /* Return value */
    uint offset;               /* de-endianed offset in various situations */
    uint tags;                 /* number of tags in current ifd */
    uint type;                 /* de-endianed type of tag */
    uint count;                /* de-endianed count of elements in a tag */
    uint tiff = 0;             /* offset to active tiff header */
    uint endian = 0;           /* detected endian of data */

    jpeg_saved_marker_ptr exif_marker;      /* Location of the Exif APP1 marker */
    jpeg_saved_marker_ptr cmarker;          /* Location to check for Exif APP1 marker */

    const char leth[] = {0x49, 0x49, 0x2a, 0x00}; /* Little endian TIFF header */
    const char beth[] = {0x4d, 0x4d, 0x00, 0x2a}; /* Big endian TIFF header */

#define EXIF_IDENT_STRING   "Exif\000\000"
#define EXIF_ORIENT_TAGID   0x112

    /* check for Exif marker (also called the APP1 marker) */
    exif_marker = NULL;
    cmarker = make_list;

    while (cmarker) {
        if (cmarker->data_length >= 32 &&
            cmarker->marker == JPEG_EXIF_MARKER) {
            /* The Exif APP1 marker should contain a unique
               identification string ("Exif\0\0"). Check for it. */
            if (!memcmp(cmarker->data, EXIF_IDENT_STRING, 6)) {
                exif_marker = cmarker;
            }
        }
        cmarker = cmarker->next;
    }

    /* Did we find the Exif APP1 marker? */
    if (exif_marker == NULL)
        return 0;

    /* Check for TIFF header and catch endianess */
    i = 0;

    /* Just skip data until TIFF header - it should be within 16 bytes from marker start.
       Normal structure relative to APP1 marker -
            0x0000: APP1 marker entry = 2 bytes
            0x0002: APP1 length entry = 2 bytes
            0x0004: Exif Identifier entry = 6 bytes
            0x000A: Start of TIFF header (Byte order entry) - 4 bytes
                    - This is what we look for, to determine endianess.
            0x000E: 0th IFD offset pointer - 4 bytes

            exif_marker->data points to the first data after the APP1 marker
            and length entries, which is the exif identification string.
            The TIFF header should thus normally be found at i=6, below,
            and the pointer to IFD0 will be at 6+4 = 10.
    */

    while (i < 16) {
        /* Little endian TIFF header */
        if (memcmp(&exif_marker->data[i], leth, 4) == 0) {
            endian = G_LITTLE_ENDIAN;
        }
            /* Big endian TIFF header */
        else if (memcmp(&exif_marker->data[i], beth, 4) == 0) {
            endian = G_BIG_ENDIAN;
        }
            /* Keep looking through buffer */
        else {
            i++;
            continue;
        }
        /* We have found either big or little endian TIFF header */
        tiff = i;
        break;
    }

    /* So did we find a TIFF header or did we just hit end of buffer? */
    if (tiff == 0)
        return 0;

    /* Read out the offset pointer to IFD0 */
    offset = de_get32(&exif_marker->data[i] + 4, endian);
    i = i + offset;

    /* Check that we still are within the buffer and can read the tag count */

    if (i > exif_marker->data_length - 2)
        return 0;

    /* Find out how many tags we have in IFD0. As per the TIFF spec, the first
       two bytes of the IFD contain a count of the number of tags. */
    tags = de_get16(&exif_marker->data[i], endian);
    i = i + 2;

    /* Check that we still have enough data for all tags to check. The tags
       are listed in consecutive 12-byte blocks. The tag ID, type, size, and
       a pointer to the actual value, are packed into these 12 byte entries. */
    if (tags * 12U > exif_marker->data_length - i)
        return 0;

    /* Check through IFD0 for tags of interest */
    while (tags--) {
        tag_type = de_get16(&exif_marker->data[i], endian);
        /* Is this the orientation tag? */
        if (tag_type == EXIF_ORIENT_TAGID) {
            type = de_get16(&exif_marker->data[i + 2], endian);
            count = de_get32(&exif_marker->data[i + 4], endian);

            /* Check that type and count fields are OK. The orientation field
               will consist of a single (count=1) 2-byte integer (type=3). */
            if (type != 3 || count != 1)
                return 0;

            /* Return the orientation value. Within the 12-byte block, the
               pointer to the actual data is at offset 8. */
            ret = de_get16(&exif_marker->data[i + 8], endian);
            return (ret <= 8) ? ret : 0;
        }
        /* move the pointer to the next 12-byte tag field. */
        i = i + 12;
    }

    return 0;     /* No EXIF Orientation tag found */
}