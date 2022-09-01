/*
   This file is part of DirectFB.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
*/

#ifndef __DIRECTFB_H__
#define __DIRECTFB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <dfb_types.h>
#include <direct/interface.h>
#include <directfb_build.h>
#include <directfb_keyboard.h>

/*
 * Main interface of DirectFB.
 */
D_DECLARE_INTERFACE( IDirectFB )

/*
 * Interface to different display outputs, encoders, connector settings, power management and synchronization.
 */
D_DECLARE_INTERFACE( IDirectFBScreen )

/*
 * Layer interface for configuration, window stack usage or direct surface access, with shared/exclusive context.
 */
D_DECLARE_INTERFACE( IDirectFBDisplayLayer )

/*
 * Interface to a surface object, being a graphics context for rendering and state control, buffer operations, palette
 * access and sub area translate'n'clip logic.
 */
D_DECLARE_INTERFACE( IDirectFBSurface )

/*
 * Interface to a surface allocation object, being an actual buffer being allocated for a surface.
 */
D_DECLARE_INTERFACE( IDirectFBSurfaceAllocation )

/*
 * Interface for read/write access to the colors of a palette object and for cloning it.
 */
D_DECLARE_INTERFACE( IDirectFBPalette )

/*
 * Input device interface for keymap access, event buffers and state queries.
 */
D_DECLARE_INTERFACE( IDirectFBInputDevice )

/*
 * Interface to a local event buffer to send/receive events, wait for events, abort waiting or reset buffer.
 */
D_DECLARE_INTERFACE( IDirectFBEventBuffer )

/*
 * Interface to a window object, controlling appearance and focus, positioning and stacking, event buffers and surface
 * access.
 */
D_DECLARE_INTERFACE( IDirectFBWindow )

/*
 * Interface to a font provider, getting metrics, measuring strings or single characters, query/choose encodings.
 */
D_DECLARE_INTERFACE( IDirectFBFont )

/*
 * Interface to an image provider, retrieving information about the image and rendering it to a surface.
 */
D_DECLARE_INTERFACE( IDirectFBImageProvider )

/*
 * Interface to a video provider for playback with advanced control and basic stream information.
 */
D_DECLARE_INTERFACE( IDirectFBVideoProvider )

/*
 * Data buffer interface, providing unified access to different kinds of data storage and live feed.
 */
D_DECLARE_INTERFACE( IDirectFBDataBuffer )

/*
 * OpenGL interface.
 */
D_DECLARE_INTERFACE( IDirectFBGL )

/**********************************************************************************************************************/

/*
 * Checks for a certain DirectFB version.
 * In case of an error a message is returned describing
 * the mismatch.
 */
const char DIRECTFB_API *DirectFBCheckVersion (
     unsigned int                            required_major,     /* major version */
     unsigned int                            required_minor,     /* minor version */
     unsigned int                            required_micro      /* micro version */
);

/*
 * Parses the command-line and initializes some variables.
 * You absolutely need to call this before doing anything else.
 * Removes all options used by DirectFB from argv.
 */
DFBResult  DIRECTFB_API  DirectFBInit (
     int                                    *argc,               /* pointer to main()'s argc */
     char                                 *(*argv[])             /* pointer to main()'s argv */
);

/*
 * Sets configuration parameters supported on command line and in
 * config file. Can only be called before DirectFBCreate but after
 * DirectFBInit.
 */
DFBResult  DIRECTFB_API  DirectFBSetOption (
     const char                             *name,               /* option name */
     const char                             *value               /* option value */
);

/*
 * Creates the main interface.
 */
DFBResult  DIRECTFB_API  DirectFBCreate (
     IDirectFB                             **ret_interface       /* pointer to the created interface */
);

/*
 * Prints a description of the result code along with an
 * optional message that is put in front with a colon.
 */
DFBResult  DIRECTFB_API  DirectFBError (
     const char                             *msg,                /* optional message */
     DFBResult                               result              /* result code to interpret */
);

/*
 * Returns a string describing result.
 */
const char DIRECTFB_API *DirectFBErrorString (
     DFBResult                               result              /* result code to describe */
);

/*
 * Behaves like DirectFBError, but shuts down the calling
 * application.
 */
DFBResult  DIRECTFB_API  DirectFBErrorFatal (
     const char                             *msg,                /* optional message */
     DFBResult                               result              /* result code to interpret */
);

/**********************************************************************************************************************/

/*
 * A rectangle specified by two points.
 *
 * The defined rectangle includes the top left but not the
 * bottom right endpoint.
 */
typedef struct {
     int                                     x1;                 /* X coordinate of top-left point (inclusive) */
     int                                     y1;                 /* Y coordinate of top-left point (inclusive) */
     int                                     x2;                 /* X coordinate of lower-right point (exclusive) */
     int                                     y2;                 /* Y coordinate of lower-right point (exclusive) */
} DFBBox;

/*
 * A color defined by channels with 8bit each.
 */
typedef struct {
     u8                                      a;                  /* alpha channel */
     u8                                      r;                  /* red channel */
     u8                                      g;                  /* green channel */
     u8                                      b;                  /* blue channel */
} DFBColor;

/*
 * Flags defining which fields of a DFBColorAdjustment are
 * valid.
 */
typedef enum {
     DCAF_NONE                             = 0x00000000,         /* None of these. */

     DCAF_BRIGHTNESS                       = 0x00000001,         /* Brightness field is valid. */
     DCAF_CONTRAST                         = 0x00000002,         /* Contrast field is valid. */
     DCAF_HUE                              = 0x00000004,         /* Hue field is valid. */
     DCAF_SATURATION                       = 0x00000008,         /* Saturation field is valid. */

     DCAF_ALL                              = 0x0000000F          /* All of these. */
} DFBColorAdjustmentFlags;

/*
 * Color Adjustment used to adjust video colors.
 *
 * All fields are in the range 0x0 to 0xffff with 0x8000 as the
 * default value (no adjustment).
 */
typedef struct {
     DFBColorAdjustmentFlags                 flags;              /* Validation of fields. */

     u16                                     brightness;         /* Color brightness. */
     u16                                     contrast;           /* Color contrast. */
     u16                                     hue;                /* Color hue. */
     u16                                     saturation;         /* Color saturation. */
} DFBColorAdjustment;

/*
 * A color key defined by R,G,B and eventually a color index.
 */
typedef struct {
     u8                                      index;              /* color index */
     u8                                      r;                  /* red channel */
     u8                                      g;                  /* green channel */
     u8                                      b;                  /* blue channel */
} DFBColorKey;

/*
 * Color key polarity.
 */
typedef enum {
     DCKP_DEFAULT                          = 0x00000000,         /* default */
     DCKP_OTHER                            = 0x00000001          /* other */
} DFBColorKeyPolarity;

/*
 * Extended color key definition.
 */
typedef struct {
     DFBColorKeyPolarity                     polarity;           /* color key polarity */
     DFBColor                                lower;              /* lower color key */
     DFBColor                                upper;              /* upper color key */
} DFBColorKeyExtended;

/*
 * A color defined by channels with 8bit each.
 */
typedef struct {
     u8                                      a;                  /* alpha channel */
     u8                                      y;                  /* luma channel */
     u8                                      u;                  /* chroma channel */
     u8                                      v;                  /* chroma channel */
} DFBColorYUV;

/*
 * Convolution filter.
 */
typedef struct {
     s32                                     kernel[9];          /* The kernel consists of 3x3 matrix. */
     s32                                     scale;              /* Scale. */
     s32                                     bias;               /* Bias. */
} DFBConvolutionFilter;

/*
 * A dimension specified by width and height.
 */
typedef struct {
     int                                     w;                  /* width of it */
     int                                     h;                  /* height of it */
} DFBDimension;

/*
 * Insets specify a distance from each edge of a rectangle.
 *
 * Positive values always mean outside.
 */
typedef struct {
     int                                     l;                  /* distance from left edge */
     int                                     t;                  /* distance from top edge */
     int                                     r;                  /* distance from right edge */
     int                                     b;                  /* distance from bottom edge */
} DFBInsets;

/*
 * A rectangle specified by normalized coordinates.
 *
 * E.g. using 0.0, 0.0, 1.0, 1.0 would specify the whole screen.
 */
typedef struct {
     float                                   x;                  /* normalized X coordinate */
     float                                   y;                  /* normalized Y coordinate */
     float                                   w;                  /* normalized width */
     float                                   h;                  /* normalized height */
} DFBLocation;

/*
 * A point specified by x/y coordinates.
 */
typedef struct {
     int                                     x;                  /* X coordinate of it */
     int                                     y;                  /* Y coordinate of it */
} DFBPoint;

/*
 * A rectangle specified by a point and a dimension.
 */
typedef struct {
     int                                     x;                  /* X coordinate of its top-left point */
     int                                     y;                  /* Y coordinate of its top-left point */
     int                                     w;                  /* width of it */
     int                                     h;                  /* height of it */
} DFBRectangle;

/*
 * A region specified by two points.
 *
 * The defined region includes both endpoints.
 */
typedef struct {
     int                                     x1;                 /* X coordinate of top-left point */
     int                                     y1;                 /* Y coordinate of top-left point */
     int                                     x2;                 /* X coordinate of lower-right point */
     int                                     y2;                 /* Y coordinate of lower-right point */
} DFBRegion;

/*
 * A horizontal line specified by x and width.
 */
typedef struct {
     int                                     x;                  /* X coordinate */
     int                                     w;                  /* width of span */
} DFBSpan;

/*
 * A trapezoid specified by two points with a width each.
 */
typedef struct {
     int                                     x1;                 /* X coordinate of first span */
     int                                     y1;                 /* Y coordinate of first span  */
     int                                     w1;                 /* width of first span */
     int                                     x2;                 /* X coordinate of second span */
     int                                     y2;                 /* Y coordinate of second span */
     int                                     w2;                 /* width of second span */
} DFBTrapezoid;

/*
 * A triangle specified by three points.
 */
typedef struct {
     int                                     x1;                 /* X coordinate of first edge */
     int                                     y1;                 /* Y coordinate of first edge */
     int                                     x2;                 /* X coordinate of second edge */
     int                                     y2;                 /* Y coordinate of second edge */
     int                                     x3;                 /* X coordinate of third edge */
     int                                     y3;                 /* Y coordinate of third edge */
} DFBTriangle;

/*
 * Way of building triangles from the list of vertices.
 */
typedef enum {
     DTTF_LIST                             = 0x00000000,         /* 0/1/2  3/4/5  6/7/8 ... */
     DTTF_STRIP                            = 0x00000001,         /* 0/1/2  1/2/3  2/3/4 ... */
     DTTF_FAN                              = 0x00000002          /* 0/1/2  0/2/3  0/3/4 ... */
} DFBTriangleFormation;

/*
 * Transformed vertex of a textured triangle.
 */
typedef struct {
     float                                   x;                  /* Destination X coordinate (in pixels) */
     float                                   y;                  /* Destination Y coordinate (in pixels) */
     float                                   z;                  /* Z coordinate */
     float                                   w;                  /* W coordinate */

     float                                   s;                  /* Texture S coordinate */
     float                                   t;                  /* Texture T coordinate */
} DFBVertex;

/**********************************************************************************************************************/

typedef unsigned int DFBColorID;
typedef unsigned int DFBDisplayLayerID;
typedef unsigned int DFBDisplayLayerIDs;
typedef unsigned int DFBDisplayLayerSourceID;
typedef unsigned int DFBInputDeviceID;
typedef unsigned int DFBScreenID;
typedef unsigned int DFBSurfaceID;
typedef unsigned int DFBTextEncodingID;
typedef unsigned int DFBWindowID;

/* Predefined color ids. */
#define DCID_PRIMARY                         0x00000000          /* primary color */
#define DCID_OUTLINE                         0x00000001          /* outline color */

/* Predefined display layer ids. */
#define DLID_PRIMARY                         0x00000000          /* primary layer */

/* Predefined display layer source ids */
#define DLSID_SURFACE                        0x00000000          /* surface layer source */

/* Predefined input device ids. */
#define DIDID_KEYBOARD                       0x00000000          /* primary keyboard */
#define DIDID_MOUSE                          0x00000001          /* primary mouse */
#define DIDID_JOYSTICK                       0x00000002          /* primary joystick */
#define DIDID_REMOTE                         0x00000003          /* primary remote control */
#define DIDID_ANY                            0x00000010          /* no primary device */

/* Predefined screen ids. */
#define DSCID_PRIMARY                        0x00000000          /* primary screen */

/* Predefined text encoding ids. */
#define DTEID_UTF8                           0x00000000          /* UTF-8 */
#define DTEID_OTHER                          0x00000001          /* Other */

/**********************************************************************************************************************/

/*************
 * IDirectFB *
 *************/

/*
 * The cooperative level controls the main interface's behaviour
 * in functions like SetVideoMode() or CreateSurface() for the
 * primary.
 */
typedef enum {
     DFSCL_NORMAL                          = 0x00000000,         /* Normal shared access, primary surface will be the
                                                                    buffer of an implicitly created window at the
                                                                    resolution given by SetVideoMode(). */
     DFSCL_FULLSCREEN                      = 0x00000001,         /* Application grabs the primary layer, SetVideoMode()
                                                                    automates layer control. Primary surface is the
                                                                    primary layer surface. */
     DFSCL_EXCLUSIVE                       = 0x00000002          /* All but the primary layer will be disabled, the
                                                                    application has full control over layers if desired,
                                                                    other applications have no input/output/control.
                                                                    Primary surface is the primary layer surface. */
} DFBCooperativeLevel;

/*
 * Mask of accelerated functions.
 */
typedef enum {
     DFXL_NONE                             = 0x00000000,         /* None of these. */

     DFXL_FILLRECTANGLE                    = 0x00000001,         /* FillRectangle() is accelerated. */
     DFXL_DRAWRECTANGLE                    = 0x00000002,         /* DrawRectangle() is accelerated. */
     DFXL_DRAWLINE                         = 0x00000004,         /* DrawLine() is accelerated. */
     DFXL_FILLTRIANGLE                     = 0x00000008,         /* FillTriangle() is accelerated. */
     DFXL_FILLTRAPEZOID                    = 0x00000010,         /* FillTrapezoids() is accelerated. */
     DFXL_FILLQUADRANGLE                   = 0x00000020,         /* FillQuadrangles() is accelerated. */

     DFXL_DRAWMONOGLYPH                    = 0x00001000,         /* DrawMonoGlyphs() is accelerated. */

     DFXL_BLIT                             = 0x00010000,         /* Blit() is accelerated. */
     DFXL_STRETCHBLIT                      = 0x00020000,         /* StretchBlit() is accelerated. */
     DFXL_TEXTRIANGLES                     = 0x00040000,         /* TextureTriangles() is accelerated. */
     DFXL_BLIT2                            = 0x00080000,         /* Blit2() is accelerated. */

     DFXL_DRAWSTRING                       = 0x01000000,         /* DrawString() is accelerated. */

     DFXL_ALL                              = 0x010F003F,         /* All drawing/blitting functions. */
     DFXL_ALL_DRAW                         = 0x0000103F,         /* All drawing functions. */
     DFXL_ALL_BLIT                         = 0x010F0000          /* All blitting functions. */
} DFBAccelerationMask;

#define DFB_DRAWING_FUNCTION(a)       ((a) & 0x0000FFFF)
#define DFB_BLITTING_FUNCTION(a)      ((a) & 0xFFFF0000)

/*
 * Flags controlling blitting commands.
 */
typedef enum {
     DSBLIT_NOFX                           = 0x00000000,         /* uses none of the effects */
     DSBLIT_BLEND_ALPHACHANNEL             = 0x00000001,         /* enables blending and uses
                                                                    alphachannel from source */
     DSBLIT_BLEND_COLORALPHA               = 0x00000002,         /* enables blending and uses
                                                                    alpha value from color */
     DSBLIT_COLORIZE                       = 0x00000004,         /* modulates source color with
                                                                    the color's r/g/b values */
     DSBLIT_SRC_COLORKEY                   = 0x00000008,         /* don't blit pixels matching the source color key */
     DSBLIT_DST_COLORKEY                   = 0x00000010,         /* write to destination only if the destination pixel
                                                                    matches the destination color key */
     DSBLIT_SRC_PREMULTIPLY                = 0x00000020,         /* modulates the source color with the (modulated)
                                                                    source alpha */
     DSBLIT_DST_PREMULTIPLY                = 0x00000040,         /* modulates the dest. color with the dest. alpha */
     DSBLIT_DEMULTIPLY                     = 0x00000080,         /* divides the color by the alpha before writing the
                                                                    data to the destination */
     DSBLIT_DEINTERLACE                    = 0x00000100,         /* deinterlaces the source during blitting by reading
                                                                    only one field (every second line of full
                                                                    image) scaling it vertically by factor two */
     DSBLIT_SRC_PREMULTCOLOR               = 0x00000200,         /* modulates the source color with the color alpha */
     DSBLIT_XOR                            = 0x00000400,         /* bitwise xor the destination pixels with the
                                                                    source pixels after premultiplication */
     DSBLIT_INDEX_TRANSLATION              = 0x00000800,         /* do fast indexed to indexed translation,
                                                                    this flag is mutual exclusive with all others */
     DSBLIT_ROTATE180                      = 0x00001000,         /* rotate the image by 180 degree */
     DSBLIT_ROTATE90                       = 0x00002000,         /* rotate the image by 90 degree */
     DSBLIT_ROTATE270                      = 0x00004000,         /* rotate the image by 270 degree */

     DSBLIT_COLORKEY_PROTECT               = 0x00010000,         /* make sure written pixels don't match color key */
     DSBLIT_SRC_COLORKEY_EXTENDED          = 0x00020000,         /* use extended source color key */
     DSBLIT_DST_COLORKEY_EXTENDED          = 0x00040000,         /* use extended destination color key */

     DSBLIT_SRC_MASK_ALPHA                 = 0x00100000,         /* modulate source alpha channel with alpha channel
                                                                    from source mask */
     DSBLIT_SRC_MASK_COLOR                 = 0x00200000,         /* modulate source color channels with color channels
                                                                    from source mask */

     DSBLIT_FLIP_HORIZONTAL                = 0x01000000,         /* flip the image horizontally */
     DSBLIT_FLIP_VERTICAL                  = 0x02000000,         /* flip the image vertically */

     DSBLIT_SRC_COLORMATRIX                = 0x08000000,         /* use source color matrix setting */
     DSBLIT_SRC_CONVOLUTION                = 0x10000000          /* use source convolution filter */
} DFBSurfaceBlittingFlags;

/*
 * Flags controlling drawing commands.
 */
typedef enum {
     DSDRAW_NOFX                           = 0x00000000,         /* uses none of the effects */
     DSDRAW_BLEND                          = 0x00000001,         /* uses alpha from color */
     DSDRAW_DST_COLORKEY                   = 0x00000002,         /* write to destination only if the destination pixel
                                                                    matches the destination color key */
     DSDRAW_SRC_PREMULTIPLY                = 0x00000004,         /* multiplies the color's rgb channels by the alpha
                                                                    channel before drawing */
     DSDRAW_DST_PREMULTIPLY                = 0x00000008,         /* modulates the dest. color with the dest. alpha */
     DSDRAW_DEMULTIPLY                     = 0x00000010,         /* divides the color by the alpha before writing the
                                                                    data to the destination */
     DSDRAW_XOR                            = 0x00000020          /* bitwise xor the destination pixels with the
                                                                    specified color after premultiplication */
} DFBSurfaceDrawingFlags;

#define DFB_GRAPHICS_DRIVER_INFO_NAME_LENGTH   40
#define DFB_GRAPHICS_DRIVER_INFO_VENDOR_LENGTH 60

/*
 * Driver information.
 */
typedef struct {
     int                                     major;              /* Major version */
     int                                     minor;              /* Minor version */

     char name[DFB_GRAPHICS_DRIVER_INFO_NAME_LENGTH];            /* Driver name */
     char vendor[DFB_GRAPHICS_DRIVER_INFO_VENDOR_LENGTH];        /* Driver vendor */
} DFBGraphicsDriverInfo;

#define DFB_GRAPHICS_DEVICE_DESC_NAME_LENGTH   48
#define DFB_GRAPHICS_DEVICE_DESC_VENDOR_LENGTH 64

/*
 * Description of the graphics device capabilities.
 */
typedef struct {
     DFBAccelerationMask                     acceleration_mask;  /* Accelerated functions */
     DFBSurfaceBlittingFlags                 blitting_flags;     /* Supported blitting flags */
     DFBSurfaceDrawingFlags                  drawing_flags;      /* Supported drawing flags */
     unsigned int                            video_memory;       /* Amount of video memory in bytes */
     char name[DFB_GRAPHICS_DEVICE_DESC_NAME_LENGTH];            /* Device/Chipset name */
     char vendor[DFB_GRAPHICS_DEVICE_DESC_VENDOR_LENGTH];        /* Device vendor */
     DFBGraphicsDriverInfo                   driver;             /* Device driver information */
} DFBGraphicsDeviceDescription;

/*
 * Flags defining which fields of a DFBSurfaceDescription are
 * valid.
 */
typedef enum {
     DSDESC_NONE                           = 0x00000000,         /* None of these. */

     DSDESC_CAPS                           = 0x00000001,         /* caps field is valid. */
     DSDESC_WIDTH                          = 0x00000002,         /* width field is valid. */
     DSDESC_HEIGHT                         = 0x00000004,         /* height field is valid. */
     DSDESC_PIXELFORMAT                    = 0x00000008,         /* pixelformat field is valid. */
     DSDESC_PREALLOCATED                   = 0x00000010,         /* Surface uses data that has been preallocated by the
                                                                    application. The field array preallocated has to be
                                                                    set using the first element for the front buffer and
                                                                    eventually the second one for the back buffer. */
     DSDESC_PALETTE                        = 0x00000020,         /* Initialize the surfaces palette with the entries
                                                                    specified in the description. */
     DSDESC_COLORSPACE                     = 0x00000040,         /* colorspace field is valid. */

     DSDESC_RESOURCE_ID                    = 0x00000100,         /* User defined resource id for general purpose
                                                                    surfaces is specified, or resource id of window,
                                                                    layer, user is returned. */
     DSDESC_HINTS                          = 0x00000200,         /* Flags for optimized allocation and pixel format
                                                                    selection are set. */

     DSDESC_ALL                            = 0x0000037F          /* All of these. */
} DFBSurfaceDescriptionFlags;

/*
 * The surface capabilities.
 */
typedef enum {
     DSCAPS_NONE                           = 0x00000000,         /* None of these. */

     DSCAPS_PRIMARY                        = 0x00000001,         /* It's the primary surface. */
     DSCAPS_SYSTEMONLY                     = 0x00000002,         /* Surface data is permanently stored in system memory.
                                                                    There's no video memory allocation/storage. */
     DSCAPS_VIDEOONLY                      = 0x00000004,         /* Surface data is permanently stored in video memory.
                                                                    There's no system memory allocation/storage. */
     DSCAPS_GL                             = 0x00000008,         /* Surface data is stored in memory that can be
                                                                    accessed by a GL accelerator. */
     DSCAPS_DOUBLE                         = 0x00000010,         /* Surface is double buffered. */
     DSCAPS_SUBSURFACE                     = 0x00000020,         /* Surface is just a sub area of another
                                                                    one sharing the surface data. */
     DSCAPS_INTERLACED                     = 0x00000040,         /* Each buffer contains interlaced video (or graphics)
                                                                    data consisting of two fields.
                                                                    Their lines are stored interleaved. One field's
                                                                    height is a half of the surface's height. */
     DSCAPS_SEPARATED                      = 0x00000080,         /* For usage with DSCAPS_INTERLACED.
                                                                    DSCAPS_SEPARATED specifies that the fields are not
                                                                    interleaved line by line in the buffer.
                                                                    The first field is followed by the second one. */
     DSCAPS_STATIC_ALLOC                   = 0x00000100,         /* The amount of video or system memory allocated for
                                                                    the surface is never less than its initial value.
                                                                    This way a surface can be resized (smaller and
                                                                    bigger up to the initial size) without reallocation
                                                                    of the buffers. It's useful for surfaces that need a
                                                                    guaranteed space in video memory after resizing. */
     DSCAPS_TRIPLE                         = 0x00000200,         /* Surface is triple buffered. */

     DSCAPS_PREMULTIPLIED                  = 0x00001000,         /* Surface stores data with premultiplied alpha. */

     DSCAPS_DEPTH                          = 0x00010000,         /* A depth buffer is allocated. */
     DSCAPS_STEREO                         = 0x00020000,         /* Both left & right buffers are allocated. Only valid
                                                                    with windows and layers with the DLOP_STEREO or
                                                                    DWCAPS_STEREO flags set. */

     DSCAPS_SHARED                         = 0x00100000,         /* The surface will be accessible among processes. */

     DSCAPS_ROTATED                        = 0x01000000,         /* The back buffers are allocated with swapped
                                                                    width/height. */

     DSCAPS_ALL                            = 0x011113FF,         /* All of these. */

     DSCAPS_FLIPPING                       = DSCAPS_DOUBLE |
                                             DSCAPS_TRIPLE       /* Surface needs Flip() calls to make
                                                                    updates/changes visible/usable. */
} DFBSurfaceCapabilities;

/*
 * Encodes format constants in the following way (bit 31 - 0):
 *
 * lkjj:hhgg | gfff:eeed | cccc:bbbb | baaa:aaaa
 *
 * a) pixelformat index
 * b) effective color (or index) bits per pixel of format
 * c) effective alpha bits per pixel of format
 * d) alpha channel present
 * e) bytes per "pixel in a row" (1/8 fragment, i.e. bits)
 * f) bytes per "pixel in a row" (decimal part, i.e. bytes)
 * g) smallest number of pixels aligned to byte boundary
 * h) multiplier for planes minus one (1/4 fragment)
 * j) multiplier for planes minus one (decimal part)
 * k) color and/or alpha lookup table present
 * l) alpha channel is inverted
 */
#define DFB_SURFACE_PIXELFORMAT(index,color_bits,     \
                                alpha_bits,has_alpha, \
                                row_bits,row_bytes,   \
                                align,mul_f,mul_d,    \
                                has_lut,inv_alpha)    \
     ( (((index     ) & 0x7F)      ) |                \
       (((color_bits) & 0x1F) <<  7) |                \
       (((alpha_bits) & 0x0F) << 12) |                \
       (((has_alpha ) ? 1 :0) << 16) |                \
       (((row_bits  ) & 0x07) << 17) |                \
       (((row_bytes ) & 0x07) << 20) |                \
       (((align     ) & 0x07) << 23) |                \
       (((mul_f     ) & 0x03) << 26) |                \
       (((mul_d     ) & 0x03) << 28) |                \
       (((has_lut   ) ? 1 :0) << 30) |                \
       (((inv_alpha ) ? 1 :0) << 31) )

/*
 * Pixel format of a surface.
 */
typedef enum {
     /* unknown or unspecified format */
     DSPF_UNKNOWN    = 0x00000000,

     /* 16 bit  ARGB (2 byte, alpha 1@15, red 5@10, green 5@5, blue 5@0) */
     DSPF_ARGB1555   = DFB_SURFACE_PIXELFORMAT(  0, 15, 1, 1, 0, 2, 0, 0, 0, 0, 0 ),

     /* 16 bit   RGB (2 byte, red 5@11, green 6@5, blue 5@0) */
     DSPF_RGB16      = DFB_SURFACE_PIXELFORMAT(  1, 16, 0, 0, 0, 2, 0, 0, 0, 0, 0 ),

     /* 24 bit   RGB (3 byte, red 8@16, green 8@8, blue 8@0) */
     DSPF_RGB24      = DFB_SURFACE_PIXELFORMAT(  2, 24, 0, 0, 0, 3, 0, 0, 0, 0, 0 ),

     /* 24 bit   RGB (4 byte, nothing@24, red 8@16, green 8@8, blue 8@0) */
     DSPF_RGB32      = DFB_SURFACE_PIXELFORMAT(  3, 24, 0, 0, 0, 4, 0, 0, 0, 0, 0 ),

     /* 32 bit  ARGB (4 byte, alpha 8@24, red 8@16, green 8@8, blue 8@0) */
     DSPF_ARGB       = DFB_SURFACE_PIXELFORMAT(  4, 24, 8, 1, 0, 4, 0, 0, 0, 0, 0 ),

     /*  8 bit alpha (1 byte, alpha 8@0), e.g. anti-aliased glyphs */
     DSPF_A8         = DFB_SURFACE_PIXELFORMAT(  5,  0, 8, 1, 0, 1, 0, 0, 0, 0, 0 ),

     /* 16 bit   YUV (4 byte/ 2 pixel, macropixel contains CbYCrY [31:0]) */
     DSPF_YUY2       = DFB_SURFACE_PIXELFORMAT(  6, 16, 0, 0, 0, 2, 0, 0, 0, 0, 0 ),

     /*  8 bit   RGB (1 byte, red 3@5, green 3@2, blue 2@0) */
     DSPF_RGB332     = DFB_SURFACE_PIXELFORMAT(  7,  8, 0, 0, 0, 1, 0, 0, 0, 0, 0 ),

     /* 16 bit   YUV (4 byte/ 2 pixel, macropixel contains YCbYCr [31:0]) */
     DSPF_UYVY       = DFB_SURFACE_PIXELFORMAT(  8, 16, 0, 0, 0, 2, 0, 0, 0, 0, 0 ),

     /* 12 bit   YUV (8 bit Y plane followed by quarter-size 8 bit U/V planes) */
     DSPF_I420       = DFB_SURFACE_PIXELFORMAT(  9, 12, 0, 0, 0, 1, 0, 2, 0, 0, 0 ),

     /* 12 bit   YUV (8 bit Y plane followed by quarter-size 8 bit V/U planes) */
     DSPF_YV12       = DFB_SURFACE_PIXELFORMAT( 10, 12, 0, 0, 0, 1, 0, 2, 0, 0, 0 ),

     /*  8 bit   LUT (8 bit color and alpha lookup from palette) */
     DSPF_LUT8       = DFB_SURFACE_PIXELFORMAT( 11,  8, 0, 1, 0, 1, 0, 0, 0, 1, 0 ),

     /*  8 bit  ALUT (1 byte, alpha 4@4, color lookup 4@0) */
     DSPF_ALUT44     = DFB_SURFACE_PIXELFORMAT( 12,  4, 4, 1, 0, 1, 0, 0, 0, 1, 0 ),

     /* 32 bit  ARGB (4 byte, inv. alpha 8@24, red 8@16, green 8@8, blue 8@0) */
     DSPF_AiRGB      = DFB_SURFACE_PIXELFORMAT( 13, 24, 8, 1, 0, 4, 0, 0, 0, 0, 1 ),

     /*  1 bit alpha (1 byte/ 8 pixel, most significant bit used first) */
     DSPF_A1         = DFB_SURFACE_PIXELFORMAT( 14,  0, 1, 1, 1, 0, 7, 0, 0, 0, 0 ),

     /* 12 bit   YUV (8 bit Y plane followed by quarter-size 16 bit Cb|Cr [7:0|7:0] plane) */
     DSPF_NV12       = DFB_SURFACE_PIXELFORMAT( 15, 12, 0, 0, 0, 1, 0, 2, 0, 0, 0 ),

     /* 16 bit   YUV (8 bit Y plane followed by half-size 16 bit Cb|Cr [7:0|7:0] plane) */
     DSPF_NV16       = DFB_SURFACE_PIXELFORMAT( 16, 16, 0, 0, 0, 1, 0, 0, 1, 0, 0 ),

     /* 16 bit  ARGB (2 byte, alpha 2@14, red 5@9, green 5@4, blue 4@0) */
     DSPF_ARGB2554   = DFB_SURFACE_PIXELFORMAT( 17, 14, 2, 1, 0, 2, 0, 0, 0, 0, 0 ),

     /* 16 bit  ARGB (2 byte, alpha 4@12, red 4@8, green 4@4, blue 4@0) */
     DSPF_ARGB4444   = DFB_SURFACE_PIXELFORMAT( 18, 12, 4, 1, 0, 2, 0, 0, 0, 0, 0 ),

     /* 16 bit  RGBA (2 byte, red 4@12, green 4@8, blue 4@4, alpha 4@0) */
     DSPF_RGBA4444   = DFB_SURFACE_PIXELFORMAT( 19, 12, 4, 1, 0, 2, 0, 0, 0, 0, 0 ),

     /* 12 bit   YUV (8 bit Y plane followed by quarter-size 16 bit Cr|Cb [7:0|7:0] plane) */
     DSPF_NV21       = DFB_SURFACE_PIXELFORMAT( 20, 12, 0, 0, 0, 1, 0, 2, 0, 0, 0 ),

     /* 32 bit  AYUV (4 byte, alpha 8@24, Y 8@16, Cb 8@8, Cr 8@0) */
     DSPF_AYUV       = DFB_SURFACE_PIXELFORMAT( 21, 24, 8, 1, 0, 4, 0, 0, 0, 0, 0 ),

     /*  4 bit alpha (1 byte/ 2 pixel, more significant nibble used first) */
     DSPF_A4         = DFB_SURFACE_PIXELFORMAT( 22,  0, 4, 1, 4, 0, 1, 0, 0, 0, 0 ),

     /*  1 bit alpha (3 byte, alpha 1@18, red 6@12, green 6@6, blue 6@0) */
     DSPF_ARGB1666   = DFB_SURFACE_PIXELFORMAT( 23, 18, 1, 1, 0, 3, 0, 0, 0, 0, 0 ),

     /*  6 bit alpha (3 byte, alpha 6@18, red 6@12, green 6@6, blue 6@0) */
     DSPF_ARGB6666   = DFB_SURFACE_PIXELFORMAT( 24, 18, 6, 1, 0, 3, 0, 0, 0, 0, 0 ),

     /* 24 bit   RGB (3 byte, nothing @18, red 6@12, green 6@6, blue 6@0) */
     DSPF_RGB18      = DFB_SURFACE_PIXELFORMAT( 25, 18, 0, 0, 0, 3, 0, 0, 0, 0, 0 ),

     /*  2 bit   LUT (1 byte/ 4 pixel, 2 bit color and alpha lookup from palette) */
     DSPF_LUT2       = DFB_SURFACE_PIXELFORMAT( 26,  2, 0, 1, 2, 0, 3, 0, 0, 1, 0 ),

     /* 16 bit   RGB (2 byte, nothing @12, red 4@8, green 4@4, blue 4@0) */
     DSPF_RGB444     = DFB_SURFACE_PIXELFORMAT( 27, 12, 0, 0, 0, 2, 0, 0, 0, 0, 0 ),

     /* 16 bit   RGB (2 byte, nothing @15, red 5@10, green 5@5, blue 5@0) */
     DSPF_RGB555     = DFB_SURFACE_PIXELFORMAT( 28, 15, 0, 0, 0, 2, 0, 0, 0, 0, 0 ),

     /* 16 bit   BGR (2 byte, nothing @15, blue 5@10, green 5@5, red 5@0) */
     DSPF_BGR555     = DFB_SURFACE_PIXELFORMAT( 29, 15, 0, 0, 0, 2, 0, 0, 0, 0, 0 ),

     /* 16 bit  RGBA (2 byte, red 5@11, green 5@6, blue 5@1, alpha 1@0) */
     DSPF_RGBA5551   = DFB_SURFACE_PIXELFORMAT( 30, 15, 1, 1, 0, 2, 0, 0, 0, 0, 0 ),

     /* 24 bit   YUV (8 bit Y plane followed by 8 bit U/V planes) */
     DSPF_Y444       = DFB_SURFACE_PIXELFORMAT( 31, 24, 0, 0, 0, 1, 0, 0, 2, 0, 0 ),

     /* 24 bit  ARGB (3 byte, alpha 8@16, red 5@11, green 6@5, blue 5@0) */
     DSPF_ARGB8565   = DFB_SURFACE_PIXELFORMAT( 32, 16, 8, 1, 0, 3, 0, 0, 0, 0, 0 ),

     /* 32 bit  AVYU (4 byte, alpha 8@24, Cr 8@16, Y 8@8, Cb 8@0) */
     DSPF_AVYU       = DFB_SURFACE_PIXELFORMAT( 33, 24, 8, 1, 0, 4, 0, 0, 0, 0, 0 ),

     /* 24 bit   VYU (3 byte, Cr 8@16, Y 8@8, Cb 8@0) */
     DSPF_VYU        = DFB_SURFACE_PIXELFORMAT( 34, 24, 0, 0, 0, 3, 0, 0, 0, 0, 0 ),

     /*  1 bit alpha (1 byte/ 8 pixel, least significant bit used first) */
     DSPF_A1_LSB     = DFB_SURFACE_PIXELFORMAT( 35,  0, 1, 1, 1, 0, 7, 0, 0, 0, 0 ),

     /* 16 bit   YUV (8 bit Y plane followed by half-size 8 bit V/U planes) */
     DSPF_YV16       = DFB_SURFACE_PIXELFORMAT( 36, 16, 0, 0, 0, 1, 0, 0, 1, 0, 0 ),

     /* 32 bit  ABGR (4 byte, alpha 8@24, blue 8@16, green 8@8, red 8@0) */
     DSPF_ABGR       = DFB_SURFACE_PIXELFORMAT( 37, 24, 8, 1, 0, 4, 0, 0, 0, 0, 0 ),

     /* 32 bit RGBAF (4 byte, red 8@24, green 8@16, blue 8@8, alpha 7@1, flash 1@0 */
     DSPF_RGBAF88871 = DFB_SURFACE_PIXELFORMAT( 38, 24, 7, 1, 0, 4, 0, 0, 0, 0, 0 ),

     /*  4 bit   LUT (1 byte/ 2 pixel, 4 bit color and alpha lookup from palette) */
     DSPF_LUT4       = DFB_SURFACE_PIXELFORMAT( 39,  4, 0, 1, 4, 0, 1, 0, 0, 1, 0 ),

     /* 16 bit   LUT (1 byte alpha and 8 bit color lookup from palette) */
     DSPF_ALUT8      = DFB_SURFACE_PIXELFORMAT( 40,  8, 8, 1, 0, 2, 0, 0, 0, 1, 0 ),

     /*  1 bit   LUT (1 byte/ 8 pixel, 1 bit color and alpha lookup from palette) */
     DSPF_LUT1       = DFB_SURFACE_PIXELFORMAT( 41,  1, 0, 1, 1, 0, 7, 0, 0, 1, 0 ),

     /* 16 bit   YUV (8 bit Y plane followed by half-size 16 bit Cr|Cb [7:0|7:0] plane) */
     DSPF_NV61       = DFB_SURFACE_PIXELFORMAT( 42, 16, 0, 0, 0, 1, 0, 0, 1, 0, 0 ),

     /* 16 bit   YUV (8 bit Y plane followed by half-size 8 bit U/V planes) */
     DSPF_Y42B       = DFB_SURFACE_PIXELFORMAT( 43, 16, 0, 0, 0, 1, 0, 0, 1, 0, 0 ),

     /* 24 bit   YUV (8 bit Y plane followed by 8 bit V/U planes) */
     DSPF_YV24       = DFB_SURFACE_PIXELFORMAT( 44, 24, 0, 0, 0, 1, 0, 0, 2, 0, 0 )
} DFBSurfacePixelFormat;

#define DSPF_YUV420P                         DSPF_I420
#define DSPF_YUV422P                         DSPF_Y42B
#define DSPF_YUV444P                         DSPF_Y444

/* Number of pixelformats defined. */
#define DFB_NUM_PIXELFORMATS                 45

/* These macros extract information about the pixel format. */
#define DFB_PIXELFORMAT_INDEX(fmt)           (((fmt) & 0x0000007F)      )

#define DFB_COLOR_BITS_PER_PIXEL(fmt)        (((fmt) & 0x00000F80) >>  7)

#define DFB_ALPHA_BITS_PER_PIXEL(fmt)        (((fmt) & 0x0000F000) >> 12)

#define DFB_PIXELFORMAT_HAS_ALPHA(fmt)       (((fmt) & 0x00010000) !=  0)

#define DFB_BITS_PER_PIXEL(fmt)              (((fmt) & 0x007E0000) >> 17)

#define DFB_BYTES_PER_PIXEL(fmt)             (((fmt) & 0x00700000) >> 20)

#define DFB_BYTES_PER_LINE(fmt,width)        (((((fmt) & 0x007E0000) >> 17) * (width) + 7) >> 3)

#define DFB_PIXELFORMAT_ALIGNMENT(fmt)       (((fmt) & 0x03800000) >> 23)

#define DFB_PLANE_MULTIPLY(fmt,height)       ((((((fmt) & 0x3C000000) >> 26) + 4) * (height)) >> 2)

#define DFB_PIXELFORMAT_IS_INDEXED(fmt)      (((fmt) & 0x40000000) !=  0)

#define DFB_PLANAR_PIXELFORMAT(fmt)          (((fmt) & 0x3C000000) !=  0)

#define DFB_PIXELFORMAT_INV_ALPHA(fmt)       (((fmt) & 0x80000000) !=  0)

#define DFB_COLOR_IS_RGB(fmt)        \
     (((fmt) == DSPF_ARGB1555)    || \
      ((fmt) == DSPF_RGB16)       || \
      ((fmt) == DSPF_RGB24)       || \
      ((fmt) == DSPF_RGB32)       || \
      ((fmt) == DSPF_ARGB)        || \
      ((fmt) == DSPF_RGB332)      || \
      ((fmt) == DSPF_AiRGB)       || \
      ((fmt) == DSPF_ARGB2554)    || \
      ((fmt) == DSPF_ARGB4444)    || \
      ((fmt) == DSPF_RGBA4444)    || \
      ((fmt) == DSPF_ARGB1666)    || \
      ((fmt) == DSPF_ARGB6666)    || \
      ((fmt) == DSPF_RGB18)       || \
      ((fmt) == DSPF_RGB444)      || \
      ((fmt) == DSPF_RGB555)      || \
      ((fmt) == DSPF_BGR555)      || \
      ((fmt) == DSPF_ARGB8565)    || \
      ((fmt) == DSPF_ABGR)        || \
      ((fmt) == DSPF_RGBAF88871))

#define DFB_COLOR_IS_YUV(fmt)        \
     (((fmt) == DSPF_YUY2)        || \
      ((fmt) == DSPF_UYVY)        || \
      ((fmt) == DSPF_I420)        || \
      ((fmt) == DSPF_YV12)        || \
      ((fmt) == DSPF_NV12)        || \
      ((fmt) == DSPF_NV16)        || \
      ((fmt) == DSPF_NV21)        || \
      ((fmt) == DSPF_AYUV)        || \
      ((fmt) == DSPF_Y444)        || \
      ((fmt) == DSPF_AVYU)        || \
      ((fmt) == DSPF_VYU)         || \
      ((fmt) == DSPF_YV16)        || \
      ((fmt) == DSPF_NV61)        || \
      ((fmt) == DSPF_Y42B)        || \
      ((fmt) == DSPF_YV24))

/*
 * Hint flags for optimized allocation, format selection etc.
 */
typedef enum {
     DSHF_NONE                             = 0x00000000,         /* None of these. */

     DSHF_LAYER                            = 0x00000001,         /* Surface optimized for display layer usage. */
     DSHF_WINDOW                           = 0x00000002,         /* Surface optimized for being a window buffer. */
     DSHF_CURSOR                           = 0x00000004,         /* Surface optimized for usage as a cursor shape. */
     DSHF_FONT                             = 0x00000008,         /* Surface optimized for text rendering. */

     DSHF_ALL                              = 0x0000000F          /* All of these. */
} DFBSurfaceHintFlags;

/*
 * Color space used by the colors in the surface.
 */
typedef enum {
     DSCS_UNKNOWN                          = 0x00000000,         /* unknown color space */

     DSCS_RGB                              = 0x00000001,         /* standard RGB */
     DSCS_BT601                            = 0x00000002,         /* ITU BT.601 */

     DSCS_BT709                            = 0x00000004,         /* ITU BT.709 */
     DSCS_BT2020                           = 0x00000005          /* ITU BT.2020 */
} DFBSurfaceColorSpace;

#define DFB_COLORSPACE_IS_COMPATIBLE(cs,fmt)               \
     ((DFB_COLOR_IS_RGB((fmt)) &&  ((cs) == DSCS_RGB))  || \
      (DFB_COLOR_IS_YUV((fmt)) && (((cs) == DSCS_BT601) || \
                                   ((cs) == DSCS_BT709) || \
                                   ((cs) == DSCS_BT2020))))

#define DFB_COLORSPACE_DEFAULT(fmt) \
     (DFB_COLOR_IS_RGB((fmt)) ? DSCS_RGB : DFB_COLOR_IS_YUV((fmt)) ? DSCS_BT601 : DSCS_UNKNOWN)

/*
 * Description of the surface that is to be created.
 */
typedef struct {
     DFBSurfaceDescriptionFlags              flags;              /* field validation */

     DFBSurfaceCapabilities                  caps;               /* capabilities */
     int                                     width;              /* pixel width */
     int                                     height;             /* pixel height */
     DFBSurfacePixelFormat                   pixelformat;        /* pixel format */
     struct {
          void                              *data;               /* data pointer of existing buffer */
          int                                pitch;              /* pitch of buffer */
     } preallocated[3];                                          /* preallocated data */
     struct {
          const DFBColor                    *entries;            /* palette entries */
          unsigned int                       size;               /* number of entries */
     } palette;                                                  /* initial palette */
     unsigned long                           resource_id;        /* universal resource id, either user specified for
                                                                    general purpose surfaces or id of layer or window */
     DFBSurfaceHintFlags                     hints;              /* usage hints for optimized allocation,
                                                                    format selection, etc */
     DFBSurfaceColorSpace                    colorspace;         /* color space */
} DFBSurfaceDescription;

/*
 * Flags defining which fields of a DFBPaletteDescription are
 * valid.
 */
typedef enum {
     DPDESC_CAPS                           = 0x00000001,         /* Specify palette capabilities. */
     DPDESC_SIZE                           = 0x00000002,         /* Specify number of entries. */
     DPDESC_ENTRIES                        = 0x00000004          /* Initialize the palette with the
                                                                    entries specified in the description. */
} DFBPaletteDescriptionFlags;

/*
 * The palette capabilities.
 */
typedef enum {
     DPCAPS_NONE                           = 0x00000000          /* None of these. */
} DFBPaletteCapabilities;

/*
 * Description of the palette that is to be created.
 */
typedef struct {
     DFBPaletteDescriptionFlags              flags;              /* Validation of fields. */

     DFBPaletteCapabilities                  caps;               /* Palette capabilities. */
     unsigned int                            size;               /* Number of entries. */
     const DFBColor                         *entries;            /* Preset palette entries. */
} DFBPaletteDescription;

/*
 * Capabilities of a screen.
 */
typedef enum {
     DSCCAPS_NONE                          = 0x00000000,         /* None of these. */

     DSCCAPS_VSYNC                         = 0x00000001,         /* Synchronization with the
                                                                    vertical retrace supported. */
     DSCCAPS_POWER_MANAGEMENT              = 0x00000002,         /* Power management supported. */

     DSCCAPS_MIXERS                        = 0x00000010,         /* Has mixers. */
     DSCCAPS_ENCODERS                      = 0x00000020,         /* Has encoders. */
     DSCCAPS_OUTPUTS                       = 0x00000040,         /* Has outputs. */

     DSCCAPS_ALL                           = 0x00000073          /* All of these. */
} DFBScreenCapabilities;

#define DFB_SCREEN_DESC_NAME_LENGTH          32

/*
 * Description of the display encoder capabilities.
 */
typedef struct {
     DFBScreenCapabilities                   caps;               /* Capability flags of the screen. */
     char name[DFB_SCREEN_DESC_NAME_LENGTH];                     /* Rough description. */
     int                                     mixers;             /* Number of mixers available. */
     int                                     encoders;           /* Number of display encoders available. */
     int                                     outputs;            /* Number of output connectors available. */
} DFBScreenDescription;

/*
 * Type of display layer for basic classification.
 */
typedef enum {
     DLTF_NONE                             = 0x00000000,         /* Unclassified, no specific type. */

     DLTF_GRAPHICS                         = 0x00000001,         /* Can be used for graphics output. */
     DLTF_VIDEO                            = 0x00000002,         /* Can be used for live video output.*/
     DLTF_STILL_PICTURE                    = 0x00000004,         /* Can be used for single frames. */
     DLTF_BACKGROUND                       = 0x00000008,         /* Can be used as a background layer.*/

     DLTF_ALL                              = 0x0000000F          /* All type flags set. */
} DFBDisplayLayerTypeFlags;

/*
 * Capabilities of a display layer.
 */
typedef enum {
     DLCAPS_NONE                           = 0x00000000,         /* None of these. */

     DLCAPS_SURFACE                        = 0x00000001,         /* The layer has a surface that can be drawn to. This
                                                                    may not be provided by layers that display realtime
                                                                    data, e.g. from an MPEG decoder chip. Playback
                                                                    control may be provided by an external API. */
     DLCAPS_OPACITY                        = 0x00000002,         /* The layer supports blending with layer(s) below
                                                                    based on a global alpha factor. */
     DLCAPS_ALPHACHANNEL                   = 0x00000004,         /* The layer supports blending with layer(s) below
                                                                    based on each pixel's alpha value. */
     DLCAPS_SCREEN_LOCATION                = 0x00000008,         /* The layer location on the screen can be changed,
                                                                    this includes position and size as normalized
                                                                    values.
                                                                    This implies DLCAPS_SCREEN_POSITION and _SIZE. */
     DLCAPS_FLICKER_FILTERING              = 0x00000010,         /* Flicker filtering can be enabled for smooth output
                                                                    on interlaced display devices. */
     DLCAPS_DEINTERLACING                  = 0x00000020,         /* The layer provides optional deinterlacing for
                                                                    displaying interlaced video data on progressive
                                                                    display devices. */
     DLCAPS_SRC_COLORKEY                   = 0x00000040,         /* A specific color can be declared as transparent. */
     DLCAPS_DST_COLORKEY                   = 0x00000080,         /* A specific color of layers below can be specified
                                                                    as the color of the only locations where the layer
                                                                    is visible. */
     DLCAPS_BRIGHTNESS                     = 0x00000100,         /* Adjustment of brightness is supported. */
     DLCAPS_CONTRAST                       = 0x00000200,         /* Adjustment of contrast is supported. */
     DLCAPS_HUE                            = 0x00000400,         /* Adjustment of hue is supported. */
     DLCAPS_SATURATION                     = 0x00000800,         /* Adjustment of saturation is supported. */
     DLCAPS_LEVELS                         = 0x00001000,         /* Adjustment of the layer's level
                                                                    (z position) is supported. */
     DLCAPS_FIELD_PARITY                   = 0x00002000,         /* Field parity can be selected */
     DLCAPS_WINDOWS                        = 0x00004000,         /* Hardware window support. */
     DLCAPS_SOURCES                        = 0x00008000,         /* Sources can be selected. */
     DLCAPS_ALPHA_RAMP                     = 0x00010000,         /* Alpha values for formats with one or two alpha bits
                                                                    can be chosen, i.e. using ARGB1555 or ARGB2554 the
                                                                    user can define the meaning of the two or four
                                                                    possibilities. In short, this feature provides a
                                                                    lookup table for the alpha bits of these formats. */
     DLCAPS_PREMULTIPLIED                  = 0x00020000,         /* Surfaces with premultiplied alpha are supported. */

     DLCAPS_SCREEN_POSITION                = 0x00100000,         /* The layer position on the screen can be changed. */
     DLCAPS_SCREEN_SIZE                    = 0x00200000,         /* The layer size (defined by its source rectangle)
                                                                    can be scaled to a different size on the screen
                                                                    (defined by its screen/destination rectangle or
                                                                    its normalized size) and does not have to be 1:1
                                                                    with it. */
     DLCAPS_CLIP_REGIONS                   = 0x00400000,         /* Supports SetClipRegions(). */

     DLCAPS_LR_MONO                        = 0x01000000,         /* Supports L/R mono stereoscopic display. */
     DLCAPS_STEREO                         = 0x02000000,         /* Supports independent L/R stereoscopic display. */

     DLCAPS_ALL                            = 0x0373FFFF          /* All of these. */
} DFBDisplayLayerCapabilities;

#define DFB_DISPLAY_LAYER_DESC_NAME_LENGTH   32

/*
 * Description of the display layer capabilities.
 */
typedef struct {
     DFBDisplayLayerTypeFlags                type;               /* Classification of the display layer. */
     DFBDisplayLayerCapabilities             caps;               /* Capability flags of the display layer. */
     char name[DFB_DISPLAY_LAYER_DESC_NAME_LENGTH];              /* Display layer name. */
     int                                     level;              /* Default level. */
     int                                     regions;            /* Number of concurrent regions supported:
                                                                      -1 = unlimited,
                                                                       0 = unknown/one,
                                                                      >0 = actual number */
     int                                     sources;            /* Number of selectable sources. */
     int                                     clip_regions;       /* Number of clipping regions. */
     DFBSurfaceCapabilities                  surface_caps;       /* Capabilities. */
     unsigned int                            surface_accessor;   /* Accessor. */
} DFBDisplayLayerDescription;

/*
 * Type of input device for basic classification.
 */
typedef enum {
     DIDTF_NONE                            = 0x00000000,         /* Unclassified, no specific type. */

     DIDTF_KEYBOARD                        = 0x00000001,         /* Can act as a keyboard. */
     DIDTF_MOUSE                           = 0x00000002,         /* Can be used as a mouse. */
     DIDTF_JOYSTICK                        = 0x00000004,         /* Can be used as a joystick. */
     DIDTF_REMOTE                          = 0x00000008,         /* Is a remote control. */
     DIDTF_VIRTUAL                         = 0x00000010,         /* Is a virtual input device. */

     DIDTF_ALL                             = 0x0000001F          /* All type flags set. */
} DFBInputDeviceTypeFlags;

/*
 * Basic input device features.
 */
typedef enum {
     DICAPS_KEYS                           = 0x00000001,         /* device supports key events */
     DICAPS_AXES                           = 0x00000002,         /* device supports axis events */
     DICAPS_BUTTONS                        = 0x00000004,         /* device supports button events */

     DICAPS_ALL                            = 0x00000007          /* all capabilities */
} DFBInputDeviceCapabilities;

/*
 * Axis identifier (index) for mouse or joystick.
 *
 * The X, Y and Z axis are predefined. To access other axes,
 * use DIAI_FIRST plus a zero based index, e.g. the 4th axis
 * would be (DIAI_FIRST + 3).
 */
typedef enum {
     DIAI_X                                = 0x00000000,         /* X axis */
     DIAI_Y                                = 0x00000001,         /* Y axis */
     DIAI_Z                                = 0x00000002,         /* Z axis */
     DIAI_FIRST                            = DIAI_X,             /* other axis:
                                                                    DIAI_FIRST + zero based index */
     DIAI_LAST                             = 0x0000001F          /* 32 axes maximum */
} DFBInputDeviceAxisIdentifier;

/*
 * Identifier (index) for e.g. mouse or joystick buttons.
 */
typedef enum {
     DIBI_LEFT                             = 0x00000000,         /* left mouse button */
     DIBI_RIGHT                            = 0x00000001,         /* right mouse button */
     DIBI_MIDDLE                           = 0x00000002,         /* middle mouse button */
     DIBI_FIRST                            = DIBI_LEFT,          /* other buttons:
                                                                    DIBI_FIRST + zero based index */
     DIBI_LAST                             = 0x0000001F          /* 32 buttons maximum */
} DFBInputDeviceButtonIdentifier;

#define DFB_INPUT_DEVICE_DESC_NAME_LENGTH    32
#define DFB_INPUT_DEVICE_DESC_VENDOR_LENGTH  40

/*
 * Description of the input device capabilities.
 */
typedef struct {
     DFBInputDeviceTypeFlags                 type;               /* classification of input device */
     DFBInputDeviceCapabilities              caps;               /* capabilities, validates the following fields */
     int                                     min_keycode;        /* minimum hardware keycode or -1 if no differentiation
                                                                    between hardware keys is made */
     int                                     max_keycode;        /* maximum hardware keycode or -1 if no differentiation
                                                                    between hardware keys is made */
     DFBInputDeviceAxisIdentifier            max_axis;           /* highest axis identifier */
     DFBInputDeviceButtonIdentifier          max_button;         /* highest button identifier */
     char name[DFB_INPUT_DEVICE_DESC_NAME_LENGTH];               /* device name */
     char vendor[DFB_INPUT_DEVICE_DESC_VENDOR_LENGTH];           /* device vendor */
     int                                     vendor_id;          /* vendor ID */
     int                                     product_id;         /* product ID */
} DFBInputDeviceDescription;

/*
 * Flags defining which fields of a DFBFontDescription are
 * valid.
 */
typedef enum {
     DFDESC_ATTRIBUTES                     = 0x00000001,         /* attributes field is valid */
     DFDESC_HEIGHT                         = 0x00000002,         /* height is specified */
     DFDESC_WIDTH                          = 0x00000004,         /* width is specified */
     DFDESC_INDEX                          = 0x00000008,         /* index is specified */
     DFDESC_FIXEDADVANCE                   = 0x00000010,         /* specify a fixed advance overriding any character
                                                                    advance of fixed or proportional fonts */
     DFDESC_FRACT_HEIGHT                   = 0x00000020,         /* fractional height is set */
     DFDESC_FRACT_WIDTH                    = 0x00000040,         /* fractional width is set */
     DFDESC_OUTLINE_WIDTH                  = 0x00000080,         /* outline width is set */
     DFDESC_OUTLINE_OPACITY                = 0x00000100,         /* outline opacity is set */
     DFDESC_ROTATION                       = 0x00000200          /* rotation is set */
} DFBFontDescriptionFlags;

/*
 * Flags describing how to load a font.
 *
 * These flags describe how a font is loaded and affect how the
 * glyphs are drawn. There is no way to change this after the
 * font has been loaded.
 * If you need to render a font with different attributes, you
 * have to create multiple FontProviders of the same font file.
 */
typedef enum {
     DFFA_NONE                             = 0x00000000,         /* none of these flags */

     DFFA_NOKERNING                        = 0x00000001,         /* don't use kerning */
     DFFA_NOHINTING                        = 0x00000002,         /* don't use hinting */
     DFFA_MONOCHROME                       = 0x00000004,         /* don't use anti-aliasing */
     DFFA_NOCHARMAP                        = 0x00000008,         /* no char map, glyph indices are specified directly */
     DFFA_FIXEDCLIP                        = 0x00000010,         /* width fixed advance, clip to it */
     DFFA_NOBITMAP                         = 0x00000020,         /* ignore bitmap strikes; for bitmap-only fonts this
                                                                    flag is ignored */
     DFFA_OUTLINED                         = 0x00000040,         /* outlined font */
     DFFA_AUTOHINTING                      = 0x00000080,         /* prefer auto-hinter over the font's native hinter */
     DFFA_SOFTHINTING                      = 0x00000100,         /* use a lighter hinting algorithm that produces glyphs
                                                                    that are more fuzzy but better resemble the original
                                                                    shape */
     DFFA_STYLE_ITALIC                     = 0x00000200,         /* load italic style */
     DFFA_VERTICAL_LAYOUT                  = 0x00000400,         /* load vertical layout */
     DFFA_STYLE_BOLD                       = 0x00000800          /* load bold style */
} DFBFontAttributes;

/*
 * Description of how to load glyphs from a font file.
 *
 * The attributes control how the glyphs are rendered. Width and
 * height can be used to specify the desired face size in
 * pixels. If you are loading a non-scalable font, you shouldn't
 * specify a font size.
 * Please note that the height value in the DFBFontDescription
 * doesn't correspond to the height returned by GetHeight().
 * The index field controls which face is loaded from a font
 * file that provides a collection of faces.
 * Fractional sizes (fract_height and fract_width) are 26.6
 * fixed point integers and override the pixel sizes if both are
 * specified.
 * Outline parameters are ignored if DFFA_OUTLINED is not used.
 * To change the default values of 1.0 each use
 * DFDESC_OUTLINE_WIDTH and/or DFDESC_OUTLINE_OPACITY.
 * The rotation value is a 0.24 fixed point number of rotations.
 * Use the macros DFB_DEGREES and DFB_RADIANS to convert from
 * those units.
 */
typedef struct {
     DFBFontDescriptionFlags                 flags;              /* field validation */

     DFBFontAttributes                       attributes;         /* font attributes */
     int                                     height;             /* font height */
     int                                     width;              /* font width */
     unsigned int                            index;              /* font index */
     int                                     fixed_advance;      /* fixed advance */
     int                                     fract_height;       /* fractional height */
     int                                     fract_width;        /* fractional width */
     int                                     outline_width;      /* outline width as 16.16 fixed point integer */
     int                                     outline_opacity;    /* outline opacity as 16.16 fixed point integer */
     int                                     rotation;           /* font rotation */
} DFBFontDescription;

/*
 * Flags defining which fields of a DFBDataBufferDescription are
 * valid.
 */
typedef enum {
     DBDESC_FILE                           = 0x00000001,         /* Create a static buffer for the
                                                                    specified filename. */
     DBDESC_MEMORY                         = 0x00000002          /* Create a static buffer for the
                                                                    specified memory area. */
} DFBDataBufferDescriptionFlags;

/*
 * Description of a data buffer that is to be created.
 */
typedef struct {
     DFBDataBufferDescriptionFlags           flags;              /* field validation */

     const char                             *file;               /* for file based data buffers */
     struct {
          const void                        *data;               /* static data pointer */
          unsigned int                       length;             /* length of buffer */
     } memory;                                                   /* memory based buffers */
} DFBDataBufferDescription;

/*
 * Called for each supported video mode.
 */
typedef DFBEnumerationResult (*DFBVideoModeCallback) (
     int                                     width,
     int                                     height,
     int                                     bpp,
     void                                   *callbackdata
);

/*
 * Called for each existing screen.
 * 'screen_id' can be used to get an interface to the screen.
 */
typedef DFBEnumerationResult (*DFBScreenCallback) (
     DFBScreenID                             screen_id,
     DFBScreenDescription                    desc,
     void                                   *callbackdata
);

/*
 * Called for each existing display layer.
 * 'layer_id' can be used to get an interface to the layer.
 */
typedef DFBEnumerationResult (*DFBDisplayLayerCallback) (
     DFBDisplayLayerID                       layer_id,
     DFBDisplayLayerDescription              desc,
     void                                   *callbackdata
);

/*
 * Called for each existing input device.
 * 'device_id' can be used to get an interface to the device.
 */
typedef DFBEnumerationResult (*DFBInputDeviceCallback) (
     DFBInputDeviceID                        device_id,
     DFBInputDeviceDescription               desc,
     void                                   *callbackdata
);

/*
 * IDirectFB is the main interface. It can be retrieved by a
 * call to DirectFBCreate(). It's the only interface with a
 * global creation facility. Other interfaces are created by
 * this interface or interfaces created by it.
 *
 * Hardware capabilities such as the amount of video memory or
 * a list of supported drawing/blitting functions and flags can
 * be retrieved. It also provides enumeration of all supported
 * video modes.
 *
 * Input devices and display layers> that are present can be
 * enumerated via a callback mechanism.The callback is given the
 * capabilities and the device or layer ID.
 * An interface to specific input devices or display layers
 * can be retrieved by passing the device or layer ID to the
 * corresponding method.
 *
 * Surfaces for general purpose use can be created via
 * CreateSurface(). These surfaces are so called "offscreen
 * surfaces" and could be used for sprites or icons.
 *
 * The primary surface is an abstraction and API shortcut for
 * getting a surface for visual output. Fullscreen games for
 * example have the whole screen as their primary surface.
 * Alternatively fullscreen applications can be forced to run
 * in a window. The primary surface is also created via
 * CreateSurface() but with the special capability
 * DSCAPS_PRIMARY.
 *
 * The cooperative level selects the type of the primary
 * surface. With a call to SetCooperativeLevel() the
 * application can choose between the surface of an implicitly
 * created window and the surface of the primary layer
 * (deactivating the window stack). The application doesn't need
 * to have any extra functionality to run in a window. If the
 * application is forced to run in a window the call to
 * SetCooperativeLevel() fails with DFB_ACCESSDENIED.
 * Applications that want to be "window aware" shouldn't exit on
 * this error.
 *
 * The video mode can be changed via SetVideoMode() and is the
 * size and depth of the primary surface, i.e. the * screen when
 * in exclusive cooperative level.
 * Without exclusive access SetVideoMode() sets the size of the
 * implicitly created window.
 *
 * Event buffers can be created with an option to automatically
 * attach input devices matching the specified capabilities.
 * If DICAPS_NONE is passed, an event buffer with nothing
 * attached to is created. An event buffer can be attached to
 * input devices and windows.
 *
 * Fonts, images and videos are created by this interface. There
 * are different implementations for different content types. On
 * creation a suitable implementation is automatically chosen.
 */
D_DEFINE_INTERFACE( IDirectFB,

   /** Cooperative level, video mode **/

     /*
      * Put the interface into the specified cooperative level.
      *
      * Function fails with DFB_LOCKED if another instance is
      * already in a cooperative level other than DFSCL_NORMAL.
      */
     DFBResult (*SetCooperativeLevel) (
          IDirectFB                         *thiz,
          DFBCooperativeLevel                level
     );

     /*
      * Switch the current video mode (primary layer).
      *
      * If in shared cooperative level this function sets the
      * resolution of the window that is created implicitly for
      * the primary surface.
      * The following values are valid for bpp:
      *   2, 8, 12, 14, 15, 18, 24, 32.
      * These will result in the following formats, respectively
      * DSPF_LUT2, DSPF_LUT8, DSPF_ARGB4444, DSPF_ARGB2554,
      * DSPF_ARGB1555, DSPF_RGB16, DSPF_RGB18, DSPF_RGB24,
      * DSPF_RGB32.
      */
     DFBResult (*SetVideoMode) (
          IDirectFB                         *thiz,
          int                                width,
          int                                height,
          int                                bpp
     );

   /** Hardware capabilities **/

     /*
      * Get a description of the graphics device.
      *
      * For more detailed information use GetAccelerationMask().
      */
     DFBResult (*GetDeviceDescription) (
          IDirectFB                         *thiz,
          DFBGraphicsDeviceDescription      *ret_desc
     );

     /*
      * Enumerate supported video modes.
      *
      * Calls the given callback for all available video modes.
      * Useful to select a certain mode to be used with
      * SetVideoMode().
      */
     DFBResult (*EnumVideoModes) (
          IDirectFB                         *thiz,
          DFBVideoModeCallback               callback,
          void                              *callbackdata
     );

   /** Surfaces & Palettes **/

     /*
      * Create a surface matching the specified description.
      */
     DFBResult (*CreateSurface) (
          IDirectFB                         *thiz,
          const DFBSurfaceDescription       *desc,
          IDirectFBSurface                 **ret_interface
     );

     /*
      * Create a palette matching the specified description.
      *
      * Passing a NULL description creates a default palette
      * with 256 entries filled with colors matching the
      * RGB332 format.
      */
     DFBResult (*CreatePalette) (
          IDirectFB                         *thiz,
          const DFBPaletteDescription       *desc,
          IDirectFBPalette                 **ret_interface
     );

   /** Screens **/

     /*
      * Enumerate all existing screen.
      *
      * Calls the given callback for each available screen.
      * The callback is passed the screen id that can be used
      * to retrieve an interface to a specific screen using
      * GetScreen().
      */
     DFBResult (*EnumScreens) (
          IDirectFB                         *thiz,
          DFBScreenCallback                  callback,
          void                              *callbackdata
     );

     /*
      * Retrieve an interface to a specific screen.
      */
     DFBResult (*GetScreen) (
          IDirectFB                         *thiz,
          DFBScreenID                        screen_id,
          IDirectFBScreen                  **ret_interface
     );

   /** Display Layers **/

     /*
      * Enumerate all existing display layers.
      *
      * Calls the given callback for each available display
      * layer. The callback is passed the layer id that can be
      * used to retrieve an interface to a specific layer using
      * GetDisplayLayer().
      */
     DFBResult (*EnumDisplayLayers) (
          IDirectFB                         *thiz,
          DFBDisplayLayerCallback            callback,
          void                              *callbackdata
     );

     /*
      * Retrieve an interface to a specific display layer.
      *
      * The default 'layer_id' is DLID_PRIMARY.
      * Others can be obtained using EnumDisplayLayers().
      */
     DFBResult (*GetDisplayLayer) (
          IDirectFB                         *thiz,
          DFBDisplayLayerID                  layer_id,
          IDirectFBDisplayLayer            **ret_interface
     );

   /** Input Devices **/

     /*
      * Enumerate all existing input devices.
      *
      * Calls the given callback for all available input
      * devices.
      * The callback is passed the device id that can be used to
      * retrieve an interface on a specific device using
      * GetInputDevice().
      */
     DFBResult (*EnumInputDevices) (
          IDirectFB                         *thiz,
          DFBInputDeviceCallback             callback,
          void                              *callbackdata
     );

     /*
      * Retrieve an interface to a specific input device.
      */
     DFBResult (*GetInputDevice) (
          IDirectFB                         *thiz,
          DFBInputDeviceID                   device_id,
          IDirectFBInputDevice             **ret_interface
     );

     /*
      * Create a buffer for events.
      *
      * Creates an empty event buffer without event sources
      * connected to it.
      */
     DFBResult (*CreateEventBuffer) (
          IDirectFB                         *thiz,
          IDirectFBEventBuffer             **ret_interface
     );

     /*
      * Create a buffer for events with input devices connected.
      *
      * Creates an event buffer and attaches all input devices
      * with matching capabilities. If no input devices match,
      * e.g. by specifying DICAPS_NONE, a buffer will be
      * returned that has no event sources connected to it.
      * If 'global' is DFB_FALSE, events will only be delivered
      * if this instance of IDirectFB has a focused primary
      * (either running fullscreen or running in windowed mode
      * with the window being focused).
      * If 'global' is DFB_TRUE no event will be discarded.
      */
     DFBResult (*CreateInputEventBuffer) (
          IDirectFB                         *thiz,
          DFBInputDeviceCapabilities         caps,
          DFBBoolean                         global,
          IDirectFBEventBuffer             **ret_interface
     );

   /** Media **/

     /*
      * Create an image provider for the specified file.
      */
     DFBResult (*CreateImageProvider) (
          IDirectFB                         *thiz,
          const char                        *filename,
          IDirectFBImageProvider           **ret_interface
     );

     /*
      * Create a video provider.
      */
     DFBResult (*CreateVideoProvider) (
          IDirectFB                         *thiz,
          const char                        *filename,
          IDirectFBVideoProvider           **ret_interface
     );

     /*
      * Load a font from the specified file given a description
      * of how to load the glyphs.
      */
     DFBResult (*CreateFont) (
          IDirectFB                         *thiz,
          const char                        *filename,
          const DFBFontDescription          *desc,
          IDirectFBFont                    **ret_interface
     );

     /*
      * Create a data buffer.
      *
      * If no description is specified (NULL), a streamed
      * data buffer is created.
      */
     DFBResult (*CreateDataBuffer) (
          IDirectFB                         *thiz,
          const DFBDataBufferDescription    *desc,
          IDirectFBDataBuffer              **ret_interface
     );

   /** Clipboard **/

     /*
      * Set clipboard content.
      */
     DFBResult (*SetClipboardData) (
          IDirectFB                         *thiz,
          const char                        *mime_type,
          const void                        *clip_data,
          unsigned int                       size,
          struct timeval                    *timestamp
     );

     /*
      * Get clipboard content.
      *
      * Memory returned in '*ret_mimetype' and '*ret_clip_data'
      * has to be freed.
      */
     DFBResult (*GetClipboardData) (
          IDirectFB                         *thiz,
          char                             **ret_mimetype,
          void                             **ret_clip_data,
          unsigned int                      *ret_size
     );

     /*
      * Get time stamp of last SetClipboardData() call.
      */
     DFBResult (*GetClipboardTimeStamp) (
          IDirectFB                         *thiz,
          struct timeval                    *ret_timestamp
     );

   /** Misc **/

     /*
      * Suspend, no other calls are allowed until Resume()
      * has been called.
      */
     DFBResult (*Suspend) (
          IDirectFB                         *thiz
     );

     /*
      * Resume, only to be called after Suspend().
      */
     DFBResult (*Resume) (
          IDirectFB                         *thiz
     );

     /*
      * Wait until graphics card is idle,
      * i.e. finish all drawing/blitting functions.
      */
     DFBResult (*WaitIdle) (
          IDirectFB                         *thiz
     );

     /*
      * Wait for next vertical retrace.
      */
     DFBResult (*WaitForSync) (
          IDirectFB                         *thiz
     );

   /** Extensions **/

     /*
      * Load an implementation of a specific interface type.
      *
      * This methods loads an interface implementation of the
      * specified 'type' of interface, e.g. "IDirectFBWindows".
      * A specific implementation can be forced with the
      * optional 'implementation' argument.
      * Implementations are passed 'arg' during probing and
      * construction.
      * If an implementation has been successfully probed and
      * the interface has been constructed, the resulting
      * interface pointer is stored in 'interface'.
      */
     DFBResult (*GetInterface) (
          IDirectFB                         *thiz,
          const char                        *type,
          const char                        *implementation,
          void                              *arg,
          void                             **ret_interface
     );

   /** Surfaces **/

     /*
      * Get a surface by ID.
      */
     DFBResult (*GetSurface) (
          IDirectFB                         *thiz,
          DFBSurfaceID                       surface_id,
          IDirectFBSurface                 **ret_interface
     );
)

/*******************
 * IDirectFBScreen *
 *******************/

/*
 * Screen Power Mode.
 */
typedef enum {
     DSPM_ON                               = 0x00000000,         /* On. */
     DSPM_STANDBY                          = 0x00000001,         /* Standby. */
     DSPM_SUSPEND                          = 0x00000002,         /* Suspend. */
     DSPM_OFF                              = 0x00000003          /* Off. */
} DFBScreenPowerMode;

/*
 * Capabilities of a mixer.
 */
typedef enum {
     DSMCAPS_NONE                          = 0x00000000,         /* None of these. */

     DSMCAPS_FULL                          = 0x00000001,         /* Can mix full tree as specified in the
                                                                    description. */
     DSMCAPS_SUB_LEVEL                     = 0x00000002,         /* Can set a maximum layer level,
                                                                    e.g. to exclude an OSD from VCR output. */
     DSMCAPS_SUB_LAYERS                    = 0x00000004,         /* Can select a number of layers individually
                                                                    as specified in the description. */
     DSMCAPS_BACKGROUND                    = 0x00000008          /* Background color is configurable. */
} DFBScreenMixerCapabilities;

#define DFB_SCREEN_MIXER_DESC_NAME_LENGTH    24

/*
 * Description of a mixer.
 */
typedef struct {
     DFBScreenMixerCapabilities              caps;               /* Mixer capabilities. */
     DFBDisplayLayerIDs                      layers;             /* Visible layers if the full tree is selected. */
     int                                     sub_num;            /* Number of layers that can be selected
                                                                    in sub mode. */
     DFBDisplayLayerIDs                      sub_layers;         /* Layers available for sub mode
                                                                    with layer selection. */
     char name[DFB_SCREEN_MIXER_DESC_NAME_LENGTH];               /* Mixer name. */
} DFBScreenMixerDescription;

/*
 * Flags for mixer configuration.
 */
typedef enum {
     DSMCONF_NONE                          = 0x00000000,         /* None of these. */

     DSMCONF_TREE                          = 0x00000001,         /* (Sub) tree is selected. */
     DSMCONF_LEVEL                         = 0x00000002,         /* Level is specified. */
     DSMCONF_LAYERS                        = 0x00000004,         /* Layer selection is set. */

     DSMCONF_BACKGROUND                    = 0x00000010,         /* Background color is set. */

     DSMCONF_ALL                           = 0x00000017          /* All of these. */
} DFBScreenMixerConfigFlags;

/*
 * (Sub) tree selection.
 */
typedef enum {
     DSMT_UNKNOWN                          = 0x00000000,         /* Unknown mode */

     DSMT_FULL                             = 0x00000001,         /* Full tree. */
     DSMT_SUB_LEVEL                        = 0x00000002,         /* Sub tree via maximum level. */
     DSMT_SUB_LAYERS                       = 0x00000003          /* Sub tree via layer selection. */
} DFBScreenMixerTree;

/*
 * Configuration of a mixer.
 */
typedef struct {
     DFBScreenMixerConfigFlags               flags;              /* Validates struct members. */

     DFBScreenMixerTree                      tree;               /* Selected (sub) tree. */
     int                                     level;              /* Maximum level of sub level mode. */
     DFBDisplayLayerIDs                      layers;             /* Layers for sub layers mode. */
     DFBColor                                background;         /* Background color. */
} DFBScreenMixerConfig;

/*
 * Capabilities of a display encoder.
 */
typedef enum {
     DSECAPS_NONE                          = 0x00000000,         /* None of these. */

     DSECAPS_TV_STANDARDS                  = 0x00000001,         /* TV standards can be selected. */
     DSECAPS_TEST_PICTURE                  = 0x00000002,         /* Test picture generation supported. */
     DSECAPS_MIXER_SEL                     = 0x00000004,         /* Mixer can be selected. */
     DSECAPS_OUT_SIGNALS                   = 0x00000008,         /* Different output signals are supported. */
     DSECAPS_SCANMODE                      = 0x00000010,         /* Can switch between interlaced and progressive
                                                                    output. */
     DSECAPS_FREQUENCY                     = 0x00000020,         /* Can switch between different frequencies. */

     DSECAPS_BRIGHTNESS                    = 0x00000100,         /* Adjustment of brightness is supported. */
     DSECAPS_CONTRAST                      = 0x00000200,         /* Adjustment of contrast is supported. */
     DSECAPS_HUE                           = 0x00000400,         /* Adjustment of hue is supported. */
     DSECAPS_SATURATION                    = 0x00000800,         /* Adjustment of saturation is supported. */
     DSECAPS_CONNECTORS                    = 0x00001000,         /* Select output connector(s). */
     DSECAPS_SLOW_BLANKING                 = 0x00002000,         /* Slow Blanking on outputs is supported. */
     DSECAPS_RESOLUTION                    = 0x00004000,         /* Different encoder resolutions supported */
     DSECAPS_FRAMING                       = 0x00008000,         /* Can select picture framing mode for stereo */
     DSECAPS_ASPECT_RATIO                  = 0x00010000,         /* Can specify display aspect ratio */

     DSECAPS_ALL                           = 0x0001FF3F          /* All of these. */
} DFBScreenEncoderCapabilities;

/*
 * Type of display encoder.
 */
typedef enum {
     DSET_UNKNOWN                          = 0x00000000,         /* Unknown type */

     DSET_CRTC                             = 0x00000001,         /* Encoder is a CRTC. */
     DSET_TV                               = 0x00000002,         /* TV output encoder. */
     DSET_DIGITAL                          = 0x00000004          /* Support signals other than SD TV standards. */
} DFBScreenEncoderType;

/*
 * TV standards.
 */
typedef enum {
     DSETV_UNKNOWN                         = 0x00000000,         /* Unknown standard */

     DSETV_PAL                             = 0x00000001,         /* PAL */
     DSETV_NTSC                            = 0x00000002,         /* NTSC */
     DSETV_SECAM                           = 0x00000004,         /* SECAM */
     DSETV_PAL_60                          = 0x00000008,         /* PAL-60 */
     DSETV_PAL_BG                          = 0x00000010,         /* PAL BG support (specific) */
     DSETV_PAL_I                           = 0x00000020,         /* PAL I support (specific) */
     DSETV_PAL_M                           = 0x00000040,         /* PAL M support (specific) */
     DSETV_PAL_N                           = 0x00000080,         /* PAL N support (specific) */
     DSETV_PAL_NC                          = 0x00000100,         /* PAL NC support (specific) */
     DSETV_NTSC_M_JPN                      = 0x00000200,         /* NTSC_JPN support */
     DSETV_DIGITAL                         = 0x00000400,         /* TV standards from the digital domain */
     DSETV_NTSC_443                        = 0x00000800,         /* NTSC with 4.43MHz colour carrier */
     DSETV_ALL                             = 0x00000FFF          /* All TV Standards */
} DFBScreenEncoderTVStandards;

/*
 * Type of output signal.
 */
typedef enum {
     DSOS_NONE                             = 0x00000000,         /* No signal */

     DSOS_VGA                              = 0x00000001,         /* VGA signal */
     DSOS_YC                               = 0x00000002,         /* Y/C signal */
     DSOS_CVBS                             = 0x00000004,         /* CVBS signal */
     DSOS_RGB                              = 0x00000008,         /* R/G/B signal */
     DSOS_YCBCR                            = 0x00000010,         /* Y/Cb/Cr signal */
     DSOS_HDMI                             = 0x00000020,         /* HDMI signal */
     DSOS_656                              = 0x00000040,         /* 656 Digital output signal */
     DSOS_DSI                              = 0x00000080          /* DSI signal */
} DFBScreenOutputSignals;

/*
 * Type of output connector.
 */
typedef enum {
     DSOC_UNKNOWN                          = 0x00000000,         /* Unknown type */

     DSOC_VGA                              = 0x00000001,         /* VGA connector */
     DSOC_SCART                            = 0x00000002,         /* SCART connector */
     DSOC_YC                               = 0x00000004,         /* Y/C connector */
     DSOC_CVBS                             = 0x00000008,         /* CVBS connector */
     DSOC_SCART2                           = 0x00000010,         /* 2nd SCART connector */
     DSOC_COMPONENT                        = 0x00000020,         /* Component video connector */
     DSOC_HDMI                             = 0x00000040,         /* HDMI connector */
     DSOC_656                              = 0x00000080,         /* DVO connector */
     DSOC_DSI                              = 0x00000100          /* DSI connector */
} DFBScreenOutputConnectors;

/*
 * Resolutions.
 */
typedef enum {
    DSOR_UNKNOWN                           = 0x00000000,         /* Unknown Resolution */

    DSOR_640_480                           = 0x00000001,         /* 640x480 Resolution */
    DSOR_720_480                           = 0x00000002,         /* 720x480 Resolution */
    DSOR_720_576                           = 0x00000004,         /* 720x576 Resolution */
    DSOR_800_600                           = 0x00000008,         /* 800x600 Resolution */
    DSOR_1024_768                          = 0x00000010,         /* 1024x768 Resolution */
    DSOR_1152_864                          = 0x00000020,         /* 1152x864 Resolution */
    DSOR_1280_720                          = 0x00000040,         /* 1280x720 Resolution */
    DSOR_1280_768                          = 0x00000080,         /* 1280x768 Resolution */
    DSOR_1280_960                          = 0x00000100,         /* 1280x960 Resolution */
    DSOR_1280_1024                         = 0x00000200,         /* 1280x1024 Resolution */
    DSOR_1400_1050                         = 0x00000400,         /* 1400x1050 Resolution */
    DSOR_1600_1200                         = 0x00000800,         /* 1600x1200 Resolution */
    DSOR_1920_1080                         = 0x00001000,         /* 1920x1080 Resolution */
    DSOR_960_540                           = 0x00002000,         /* 960x540 Resolution */
    DSOR_1440_540                          = 0x00004000,         /* 1440x540 Resolution */
    DSOR_800_480                           = 0x00008000,         /* 800x480 Resolution */
    DSOR_1024_600                          = 0x00010000,         /* 1024x600 Resolution */
    DSOR_1366_768                          = 0x00020000,         /* 1366x768 Resolution */
    DSOR_1920_1200                         = 0x00040000,         /* 1920x1200 Resolution */
    DSOR_2560_1440                         = 0x00080000,         /* 2560x1440 Resolution */
    DSOR_2560_1600                         = 0x00100000,         /* 2650x1600 Resolution */
    DSOR_3840_2160                         = 0x00200000,         /* 3840x2160 Resolution */
    DSOR_4096_2160                         = 0x00400000,         /* 4096x2160 Resolution */
    DSOR_ALL                               = 0x004FFFFF          /* All Resolutions */
} DFBScreenOutputResolution;

/*
 * Encoder picture delivery method.
 */
typedef enum {
     DSEPF_UNKNOWN                         = 0x00000000,         /* Unknown encoder picture framing */

     DSEPF_MONO                            = 0x00000001,         /* Normal output to non-stereoscopic (3D) TV.
                                                                    No L/R content provided to TV. Frame is output on
                                                                    each vsync. */
     DSEPF_STEREO_SIDE_BY_SIDE_HALF        = 0x00000002,         /* L/R frames are downscaled horizontally by 2 and
                                                                    packed side-by-side into a single frame, left on
                                                                    left half of frame. The packed frame is output on
                                                                    each vsync. Some stereoscopic TV's support this mode
                                                                    using HDMI v1.3 and a special menu configuration. */
     DSEPF_STEREO_TOP_AND_BOTTOM           = 0x00000004,         /* L/R frames are downscaled vertically by 2 and packed
                                                                    into a single frame, left on top. The packed frame
                                                                    is output on each vsync. Some stereoscopic TV's
                                                                    support this mode using HDMI v1.3 and a special menu
                                                                    configuration. */
     DSEPF_STEREO_FRAME_PACKING            = 0x00000008,         /* Full resolution L/R frames or fields are delivered
                                                                    sequentially to the TV, alternating left & right
                                                                    with an active space between each video frame. Vsync
                                                                    occurs after each sequence of: vblank, left eye
                                                                    video frame, active space, right eye video frame.
                                                                    Requires HDMI v1.4a. */
     DSEPF_STEREO_SIDE_BY_SIDE_FULL        = 0x00000010,         /* L/R frames are packed side-by-side into a double
                                                                    width single frame, left on left half of frame.
                                                                    The packed frame is output on each vsync.
                                                                    Requires HDMI v1.4a. */
     DSEPF_ALL                             = 0x0000001F
} DFBScreenEncoderPictureFraming;

/*
 * Display aspect ratio.
 */
typedef enum {
    DFB_ASPECT_RATIO_eAuto                 = 0x00000000,         /* 4x3 for SD and 480p, 16x9 for HD (including 720p,
                                                                    1080i, etc.) */
    DFB_ASPECT_RATIO_e4x3                  = 0x00000001,         /* 4x3 */
    DFB_ASPECT_RATIO_e16x9                 = 0x00000002          /* 16x9 */
} DFBDisplayAspectRatio;

#define DFB_SCREEN_ENCODER_DESC_NAME_LENGTH    24

/*
 * Description of a display encoder.
 */
typedef struct {
     DFBScreenEncoderCapabilities            caps;               /* Encoder capabilities. */
     DFBScreenEncoderType                    type;               /* Type of encoder. */
     DFBScreenEncoderTVStandards             tv_standards;       /* Supported TV standards. */
     DFBScreenOutputSignals                  out_signals;        /* Supported output signals. */
     DFBScreenOutputConnectors               all_connectors;     /* Supported output connectors */
     DFBScreenOutputResolution               all_resolutions;    /* Supported Resolutions. */
     char name[DFB_SCREEN_ENCODER_DESC_NAME_LENGTH];             /* Encoder name. */
     DFBScreenEncoderPictureFraming          all_framing;        /* Supported HDMI signaling modes. */
     DFBDisplayAspectRatio                   all_aspect_ratio;   /* Supported display aspect ratios. */
} DFBScreenEncoderDescription;

/*
 * Flags for display encoder configuration.
 */
typedef enum {
     DSECONF_NONE                          = 0x00000000,         /* None of these. */

     DSECONF_TV_STANDARD                   = 0x00000001,         /* Set TV standard. */
     DSECONF_TEST_PICTURE                  = 0x00000002,         /* Set test picture mode. */
     DSECONF_MIXER                         = 0x00000004,         /* Select mixer. */
     DSECONF_OUT_SIGNALS                   = 0x00000008,         /* Select generated output signal(s). */
     DSECONF_SCANMODE                      = 0x00000010,         /* Select interlaced or progressive output. */
     DSECONF_TEST_COLOR                    = 0x00000020,         /* Set color for DSETP_SINGLE. */
     DSECONF_ADJUSTMENT                    = 0x00000040,         /* Set color adjustment. */
     DSECONF_FREQUENCY                     = 0x00000080,         /* Set Output Frequency*/
     DSECONF_CONNECTORS                    = 0x00000100,         /* Select output connector(s). */
     DSECONF_SLOW_BLANKING                 = 0x00000200,         /* Can select slow blanking support. */
     DSECONF_RESOLUTION                    = 0x00000400,         /* Can change resolution of the encoder.*/
     DSECONF_FRAMING                       = 0x00000800,         /* Set method for delivering pictures to display. */
     DSECONF_ASPECT_RATIO                  = 0x00001000,         /* Set display aspect ratio. */

     DSECONF_ALL                           = 0x00001FFF          /* All of these. */
} DFBScreenEncoderConfigFlags;

/*
 * Test picture mode.
 */
typedef enum {
     DSETP_OFF                             = 0x00000000,         /* Disable test picture. */

     DSETP_MULTI                           = 0x00000001,         /* Show color bars. */
     DSETP_SINGLE                          = 0x00000002,         /* Whole screen as defined in configuration. */

     DSETP_WHITE                           = 0x00000010,         /* Whole screen (ff, ff, ff). */
     DSETP_YELLOW                          = 0x00000020,         /* Whole screen (ff, ff, 00). */
     DSETP_CYAN                            = 0x00000030,         /* Whole screen (00, ff, ff). */
     DSETP_GREEN                           = 0x00000040,         /* Whole screen (00, ff, 00). */
     DSETP_MAGENTA                         = 0x00000050,         /* Whole screen (ff, 00, ff). */
     DSETP_RED                             = 0x00000060,         /* Whole screen (ff, 00, 00). */
     DSETP_BLUE                            = 0x00000070,         /* Whole screen (00, 00, ff). */
     DSETP_BLACK                           = 0x00000080          /* Whole screen (00, 00, 00). */
} DFBScreenEncoderTestPicture;

/*
 * Type of slow blanking signalling.
 */
typedef enum {
     DSOSB_OFF                             = 0x00000000,         /* No signal. */
     DSOSB_16x9                            = 0x00000001,         /* 16x9 Widescreen signalling. */
     DSOSB_4x3                             = 0x00000002,         /* 4x3 widescreen signalling. */
     DSOSB_FOLLOW                          = 0x00000004,         /* Follow signalling. */
     DSOSB_MONITOR                         = 0x00000008          /* Monitor. */
} DFBScreenOutputSlowBlankingSignals;

/*
 * Scan modes.
 */
typedef enum {
     DSESM_UNKNOWN                         = 0x00000000,         /* Unknown mode. */

     DSESM_INTERLACED                      = 0x00000001,         /* Interlaced scan mode. */
     DSESM_PROGRESSIVE                     = 0x00000002          /* Progressive scan mode. */
} DFBScreenEncoderScanMode;

/*
 * Frequency of output signal.
 */
typedef enum {
     DSEF_UNKNOWN                          = 0x00000000,         /* Unknown Frequency. */

     DSEF_25HZ                             = 0x00000001,         /* 25 Hz Output. */
     DSEF_29_97HZ                          = 0x00000002,         /* 29.97 Hz Output. */
     DSEF_50HZ                             = 0x00000004,         /* 50 Hz Output. */
     DSEF_59_94HZ                          = 0x00000008,         /* 59.94 Hz Output. */
     DSEF_60HZ                             = 0x00000010,         /* 60 Hz Output. */
     DSEF_75HZ                             = 0x00000020,         /* 75 Hz Output. */
     DSEF_30HZ                             = 0x00000040,         /* 30 Hz Output. */
     DSEF_24HZ                             = 0x00000080,         /* 24 Hz Output. */
     DSEF_23_976HZ                         = 0x00000100          /* 23.976 Hz Output. */
} DFBScreenEncoderFrequency;

/*
 * Configuration of a display encoder.
 */
typedef struct {
     DFBScreenEncoderConfigFlags             flags;              /* Validates struct members. */

     DFBScreenEncoderTVStandards             tv_standard;        /* TV standard. */
     DFBScreenEncoderTestPicture             test_picture;       /* Test picture mode. */
     int                                     mixer;              /* Selected mixer. */
     DFBScreenOutputSignals                  out_signals;        /* Generated output signals. */
     DFBScreenOutputConnectors               out_connectors;     /* Selected output connector(s). */
     DFBScreenOutputSlowBlankingSignals      slow_blanking;      /* Slow Blanking signals. */
     DFBScreenEncoderScanMode                scanmode;           /* Interlaced or progressive output. */
     DFBColor                                test_color;         /* Color for DSETP_SINGLE. */
     DFBColorAdjustment                      adjustment;         /* Color adjustment. */
     DFBScreenEncoderFrequency               frequency;          /* Selected Output Frequency*/
     DFBScreenOutputResolution               resolution;         /* Selected Output resolution*/
     DFBScreenEncoderPictureFraming          framing;            /* Selected picture delivery method. */
     DFBDisplayAspectRatio                   aspect_ratio;       /* screen aspect ratio */
} DFBScreenEncoderConfig;

/*
 * Capabilities of an output.
 */
typedef enum {
     DSOCAPS_NONE                          = 0x00000000,         /* None of these. */

     DSOCAPS_CONNECTORS                    = 0x00000001,         /* Output connectors are available. */

     DSOCAPS_ENCODER_SEL                   = 0x00000010,         /* Encoder can be selected. */
     DSOCAPS_SIGNAL_SEL                    = 0x00000020,         /* Signal(s) can be selected. */
     DSOCAPS_CONNECTOR_SEL                 = 0x00000040,         /* Connector(s) can be selected. */
     DSOCAPS_SLOW_BLANKING                 = 0x00000080,         /* Slow Blanking on outputs is supported. */
     DSOCAPS_RESOLUTION                    = 0x00000100,         /* Output Resolution can be changed. */

     DSOCAPS_ALL                           = 0x000001F1          /* All of these. */
} DFBScreenOutputCapabilities;

#define DFB_SCREEN_OUTPUT_DESC_NAME_LENGTH   24

/*
 * Description of a screen output.
 */
typedef struct {
     DFBScreenOutputCapabilities             caps;               /* Screen capabilities. */

     DFBScreenOutputConnectors               all_connectors;     /* Output connectors. */
     DFBScreenOutputSignals                  all_signals;        /* Output signals. */
     DFBScreenOutputResolution               all_resolutions;    /* Output Resolutions */

     char name[DFB_SCREEN_OUTPUT_DESC_NAME_LENGTH];              /* Output name */
} DFBScreenOutputDescription;

/*
 * Flags for screen output configuration.
 */
typedef enum {
     DSOCONF_NONE                          = 0x00000000,         /* None of these. */

     DSOCONF_ENCODER                       = 0x00000001,         /* Set encoder the signal(s) comes from. */
     DSOCONF_SIGNALS                       = 0x00000002,         /* Select signal(s) from encoder. */
     DSOCONF_CONNECTORS                    = 0x00000004,         /* Select output connector(s). */
     DSOCONF_SLOW_BLANKING                 = 0x00000008,         /* Can select slow blanking support. */
     DSOCONF_RESOLUTION                    = 0x00000010,         /* Can change output resolution */

     DSOCONF_ALL                           = 0x0000001F          /* All of these. */
} DFBScreenOutputConfigFlags;

/*
 * Configuration of an output.
 */
typedef struct {
     DFBScreenOutputConfigFlags              flags;              /* Validates struct members. */

     int                                     encoder;            /* Chosen encoder. */
     DFBScreenOutputSignals                  out_signals;        /* Selected encoder signal(s). */
     DFBScreenOutputConnectors               out_connectors;     /* Selected output connector(s). */
     DFBScreenOutputSlowBlankingSignals      slow_blanking;      /* Slow Blanking signals. */
     DFBScreenOutputResolution               resolution;         /* Output Resolution */
} DFBScreenOutputConfig;

/*
 * IDirectFBScreen is the screen interface.
 */
D_DEFINE_INTERFACE( IDirectFBScreen,

   /** Retrieving information **/

     /*
      * Get the unique screen ID.
      */
     DFBResult (*GetID) (
          IDirectFBScreen                   *thiz,
          DFBScreenID                       *ret_screen_id
     );

     /*
      * Get a description of this screen, i.e. the capabilities.
      */
     DFBResult (*GetDescription) (
          IDirectFBScreen                   *thiz,
          DFBScreenDescription              *ret_desc
     );

     /*
      * Get the screen's width and height in pixels.
      */
     DFBResult (*GetSize) (
          IDirectFBScreen                   *thiz,
          int                               *ret_width,
          int                               *ret_height
     );

   /** Display Layers **/

     /*
      * Enumerate all existing display layers for this screen.
      *
      * Calls the given callback for each available display
      * layer. The callback is passed the layer id that can be
      * used to retrieve an interface to a specific layer using
      * GetDisplayLayer().
      */
     DFBResult (*EnumDisplayLayers) (
          IDirectFBScreen                   *thiz,
          DFBDisplayLayerCallback            callback,
          void                              *callbackdata
     );

   /** Power management **/

     /*
      * Set screen power mode.
      */
     DFBResult (*SetPowerMode) (
          IDirectFBScreen                   *thiz,
          DFBScreenPowerMode                 mode
     );

   /** Synchronization **/

     /*
      * Wait for next vertical retrace.
      */
     DFBResult (*WaitForSync) (
          IDirectFBScreen                   *thiz
     );

   /** Mixers **/

     /*
      * Get a description of available mixers.
      *
      * All descriptions are written to the array pointed to by
      * 'ret_descriptions'. The number of mixers is returned by
      * GetDescription().
      */
     DFBResult (*GetMixerDescriptions) (
          IDirectFBScreen                   *thiz,
          DFBScreenMixerDescription         *ret_descriptions
     );

     /*
      * Get current mixer configuration.
      */
     DFBResult (*GetMixerConfiguration) (
          IDirectFBScreen                   *thiz,
          int                                mixer,
          DFBScreenMixerConfig              *ret_config
     );

     /*
      * Test mixer configuration.
      *
      * If configuration fails and 'ret_failed' is not NULL, it
      * will indicate which fields of the configuration caused
      * the error.
      */
     DFBResult (*TestMixerConfiguration) (
          IDirectFBScreen                   *thiz,
          int                                mixer,
          const DFBScreenMixerConfig        *config,
          DFBScreenMixerConfigFlags         *ret_failed
     );

     /*
      * Set mixer configuration.
      */
     DFBResult (*SetMixerConfiguration) (
          IDirectFBScreen                   *thiz,
          int                                mixer,
          const DFBScreenMixerConfig        *config
     );

   /** Encoders **/

     /*
      * Get a description of available display encoders.
      *
      * All descriptions are written to the array pointed to by
      * 'ret_descriptions'. The number of encoders is returned
      * by GetDescription().
      */
     DFBResult (*GetEncoderDescriptions) (
          IDirectFBScreen                   *thiz,
          DFBScreenEncoderDescription       *ret_descriptions
     );

     /*
      * Get current encoder configuration.
      */
     DFBResult (*GetEncoderConfiguration) (
          IDirectFBScreen                   *thiz,
          int                                encoder,
          DFBScreenEncoderConfig            *ret_config
     );

     /*
      * Test encoder configuration.
      *
      * If configuration fails and 'ret_failed' is not NULL, it
      * will indicate which fields of the configuration caused
      * the error.
      */
     DFBResult (*TestEncoderConfiguration) (
          IDirectFBScreen                   *thiz,
          int                                encoder,
          const DFBScreenEncoderConfig      *config,
          DFBScreenEncoderConfigFlags       *ret_failed
     );

     /*
      * Set encoder configuration.
      */
     DFBResult (*SetEncoderConfiguration) (
          IDirectFBScreen                   *thiz,
          int                                encoder,
          const DFBScreenEncoderConfig      *config
     );

   /** Outputs **/

     /*
      * Get a description of available outputs.
      *
      * All descriptions are written to the array pointed to by
      * 'ret_descriptions'. The number of outputs is returned
      * by GetDescription().
      */
     DFBResult (*GetOutputDescriptions) (
          IDirectFBScreen                   *thiz,
          DFBScreenOutputDescription        *ret_descriptions
     );

     /*
      * Get current output configuration.
      */
     DFBResult (*GetOutputConfiguration) (
          IDirectFBScreen                   *thiz,
          int                                output,
          DFBScreenOutputConfig             *ret_config
     );

     /*
      * Test output configuration.
      *
      * If configuration fails and 'ret_failed' is not NULL, it
      * will indicate which fields of the configuration caused
      * the error.
      */
     DFBResult (*TestOutputConfiguration) (
          IDirectFBScreen                   *thiz,
          int                                output,
          const DFBScreenOutputConfig       *config,
          DFBScreenOutputConfigFlags        *ret_failed
     );

     /*
      * Set output configuration.
      */
     DFBResult (*SetOutputConfiguration) (
          IDirectFBScreen                   *thiz,
          int                                output,
          const DFBScreenOutputConfig       *config
     );

   /** Synchronization **/

     /*
      * Return current VSync count.
      */
     DFBResult (*GetVSyncCount) (
          IDirectFBScreen                   *thiz,
          unsigned long                     *ret_count
     );
)

/*************************
 * IDirectFBDisplayLayer *
 *************************/

/*
 * Capabilities of a display layer source.
 */
typedef enum {
     DDLSCAPS_NONE                         = 0x00000000,         /* None of these. */

     DDLSCAPS_SURFACE                      = 0x00000001,         /* Source has an accessable surface. */

     DDLSCAPS_ALL                          = 0x00000001          /* All of these. */
} DFBDisplayLayerSourceCaps;

#define DFB_DISPLAY_LAYER_SOURCE_DESC_NAME_LENGTH    24

/*
 * Description of a display layer source.
 */
typedef struct {
     DFBDisplayLayerSourceID                 source_id;          /* ID of the source. */

     char name[DFB_DISPLAY_LAYER_SOURCE_DESC_NAME_LENGTH];       /* Name of the source. */

     DFBDisplayLayerSourceCaps               caps;               /* Capabilites of the source. */
} DFBDisplayLayerSourceDescription;

/*
 * Cooperative level handling the access permissions.
 */
typedef enum {
     DLSCL_SHARED                          = 0x00000000,         /* Shared access. */
     DLSCL_EXCLUSIVE                       = 0x00000001,         /* Exclusive access, fullscreen/mode switching. */
     DLSCL_ADMINISTRATIVE                  = 0x00000002          /* Administrative access, enumerate windows,
                                                                    control them. */
} DFBDisplayLayerCooperativeLevel;

/*
 * Layer configuration flags.
 */
typedef enum {
     DLCONF_NONE                           = 0x00000000,         /* none of these */

     DLCONF_WIDTH                          = 0x00000001,         /* layer width */
     DLCONF_HEIGHT                         = 0x00000002,         /* layer height */
     DLCONF_PIXELFORMAT                    = 0x00000004,         /* pixel format */
     DLCONF_BUFFERMODE                     = 0x00000008,         /* buffer mode */
     DLCONF_OPTIONS                        = 0x00000010,         /* layer options */
     DLCONF_SOURCE                         = 0x00000020,         /* layer source */
     DLCONF_SURFACE_CAPS                   = 0x00000040,         /* surface capabilities */
     DLCONF_COLORSPACE                     = 0x00000080,         /* color space */

     DLCONF_ALL                            = 0x000000FF          /* all of these */
} DFBDisplayLayerConfigFlags;

/*
 * Layer Buffer Mode.
 */
typedef enum {
     DLBM_UNKNOWN                          = 0x00000000,         /* unknown buffer mode */

     DLBM_FRONTONLY                        = 0x00000001,         /* no backbuffer */
     DLBM_BACKVIDEO                        = 0x00000002,         /* backbuffer in video memory */
     DLBM_BACKSYSTEM                       = 0x00000004,         /* backbuffer in system memory */
     DLBM_TRIPLE                           = 0x00000008,         /* triple buffering */
     DLBM_WINDOWS                          = 0x00000010          /* no layer buffers at all,
                                                                    using buffer of each window */
} DFBDisplayLayerBufferMode;

/*
 * Layer options used to enable some capabilities like
 * flicker filtering or colorkeying.
 */
typedef enum {
     DLOP_NONE                             = 0x00000000,         /* None of these. */

     DLOP_ALPHACHANNEL                     = 0x00000001,         /* Make usage of alpha channel for blending on a pixel
                                                                    per pixel basis. */
     DLOP_FLICKER_FILTERING                = 0x00000002,         /* Enable flicker filtering. */
     DLOP_DEINTERLACING                    = 0x00000004,         /* Enable deinterlacing of an interlaced source. */
     DLOP_SRC_COLORKEY                     = 0x00000008,         /* Enable source color key. */
     DLOP_DST_COLORKEY                     = 0x00000010,         /* Enable destination color key. */
     DLOP_OPACITY                          = 0x00000020,         /* Make usage of the global alpha factor set by
                                                                    SetOpacity(). */
     DLOP_FIELD_PARITY                     = 0x00000040,         /* Set field parity. */

     DLOP_LR_MONO                          = 0x00000100,         /* Layer has a single set of surface buffers and a
                                                                    stereo depth. The number of buffers in each set is
                                                                    deteremined by DSCAPS_DOUBLE, DSCAPS_TRIPLE, etc.
                                                                    If they exist, the windows on this layer must not be
                                                                    stereo or L/R mono, otherwise window information
                                                                    will be lost when they are composited to the layer.
                                                                    The layer contents (composited windows if they
                                                                    exist) will be shifted horizontally left and right
                                                                    by the stereo depth value when the layer is
                                                                    composited on the display screen. */
     DLOP_STEREO                           = 0x00000200,         /* Layer has two independent sets of surface buffers
                                                                    (left eye and right eye buffers), each with unique
                                                                    content. The number of buffers in each set is
                                                                    deteremined by DSCAPS_DOUBLE, DSCAPS_TRIPLE, etc.
                                                                    This option is required if any of the windows on
                                                                    this layer have DWCAPS_STEREO or DWCAPS_LR_MONO set,
                                                                    otherwise the stereo or L/R depth content of the
                                                                    windows cannot be preserved when compositing to the
                                                                    layer. */

     DLOP_ALL                              = 0x000003FF          /* All of these. */
} DFBDisplayLayerOptions;

/*
 * Layer configuration.
 */
typedef struct {
     DFBDisplayLayerConfigFlags              flags;              /* Validates struct members. */

     int                                     width;              /* Pixel width. */
     int                                     height;             /* Pixel height. */
     DFBSurfacePixelFormat                   pixelformat;        /* Pixel format. */
     DFBSurfaceColorSpace                    colorspace;         /* Color space. */
     DFBDisplayLayerBufferMode               buffermode;         /* Buffer mode. */
     DFBDisplayLayerOptions                  options;            /* Enable capabilities. */
     DFBDisplayLayerSourceID                 source;             /* Selected layer source. */
     DFBSurfaceCapabilities                  surface_caps;       /* Choose surface capabilities, available:
                                                                    INTERLACED, SEPARATED, PREMULTIPLIED. */
} DFBDisplayLayerConfig;

#define DLSO_FIXED_LIMIT                     0x7f                /* Stereo fixed depth value must be between
                                                                    +DLSO_FIXED_LIMIT and -DLSO_FIXED_LIMIT. */

/*
 * Background mode defining how to erase/initialize the area for
 * a windowstack repaint.
 */
typedef enum {
     DLBM_DONTCARE                         = 0x00000000,         /* do not clear the layer before repainting the window
                                                                    stack */
     DLBM_COLOR                            = 0x00000001,         /* fill with solid color set by SetBackgroundColor() */
     DLBM_IMAGE                            = 0x00000002,         /* use an image set by SetBackgroundImage() */
     DLBM_TILE                             = 0x00000003          /* use a tiled image set by SetBackgroundImage() */
} DFBDisplayLayerBackgroundMode;

/*
 * Flags defining which fields of a DFBWindowDescription are
 * valid.
 */
typedef enum {
     DWDESC_CAPS                           = 0x00000001,         /* caps field is valid. */
     DWDESC_WIDTH                          = 0x00000002,         /* width field is valid. */
     DWDESC_HEIGHT                         = 0x00000004,         /* height field is valid. */
     DWDESC_PIXELFORMAT                    = 0x00000008,         /* pixelformat field is valid */
     DWDESC_POSX                           = 0x00000010,         /* posx field is valid */
     DWDESC_POSY                           = 0x00000020,         /* posy field is valid */
     DWDESC_SURFACE_CAPS                   = 0x00000040,         /* Create the window surface with
                                                                    special capabilities. */
     DWDESC_PARENT                         = 0x00000080,         /* This window has a parent according to
                                                                    parent_id field. */
     DWDESC_OPTIONS                        = 0x00000100,         /* Initial window options have been set. */
     DWDESC_STACKING                       = 0x00000200,         /* Initial stacking class has been set. */
     DWDESC_TOPLEVEL_ID                    = 0x00000400,         /* The top level window is set in toplevel_id field. */
     DWDESC_COLORSPACE                     = 0x00000800,         /* colorspace field is valid */
     DWDESC_RESOURCE_ID                    = 0x00001000          /* resource_id for window surface creation
                                                                    has been set. */
} DFBWindowDescriptionFlags;

/*
 * Capabilities a window can have.
 */
typedef enum {
     DWCAPS_NONE                           = 0x00000000,         /* None of these. */

     DWCAPS_ALPHACHANNEL                   = 0x00000001,         /* The window has an alphachannel for pixel-per-pixel
                                                                    blending. */
     DWCAPS_DOUBLEBUFFER                   = 0x00000002,         /* The window's surface is double buffered.
                                                                    This is very useful to avoid visibility of content
                                                                    that is still in preparation. Normally a window's
                                                                    content can get visible before an update if there is
                                                                    another reason causing a window stack repaint. */
     DWCAPS_INPUTONLY                      = 0x00000004,         /* The window has no surface. You can not draw to it
                                                                    but it receives events. */
     DWCAPS_NODECORATION                   = 0x00000008,         /* The window won't be decorated. */
     DWCAPS_SUBWINDOW                      = 0x00000010,         /* Not a top level window. */
     DWCAPS_COLOR                          = 0x00000020,         /* The window has no buffer.
                                                                    It consumes no backing store. It is filled with a
                                                                    constant color and it receives events. The color is
                                                                    never specified premultiplied. */

     DWCAPS_NOFOCUS                        = 0x00000100,         /* Window will never get focus or receive key events,
                                                                    unless it grabs them. */

     DWCAPS_LR_MONO                        = 0x00001000,         /* Window has a single set of surface buffers and a
                                                                    stereo depth. The number of buffers in each set is
                                                                    deteremined by DSCAPS_DOUBLE, DSCAPS_TRIPLE, etc.
                                                                    Selecting this option requires the underlying layer
                                                                    to have DLOP_STEREO set, otherwise the stereo depth
                                                                    for the left and right eye cannot be preserved when
                                                                    compositing to the underlying layer. The buffer is
                                                                    composited to both the left and right eye buffers of
                                                                    the layer with an x-axis right and left shift of
                                                                    depth pixels, respectively. */
     DWCAPS_STEREO                         = 0x00002000,         /* Window has 2 independent sets of surface buffers
                                                                    (left eye & right eye buffers), each with unique
                                                                    content.
                                                                    The number of buffers in each set is deteremined by
                                                                    DSCAPS_DOUBLE, DSCAPS_TRIPLE, etc as usual.
                                                                    Selecting this option requires the underlying layer
                                                                    to have DLOP_STEREO set, otherwise the independent
                                                                    content of the left and right eye cannot be
                                                                    preserved when compositing to the layer. */

     DWCAPS_ALL                            = 0x0000313F          /* All of these. */
} DFBWindowCapabilities;

/*
 * Flags controlling the appearance and behaviour of the window.
 */
typedef enum {
     DWOP_NONE                             = 0x00000000,         /* None of these. */

     DWOP_COLORKEYING                      = 0x00000001,         /* Enable color key. */
     DWOP_ALPHACHANNEL                     = 0x00000002,         /* Enable alpha blending using the window's alpha
                                                                    channel. */
     DWOP_OPAQUE_REGION                    = 0x00000004,         /* Overrides DWOP_ALPHACHANNEL for the region set by
                                                                    SetOpaqueRegion() */
     DWOP_SHAPED                           = 0x00000008,         /* Window doesn't receive mouse events for invisible
                                                                    regions, must be used with DWOP_ALPHACHANNEL or
                                                                    DWOP_COLORKEYING. */
     DWOP_KEEP_POSITION                    = 0x00000010,         /* Window can't be moved with the mouse. */
     DWOP_KEEP_SIZE                        = 0x00000020,         /* Window can't be resized with the mouse. */
     DWOP_KEEP_STACKING                    = 0x00000040,         /* Window can't be raised or lowered with the mouse. */

     DWOP_GHOST                            = 0x00001000,         /* Never get focus or input, clicks will go through,
                                                                    implies DWOP_KEEP. */
     DWOP_INDESTRUCTIBLE                   = 0x00002000,         /* Window can't be destroyed by internal shortcut. */
     DWOP_INPUTONLY                        = 0x00004000,         /* The window will be input only. It will receive
                                                                    events but is not shown. Note that toggling this bit
                                                                    will not free/assign the window surface. */
     DWOP_STEREO_SIDE_BY_SIDE_HALF         = 0x00008000,         /* Treat single buffer as combined left/right buffers,
                                                                    side by side. */
     DWOP_SCALE                            = 0x00010000,         /* Surface won't be changed if window size on screen
                                                                    changes. The surface can be resized separately using
                                                                    ResizeSurface(). */

     DWOP_KEEP_ABOVE                       = 0x00100000,         /* Keep window above parent window. */
     DWOP_KEEP_UNDER                       = 0x00200000,         /* Keep window under parent window. */
     DWOP_FOLLOW_BOUNDS                    = 0x00400000,         /* Follow window bounds from parent. */

     DWOP_ALL                              = 0x0071F07F          /* all possible options */
} DFBWindowOptions;

/*
 * The stacking class restricts the stacking order of windows.
 */
typedef enum {
     DWSC_MIDDLE                           = 0x00000000,        /* This is the default stacking class of new windows. */
     DWSC_UPPER                            = 0x00000001,        /* Window is always above windows in the middle stacking
                                                                   class. Only windows that are also in the upper
                                                                   stacking class can get above them. */
     DWSC_LOWER                            = 0x00000002         /* Window is always below windows in the middle stacking
                                                                   class. Only windows that are also in the lower
                                                                   stacking class can get below them. */
} DFBWindowStackingClass;

/*
 * Description of the window that is to be created.
 */
typedef struct {
     DFBWindowDescriptionFlags               flags;              /* field validation */

     DFBWindowCapabilities                   caps;               /* capabilities */
     int                                     width;              /* pixel width */
     int                                     height;             /* pixel height */
     DFBSurfacePixelFormat                   pixelformat;        /* pixel format */
     int                                     posx;               /* distance from left layer border */
     int                                     posy;               /* distance from upper layer border */
     DFBSurfaceCapabilities                  surface_caps;       /* surface capabilities */
     DFBWindowID                             parent_id;          /* window id of parent window */
     DFBWindowOptions                        options;            /* initial window options */
     DFBWindowStackingClass                  stacking;           /* initial stacking class */
     unsigned long                           resource_id;        /* resource id used to create the window surface */
     DFBWindowID                             toplevel_id;        /* top level window: if not zero, window will be a sub
                                                                    window */
     DFBSurfaceColorSpace                    colorspace;         /* color space */
} DFBWindowDescription;

/*
 * IDirectFBDisplayLayer is the display layer interface.
 */
D_DEFINE_INTERFACE( IDirectFBDisplayLayer,

   /** Retrieving information **/

     /*
      * Get the unique layer ID.
      */
     DFBResult (*GetID) (
          IDirectFBDisplayLayer             *thiz,
          DFBDisplayLayerID                 *ret_layer_id
     );

     /*
      * Get a description of this display layer,
        i.e. the capabilities.
      */
     DFBResult (*GetDescription) (
          IDirectFBDisplayLayer             *thiz,
          DFBDisplayLayerDescription        *ret_desc
     );

     /*
      * Get a description of available sources.
      *
      * All descriptions are written to the array pointed to by
      * ret_descriptions. The number of sources is returned by
      * GetDescription().
      */
     DFBResult (*GetSourceDescriptions) (
          IDirectFBDisplayLayer             *thiz,
          DFBDisplayLayerSourceDescription  *ret_descriptions
     );

     /*
      * For an interlaced display, this returns the currently
      * inactive field: 0 for the top field, and 1 for the
      * bottom field.
      *
      * The inactive field is the one you should draw to next
      * to avoid tearing, the active field is the one currently
      * being displayed.
      * For a progressive output, this should always return 0.
      */
     DFBResult (*GetCurrentOutputField) (
          IDirectFBDisplayLayer             *thiz,
          int                               *ret_field
     );

   /** Interfaces **/

     /*
      * Get an interface to layer's surface.
      *
      * Only available in exclusive mode.
      */
     DFBResult (*GetSurface) (
          IDirectFBDisplayLayer             *thiz,
          IDirectFBSurface                 **ret_interface
     );

     /*
      * Get an interface to the screen to which the layer
      * belongs.
      */
     DFBResult (*GetScreen) (
          IDirectFBDisplayLayer             *thiz,
          IDirectFBScreen                  **ret_interface
     );

   /** Configuration **/

     /*
      * Set cooperative level to get control over the layer or
      * the windows within this layer.
      */
     DFBResult (*SetCooperativeLevel) (
          IDirectFBDisplayLayer             *thiz,
          DFBDisplayLayerCooperativeLevel    level
     );

     /*
      * Get current layer configuration.
      */
     DFBResult (*GetConfiguration) (
          IDirectFBDisplayLayer             *thiz,
          DFBDisplayLayerConfig             *ret_config
     );

     /*
      * Test layer configuration.
      *
      * If configuration fails and 'ret_failed' is not NULL, it
      * will indicate which fields of the configuration caused
      * the error.
      */
     DFBResult (*TestConfiguration) (
          IDirectFBDisplayLayer             *thiz,
          const DFBDisplayLayerConfig       *config,
          DFBDisplayLayerConfigFlags        *ret_failed
     );

     /*
      * Set layer configuration.
      *
      * Only available in exclusive or administrative mode.
      */
     DFBResult (*SetConfiguration) (
          IDirectFBDisplayLayer             *thiz,
          const DFBDisplayLayerConfig       *config
     );

   /** Layout **/

     /*
      * Set location on screen as normalized values.
      *
      * So the whole screen is 0.0, 0.0, 1.0, 1.0.
      */
     DFBResult (*SetScreenLocation) (
          IDirectFBDisplayLayer             *thiz,
          float                              x,
          float                              y,
          float                              width,
          float                              height
     );

     /*
      * Set location on screen in pixels.
      */
     DFBResult (*SetScreenPosition) (
          IDirectFBDisplayLayer             *thiz,
          int                                x,
          int                                y
     );

     /*
      * Set location on screen in pixels.
      */
     DFBResult (*SetScreenRectangle) (
          IDirectFBDisplayLayer             *thiz,
          int                                x,
          int                                y,
          int                                width,
          int                                height
     );

     /*
      * Get stereo depth.
      */
     DFBResult (*GetStereoDepth) (
         IDirectFBDisplayLayer              *thiz,
         bool                               *follow_video,
         int                                *ret_z
    );

     /*
      * Set stereo depth.
      *
      * If 'follow_video' is true then the pixel offset value
      * from the video metadata will be used to set the
      * perceived depth.
      * Otherwise, the z value specified will cause the left eye
      * buffer content to be shifted on the x-axis by +z and the
      * right eye buffer to be shifted by -z. A positive z value
      * will cause the layer to appear closer than the TV plane
      * while a negative z value will make the layer appear
      * farther away. The depth is limited to a value between
      * +DLSO_FIXED_LIMIT and -DLSO_FIXED_LIMIT.
      */
     DFBResult (*SetStereoDepth) (
         IDirectFBDisplayLayer              *thiz,
         bool                                follow_video,
         int                                 z
    );

   /** Misc Settings **/

     /*
      * Set global alpha factor for blending with
      * layer(s) below.
      */
     DFBResult (*SetOpacity) (
          IDirectFBDisplayLayer             *thiz,
          u8                                 opacity
     );

     /*
      * Set the source rectangle.
      *
      * Only this part of the layer will be displayed.
      */
     DFBResult (*SetSourceRectangle) (
          IDirectFBDisplayLayer             *thiz,
          int                                x,
          int                                y,
          int                                width,
          int                                height
     );

     /*
      * For an interlaced display, this sets the field parity.
      *
      * 'field' is set to 0 for top field first, and set to 1
      * for bottom field first.
      */
     DFBResult (*SetFieldParity) (
          IDirectFBDisplayLayer             *thiz,
          int                                field
     );

     /*
      * Set the clipping region(s).
      *
      * If supported, this method sets the clipping 'regions'
      * that are used to to enable or disable visibility of
      * parts of the layer. The 'num_regions' must not exceed
      * the limit as stated in the display layer description.
      * If 'positive' is DFB_TRUE the layer will be shown only
      * in these regions, otherwise it's shown as usual except
      * in these regions.
      */
     DFBResult (*SetClipRegions) (
          IDirectFBDisplayLayer             *thiz,
          const DFBRegion                   *regions,
          int                                num_regions,
          DFBBoolean                         positive
     );

   /** Color keys **/

     /*
      * Set the source color key.
      *
      * If a pixel of the layer matches this color the
      * underlying pixel is visible at this point.
      */
     DFBResult (*SetSrcColorKey) (
          IDirectFBDisplayLayer             *thiz,
          u8                                 r,
          u8                                 g,
          u8                                 b
     );

     /*
      * Set the destination color key.
      *
      * The layer is only visible at points where the underlying
      * pixel matches this color.
      */
     DFBResult (*SetDstColorKey) (
          IDirectFBDisplayLayer             *thiz,
          u8                                 r,
          u8                                 g,
          u8                                 b
     );

   /** Z Order **/

     /*
      * Get the current display layer level.
      *
      * The level describes the z axis position of a layer. The
      * primary layer is always on level zero unless a special
      * driver adds support for level adjustment on the primary
      * layer. Layers above have a positive level, e.g. video
      * overlays. Layers below have a negative level, e.g. video
      * underlays or background layers.
      */
     DFBResult (*GetLevel) (
          IDirectFBDisplayLayer             *thiz,
          int                               *ret_level
     );

     /*
      * Set the display layer level.
      *
      * Moves the layer to the specified level. The order of all
      * other layers won't be changed. Note that only a few
      * layers support level adjustment which is reflected by
      * their capabilities.
      */
     DFBResult (*SetLevel) (
          IDirectFBDisplayLayer             *thiz,
          int                                level
     );

   /** Background handling **/

     /*
      * Set the erase behaviour for windowstack repaints.
      *
      * Only available in exclusive or administrative mode.
      */
     DFBResult (*SetBackgroundMode) (
          IDirectFBDisplayLayer             *thiz,
          DFBDisplayLayerBackgroundMode      mode
     );

     /*
      * Set the background image for the imaged background mode.
      *
      * Only available in exclusive or administrative mode.
      */
     DFBResult (*SetBackgroundImage) (
          IDirectFBDisplayLayer             *thiz,
          IDirectFBSurface                  *surface
     );

     /*
      * Set the color for a solid colored background.
      *
      * Only available in exclusive or administrative mode.
      */
     DFBResult (*SetBackgroundColor) (
          IDirectFBDisplayLayer             *thiz,
          u8                                 r,
          u8                                 g,
          u8                                 b,
          u8                                 a
     );

   /** Color adjustment **/

     /*
      * Get the layers color adjustment.
      */
     DFBResult (*GetColorAdjustment) (
          IDirectFBDisplayLayer             *thiz,
          DFBColorAdjustment                *ret_adj
     );

     /*
      * Set the layers color adjustment.
      *
      * Only available in exclusive or administrative mode.
      *
      * This function only has an effect if the underlying
      * hardware supports this operation. Check the layers
      * capabilities to find out if this is the case.
      */
     DFBResult (*SetColorAdjustment) (
          IDirectFBDisplayLayer             *thiz,
          const DFBColorAdjustment          *adj
     );

   /** Windows **/

     /*
      * Create a window within this layer given a description of
      * the window that is to be created.
      */
     DFBResult (*CreateWindow) (
          IDirectFBDisplayLayer             *thiz,
          const DFBWindowDescription        *desc,
          IDirectFBWindow                  **ret_interface
     );

     /*
      * Retrieve an interface to an existing window.
      *
      * The window is identified by its window id.
      */
     DFBResult (*GetWindow) (
          IDirectFBDisplayLayer             *thiz,
          DFBWindowID                        window_id,
          IDirectFBWindow                  **ret_interface
     );

   /** Cursor handling **/

     /*
      * Enable/disable the mouse cursor for this layer.
      *
      * Windows on a layer will only receive motion events if
      * the cursor is enabled. This function is only available
      * in exclusive/administrative mode.
      */
     DFBResult (*EnableCursor) (
          IDirectFBDisplayLayer             *thiz,
          int                                enable
     );

     /*
      * Return the x/y coordinates of the layer's mouse cursor.
      */
     DFBResult (*GetCursorPosition) (
          IDirectFBDisplayLayer             *thiz,
          int                               *ret_x,
          int                               *ret_y
     );

     /*
      * Move cursor to specified position.
      *
      * Handles movement like a real one, i.e. generates events.
      */
     DFBResult (*WarpCursor) (
          IDirectFBDisplayLayer             *thiz,
          int                                x,
          int                                y
     );

     /*
      * Set cursor acceleration.
      *
      * Sets the acceleration of cursor movements. The amount
      * beyond the 'threshold' will be multiplied with the
      * acceleration factor. The acceleration factor is
      * 'numerator' / 'denominator'.
      */
     DFBResult (*SetCursorAcceleration) (
          IDirectFBDisplayLayer             *thiz,
          int                                numerator,
          int                                denominator,
          int                                threshold
     );

     /*
      * Set the cursor shape and the hotspot.
      *
      * Passing NULL will restore the default cursor shape.
      */
     DFBResult (*SetCursorShape) (
          IDirectFBDisplayLayer             *thiz,
          IDirectFBSurface                  *shape,
          int                                hot_x,
          int                                hot_y
     );

     /*
      * Set the cursor opacity.
      *
      * This function is especially useful if you want to hide
      * the cursor but still want windows on this display layer
      * to receive motion events.
      * In this case, simply set the cursor opacity to zero.
      */
     DFBResult (*SetCursorOpacity) (
          IDirectFBDisplayLayer             *thiz,
          u8                                 opacity
     );

   /** Synchronization **/

     /*
      * Wait for next vertical retrace.
      */
     DFBResult (*WaitForSync) (
          IDirectFBDisplayLayer             *thiz
     );

   /** Contexts **/

     /*
      * Switch the layer context.
      *
      * Switches to the shared context unless 'exclusive'is
      * DFB_TRUE and the cooperative level of this interface is
      * DLSCL_EXCLUSIVE.
      */
     DFBResult (*SwitchContext) (
          IDirectFBDisplayLayer             *thiz,
          DFBBoolean                         exclusive
     );

   /** Rotation **/

     /*
      * Set the rotation of data within the layer.
      *
      * Only available in exclusive or administrative mode.
      * Any 'rotation' other than 0, 90, 180 or 270 is not
      * supported. No layer hardware feature usage, only rotated
      * blitting is used.
      */
     DFBResult (*SetRotation) (
          IDirectFBDisplayLayer             *thiz,
          int                                rotation
     );

     /*
      * Get the rotation of data within the layer.
      */
     DFBResult (*GetRotation) (
          IDirectFBDisplayLayer             *thiz,
          int                               *ret_rotation
     );

   /** Windows **/

     /*
      * Retrieve an interface to an existing window.
      *
      * The window is identified by its surface resource id.
      */
     DFBResult (*GetWindowByResourceID) (
          IDirectFBDisplayLayer             *thiz,
          unsigned long                      resource_id,
          IDirectFBWindow                  **ret_interface
     );

   /** Surface **/

     /*
      * Set the surface to be shown by the layer.
      *
      * Only available in exclusive mode.
      */
     DFBResult (*SetSurface) (
          IDirectFBDisplayLayer             *thiz,
          IDirectFBSurface                  *surface
     );
)

/********************
 * IDirectFBSurface *
 ********************/

/*
 * Stereo eye buffer.
 */
typedef enum {
     DSSE_NONE                             = 0x00000000,         /* None */

     DSSE_LEFT                             = 0x00000001,         /* Left eye buffers to be used for all future
                                                                    operations on this surface. */
     DSSE_RIGHT                            = 0x00000002          /* Right eye buffers to be used for all future
                                                                    operations on this surface. */
} DFBSurfaceStereoEye;

/*
 * Flags defining the type of data access.
 * These are important for surface swapping management.
 */
typedef enum {
     DSLF_READ                             = 0x00000001,         /* Request read access while surface is locked. */
     DSLF_WRITE                            = 0x00000002          /* Request write access. If specified and surface has a
                                                                    back buffer, it will be used. Otherwise, the front
                                                                    buffer is used. */
} DFBSurfaceLockFlags;

/*
 * Flipping flags controlling the behaviour of Flip().
 */
typedef enum {
     DSFLIP_NONE                           = 0x00000000,         /* None of these. */

     DSFLIP_WAIT                           = 0x00000001,         /* Flip() returns upon vertical sync. Flipping is
                                                                    still done immediately unless DSFLIP_ONSYNC is
                                                                    specified, too. */
     DSFLIP_BLIT                           = 0x00000002,         /* Copy from back buffer to front buffer rather than
                                                                    just swapping these buffers.
                                                                    This behaviour is enforced if the region passed to
                                                                    Flip() is not NULL or if the surface being flipped
                                                                    is a sub surface. */
     DSFLIP_ONSYNC                         = 0x00000004,         /* Do the actual flipping upon the next vertical sync.
                                                                    The Flip() method will still return immediately
                                                                    unless DSFLIP_WAIT is specified, too. */
     DSFLIP_PIPELINE                       = 0x00000008,         /* Advanced synchronization with the accelerator.
                                                                    It is especially for accelerators with a command
                                                                    buffer that can store more graphics operations than
                                                                    required to render one frame. */
     DSFLIP_ONCE                           = 0x00000010,         /* Cause Flip() to wait until all other updating window
                                                                    surfaces are being flipped. */

     DSFLIP_QUEUE                          = 0x00000100,         /* Queuing updates */
     DSFLIP_FLUSH                          = 0x00000200,         /* Flushing */
     DSFLIP_SWAP                           = 0x00000400,         /* Cause real flip even though region was specified. */
     DSFLIP_UPDATE                         = 0x00000800,         /* Update from front only, no swapping */
     DSFLIP_NOWAIT                         = 0x00001000,

     DSFLIP_WAITFORSYNC                    = DSFLIP_WAIT |
                                             DSFLIP_ONSYNC       /* Flip() returns after the actual flipping performed
                                                                    during the next vertical sync  */
} DFBSurfaceFlipFlags;

/*
 * Blend functions to use for source and destination blending.
 *
 * pixel color = sc * cf[sf] + dc * cf[df]
 * pixel alpha = sa * af[sf] + da * af[df]
 * sc = source color
 * sa = source alpha
 * dc = destination color
 * da = destination alpha
 * sf = source blend function
 * df = destination blend function
 * cf[x] = color factor for blend function x
 * af[x] = alpha factor for blend function x
 *
 */
typedef enum {
     DSBF_UNKNOWN                          = 0x00000000,         /* unknown blend function */

     DSBF_ZERO                             = 0x00000001,         /* cf:    0           af:    0 */
     DSBF_ONE                              = 0x00000002,         /* cf:    1           af:    1 */
     DSBF_SRCCOLOR                         = 0x00000003,         /* cf:   sc           af:   sa */
     DSBF_INVSRCCOLOR                      = 0x00000004,         /* cf: 1-sc           af: 1-sa */
     DSBF_SRCALPHA                         = 0x00000005,         /* cf:   sa           af:   sa */
     DSBF_INVSRCALPHA                      = 0x00000006,         /* cf: 1-sa           af: 1-sa */
     DSBF_DESTALPHA                        = 0x00000007,         /* cf:   da           af:   da */
     DSBF_INVDESTALPHA                     = 0x00000008,         /* cf: 1-da           af: 1-da */
     DSBF_DESTCOLOR                        = 0x00000009,         /* cf:   dc           af:   da */
     DSBF_INVDESTCOLOR                     = 0x0000000A,         /* cf: 1-dc           af: 1-da */
     DSBF_SRCALPHASAT                      = 0x0000000B          /* cf: min(sa, 1-da)  af:    1 */
} DFBSurfaceBlendFunction;

/*
 * Available Porter/Duff rules.
 *
 * pixel = (source * fs + destination * fd)
 * sa = source alpha,
 * da = destination alpha
 */
typedef enum {
     DSPD_NONE                             = 0x00000000,         /* fs:   sa           fd: 1-sa */

     DSPD_CLEAR                            = 0x00000001,         /* fs:    0           fd:    0 */
     DSPD_SRC                              = 0x00000002,         /* fs:    1           fd:    0 */
     DSPD_SRC_OVER                         = 0x00000003,         /* fs:    1           fd: 1-sa */
     DSPD_DST_OVER                         = 0x00000004,         /* fs: 1-da           fd:    1 */
     DSPD_SRC_IN                           = 0x00000005,         /* fs:   da           fd:    0 */
     DSPD_DST_IN                           = 0x00000006,         /* fs:    0           fd:   sa */
     DSPD_SRC_OUT                          = 0x00000007,         /* fs: 1-da           fd:    0 */
     DSPD_DST_OUT                          = 0x00000008,         /* fs:    0           fd: 1-sa */
     DSPD_SRC_ATOP                         = 0x00000009,         /* fs:   da           fd: 1-sa */
     DSPD_DST_ATOP                         = 0x0000000A,         /* fs: 1-da           fd:   sa */
     DSPD_ADD                              = 0x0000000B,         /* fs:    1           fd:    1 */
     DSPD_XOR                              = 0x0000000C,         /* fs: 1-da           fd: 1-sa */
     DSPD_DST                              = 0x0000000D          /* fs:    0           fd:    1 */
} DFBSurfacePorterDuffRule;

/*
 * Flags controlling the text layout.
 */
typedef enum {
     DSTF_NONE                             = 0x00000000,         /* no flags */

     DSTF_LEFT                             = 0x00000000,         /* left aligned */
     DSTF_CENTER                           = 0x00000001,         /* horizontally centered */
     DSTF_RIGHT                            = 0x00000002,         /* right aligned */
     DSTF_TOP                              = 0x00000004,         /* 'y' specifies the top instead of the baseline */
     DSTF_BOTTOM                           = 0x00000008,         /* 'y' specifies the bottom instead of the baseline */
     DSTF_OUTLINE                          = 0x00000010,         /* enables outline rendering */
     DSTF_BLEND_FUNCS                      = 0x00000020,         /* keeps src/dst blend functions as set on
                                                                    destination */
     DSTF_TOPLEFT                          = DSTF_TOP |
                                             DSTF_LEFT,          /* left aligned, 'y' specifying the top */
     DSTF_TOPCENTER                        = DSTF_TOP |
                                             DSTF_CENTER,        /* horizontally centered, 'y' specifying the top */
     DSTF_TOPRIGHT                         = DSTF_TOP |
                                             DSTF_RIGHT,         /* right aligned, 'y' specifying the top */
     DSTF_BOTTOMLEFT                       = DSTF_BOTTOM |
                                             DSTF_LEFT,          /* left aligned, 'y' specifying the bottom */
     DSTF_BOTTOMCENTER                     = DSTF_BOTTOM |
                                             DSTF_CENTER,        /* horizontally centered, 'y' specifying the bottom */
     DSTF_BOTTOMRIGHT                      = DSTF_BOTTOM |
                                             DSTF_RIGHT          /* right aligned, 'y' specifying the bottom */
} DFBSurfaceTextFlags;

/*
 * Options for drawing and blitting operations.
 */
typedef enum {
     DSRO_NONE                             = 0x00000000,         /* None of these. */

     DSRO_SMOOTH_UPSCALE                   = 0x00000001,         /* Use interpolation for upscale StretchBlit(). */
     DSRO_SMOOTH_DOWNSCALE                 = 0x00000002,         /* Use interpolation for downscale StretchBlit(). */
     DSRO_MATRIX                           = 0x00000004,         /* Use the transformation matrix set via
                                                                    SetMatrix(). */
     DSRO_ANTIALIAS                        = 0x00000008,         /* Enable anti-aliasing for edges (alpha blending must
                                                                    be enabled). */

     DSRO_ALL                              = 0x0000000F          /* All of these. */
} DFBSurfaceRenderOptions;

/*
 * Flags controlling surface masks set via SetSourceMask().
 */
typedef enum {
     DSMF_NONE                             = 0x00000000,         /* None of these. */

     DSMF_STENCIL                          = 0x00000001,         /* Take 'x' and 'y' as fixed start coordinates in the
                                                                    mask. */

     DSMF_ALL                              = 0x00000001          /* All of these. */
} DFBSurfaceMaskFlags;

/*
 * Monochrome glyph attributes.
 */
typedef struct {
     int                                     width;              /* glyph width */
     int                                     height;             /* glyph height */
     int                                     rowbyte;            /* glyph rowbyte */
     int                                     bitoffset;          /* glyph bitoffset */
     int                                     fgcolor;            /* foreground color */
     int                                     bgcolor;            /* background color */
     int                                     hzoom;              /* horizontal zoom factor */
     int                                     vzoom;              /* vertical zoom factor */
} DFBMonoGlyphAttributes;

/*
 * Frame time configuration flags.
 */
typedef enum {
     DFTCF_NONE                            = 0x00000000,         /* None of these. */

     DFTCF_INTERVAL                        = 0x00000001,         /* Interval is specified, otherwise the interval is set
                                                                    automatically depending on screen refresh. */
     DFTCF_MAX_ADVANCE                     = 0x00000002,         /* Maximum time to render in advance, GetFrameTime()
                                                                    will block to keep the limit. */

     DFTCF_ALL                             = 0x00000003,         /* All of these. */
} DFBFrameTimeConfigFlags;

/*
 * Frame time configuration.
 */
typedef struct {
     DFBFrameTimeConfigFlags                 flags;              /* Validation of fields. */

     long long                               interval;           /* Interval time. */
     long long                               max_advance;        /* Maximum time ahead for rendering frames. */
} DFBFrameTimeConfig;

/*
 * Buffer role.
 */
typedef enum {
     DSBR_FRONT                            = 0x00000000,         /* Front buffer. */
     DSBR_BACK                             = 0x00000001,         /* Back buffer */
     DSBR_IDLE                             = 0x00000002          /* Idle buffer. */
} DFBSurfaceBufferRole;

/*
 * IDirectFBSurface is the surface interface.
 */
D_DEFINE_INTERFACE( IDirectFBSurface,

   /** Retrieving information **/

     /*
      * Return the capabilities of this surface.
      */
     DFBResult (*GetCapabilities) (
          IDirectFBSurface                  *thiz,
          DFBSurfaceCapabilities            *ret_caps
     );

     /*
      * Get the surface's position in pixels.
      */
     DFBResult (*GetPosition) (
          IDirectFBSurface                  *thiz,
          int                               *ret_x,
          int                               *ret_y
     );

     /*
      * Get the surface's width and height in pixels.
      */
     DFBResult (*GetSize) (
          IDirectFBSurface                  *thiz,
          int                               *ret_width,
          int                               *ret_height
     );

     /*
      * Created sub surfaces might be clipped by their parents,
      * this function returns the resulting rectangle relative
      * to this surface.
      *
      * For non sub surfaces this function returns
      * { 0, 0, width, height }.
      */
     DFBResult (*GetVisibleRectangle) (
          IDirectFBSurface                  *thiz,
          DFBRectangle                      *ret_rect
     );

     /*
      * Get the current pixel format.
      */
     DFBResult (*GetPixelFormat) (
          IDirectFBSurface                  *thiz,
          DFBSurfacePixelFormat             *ret_pixelformat
     );

     /*
      * Get the current color space.
      */
     DFBResult (*GetColorSpace) (
          IDirectFBSurface                  *thiz,
          DFBSurfaceColorSpace              *ret_colorspace
     );

     /*
      * Get a mask of drawing functions that are hardware
      * accelerated with the current settings.
      *
      * If a source surface is specified the mask will also
      * contain accelerated blitting functions. Note that there
      * is no guarantee that these will actually be accelerated
      * since the surface storage (video/system) is examined
      * only when something actually gets drawn or blitted.
      */
     DFBResult (*GetAccelerationMask) (
          IDirectFBSurface                  *thiz,
          IDirectFBSurface                  *source,
          DFBAccelerationMask               *ret_mask
     );

   /** Palette & Alpha Ramp **/

     /*
      * Get access to the surface's palette.
      *
      * Returns an interface that can be used to gain read
      * and/or write access to the surface's palette.
      */
     DFBResult (*GetPalette) (
          IDirectFBSurface                  *thiz,
          IDirectFBPalette                 **ret_interface
     );

     /*
      * Change the surface's palette.
      */
     DFBResult (*SetPalette) (
          IDirectFBSurface                  *thiz,
          IDirectFBPalette                  *palette
     );

     /*
      * Set the alpha ramp for formats with one or two
      * alpha bits.
      *
      * Either all four values or the first and the last one
      * are used, depending on the format.
      * Default values are: 0x00, 0x55, 0xaa, 0xff.
      */
     DFBResult (*SetAlphaRamp) (
          IDirectFBSurface                  *thiz,
          u8                                 a0,
          u8                                 a1,
          u8                                 a2,
          u8                                 a3
     );

   /** Buffer operations **/

     /*
      * Get the current stereo eye.
      *
      * Only applicable to window/layer surfaces with the
      * DWCAPS_STEREO or DLOP_STEREO option. This method will
      * retrieve which set of buffers (left or right) is
      * currently active for operations on this surface.
      */
     DFBResult (*GetStereoEye) (
          IDirectFBSurface                  *thiz,
          DFBSurfaceStereoEye               *ret_eye
     );

     /*
      * Select the stereo eye for future operations.
      *
      * Only applicable to window/layer surfaces with the
      * DWCAPS_STEREO or DLOP_STEREO option. This method will
      * specify which set of buffers (left or right) is to be
      * used for future operations on this surface.
      */
     DFBResult (*SetStereoEye) (
          IDirectFBSurface                  *thiz,
          DFBSurfaceStereoEye                eye
     );

     /*
      * Lock the surface for the access type specified.
      *
      * Returns a data pointer and the line pitch of it.
      * Note: if the surface is double/triple buffered and the
      * DSLF_WRITE flag is specified, the pointer is to the back
      * buffer. In all other cases, the pointer is to the front
      * buffer.
      */
     DFBResult (*Lock) (
          IDirectFBSurface                  *thiz,
          DFBSurfaceLockFlags                flags,
          void                             **ret_ptr,
          int                               *ret_pitch
     );

     /*
      * Return the framebuffer offset of a locked surface.
      */
     DFBResult (*GetFramebufferOffset) (
          IDirectFBSurface                  *thiz,
          int                               *offset
     );

     /*
      * Unlock the surface after direct access.
      */
     DFBResult (*Unlock) (
          IDirectFBSurface                  *thiz
     );

     /*
      * Flip/Update surface buffers.
      *
      * If no region is specified the whole surface is flipped,
      * otherwise blitting is used to update the region.
      * If surface capabilities don't include DSCAPS_FLIPPING,
      * this method has the effect to make visible changes
      * made to the surface contents.
      */
     DFBResult (*Flip) (
          IDirectFBSurface                  *thiz,
          const DFBRegion                   *region,
          DFBSurfaceFlipFlags                flags
     );

     /*
      * Flip/Update stereo surface buffers.
      *
      * Flips both the left and right buffers simultaneously to
      * ensure synchronization between the two.
      * Only applicable to window and layer surfaces with the
      * DWCAPS_STEREO or DLOP_STEREO option. This method will
      * fail with all other surfaces.
      * If no region is specified the whole surface is flipped,
      * otherwise blitting is used to update the region.
      * If surface capabilities don't include DSCAPS_FLIPPING,
      * this method has the effect to make visible changes made
      * to the surface contents.
      */
     DFBResult (*FlipStereo) (
          IDirectFBSurface                  *thiz,
          const DFBRegion                   *left_region,
          const DFBRegion                   *right_region,
          DFBSurfaceFlipFlags                flags
     );

     /*
      * Set the active field.
      *
      * Interlaced surfaces consist of two fields. Software
      * driven deinterlacing uses this method to manually switch
      * the field that is displayed, e.g. scaled up vertically
      * by two.
      */
     DFBResult (*SetField) (
          IDirectFBSurface                  *thiz,
          int                                field
     );

     /*
      * Clear the surface and its depth buffer if existent.
      *
      * Fills the whole (sub) surface with the specified color
      * while ignoring drawing flags and color of the current
      * state, but limited to the current clip.
      * As with all drawing and blitting functions the
      * backbuffer is written to. If you are initializing a
      * double buffered surface you may want to clear both
      * buffers by doing a Clear-Flip-Clear sequence.
      */
     DFBResult (*Clear) (
          IDirectFBSurface                  *thiz,
          u8                                 r,
          u8                                 g,
          u8                                 b,
          u8                                 a
     );

   /** Drawing/blitting control **/

     /*
      * Set the clipping region used to limit the area for
      * drawing, blitting and text functions.
      *
      * If no region is specified (NULL passed) the clip is set
      * to the surface extents (initial clip).
      */
     DFBResult (*SetClip) (
          IDirectFBSurface                  *thiz,
          const DFBRegion                   *clip
     );

     /*
      * Get the clipping region used to limit the area for
      * drawing, blitting and text functions.
      */
     DFBResult (*GetClip) (
          IDirectFBSurface                  *thiz,
          DFBRegion                         *ret_clip
     );

     /*
      * Set the color used for drawing/text functions or
      * alpha/color modulation (blitting functions).
      *
      * If you are not using the alpha value it should be set to
      * 0xff to ensure visibility when the code is ported to or
      * used for surfaces with an alpha channel.
      */
     DFBResult (*SetColor) (
          IDirectFBSurface                  *thiz,
          u8                                 r,
          u8                                 g,
          u8                                 b,
          u8                                 a
     );

     /*
      * Set the color like with SetColor() but using an index to
      * the color/alpha lookup table.
      *
      * This method is only supported by surfaces with an
      * indexed pixelformat, e.g. DSPF_LUT8. For these formats
      * this method should be used instead of SetColor().
      */
     DFBResult (*SetColorIndex) (
          IDirectFBSurface                  *thiz,
          unsigned int                       index
     );

     /*
      * Set the blend function that applies to the source.
      */
     DFBResult (*SetSrcBlendFunction) (
          IDirectFBSurface                  *thiz,
          DFBSurfaceBlendFunction            function
     );

     /*
      * Set the blend function that applies to the destination.
      */
     DFBResult (*SetDstBlendFunction) (
          IDirectFBSurface                  *thiz,
          DFBSurfaceBlendFunction            function
     );

     /*
      * Set the source and destination blend function by
      * specifying a Porter/Duff rule.
      */
     DFBResult (*SetPorterDuff) (
          IDirectFBSurface                  *thiz,
          DFBSurfacePorterDuffRule           rule
     );

     /*
      * Set the source color key, i.e. the color that is
      * excluded when blitting from this surface to another
      * that has source color keying enabled.
      */
     DFBResult (*SetSrcColorKey) (
          IDirectFBSurface                  *thiz,
          u8                                 r,
          u8                                 g,
          u8                                 b
     );

     /*
      * Set the source color key like with SetSrcColorKey() but
      * using an index to the color/alpha lookup table.
      *
      * This method is only supported by surfaces with an
      * indexed pixelformat, e.g. DSPF_LUT8. For these formats
      * this method should be used instead of SetSrcColorKey().
      */
     DFBResult (*SetSrcColorKeyIndex) (
          IDirectFBSurface                  *thiz,
          unsigned int                       index
     );

     /*
      * Set the destination color key, i.e. the only color that
      * gets overwritten by drawing and blitting to this surface
      * when destination color keying is enabled.
      */
     DFBResult (*SetDstColorKey) (
          IDirectFBSurface                  *thiz,
          u8                                 r,
          u8                                 g,
          u8                                 b
     );

     /*
      * Set the destination color key like with SetDstColorKey()
      * but using an index to the color/alpha lookup table.
      *
      * This method is only supported by surfaces with an
      * indexed pixelformat, e.g. DSPF_LUT8. For these formats
      * this method should be used instead of SetDstColorKey().
      */
     DFBResult (*SetDstColorKeyIndex) (
          IDirectFBSurface                  *thiz,
          unsigned int                       index
     );

   /** Blitting functions **/

     /*
      * Set the flags for all subsequent blitting commands.
      */
     DFBResult (*SetBlittingFlags) (
          IDirectFBSurface                  *thiz,
          DFBSurfaceBlittingFlags            flags
     );

     /*
      * Blit an area from the source to this surface.
      *
      * Pass a NULL rectangle to use the whole source surface.
      * Source may be the same surface.
      */
     DFBResult (*Blit) (
          IDirectFBSurface                  *thiz,
          IDirectFBSurface                  *source,
          const DFBRectangle                *source_rect,
          int                                x,
          int                                y
     );

     /*
      * Blit an area from the source tiled to this surface.
      *
      * Pass a NULL rectangle to use the whole source surface.
      * Source may be the same surface.
      */
     DFBResult (*TileBlit) (
          IDirectFBSurface                  *thiz,
          IDirectFBSurface                  *source,
          const DFBRectangle                *source_rect,
          int                                x,
          int                                y
     );

     /*
      * Blit a bunch of areas at once.
      *
      * Source may be the same surface.
      */
     DFBResult (*BatchBlit) (
          IDirectFBSurface                  *thiz,
          IDirectFBSurface                  *source,
          const DFBRectangle                *source_rects,
          const DFBPoint                    *dest_points,
          int                                num
     );

     /*
      * Blit an area scaled from the source to the destination
      * rectangle.
      *
      * Pass a NULL rectangle to use the whole source surface.
      */
     DFBResult (*StretchBlit) (
          IDirectFBSurface                  *thiz,
          IDirectFBSurface                  *source,
          const DFBRectangle                *source_rect,
          const DFBRectangle                *dest_rect
     );

     /*
      * Texture mapping support.
      *
      * Maps a 'texture' onto triangles being built from
      * 'vertices' according to the chosen 'formation'.
      * Optional 'indices' can be used to avoid rearrangement
      * of vertex lists, otherwise the vertex list is processed
      * consecutively, i.e. as if 'indices' are ascending
      * numbers starting at zero.
      * Either the number of 'indices' or the number of
      * 'vertices' is specified by 'num' and has to be three
      * at least. If the chosen 'formation' is DTTF_LIST,
      * it also has to be a multiple of three.
      */
     DFBResult (*TextureTriangles) (
          IDirectFBSurface                  *thiz,
          IDirectFBSurface                  *texture,
          const DFBVertex                   *vertices,
          const int                         *indices,
          int                                num,
          DFBTriangleFormation               formation
     );

   /** Drawing functions **/

     /*
      * Set the flags for all subsequent drawing commands.
      */
     DFBResult (*SetDrawingFlags) (
          IDirectFBSurface                  *thiz,
          DFBSurfaceDrawingFlags             flags
     );

     /*
      * Fill the specified rectangle with the given color
      * following the drawing flags.
      */
     DFBResult (*FillRectangle) (
          IDirectFBSurface                  *thiz,
          int                                x,
          int                                y,
          int                                w,
          int                                h
     );

     /*
      * Draw an outline of the specified rectangle with the
      * given color following the drawing flags.
      */
     DFBResult (*DrawRectangle) (
          IDirectFBSurface                  *thiz,
          int                                x,
          int                                y,
          int                                w,
          int                                h
     );

     /*
      * Draw a line from one point to the other with the given
      * color following the drawing flags.
      */
     DFBResult (*DrawLine) (
          IDirectFBSurface                  *thiz,
          int                                x1,
          int                                y1,
          int                                x2,
          int                                y2
     );

     /*
      * Draw lines.
      *
      * Draw 'num_lines' lines with the given color following
      * the drawing flags. Each line specified by a DFBRegion.
      */
     DFBResult (*DrawLines) (
          IDirectFBSurface                  *thiz,
          const DFBRegion                   *lines,
          unsigned int                       num_lines
     );

     /*
      * Fill a non-textured triangle with the given color
      * following the drawing flags.
      */
     DFBResult (*FillTriangle) (
          IDirectFBSurface                  *thiz,
          int                                x1,
          int                                y1,
          int                                x2,
          int                                y2,
          int                                x3,
          int                                y3
     );

     /*
      * Fill a bunch of rectangles with a single call.
      *
      * Fill 'num_rects' rectangles with the current color
      * following the drawing flags. Each rectangle specified
      * by a DFBRectangle.
      */
     DFBResult (*FillRectangles) (
          IDirectFBSurface                  *thiz,
          const DFBRectangle                *rects,
          unsigned int                       num_rects
     );

     /*
      * Fill spans.
      *
      * Fill 'num_spans' spans with the given color following
      * the drawing flags. Each span is specified by a DFBSpan.
      */
     DFBResult (*FillSpans) (
          IDirectFBSurface                  *thiz,
          int                                y,
          const DFBSpan                     *spans,
          unsigned int                       num_spans
     );

     /*
      * Fill a bunch of triangles with a single call.
      *
      * Fill 'num_tris' triangles with the current color
      * following the drawing flags. Each triangle specified
      * by a DFBTriangle.
      */
     DFBResult (*FillTriangles) (
          IDirectFBSurface                  *thiz,
          const DFBTriangle                 *tris,
          unsigned int                       num_tris
     );

   /** Text functions **/

     /*
      * Set the font used by DrawString() and DrawGlyph().
      * You can pass NULL here to unset the font.
      */
     DFBResult (*SetFont) (
          IDirectFBSurface                  *thiz,
          IDirectFBFont                     *font
     );

     /*
      * Get the font associated with a surface.
      *
      * This function increases the font's reference count.
      */
     DFBResult (*GetFont) (
          IDirectFBSurface                  *thiz,
          IDirectFBFont                    **ret_interface
     );

     /*
      * Draw a string at the specified position with the given
      * color following the specified flags.
      *
      * Bytes specifies the number of bytes to take from the
      * string or -1 for the complete NULL-terminated string.
      * You need to set a font using the SetFont() method before
      * calling this function.
      */
     DFBResult (*DrawString) (
          IDirectFBSurface                  *thiz,
          const char                        *text,
          int                                bytes,
          int                                x,
          int                                y,
          DFBSurfaceTextFlags                flags
     );

     /*
      * Draw a single glyph specified by its character code at
      * the specified position with the given color following
      * the specified flags.
      *
      * If font was loaded with the DFFA_NOCHARMAP flag, index
      * specifies the raw glyph index in the font.
      * You need to set a font using the SetFont() method before
      * calling this function.
      */
     DFBResult (*DrawGlyph) (
          IDirectFBSurface                  *thiz,
          unsigned int                       character,
          int                                x,
          int                                y,
          DFBSurfaceTextFlags                flags
     );

     /*
      * Change the encoding used for text rendering.
      */
     DFBResult (*SetEncoding) (
          IDirectFBSurface                  *thiz,
          DFBTextEncodingID                  encoding
     );

   /** Lightweight helpers **/

     /*
      * Get an interface to a sub area of this surface.
      *
      * No image data is duplicated, this is a clipped graphics
      * within the original surface. This is very helpful for
      * lightweight components in a GUI toolkit. The new
      * surface's state (color, drawing flags, etc.) is
      * independent from this one. So it's a handy graphics
      * context. If no rectangle is specified, the whole surface
      * (or a part if this surface is a subsurface itself) is
      * represented by the new one.
      */
     DFBResult (*GetSubSurface) (
          IDirectFBSurface                  *thiz,
          const DFBRectangle                *rect,
          IDirectFBSurface                 **ret_interface
     );

   /** OpenGL **/

     /*
      * Get a unique OpenGL context for this surface.
      */
     DFBResult (*GetGL) (
          IDirectFBSurface                  *thiz,
          IDirectFBGL                      **ret_interface
     );

   /** Debug **/

     /*
      * Dump the contents of the surface to one or two files.
      *
      * Create a PPM file containing the RGB data and a PGM file
      * with the alpha data if present.
      * The complete filenames will be
      * 'directory'/'prefix'_####.ppm for RGB and
      * 'directory'/'prefix'_####.pgm for the alpha channel
      * if present.
      * No existing files will be overwritten.
      */
     DFBResult (*Dump) (
          IDirectFBSurface                  *thiz,
          const char                        *directory,
          const char                        *prefix
     );

     /*
      * Disable hardware acceleration.
      *
      * If any function in 'mask' is set, acceleration will not
      * be used for it. Default is DFXL_NONE.
      */
     DFBResult (*DisableAcceleration) (
          IDirectFBSurface                  *thiz,
          DFBAccelerationMask                mask
     );

   /** Resources **/

     /*
      * Release possible reference to source surface.
      *
      * For performance reasons the last surface that has been
      * used for Blit() and others stays attached to the state
      * of the destination surface to save the overhead of
      * reprogramming the same values each time. The worst case
      * is generation of thumbnails using StretchBlit() from a
      * huge surface to a small one. The small thumbnail surface
      * keeps the big one alive, because no other blitting will
      * be done to the small surface afterwards. To solve this,
      * here is the method to use in such a case.
      */
     DFBResult (*ReleaseSource) (
          IDirectFBSurface                  *thiz
     );

   /** Blitting control **/

     /*
      * Set index translation table.
      *
      * Set the translation table used for fast indexed to
      * indexed pixel format conversion. A negative index means
      * that the pixel will not be written.
      * Undefined indices will be treated like negative ones.
      */
     DFBResult (*SetIndexTranslation) (
          IDirectFBSurface                  *thiz,
          const int                         *indices,
          int                                num_indices
     );

   /** Rendering **/

     /*
      * Set options affecting the output of drawing and blitting
      * operations.
      *
      * None of these is mandatory and therefore unsupported
      * flags will not cause a software fallback.
      */
     DFBResult (*SetRenderOptions) (
          IDirectFBSurface                  *thiz,
          DFBSurfaceRenderOptions            options
     );

   /** Drawing/blitting control **/

     /*
      * Set the transformation matrix.
      *
      * Enable usage of this matrix by setting DSRO_MATRIX via
      * SetRenderOptions().
      * The order in the array is from left to right and from
      * top to bottom.
      * All drawing and blitting will be transformed:
      *   X' = (X * v0 + Y * v1 + v2) / (X * v6 + Y * v7 + v8)
      *   Y' = (X * v3 + Y * v4 + v5) / (X * v6 + Y * v7 + v8)
      */
     DFBResult (*SetMatrix) (
          IDirectFBSurface                  *thiz,
          const s32                         *matrix
     );

     /*
      * Set the surface to be used as a mask for blitting.
      *
      * The 'mask' applies when DSBLIT_SRC_MASK_ALPHA or
      * DSBLIT_SRC_MASK_COLOR is used.
      * Depending on the 'flags' reading either starts at a
      * fixed location in the mask with absolute 'x' and 'y',
      * or at the same location as in the source, with 'x'and
      * and 'y' used as an offset.
      */
     DFBResult (*SetSourceMask) (
          IDirectFBSurface                  *thiz,
          IDirectFBSurface                  *mask,
          int                                x,
          int                                y,
          DFBSurfaceMaskFlags                flags
     );

   /** Lightweight helpers **/

     /*
      * Make this a sub surface or adjust the rectangle of this
      * sub surface.
      */
     DFBResult (*MakeSubSurface) (
          IDirectFBSurface                  *thiz,
          IDirectFBSurface                  *from,
          const DFBRectangle                *rect
     );

   /** Direct Write/Read **/

     /*
      * Write to the surface without the need for (Un)Lock().
      *
      * 'rect' defines the area inside the surface.
      * 'ptr' and 'pitch' specify the source.
      * The format of the surface and the source data must be
      * the same.
      */
     DFBResult (*Write) (
          IDirectFBSurface                  *thiz,
          const DFBRectangle                *rect,
          const void                        *ptr,
          int                                pitch
     );

     /*
      * Read from the surface without the need for (Un)Lock().
      *
      * 'rect' defines the area inside the surface to be read.
      * 'ptr' and 'pitch' specify the destination.
      * The destination data will have the same format as the
      * surface.
      */
     DFBResult (*Read) (
          IDirectFBSurface                  *thiz,
          const DFBRectangle                *rect,
          void                              *ptr,
          int                                pitch
     );

   /** Drawing/blitting control **/

     /*
      * Set color values used for drawing/text functions or
      * alpha/color modulation (blitting functions).
      */
     DFBResult (*SetColors) (
          IDirectFBSurface                  *thiz,
          const DFBColorID                  *ids,
          const DFBColor                    *colors,
          unsigned int                       num
     );

   /** Blitting functions **/

     /*
      * Blit a bunch of areas at once using secondary source for
      * reading instead of destination.
      *
      * Source may be the same surface.
      */
     DFBResult (*BatchBlit2) (
          IDirectFBSurface                  *thiz,
          IDirectFBSurface                  *source,
          IDirectFBSurface                  *source2,
          const DFBRectangle                *source_rects,
          const DFBPoint                    *dest_points,
          const DFBPoint                    *source2_points,
          int                                num
     );

   /** Buffer operations **/

     /*
      * Return the physical address of a locked surface.
      *
      * The surface must exist in a video memory pool.
      */
     DFBResult (*GetPhysicalAddress) (
          IDirectFBSurface                  *thiz,
          unsigned long                     *addr
     );

   /** Drawing functions **/

     /*
      * Fill a bunch of trapezoids with a single call.
      *
      * Fill 'num_traps' trapezoids with the current color
      * following the drawing flags. Each trapezoid specified
      * by a DFBTrapezoid.
      */
     DFBResult (*FillTrapezoids) (
          IDirectFBSurface                  *thiz,
          const DFBTrapezoid                *traps,
          unsigned int                       num_traps
     );

     /*
      * Fill quadrangles with the given color following the
      * drawing flags.
      */
     DFBResult (*FillQuadrangles) (
          IDirectFBSurface                  *thiz,
          const DFBPoint                    *points,
          unsigned int                       num_points
     );

   /** Extended color keys **/

     /*
      * Set extended source color keying.
      */
     DFBResult (*SetSrcColorKeyExtended) (
          IDirectFBSurface                  *thiz,
          const DFBColorKeyExtended         *colorkey_extended
     );

     /*
      * Set extended destination color keying.
      */
     DFBResult (*SetDstColorKeyExtended) (
          IDirectFBSurface                  *thiz,
          const DFBColorKeyExtended         *colorkey_extended
     );

   /** Drawing functions **/

     /*
      * Blit monochrome glyph data with attributes.
      */
     DFBResult (*DrawMonoGlyphs) (
           IDirectFBSurface                 *thiz,
           const void                       *glyph[],
           const DFBMonoGlyphAttributes     *attributes,
           const DFBPoint                   *dest_points,
           unsigned int                      num
     );

   /** Blitting control **/

     /*
      * Set the source color matrix.
      *
      * Enable usage of this matrix by setting
      * DSBLIT_SRC_COLORMATRIX via SetBlittingFlags().
      * The order in the array is from left to right and from
      * top to bottom.
      * All RGB values will be transformed:
      *   R' = R * v0 + G * v1 + B * v2  + v3
      *   G' = R * v4 + G * v5 + B * v6  + v7
      *   B' = R * v8 + G * v9 + B * v10 + v11
      */
     DFBResult (*SetSrcColorMatrix) (
          IDirectFBSurface                  *thiz,
          const s32                         *matrix
     );

     /*
      * Set the source convolution filter.
      *
      * Enable usage of this filter by setting
      * DSBLIT_SRC_CONVOLUTION via SetBlittingFlags().
      */
     DFBResult (*SetSrcConvolution) (
          IDirectFBSurface                  *thiz,
          const DFBConvolutionFilter        *filter
     );

   /** Retrieving information **/

     /*
      * Get the unique surface ID.
      */
     DFBResult (*GetID) (
          IDirectFBSurface                  *thiz,
          DFBSurfaceID                      *ret_surface_id
     );

   /** Process security **/

     /*
      * Allow access.
      */
     DFBResult (*AllowAccess) (
          IDirectFBSurface                  *thiz,
          const char                        *executable
     );

  /** Event buffers **/

     /*
      * Create an event buffer for this surface and attach it.
      */
     DFBResult (*CreateEventBuffer) (
          IDirectFBSurface                  *thiz,
          IDirectFBEventBuffer             **ret_interface
     );

     /*
      * Attach an existing event buffer to this surface.
      *
      * Note: attaching multiple times generates multiple
      * events.
      */
     DFBResult (*AttachEventBuffer) (
          IDirectFBSurface                  *thiz,
          IDirectFBEventBuffer              *buffer
     );

     /*
      * Detach an event buffer from this surface.
      */
     DFBResult (*DetachEventBuffer) (
          IDirectFBSurface                  *thiz,
          IDirectFBEventBuffer              *buffer
     );

   /** Blitting functions **/
     /*
      * Blit a bunch of areas scaled from the source to the
      * destination rectangles.
      *
      * 'source_rects' and 'dest_rects' will be modified.
      */
     DFBResult (*BatchStretchBlit) (
          IDirectFBSurface                  *thiz,
          IDirectFBSurface                  *source,
          const DFBRectangle                *source_rects,
          const DFBRectangle                *dest_rects,
          int                                num
     );

   /** Client **/

     /*
      * Put in client mode for frame synchronization.
      */
     DFBResult (*MakeClient) (
          IDirectFBSurface                  *thiz
     );

     /*
      * Acknowledge frame.
      */
     DFBResult (*FrameAck) (
          IDirectFBSurface                  *thiz,
          u32                                flip_count
     );

   /** Debug **/

     /*
      * Dump the contents of the surface to one or two files in
      * raw format.
      *
      * Create a RAW file containing the ARGB data.
      * The complete filename will be:
      * 'directory'/'prefix'_####.raw for ARGB.
      * No existing files will be overwritten.
      */
     DFBResult (*DumpRaw) (
          IDirectFBSurface                  *thiz,
          const char                        *directory,
          const char                        *prefix
     );

   /** Timing **/

     /*
      * Retrieve timestamp for next frame to be rendered.
      */
     DFBResult (*GetFrameTime) (
          IDirectFBSurface                  *thiz,
          long long                         *ret_micros
     );

     /*
      * Set configuration for GetFrameTime().
      */
     DFBResult (*SetFrameTimeConfig) (
          IDirectFBSurface                  *thiz,
          const DFBFrameTimeConfig          *config
     );

  /** Allocations **/

     /*
      * Add implementation specific handle as buffer allocation.
      */
     DFBResult (*Allocate) (
          IDirectFBSurface                  *thiz,
          DFBSurfaceBufferRole               role,
          DFBSurfaceStereoEye                eye,
          const char                        *key,
          u64                                handle,
          IDirectFBSurfaceAllocation       **ret_interface
     );

     /*
      * Acquire implementation specific handle from surface.
      */
     DFBResult (*GetAllocation) (
          IDirectFBSurface                  *thiz,
          DFBSurfaceBufferRole               role,
          DFBSurfaceStereoEye                eye,
          const char                        *key,
          IDirectFBSurfaceAllocation       **ret_interface
     );

     /*
      * Acquire implementation specific handles from surface.
      */
     DFBResult (*GetAllocations) (
          IDirectFBSurface                  *thiz,
          const char                        *key,
          unsigned int                       max_num,
          unsigned int                      *ret_num,
          IDirectFBSurfaceAllocation       **ret_interface_left,
          IDirectFBSurfaceAllocation       **ret_interface_right
     );

   /** Interface **/

     /*
      * Flush pending drawing operations.
      *
      * This function flushes the internal buffer like done
      * implicitly by Flip().
      * This can be used to transfer the interface to another
      * thread, as call buffers are bound to each thread.
      */
     DFBResult (*Flush) (
          IDirectFBSurface                  *thiz
     );
)

/******************************
 * IDirectFBSurfaceAllocation *
 ******************************/

/*
 * IDirectFBSurfaceAllocation is the surface allocation
 * interface.
 */
D_DEFINE_INTERFACE( IDirectFBSurfaceAllocation,

   /** Retrieving information **/

     /*
      * Get a description of the allocation's surface.
      */
     DFBResult (*GetDescription) (
          IDirectFBSurfaceAllocation        *thiz,
          DFBSurfaceDescription             *ret_desc
     );

   /** Access **/

     /*
      * Get the surface allocation data.
      */
     DFBResult (*GetHandle) (
          IDirectFBSurfaceAllocation        *thiz,
          u64                               *ret_handle
     );

     /*
      * Get the surface allocation pitch.
      */
     DFBResult (*GetPitch) (
          IDirectFBSurfaceAllocation        *thiz,
          int                               *ret_pitch
     );

     /*
      * Updated the allocation's surface.
      */
     DFBResult (*Updated) (
          IDirectFBSurfaceAllocation        *thiz,
          const DFBBox                      *updates,
          unsigned int                       num_updates
     );
)

/********************
 * IDirectFBPalette *
 ********************/

/*
 * IDirectFBPalette is the palette interface.
 */
D_DEFINE_INTERFACE( IDirectFBPalette,

   /** Retrieving information **/

     /*
      * Return the capabilities of this palette.
      */
     DFBResult (*GetCapabilities) (
          IDirectFBPalette                  *thiz,
          DFBPaletteCapabilities            *ret_caps
     );

     /*
      * Get the number of entries in the palette.
      */
     DFBResult (*GetSize) (
          IDirectFBPalette                  *thiz,
          unsigned int                      *ret_size
     );

   /** Palette entries **/

     /*
      * Write entries to the palette.
      *
      * Writes the specified number of entries to the palette
      * at the specified offset.
      */
     DFBResult (*SetEntries) (
          IDirectFBPalette                  *thiz,
          const DFBColor                    *entries,
          unsigned int                       num_entries,
          unsigned int                       offset
     );

     /*
      * Read entries from the palette.
      *
      * Reads the specified number of entries from the palette
      * at the specified offset.
      */
     DFBResult (*GetEntries) (
          IDirectFBPalette                  *thiz,
          DFBColor                          *ret_entries,
          unsigned int                       num_entries,
          unsigned int                       offset
     );

     /*
      * Find the best matching entry.
      *
      * Searches the map for an entry which best matches the
      * specified color.
      */
     DFBResult (*FindBestMatch) (
          IDirectFBPalette                  *thiz,
          u8                                 r,
          u8                                 g,
          u8                                 b,
          u8                                 a,
          unsigned int                      *ret_index
     );

   /** Clone **/

     /*
      * Create a copy of the palette.
      */
     DFBResult (*CreateCopy) (
          IDirectFBPalette                  *thiz,
          IDirectFBPalette                 **ret_interface
     );


   /** YUV Palette **/

     /*
      * Write entries to the palette.
      *
      * Writes the specified number of entries to the palette
      * at the specified offset.
      */
     DFBResult (*SetEntriesYUV) (
          IDirectFBPalette                  *thiz,
          const DFBColorYUV                 *entries,
          unsigned int                       num_entries,
          unsigned int                       offset
     );

     /*
      * Read entries from the palette.
      *
      * Reads the specified number of entries from the palette
      * at the specified offset.
      */
     DFBResult (*GetEntriesYUV) (
          IDirectFBPalette                  *thiz,
          DFBColorYUV                       *ret_entries,
          unsigned int                       num_entries,
          unsigned int                       offset
     );

     /*
      * Find the best matching entry.
      *
      * Searches the map for an entry which best matches the
      * specified color.
      */
     DFBResult (*FindBestMatchYUV) (
          IDirectFBPalette                  *thiz,
          u8                                 y,
          u8                                 u,
          u8                                 v,
          u8                                 a,
          unsigned int                      *ret_index
     );
)

/************************
 * IDirectFBInputDevice *
 ************************/

/*
 * Specifies whether a key is currently down.
 */
typedef enum {
     DIKS_UP                               = 0x00000000,         /* key is not pressed */
     DIKS_DOWN                             = 0x00000001          /* key is pressed */
} DFBInputDeviceKeyState;

/*
 * Flags specifying which modifiers are currently pressed.
 */
typedef enum {
     DIMM_SHIFT                            = (1<<DIMKI_SHIFT),   /* Shift key is pressed */
     DIMM_CONTROL                          = (1<<DIMKI_CONTROL), /* Control key is pressed */
     DIMM_ALT                              = (1<<DIMKI_ALT),     /* Alt key is pressed */
     DIMM_ALTGR                            = (1<<DIMKI_ALTGR),   /* AltGr key is pressed */
     DIMM_META                             = (1<<DIMKI_META),    /* Meta key is pressed */
     DIMM_SUPER                            = (1<<DIMKI_SUPER),   /* Super key is pressed */
     DIMM_HYPER                            = (1<<DIMKI_HYPER)    /* Hyper key is pressed */
} DFBInputDeviceModifierMask;

/*
 * Flags specifying which buttons are currently down.
 */
typedef enum {
     DIBM_LEFT                             = 0x00000001,         /* left mouse button */
     DIBM_RIGHT                            = 0x00000002,         /* right mouse button */
     DIBM_MIDDLE                           = 0x00000004          /* middle mouse button */
} DFBInputDeviceButtonMask;

/*
 * Specifies whether a button is currently pressed.
 */
typedef enum {
     DIBS_UP                               = 0x00000000,         /* button is not pressed */
     DIBS_DOWN                             = 0x00000001          /* button is pressed */
} DFBInputDeviceButtonState;

/*
 * Input device configuration flags.
 */
typedef enum {
     DIDCONF_NONE                          = 0x00000000,         /* None of these. */

     DIDCONF_SENSITIVITY                   = 0x00000001,         /* Set sensitivity. */
     DIDCONF_MAX_SLOTS                     = 0x00000002,         /* Set the number of possible touch contacts. */

     DIDCONF_ALL                           = 0x00000003          /* All of these. */
} DFBInputDeviceConfigFlags;

/*
 * Input device configuration.
 */
typedef struct {
     DFBInputDeviceConfigFlags               flags;              /* Validation of fields. */

     int                                     sensitivity;        /* Sensitivity value for X/Y axes. */
     int                                     max_slots;          /* Maximum mumber of possible touch contacts. */
} DFBInputDeviceConfig;

/*
 * IDirectFBInputDevice is the input device interface.
 */
D_DEFINE_INTERFACE( IDirectFBInputDevice,

   /** Retrieving information **/

     /*
      * Get the unique device ID.
      */
     DFBResult (*GetID) (
          IDirectFBInputDevice              *thiz,
          DFBInputDeviceID                  *ret_device_id
     );

     /*
      * Get a description of this device, i.e. the capabilities.
      */
     DFBResult (*GetDescription) (
          IDirectFBInputDevice              *thiz,
          DFBInputDeviceDescription         *ret_desc
     );

   /** Key mapping **/

     /*
      * Fetch one entry from the keymap for a specific hardware
      * keycode.
      */
     DFBResult (*GetKeymapEntry) (
          IDirectFBInputDevice              *thiz,
          int                                keycode,
          DFBInputDeviceKeymapEntry         *ret_entry
     );

     /*
      * Set one entry of the keymap to the specified entry.
      *
      * Each entry has 4 modifier combinations for going from
      * key to symbol.
      */
     DFBResult (*SetKeymapEntry) (
          IDirectFBInputDevice              *thiz,
          int                                keycode,
          DFBInputDeviceKeymapEntry         *entry
     );

     /*
      * Load a keymap from the specified file.
      *
      * All specified keys will overwrite the current keymap.
      * On return of an error, the keymap is in an unspecified
      * state. The file must be ASCII containing lines:
      * keycode <hw code> = <key id> = <symbol> .... (up to 4)
      */
     DFBResult (*LoadKeymap) (
          IDirectFBInputDevice              *thiz,
          char                              *filename
     );

   /** Event buffers **/

     /*
      * Create an event buffer for this device and attach it.
      */
     DFBResult (*CreateEventBuffer) (
          IDirectFBInputDevice              *thiz,
          IDirectFBEventBuffer             **ret_interface
     );

     /*
      * Attach an existing event buffer to this device.
      *
      * Note: attaching multiple times generates multiple
      * events.
      *
      */
     DFBResult (*AttachEventBuffer) (
          IDirectFBInputDevice              *thiz,
          IDirectFBEventBuffer              *buffer
     );

     /*
      * Detach an event buffer from this device.
      */
     DFBResult (*DetachEventBuffer) (
          IDirectFBInputDevice              *thiz,
          IDirectFBEventBuffer              *buffer
     );

   /** General state queries **/

     /*
      * Get the current state of one key.
      */
     DFBResult (*GetKeyState) (
          IDirectFBInputDevice              *thiz,
          DFBInputDeviceKeyIdentifier        key_id,
          DFBInputDeviceKeyState            *ret_state
     );

     /*
      * Get the current modifier mask.
      */
     DFBResult (*GetModifiers) (
          IDirectFBInputDevice              *thiz,
          DFBInputDeviceModifierMask        *ret_modifiers
     );

     /*
      * Get the current state of the key locks.
      */
     DFBResult (*GetLockState) (
          IDirectFBInputDevice              *thiz,
          DFBInputDeviceLockState           *ret_locks
     );

     /*
      * Get a mask of currently pressed buttons.
      *
      * The first button corrensponds to the right most bit.
      */
     DFBResult (*GetButtons) (
          IDirectFBInputDevice              *thiz,
          DFBInputDeviceButtonMask          *ret_buttons
     );

     /*
      * Get the state of a button.
      */
     DFBResult (*GetButtonState) (
          IDirectFBInputDevice              *thiz,
          DFBInputDeviceButtonIdentifier     button,
          DFBInputDeviceButtonState         *ret_state
     );

     /*
      * Get the current value of the specified axis.
      */
     DFBResult (*GetAxis) (
          IDirectFBInputDevice              *thiz,
          DFBInputDeviceAxisIdentifier       axis,
          int                               *ret_pos
     );

   /** Specialized queries **/

     /*
      * Utility function combining two calls to GetAxis().
      *
      * You may leave one of the x/y arguments NULL.
      */
     DFBResult (*GetXY) (
          IDirectFBInputDevice              *thiz,
          int                               *ret_x,
          int                               *ret_y
     );

   /** Configuration **/

     /*
      * Change config values for the input device.
      */
     DFBResult (*SetConfiguration) (
          IDirectFBInputDevice              *thiz,
          const DFBInputDeviceConfig        *config
     );
)

/************************
 * IDirectFBEventBuffer *
 ************************/

/*
 * Event class.
 */
typedef enum {
     DFEC_NONE                             = 0x00000000,         /* none of these */

     DFEC_INPUT                            = 0x00000001,         /* raw input event */
     DFEC_WINDOW                           = 0x00000002,         /* windowing event */
     DFEC_USER                             = 0x00000003,         /* custom event for the user */
     DFEC_UNIVERSAL                        = 0x00000004,         /* universal event for custom usage
                                                                    with variable size */
     DFEC_VIDEOPROVIDER                    = 0x00000005,         /* video provider event */
     DFEC_SURFACE                          = 0x00000006          /* surface event */
} DFBEventClass;

/*
 * The type of an input event.
 */
typedef enum {
     DIET_UNKNOWN                          = 0x00000000,         /* unknown event */

     DIET_KEYPRESS                         = 0x00000001,         /* a key is been pressed */
     DIET_KEYRELEASE                       = 0x00000002,         /* a key is been released */
     DIET_BUTTONPRESS                      = 0x00000003,         /* a (mouse) button is been pressed */
     DIET_BUTTONRELEASE                    = 0x00000004,         /* a (mouse) button is been released */
     DIET_AXISMOTION                       = 0x00000005,         /* mouse/joystick movement */
} DFBInputEventType;

/*
 * Flags defining which additional (optional) event fields are
 * valid.
 */
typedef enum {
     DIEF_NONE                             = 0x00000000,         /* No additional fields. */

     DIEF_TIMESTAMP                        = 0x00000001,         /* Timestamp is valid. */
     DIEF_AXISABS                          = 0x00000002,         /* Axis and axisabs are valid. */
     DIEF_AXISREL                          = 0x00000004,         /* Axis and axisrel are valid. */
     DIEF_KEYCODE                          = 0x00000008,         /* Used internally by the input core,
                                                                    always set at application level. */
     DIEF_KEYID                            = 0x00000010,         /* Used internally by the input core,
                                                                    always set at application level. */
     DIEF_KEYSYMBOL                        = 0x00000020,         /* Used internally by the input core,
                                                                    always set at application level. */
     DIEF_MODIFIERS                        = 0x00000040,         /* Used internally by the input core,
                                                                    always set at application level. */
     DIEF_LOCKS                            = 0x00000080,         /* Used internally by the input core,
                                                                    always set at application level. */
     DIEF_BUTTONS                          = 0x00000100,         /* Used internally by the input core,
                                                                    always set at application level. */
     DIEF_GLOBAL                           = 0x00000200,         /* Only for event buffers created by
                                                                    CreateInputEventBuffer() with global events enabled.
                                                                    Indicates that the event would have been filtered if
                                                                    the buffer hadn't been global. */
     DIEF_REPEAT                           = 0x00000400,         /* Repeated event, e.g. key or button press. */
     DIEF_FOLLOW                           = 0x00000800,         /* Another event will follow immediately,
                                                                    e.g. x/y axis. */
     DIEF_MIN                              = 0x00001000,         /* Minimum value is set,
                                                                    e.g. for absolute axis motion. */
     DIEF_MAX                              = 0x00002000          /* Maximum value is set,
                                                                    e.g. for absolute axis motion. */
} DFBInputEventFlags;

/*
 * An input event, item of an input buffer.
 */
typedef struct {
     DFBEventClass                           clazz;              /* clazz of event */
     DFBInputEventType                       type;               /* type of event */
     DFBInputDeviceID                        device_id;          /* source of event */
     DFBInputEventFlags                      flags;              /* which optional fields are valid */
     struct timeval                          timestamp;          /* time of event creation */
     int                                     key_code;           /* hardware keycode, no mapping, -1 if device
                                                                    doesn't differentiate between several keys */
     DFBInputDeviceKeyIdentifier             key_id;             /* basic mapping, modifier independent */
     DFBInputDeviceKeySymbol                 key_symbol;         /* advanced mapping, unicode compatible,
                                                                    modifier dependent */
     DFBInputDeviceModifierMask              modifiers;          /* pressed modifiers (optional) */
     DFBInputDeviceLockState                 locks;              /* active locks (optional) */
     DFBInputDeviceButtonIdentifier          button;             /* in case of a button event */
     DFBInputDeviceButtonMask                buttons;            /* mask of currently pressed buttons */
     DFBInputDeviceAxisIdentifier            axis;               /* in case of an axis event */
     int                                     axisabs;            /* absolute mouse/joystick coordinate */
     int                                     axisrel;            /* relative mouse/joystick movement */
     int                                     min;                /* minimum possible value */
     int                                     max;                /* maximum possible value */
     int                                     slot_id;            /* touch contact */
} DFBInputEvent;

/*
 * Window Event Types.
 *
 * Can also be used as flags for event filters.
 */
typedef enum {
     DWET_NONE                             = 0x00000000,         /* None of these. */

     DWET_POSITION                         = 0x00000001,         /* Window has been moved by window manager or the
                                                                    application itself. */
     DWET_SIZE                             = 0x00000002,         /* Window has been resized by window manager or the
                                                                    application itself. */
     DWET_CLOSE                            = 0x00000004,         /* Closing this window has been requested only. */
     DWET_DESTROYED                        = 0x00000008,         /* Window got destroyed by global deinitialization
                                                                    function or the application itself. */
     DWET_GOTFOCUS                         = 0x00000010,         /* Window got focus. */
     DWET_LOSTFOCUS                        = 0x00000020,         /* Window lost focus. */

     DWET_KEYDOWN                          = 0x00000100,         /* A key has gone down while window has focus. */
     DWET_KEYUP                            = 0x00000200,         /* A key has gone up while window has focus. */

     DWET_BUTTONDOWN                       = 0x00010000,         /* Mouse button went down in the window. */
     DWET_BUTTONUP                         = 0x00020000,         /* Mouse button went up in the window. */
     DWET_MOTION                           = 0x00040000,         /* Mouse cursor changed its position in window. */
     DWET_ENTER                            = 0x00080000,         /* Mouse cursor entered the window. */
     DWET_LEAVE                            = 0x00100000,         /* Mouse cursor left the window. */
     DWET_WHEEL                            = 0x00200000,         /* Mouse wheel was moved while window has focus. */

     DWET_POSITION_SIZE                    = DWET_POSITION |
                                             DWET_SIZE,          /* Initially sent to window when it's created. */

     DWET_UPDATE                           = 0x01000000,         /* Update event (upon Flip), this way another process
                                                                    can capture the output of the application. */

     DWET_ALL                              = 0x013F033F          /* All event types. */
} DFBWindowEventType;

/*
 * Flags for a window event.
 */
typedef enum {
     DWEF_NONE                             = 0x00000000,         /* None of these. */

     DWEF_RETURNED                         = 0x00000001,         /* This is a returned event, e.g. unconsumed key. */
     DWEF_RELATIVE                         = 0x00000002,         /* This is a relative motion event. */
     DWEF_REPEAT                           = 0x00000010,         /* This is a repeat event, e.g. repeating key */
     DWEF_DEVICE_ID                        = 0x00000020,         /* device_id field of DFBInputEvent is valid. */

     DWEF_ALL                              = 0x00000033          /* All of these. */
} DFBWindowEventFlags;

/*
 * Event from the windowing system.
 */
typedef struct {
     DFBEventClass                           clazz;              /* clazz of event */
     DFBWindowEventType                      type;               /* type of event */

     /* used by DWET_KEYDOWN, DWET_KEYUP */
     DFBWindowEventFlags                     flags;              /* event flags */
     DFBWindowID                             window_id;          /* window id */

     /* used by DWET_MOVE, DWET_MOTION, DWET_BUTTONDOWN,
        DWET_BUTTONUP, DWET_ENTER, DWET_LEAVE */
     int                                     x;                  /* x position of window or coordinate within window */
     int                                     y;                  /* y position of window or coordinate within window */

     /* used by DWET_MOTION, DWET_BUTTONDOWN, DWET_BUTTONUP,
        DWET_ENTER, DWET_LEAVE */
     int                                     cx;                 /* x cursor position */
     int                                     cy;                 /* y cursor position */

     /* used by DWET_WHEEL */
     int                                     step;               /* wheel step */

     /* used by DWET_RESIZE */
     int                                     w;                  /* width of window */
     int                                     h;                  /* height of window */

     /* used by DWET_KEYDOWN, DWET_KEYUP */
     int                                     key_code;           /* hardware keycode, no mapping, -1 if device doesn't
                                                                    differentiate between several keys */
     DFBInputDeviceKeyIdentifier             key_id;             /* basic mapping, modifier independent */
     DFBInputDeviceKeySymbol                 key_symbol;         /* advanced mapping, unicode compatible,
                                                                    modifier dependent */
     DFBInputDeviceModifierMask              modifiers;          /* pressed modifiers */
     DFBInputDeviceLockState                 locks;              /* active locks */

     /* used by DWET_BUTTONDOWN, DWET_BUTTONUP */
     DFBInputDeviceButtonIdentifier          button;             /* button being pressed or released */

     /* used by DWET_MOTION, DWET_BUTTONDOWN, DWET_BUTTONUP */
     DFBInputDeviceButtonMask                buttons;            /* mask of currently pressed buttons */

     struct timeval                          timestamp;          /* time stamp */
     DFBInputDeviceID                        device_id;          /* source of event */
} DFBWindowEvent;

/*
 * Event for usage by the user.
 */
typedef struct {
     DFBEventClass                           clazz;              /* clazz of event */
     unsigned int                            type;               /* custom type */
     void                                   *data;               /* custom data */
} DFBUserEvent;

/*
 * Universal event for custom usage with variable size.
 */
typedef struct {
     DFBEventClass                           clazz;              /* clazz of event */
     unsigned int                            size;               /* size of this event, minimum is
                                                                    sizeof(DFBUniversalEvent) */

     /* custom data follows, size of this data is
        'size' - sizeof(DFBUniversalEvent) */
} DFBUniversalEvent;

/*
 * Video Provider Event Types.
 *
 * Can also be used as flags for event filters.
 */
typedef enum {
     DVPET_NONE                            = 0x00000000,         /* None of these. */

     DVPET_STARTED                         = 0x00000001,         /* The video provider has started the playback. */
     DVPET_STOPPED                         = 0x00000002,         /* The video provider has stopped the playback. */
     DVPET_SPEEDCHANGE                     = 0x00000004,         /* A speed change has occurred. */
     DVPET_STREAMCHANGE                    = 0x00000008,         /* A stream description change has occurred. */
     DVPET_FATALERROR                      = 0x00000010,         /* A fatal error has occurred: restart must be done. */
     DVPET_FINISHED                        = 0x00000020,         /* The video provider has finished the playback. */
     DVPET_SURFACECHANGE                   = 0x00000040,         /* A surface description change has occurred. */
     DVPET_FRAMEDECODED                    = 0x00000080,         /* A frame has been decoded by the decoder. */
     DVPET_FRAMEDISPLAYED                  = 0x00000100,         /* A frame has been rendered to the output. */
     DVPET_DATAEXHAUSTED                   = 0x00000200,         /* There is no more data available for consumption. */
     DVPET_VIDEOACTION                     = 0x00000400,         /* An action is required on the video provider. */
     DVPET_DATALOW                         = 0x00000800,         /* The stream buffer is running low in data. */
     DVPET_DATAHIGH                        = 0x00001000,         /* The stream buffer is high. */
     DVPET_BUFFERTIMELOW                   = 0x00002000,         /* The stream buffer has less than requested playout
                                                                    time buffered. */
     DVPET_BUFFERTIMEHIGH                  = 0x00004000,         /* The stream buffer has more than requested playout
                                                                    time buffered. */

     DVPET_ALL                             = 0x00007FFF          /* All event types. */
} DFBVideoProviderEventType;

/*
 * Video Provider Event Types.
 *
 * Can also be used as flags for event filters.
 */
typedef enum {
     DVPEDST_UNKNOWN                       = 0x00000000,         /* Event is valid for unknown data. */

     DVPEDST_AUDIO                         = 0x00000001,         /* Event is valid for audio. */
     DVPEDST_VIDEO                         = 0x00000002,         /* Event is valid for video. */
     DVPEDST_DATA                          = 0x00000004,         /* Event is valid for data. */

     DVPEDST_ALL                           = 0x00000007          /* Event is valid for all data types. */

} DFBVideoProviderEventDataSubType;

/*
 * Event from the video provider
 */
typedef struct {
     DFBEventClass                           clazz;              /* clazz of event */
     DFBVideoProviderEventType               type;               /* type of event */
     DFBVideoProviderEventDataSubType        data_type;          /* data type that this event is applicable for */
     int                                     data[4];            /* custom data */
} DFBVideoProviderEvent;

/*
 * Surface Event Types.
 *
 * Can also be used as flags for event filters.
 */
typedef enum {
     DSEVT_NONE                            = 0x00000000,         /* None of these. */

     DSEVT_DESTROYED                       = 0x00000001,         /* Surface got destroyed by global deinitialization
                                                                    function or the application itself. */
     DSEVT_UPDATE                          = 0x00000002,         /* Update event. */

     DSEVT_ALL                             = 0x00000003          /* All event types. */
} DFBSurfaceEventType;

/*
 * Event from surface
 */
typedef struct {
     DFBEventClass                           clazz;              /* clazz of event */

     /* all types */
     DFBSurfaceEventType                     type;               /* type of event */
     DFBSurfaceID                            surface_id;         /* source of event */
     long long                               time_stamp;         /* time stamp */

     /* DSEVT_UPDATE */
     DFBRegion                               update;             /* region update */
     DFBRegion                               update_right;       /* right region update */
     unsigned int                            flip_count;         /* serial number of frame, modulo number of buffers */
     DFBSurfaceFlipFlags                     flip_flags;         /* flipping flags */
} DFBSurfaceEvent;

/*
 * General container for events.
 */
typedef union {
     DFBEventClass                           clazz;              /* clazz of event */
     DFBInputEvent                           input;              /* field for input events */
     DFBWindowEvent                          window;             /* field for window events */
     DFBUserEvent                            user;               /* field for user-defined events */
     DFBUniversalEvent                       universal;          /* field for universal events */
     DFBVideoProviderEvent                   videoprovider;      /* field for video provider */
     DFBSurfaceEvent                         surface;            /* field for surface events */
} DFBEvent;

#define DFB_EVENT(e)                         ((DFBEvent*) (e))

/*
 * Statistics about event buffer queue.
 */
typedef struct {
     unsigned int                            num_events;         /* Total number of events in the queue. */

     unsigned int                            DFEC_INPUT;         /* Number of input events. */
     unsigned int                            DFEC_WINDOW;        /* Number of window events. */
     unsigned int                            DFEC_USER;          /* Number of user events. */
     unsigned int                            DFEC_UNIVERSAL;     /* Number of universal events. */
     unsigned int                            DFEC_VIDEOPROVIDER; /* Number of video provider events. */

     unsigned int                            DIET_KEYPRESS;
     unsigned int                            DIET_KEYRELEASE;
     unsigned int                            DIET_BUTTONPRESS;
     unsigned int                            DIET_BUTTONRELEASE;
     unsigned int                            DIET_AXISMOTION;

     unsigned int                            DWET_POSITION;
     unsigned int                            DWET_SIZE;
     unsigned int                            DWET_CLOSE;
     unsigned int                            DWET_DESTROYED;
     unsigned int                            DWET_GOTFOCUS;
     unsigned int                            DWET_LOSTFOCUS;
     unsigned int                            DWET_KEYDOWN;
     unsigned int                            DWET_KEYUP;
     unsigned int                            DWET_BUTTONDOWN;
     unsigned int                            DWET_BUTTONUP;
     unsigned int                            DWET_MOTION;
     unsigned int                            DWET_ENTER;
     unsigned int                            DWET_LEAVE;
     unsigned int                            DWET_WHEEL;
     unsigned int                            DWET_POSITION_SIZE;

     unsigned int                            DVPET_STARTED;
     unsigned int                            DVPET_STOPPED;
     unsigned int                            DVPET_SPEEDCHANGE;
     unsigned int                            DVPET_STREAMCHANGE;
     unsigned int                            DVPET_FATALERROR;
     unsigned int                            DVPET_FINISHED;
     unsigned int                            DVPET_SURFACECHANGE;
     unsigned int                            DVPET_FRAMEDECODED;
     unsigned int                            DVPET_FRAMEDISPLAYED;
     unsigned int                            DVPET_DATAEXHAUSTED;
     unsigned int                            DVPET_DATALOW;
     unsigned int                            DVPET_VIDEOACTION;
     unsigned int                            DVPET_DATAHIGH;
     unsigned int                            DVPET_BUFFERTIMELOW;
     unsigned int                            DVPET_BUFFERTIMEHIGH;
} DFBEventBufferStats;

/*
 * IDirectFBEventBuffer is the event buffer interface.
 */
D_DEFINE_INTERFACE( IDirectFBEventBuffer,

   /** Buffer handling **/

     /*
      * Clear all events stored in this buffer.
      */
     DFBResult (*Reset) (
          IDirectFBEventBuffer              *thiz
     );

   /** Waiting for events **/

     /*
      * Wait for the next event to occur.
      *
      * Thread is idle in the meantime.
      */
     DFBResult (*WaitForEvent) (
          IDirectFBEventBuffer              *thiz
     );

     /*
      * Block until next event to occur or timeout is reached.
      *
      * Thread is idle in the meantime.
      */
     DFBResult (*WaitForEventWithTimeout) (
          IDirectFBEventBuffer              *thiz,
          unsigned int                       seconds,
          unsigned int                       milli_seconds
     );

   /** Fetching events **/

     /*
      * Get the next event and remove it from the FIFO.
      */
     DFBResult (*GetEvent) (
          IDirectFBEventBuffer              *thiz,
          DFBEvent                          *ret_event
     );

     /*
      * Get the next event but leave it there.
      */
     DFBResult (*PeekEvent) (
          IDirectFBEventBuffer              *thiz,
          DFBEvent                          *ret_event
     );

     /*
      * Check if there is a pending event in the queue.
      */
     DFBResult (*HasEvent) (
          IDirectFBEventBuffer              *thiz
     );

   /** Sending events **/

     /*
      * Put an event into the FIFO.
      *
      * This function does not wait until the event got fetched.
      */
     DFBResult (*PostEvent) (
          IDirectFBEventBuffer              *thiz,
          const DFBEvent                    *event
     );

     /*
      * Wake up any thread waiting for events in this buffer.
      *
      * This method causes any WaitForEvent() or
      * WaitForEventWithTimeout() call to return with
      * DFB_INTERRUPTED.
      */
     DFBResult (*WakeUp) (
          IDirectFBEventBuffer              *thiz
     );

   /** Special handling **/

     /*
      * Create a file descriptor for reading events.
      *
      * This method provides an alternative for reading events
      * from an event buffer. It creates a file descriptor which
      * can be used in select(), poll() or read().
      * In general only non-threaded applications which already
      * use select() or poll() need it.
      * This method flushes the event buffer. After calling this
      * method all other methods except PostEvent() will return
      * DFB_UNSUPPORTED.
      * Calling this method again will return DFB_BUSY.
      */
     DFBResult (*CreateFileDescriptor) (
          IDirectFBEventBuffer              *thiz,
          int                               *ret_fd
     );

   /** Statistics **/

     /*
      * Enable/disable collection of event buffer statistics.
      */
     DFBResult (*EnableStatistics) (
          IDirectFBEventBuffer              *thiz,
          DFBBoolean                         enable
     );

     /*
      * Query collected event buffer statistics.
      */
     DFBResult (*GetStatistics) (
          IDirectFBEventBuffer              *thiz,
          DFBEventBufferStats               *ret_stats
     );
)

/*******************
 * IDirectFBWindow *
 *******************/

/*
 * The key selection defines a mode for filtering keys while the
 * window is having the focus.
 */
typedef enum {
     DWKS_ALL                              = 0x00000000,         /* Select all keys (default). */
     DWKS_NONE                             = 0x00000001,         /* Don't select any key. */
     DWKS_LIST                             = 0x00000002          /* Select a list of keys. */
} DFBWindowKeySelection;

/*
 * Window geometry mode.
 */
typedef enum {
     DWGM_DEFAULT                          = 0x00000000,         /* Use default values. */
     DWGM_FOLLOW                           = 0x00000001,         /* Use values of parent window. */
     DWGM_RECTANGLE                        = 0x00000002,         /* Use pixel values as defined. */
     DWGM_LOCATION                         = 0x00000003          /* Use relative values as defined. */
} DFBWindowGeometryMode;

/*
 * Window geometry for positioning and scaling of windows
 * relative to their own bounds.
 */
typedef struct {
     DFBWindowGeometryMode                   mode;               /* geometry mode */
     DFBRectangle                            rectangle;          /* rectangle */
     DFBLocation                             location;           /* location */
} DFBWindowGeometry;

/*
 * Flags for window cursor.
 */
typedef enum {
     DWCF_NONE                             = 0x00000000,         /* None of these. */

     DWCF_RELATIVE                         = 0x00000001,         /* Receive relative coordinates instead of absolute. */
     DWCF_EXPLICIT                         = 0x00000002,         /* If window has focus it's also the pointer window. */
     DWCF_UNCLIPPED                        = 0x00000004,         /* Pointer is not clipped against stack boundaries. */
     DWCF_TRAPPED                          = 0x00000008,         /* Pointer is clipped against the window boundaries. */
     DWCF_FIXED                            = 0x00000010,         /* Pointer does not move at all, but still may send
                                                                    relative motion. */
     DWCF_INVISIBLE                        = 0x00000020,         /* Window cursor is not visible. */

     DWCF_ALL                              = 0x0000003F          /* All of these. */
} DFBWindowCursorFlags;

/*
 * These are hints for the window manager that indicate what
 * type of function the window has. The window manager can use
 * this when determining decoration and behaviour of the window.
 * The hint must be set before mapping the window.
 */
typedef enum {
     DWTH_NORMAL                           = 0x00000000,         /* Normal toplevel window. */
     DWTH_DIALOG                           = 0x00000001,         /* Dialog window. */
     DWTH_MENU                             = 0x00000002,         /* Window used to implement a menu. */
     DWTH_TOOLBAR                          = 0x00000003,         /* Window used to implement toolbars. */
     DWTH_SPLASHSCREEN                     = 0x00000004,         /* Window used to display a splash screen during
                                                                    application startup. */
     DWTH_UTLIITY                          = 0x00000005,         /* Utility windows which are not detached toolbars or
                                                                    dialogs. */
     DWTH_DOCK                             = 0x00000006,         /* Used for creating dock or panel windows. */
     DWTH_DESKTOP                          = 0x00000007,         /* Used for creating the desktop background window. */
     DWTH_DROPDOWN_MENU                    = 0x00000008,         /* A menu that belongs to a menubar. */
     DWTH_POPUP_MENU                       = 0x00000009,         /* A menu that does not belong to a menubar, e.g. a
                                                                    context menu. */
     DWTH_TOOLTIP                          = 0x0000000A,         /* A tooltip. */
     DWTH_NOTIFICATION                     = 0x0000000B,         /* A notification, typically a bubble that belongs to a
                                                                    status icon. */
     DWTH_COMBO                            = 0x0000000C,         /* A popup from a combo box. */
     DWTH_DND                              = 0x0000000D          /* A window that is used to implement a DND cursor. */
} DFBWindowTypeHint;

/*
 * Flags for window hint.
 */
typedef enum {
     DWHF_NONE                             = 0x00000000,         /* None of these. */

     DWHF_MODAL                            = 0x00000001,         /* Set modal hint. */
     DWHF_SKIP_TASKBAR                     = 0x00000002,         /* Set skip taskbar hint. */
     DWHF_SKIP_PAGER                       = 0x00000004,         /* Set skip pager hint. */
     DWHF_URGENCY                          = 0x00000008,         /* Set urgency hint. */

     DWHF_ALL                              = 0x0000000F          /* All of these. */
} DFBWindowHintFlags;

/*
 * Window surface swapping policy.
 */
typedef enum {
     DWSP_SYSTEMONLY                       = 0x00000000,         /* Never try to swap into video memory. */
     DWSP_VIDEOLOW                         = 0x00000001,         /* Try to store in video memory, low priority. */
     DWSP_VIDEOHIGH                        = 0x00000002,         /* Try to store in video memory, high priority. */
     DWSP_VIDEOONLY                        = 0x00000003          /* Always and only store in video memory. */
} DFBWindowSurfacePolicy;

/*
 * IDirectFBWindow is the window interface.
 */
D_DEFINE_INTERFACE( IDirectFBWindow,

   /** Retrieving information **/

     /*
      * Get the unique window ID.
      */
     DFBResult (*GetID) (
          IDirectFBWindow                   *thiz,
          DFBWindowID                       *ret_window_id
     );

     /*
      * Get the current position of this window.
      */
     DFBResult (*GetPosition) (
          IDirectFBWindow                   *thiz,
          int                               *ret_x,
          int                               *ret_y
     );

     /*
      * Get the size of the window in pixels.
      */
     DFBResult (*GetSize) (
          IDirectFBWindow                   *thiz,
          int                               *ret_width,
          int                               *ret_height
     );

   /** Close & Destroy **/

     /*
      * Send a close message to the window.
      *
      * This function sends a message of type DWET_CLOSE to the
      * window. It does not actually close window.
      */
     DFBResult (*Close) (
          IDirectFBWindow                   *thiz
     );

     /*
      * Destroy the window and sends a destruction message.
      *
      * This function sends a message of type DWET_DESTROY to
      * the window after removing it from the window stack and
      * freeing its data.
      */
     DFBResult (*Destroy) (
          IDirectFBWindow               *thiz
     );

   /** Surface & Scaling **/

     /*
      * Get an interface to the backing store surface.
      *
      * This surface has to be flipped to make previous drawing
      * commands visible, i.e. to repaint the windowstack for
      * that region.
      */
     DFBResult (*GetSurface) (
          IDirectFBWindow                   *thiz,
          IDirectFBSurface                 **ret_interface
     );

     /*
      * Resize the surface of a scalable window.
      *
      * This requires the option DWOP_SCALE.
      */
     DFBResult (*ResizeSurface) (
          IDirectFBWindow                   *thiz,
          int                                width,
          int                                height
     );

   /** Events **/

     /*
      * Create an event buffer for this window and attach it.
      */
     DFBResult (*CreateEventBuffer) (
          IDirectFBWindow                   *thiz,
          IDirectFBEventBuffer             **ret_interface
     );

     /*
      * Attach an existing event buffer to this window.
      *
      * Note: attaching multiple times generates multiple
      * events.
      */
     DFBResult (*AttachEventBuffer) (
          IDirectFBWindow                   *thiz,
          IDirectFBEventBuffer              *buffer
     );

     /*
      * Detach an event buffer from this window.
      */
     DFBResult (*DetachEventBuffer) (
          IDirectFBWindow                   *thiz,
          IDirectFBEventBuffer              *buffer
     );

     /*
      * Enable specific events to be sent to the window.
      *
      * The argument is a mask of events that will be set in the
      * window's event mask.
      * The default event mask is DWET_ALL.
      */
     DFBResult (*EnableEvents) (
          IDirectFBWindow                   *thiz,
          DFBWindowEventType                 mask
     );

     /*
      * Disable specific events from being sent to the window.
      *
      * The argument is a mask of events that will be cleared in
      * the window's event mask.
      * The default event mask is DWET_ALL.
      */
     DFBResult (*DisableEvents) (
          IDirectFBWindow                   *thiz,
          DFBWindowEventType                 mask
     );

   /** Options **/

     /*
      * Set options controlling appearance and behaviour of the
      * window.
      */
     DFBResult (*SetOptions) (
          IDirectFBWindow                   *thiz,
          DFBWindowOptions                   options
     );

     /*
      * Get options controlling appearance and behaviour of the
      * window.
      */
     DFBResult (*GetOptions) (
          IDirectFBWindow                   *thiz,
          DFBWindowOptions                  *ret_options
     );

     /*
      * Set the window color, or colorises the window.
      *
      * In case you specified DWCAPS_COLOR, this sets the window
      * draw color. In case you didn't, it colorises the window
      * with this color; this will darken the window.
      */
     DFBResult (*SetColor) (
          IDirectFBWindow                   *thiz,
          u8                                 r,
          u8                                 g,
          u8                                 b,
          u8                                 a
     );

     /*
      * Set the window color key.
      *
      * If a pixel of the window matches this color the
      * underlying window or the background is visible at this
      * point.
      */
     DFBResult (*SetColorKey) (
          IDirectFBWindow                   *thiz,
          u8                                 r,
          u8                                 g,
          u8                                 b
     );

     /*
      * Set the window color key (indexed).
      *
      * If a pixel (indexed format) of the window matches this
      * color index the underlying window or the background is
      * visible at this point.
      */
     DFBResult (*SetColorKeyIndex) (
          IDirectFBWindow                   *thiz,
          unsigned int                       index
     );

     /*
      * Set the window's global opacity factor.
      *
      * Set it to 0 to hide a window. Setting it to 0xff makes
      * the window opaque if it has no alpha channel.
      */
     DFBResult (*SetOpacity) (
          IDirectFBWindow                   *thiz,
          u8                                 opacity
     );

     /*
      * Disable alpha channel blending for one region of the
      * window.
      *
      * If DWOP_ALPHACHANNEL and DWOP_OPAQUE_REGION are set but
      * not DWOP_COLORKEYING and the opacity of the window is
      * 0xff, the window gets rendered without alpha blending
      * within the specified region. This is extremely useful
      * for alpha blended window decorations while the main
      * content stays opaque and gets rendered faster.
      */
     DFBResult (*SetOpaqueRegion) (
          IDirectFBWindow                   *thiz,
          int                                x1,
          int                                y1,
          int                                x2,
          int                                y2
     );

     /*
      * Get the current opacity factor of this window.
      */
     DFBResult (*GetOpacity) (
          IDirectFBWindow                   *thiz,
          u8                                *ret_opacity
     );

     /*
      * Bind a cursor shape to this window.
      *
      * This method will set a per-window cursor shape.
      * Everytime the cursor enters this window, the specified
      * shape is set. Passing NULL will unbind a set shape and
      * release its surface.
      */
     DFBResult (*SetCursorShape) (
          IDirectFBWindow                   *thiz,
          IDirectFBSurface                  *shape,
          int                                hot_x,
          int                                hot_y
     );

   /** Position and Size **/

     /*
      * Move the window by the specified distance.
      */
     DFBResult (*Move) (
          IDirectFBWindow                   *thiz,
          int                                dx,
          int                                dy
     );

     /*
      * Move the window to the specified coordinates.
      */
     DFBResult (*MoveTo) (
          IDirectFBWindow                   *thiz,
          int                                x,
          int                                y
     );

     /*
      * Resize the window.
      */
     DFBResult (*Resize) (
          IDirectFBWindow                   *thiz,
          int                                width,
          int                                height
     );

     /*
      * Set position and size in one step.
      */
     DFBResult (*SetBounds) (
          IDirectFBWindow                   *thiz,
          int                                x,
          int                                y,
          int                                width,
          int                                height
     );

   /** Stacking **/

     /*
      * Put the window into a specific stacking class.
      */
     DFBResult (*SetStackingClass) (
          IDirectFBWindow                   *thiz,
          DFBWindowStackingClass             stacking_class
     );

     /*
      * Raise the window by one within the window stack.
      */
     DFBResult (*Raise) (
          IDirectFBWindow                   *thiz
     );

     /*
      * Lower the window by one within the window stack.
      */
     DFBResult (*Lower) (
          IDirectFBWindow                   *thiz
     );

     /*
      * Put the window on the top of the window stack.
      */
     DFBResult (*RaiseToTop) (
          IDirectFBWindow                   *thiz
     );

     /*
      * Send a window to the bottom of the window stack.
      */
     DFBResult (*LowerToBottom) (
          IDirectFBWindow                   *thiz
     );

     /*
      * Put a window on top of another window.
      */
     DFBResult (*PutAtop) (
          IDirectFBWindow                   *thiz,
          IDirectFBWindow                   *lower
     );

     /*
      * Put a window below another window.
      */
     DFBResult (*PutBelow) (
          IDirectFBWindow                   *thiz,
          IDirectFBWindow                   *upper
     );

   /** Binding **/

     /*
      * Bind a window at the specified position of this window.
      *
      * After binding, bound window will be automatically moved
      * when this window moves to a new position. Binding the
      * same window to multiple windows is not supported.
      * Subsequent call to Bind() automatically unbounds the
      * bound window before binding it again.
      * To move the bound window to a new position call Bind()
      * again with the new coordinates.
      */
     DFBResult (*Bind) (
          IDirectFBWindow                   *thiz,
          IDirectFBWindow                   *window,
          int                                x,
          int                                y
     );

     /*
      * Unbind a window from this window.
      */
     DFBResult (*Unbind) (
          IDirectFBWindow                   *thiz,
          IDirectFBWindow                   *window
     );

   /** Focus handling **/

     /*
      * Pass the focus to this window.
      */
     DFBResult (*RequestFocus) (
          IDirectFBWindow                   *thiz
     );

     /*
      * Grab the keyboard, i.e. all following keyboard events
      * are sent to this window ignoring the focus.
      */
     DFBResult (*GrabKeyboard) (
          IDirectFBWindow                   *thiz
     );

     /*
      * Ungrab the keyboard, i.e. switch to standard key event
      * dispatching.
      */
     DFBResult (*UngrabKeyboard) (
          IDirectFBWindow                   *thiz
     );

     /*
      * Grab the pointer, i.e. all following mouse events are
      * sent to this window ignoring the focus.
      */
     DFBResult (*GrabPointer) (
          IDirectFBWindow                   *thiz
     );

     /*
      * Ungrab the pointer, i.e. switch to standard mouse event
      * dispatching.
      */
     DFBResult (*UngrabPointer) (
          IDirectFBWindow                   *thiz
     );

     /*
      * Grab a specific key, i.e. all following events of this
      * key are sent to this window ignoring the focus.
      */
     DFBResult (*GrabKey) (
          IDirectFBWindow                   *thiz,
          DFBInputDeviceKeySymbol            symbol,
          DFBInputDeviceModifierMask         modifiers
     );

     /*
      * Ungrab a specific key, i.e. switch to standard key event
      * dispatching.
      */
     DFBResult (*UngrabKey) (
          IDirectFBWindow                   *thiz,
          DFBInputDeviceKeySymbol            symbol,
          DFBInputDeviceModifierMask         modifiers
     );

   /** Key selection **/

     /*
      * Selects a mode for filtering keys while being focused.
      *
      * The 'selection' defines whether all, none or a specific
      * set of keys is selected. In case of a specific set, the
      * 'keys' array with 'num_keys' has to be provided.
      */
     DFBResult (*SetKeySelection) (
          IDirectFBWindow                   *thiz,
          DFBWindowKeySelection              selection,
          const DFBInputDeviceKeySymbol     *keys,
          unsigned int                       num_keys
     );

     /*
      * Grab all unselected (filtered out) keys.
      *
      * Unselected keys are those not selected by the focused
      * window. These keys won't be sent to that window. Instead
      * one window in the stack can collect them.
      */
     DFBResult (*GrabUnselectedKeys) (
          IDirectFBWindow                   *thiz
     );

     /*
      * Release the grab of unselected (filtered out) keys.
      */
     DFBResult (*UngrabUnselectedKeys) (
          IDirectFBWindow                   *thiz
     );

   /** Advanced Geometry **/

     /*
      * Set area of surface to be shown in window.
      */
     DFBResult (*SetSrcGeometry) (
          IDirectFBWindow                   *thiz,
          const DFBWindowGeometry           *geometry
     );

     /*
      * Set destination location of window within its bounds.
      */
     DFBResult (*SetDstGeometry) (
          IDirectFBWindow                   *thiz,
          const DFBWindowGeometry           *geometry
     );

     /*
      * Get stereo depth.
      */
     DFBResult (*GetStereoDepth) (
         IDirectFBWindow                    *thiz,
         int                                *ret_z
     );

     /*
      * Set stereo depth.
      *
      * The depth value specified will cause the left eye buffer
      * content to be shifted on the x-axis by +z and the right
      * eye buffer to be shifted by -z value. A positive z value
      * will cause the layer to appear closer than the TV plane
      * while a negative z value will make the layer appear
      * farther away. The depth is limited to a value between
      * +DLSO_FIXED_LIMIT and -DLSO_FIXED_LIMIT.
      */
     DFBResult (*SetStereoDepth) (
         IDirectFBWindow                    *thiz,
         int                                 z
     );

   /** Properties **/

     /*
      * Set property controlling appearance and behaviour of the
      * window.
      */
     DFBResult (*SetProperty) (
          IDirectFBWindow                   *thiz,
          const char                        *key,
          void                              *value,
          void                             **ret_old_value
     );

     /*
      * Get property controlling appearance and behaviour of the
      * window.
      */
     DFBResult (*GetProperty) (
          IDirectFBWindow                   *thiz,
          const char                        *key,
          void                             **ret_value
     );

     /*
      * Remove property controlling appearance and behaviour of
      * the window.
      */
     DFBResult (*RemoveProperty) (
          IDirectFBWindow                   *thiz,
          const char                        *key,
          void                             **ret_value
     );

     /*
      * Set window rotation.
      */
     DFBResult (*SetRotation) (
          IDirectFBWindow                   *thiz,
          int                                rotation
     );

   /** Association **/

     /*
      * Change the window association.
      *
      * If 'window_id' is 0, the window will be dissociated.
      */
     DFBResult (*SetAssociation) (
          IDirectFBWindow                   *thiz,
          DFBWindowID                        window_id
     );

   /** Application ID **/

     /*
      * Set application ID.
      *
      * The usage of the application ID is not imposed and can
      * be used at will by the application. Any change will be
      * notified, and as such, an application manager can be
      * used to act on any change.
      */
     DFBResult (*SetApplicationID) (
          IDirectFBWindow                   *thiz,
          unsigned long                      application_id
     );

     /*
      * Get current application ID.
      */
     DFBResult (*GetApplicationID) (
          IDirectFBWindow                   *thiz,
          unsigned long                     *ret_application_id
     );

   /** Updates **/

     /*
      * Signal start of window content updates.
      */
     DFBResult (*BeginUpdates) (
          IDirectFBWindow                   *thiz,
          const DFBRegion                   *update
     );

   /** Events **/

     /*
      * Send event.
      */
     DFBResult (*SendEvent) (
          IDirectFBWindow                   *thiz,
          const DFBWindowEvent              *event
     );

   /** Cursor **/

     /*
      * Set cursor flags (active when in focus).
      */
     DFBResult (*SetCursorFlags) (
          IDirectFBWindow                   *thiz,
          DFBWindowCursorFlags               flags
     );

     /*
      * Set cursor resolution.
      *
      * The default cursor resolution is the surface dimensions.
      */
     DFBResult (*SetCursorResolution) (
          IDirectFBWindow                   *thiz,
          const DFBDimension                *resolution
     );

     /*
      * Set cursor position within window coordinates.
      */
     DFBResult (*SetCursorPosition) (
          IDirectFBWindow                   *thiz,
          int                                x,
          int                                y
     );

   /** Geometry **/

     /*
      * Set area of surface to be shown in window.
      * Set destination location of window within its bounds.
      */
     DFBResult (*SetGeometry) (
          IDirectFBWindow                   *thiz,
          const DFBWindowGeometry           *src,
          const DFBWindowGeometry           *dst
     );

   /** Extended **/

     /*
      * Set the window type hint.
      */
     DFBResult (*SetTypeHint) (
          IDirectFBWindow                   *thiz,
          DFBWindowTypeHint                  type_hint
     );

     /*
      * Change window hint flags.
      */
     DFBResult (*ChangeHintFlags) (
          IDirectFBWindow                   *thiz,
          DFBWindowHintFlags                 clear,
          DFBWindowHintFlags                 set
     );

   /** Policy **/

     /*
      * Get the window surface swapping policy.
      */
     DFBResult (*GetPolicy) (
          IDirectFBWindow                   *thiz,
          DFBWindowSurfacePolicy            *ret_policy
     );
)

/*****************
 * IDirectFBFont *
 *****************/

/*
 * Called for each provided text encoding.
 */
typedef DFBEnumerationResult (*DFBTextEncodingCallback) (
     DFBTextEncodingID                       encoding_id,
     const char                             *name,
     void                                   *callbackdata
);

/*
 * IDirectFBFont is the font interface.
 */
D_DEFINE_INTERFACE( IDirectFBFont,

   /** Retrieving information **/

     /*
      * Get the distance from the baseline to the top of the
      * logical extents of this font.
      */
     DFBResult (*GetAscender) (
          IDirectFBFont                     *thiz,
          int                               *ret_ascender
     );

     /*
      * Get the distance from the baseline to the bottom of the
      * logical extents of this font.
      */
     DFBResult (*GetDescender) (
          IDirectFBFont                     *thiz,
          int                               *ret_descender
     );

     /*
      * Get the logical height of this font.
      *
      * This is the distance from one baseline to the next when
      * writing several lines of text. Note that this value does
      * not correspond the height value specified when loading
      * the font.
      */
     DFBResult (*GetHeight) (
          IDirectFBFont                     *thiz,
          int                               *ret_height
     );

     /*
      * Get the maximum character width.
      *
      * Not all fonts specify it correcly.
      */
     DFBResult (*GetMaxAdvance) (
          IDirectFBFont                     *thiz,
          int                               *ret_maxadvance
     );

     /*
      * Get the kerning to apply between two glyphs specified by
      * their character codes.
      */
     DFBResult (*GetKerning) (
          IDirectFBFont                     *thiz,
          unsigned int                       prev,
          unsigned int                       current,
          int                               *ret_kern_x,
          int                               *ret_kern_y
     );

   /** Measurements **/

     /*
      * Get the logical width of the specified string as if it
      * were drawn with this font.
      *
      * 'bytes' specifies the number of bytes to take from the
      * string or -1 for the complete NULL-terminated string.
      * The returned width may be different than the actual
      * drawn width of the 'text' since this function returns
      * the logical width that should be used to layout 'text'.
      * A negative width indicates right-to-left rendering.
      */
     DFBResult (*GetStringWidth) (
          IDirectFBFont                     *thiz,
          const char                        *text,
          int                                bytes,
          int                               *ret_width
     );

     /*
      * Get the logical and real extents of the specified string
      * as if it were drawn with this font.
      *
      * 'bytes' specifies the number of bytes to take from the
      * string or -1 for the complete NULL-terminated string.
      * The logical rectangle describes the typographic extents
      * and should be used to layout 'text'. The ink rectangle
      * describes the smallest rectangle containing all pixels
      * that are touched when drawing the string. If you only
      * need one of the rectangles, pass NULL for the other one.
      * The ink rectangle is guaranteed to be a valid rectangle
      * with positive width and height, while the logical
      * rectangle may have negative width indicating
      * right-to-left layout.
      * The rectangle offsets are reported relative to the
      * baseline and refer to the text being drawn using
      * DSTF_LEFT.
      */
     DFBResult (*GetStringExtents) (
          IDirectFBFont                     *thiz,
          const char                        *text,
          int                                bytes,
          DFBRectangle                      *ret_logical_rect,
          DFBRectangle                      *ret_ink_rect
     );

     /*
      * Get the extents of a glyph specified by its character
      * code.
      *
      * The rectangle describes the smallest rectangle
      * containing all pixels that are touched when drawing the
      * glyph. It is reported relative to the baseline. If you
      * only need the advance, pass NULL for the rectangle.
      * The advance describes the horizontal offset to the next
      * glyph (without kerning applied). It may be a negative
      * value indicating left-to-right rendering. If you don't
      * need this value, pass NULL for advance.
      */
     DFBResult (*GetGlyphExtents) (
          IDirectFBFont                     *thiz,
          unsigned int                       character,
          DFBRectangle                      *ret_rect,
          int                               *ret_advance
     );

     /*
      * Get the next explicit or automatic break within a string
      * along with the logical width of the text, the string
      * length, and a pointer to the next text line.
      *
      * The 'bytes' specifies the maximum number of bytes to
      * take from the string or -1 for complete NULL-terminated
      * string.
      * The 'max_width' specifies logical width of column onto
      * which the 'text' will be drawn. Then the logical width
      * of fitted text is returned in 'ret_width'. The returned
      * width may overlap the max width specified if there's
      * only one character that fits.
      * The number of characters that fit into this column is
      * returned by the 'ret_str_length'. Note that you can not
      * use this value as the number of bytes to take when using
      * DrawString() as it represents the number of characters,
      * not the number of bytes.
      * In 'ret_next_line' a pointer to the next line of text is
      * returned. This will point to NULL or the end of the
      * string if there's no more break.
      */
     DFBResult (*GetStringBreak) (
          IDirectFBFont                     *thiz,
          const char                        *text,
          int                                bytes,
          int                                max_width,
          int                               *ret_width,
          int                               *ret_str_length,
          const char                       **ret_next_line
     );

   /** Encodings **/

     /*
      * Change the default encoding used when the font is set
      * at a surface.
      */
     DFBResult (*SetEncoding) (
          IDirectFBFont                     *thiz,
          DFBTextEncodingID                  encoding
     );

     /*
      * Enumerate all provided text encodings.
      */
     DFBResult (*EnumEncodings) (
          IDirectFBFont                     *thiz,
          DFBTextEncodingCallback            callback,
          void                              *callbackdata
     );

     /*
      * Find an encoding by its name.
      */
     DFBResult (*FindEncoding) (
          IDirectFBFont                     *thiz,
          const char                        *name,
          DFBTextEncodingID                 *ret_encoding
     );

   /** Resources **/

     /*
      * Dispose resources used by the font.
      *
      * Keeps font usable, recreating resources as needed.
      */
     DFBResult (*Dispose) (
          IDirectFBFont                     *thiz
     );

   /** Measurements **/

     /*
      * Get the line spacing vector of this font.
      *
      * This is the displacement vector from one line to the
      * next when writing several lines of text. It differs
      * from the height only when the font is rotated.
      */
     DFBResult (*GetLineSpacingVector) (
          IDirectFBFont                     *thiz,
          int                               *ret_xspacing,
          int                               *ret_yspacing
     );

     /*
      * Get the extents of a glyph specified by its character
      * code (extended version).
      *
      * The rectangle describes the smallest rectangle
      * containing all pixels that are touched when drawing the
      * glyph. It is reported relative to the baseline. If you
      * only need the advance, pass NULL for the rectangle.
      * The advance describes the horizontal offset to the next
      * glyph (without kerning applied). It may be a negative
      * value indicating left-to-right rendering. If you don't
      * need this value, pass NULL for advance.
      */
     DFBResult (*GetGlyphExtentsXY) (
          IDirectFBFont                     *thiz,
          unsigned int                       character,
          DFBRectangle                      *ret_rect,
          int                               *ret_xadvance,
          int                               *ret_yadvance
     );

     /*
      * Get the position and thickness of the underline.
      */
     DFBResult (*GetUnderline) (
          IDirectFBFont                     *thiz,
          int                               *ret_underline_position,
          int                               *ret_underline_thichness
     );

   /** Retrieving information **/

     /*
      * Get the description of the font.
      */
     DFBResult (*GetDescription) (
          IDirectFBFont                     *thiz,
          DFBFontDescription                *ret_desc
     );
)

/**************************
 * IDirectFBImageProvider *
 **************************/

/*
 * Capabilities of an image.
 */
typedef enum {
     DICAPS_NONE                           = 0x00000000,         /* None of these. */

     DICAPS_ALPHACHANNEL                   = 0x00000001,         /* The image data contains an alphachannel. */
     DICAPS_COLORKEY                       = 0x00000002          /* The image has a colorkey */
} DFBImageCapabilities;

/*
 * Information about an image.
 */
typedef struct {
     DFBImageCapabilities                    caps;               /* capabilities */
     u8                                      colorkey_r;         /* colorkey red channel */
     u8                                      colorkey_g;         /* colorkey green channel */
     u8                                      colorkey_b;         /* colorkey blue channel */
} DFBImageDescription;

/*
 * Flags used by an image provider.
 */
typedef enum {
     DIRENDER_NONE                         = 0x00000000,         /* None of these. */

     DIRENDER_FAST                         = 0x00000001,         /* Select fast rendering method. */

     DIRENDER_ALL                          = 0x00000001          /* All of these. */
} DIRenderFlags;

/*
 * Return value of progressive image loading callback.
 */
typedef enum {
     DIRCR_OK                              = 0x00000000,         /* No error occurred. */
     DIRCR_ABORT                           = 0x00000001          /* Abort error */
} DIRenderCallbackResult;

/*
 * Called whenever a chunk of the image is decoded.
 */
typedef DIRenderCallbackResult (*DIRenderCallback) (
     DFBRectangle                           *rect,
     void                                   *ctx
);

/*
 * IDirectFBImageProvider is the image provider interface.
 */
D_DEFINE_INTERFACE( IDirectFBImageProvider,

   /** Retrieving information **/

     /*
      * Get a surface description that best matches the image
      * contained in the file.
      *
      * For opaque image formats the pixel format of the primary
      * layer is used. For images with alpha channel an ARGB
      * surface description is returned.
      */
     DFBResult (*GetSurfaceDescription) (
          IDirectFBImageProvider            *thiz,
          DFBSurfaceDescription             *ret_desc
     );

     /*
      * Get a description of the image.
      *
      * This includes stuff that does not belong into the
      * surface description, e.g. a colorkey.
      */
     DFBResult (*GetImageDescription) (
          IDirectFBImageProvider            *thiz,
          DFBImageDescription               *ret_desc
     );

   /** Rendering **/

     /*
      * Render the file contents into the destination contents
      * doing automatic scaling and color format conversion.
      *
      * If the image file has an alpha channel, it is rendered
      * with alpha channel if the destination surface is of the
      * ARGB pixelformat. Otherwise, transparent areas are
      * blended over a black background.
      * If a destination rectangle is specified, the rectangle
      * is clipped to the destination surface. If NULL is passed
      * as destination rectangle, the whole destination surface
      * is taken. The image is stretched to fill the rectangle.
      */
     DFBResult (*RenderTo) (
          IDirectFBImageProvider            *thiz,
          IDirectFBSurface                  *destination,
          const DFBRectangle                *dest_rect
     );

     /*
      * Register a callback for progressive image loading.
      *
      * The function is called each time a chunk of the image is
      * decoded.
      */
     DFBResult (*SetRenderCallback) (
          IDirectFBImageProvider            *thiz,
          DIRenderCallback                   callback,
          void                              *ctx
     );

     /*
      * Set hint for preferred image decoding method.
      */
     DFBResult (*SetRenderFlags) (
          IDirectFBImageProvider            *thiz,
          DIRenderFlags                      flags
     );
)

/**************************
 * IDirectFBVideoProvider *
 **************************/

/*
 * The video provider capabilities.
 */
typedef enum {
     DVCAPS_BASIC                          = 0x00000000,         /* basic ops (play, stop) */
     DVCAPS_SEEK                           = 0x00000001,         /* supports seek to a position */
     DVCAPS_SCALE                          = 0x00000002,         /* can scale the video */
     DVCAPS_INTERLACED                     = 0x00000004,         /* supports interlaced surfaces */
     DVCAPS_SPEED                          = 0x00000008,         /* supports changing playback speed */
     DVCAPS_BRIGHTNESS                     = 0x00000010,         /* supports brightness adjustment */
     DVCAPS_CONTRAST                       = 0x00000020,         /* supports contrast adjustment */
     DVCAPS_HUE                            = 0x00000040,         /* supports hue adjustment */
     DVCAPS_SATURATION                     = 0x00000080,         /* supports saturation adjustment */
     DVCAPS_INTERACTIVE                    = 0x00000100,         /* supports sending an input or window event */
     DVCAPS_VOLUME                         = 0x00000200,         /* supports volume adjustment */
} DFBVideoProviderCapabilities;

/*
 * Capabilities of an audio/video stream.
 */
typedef enum {
     DVSCAPS_NONE                          = 0x00000000,         /* None of these. */

     DVSCAPS_VIDEO                         = 0x00000001,         /* Stream contains video. */
     DVSCAPS_AUDIO                         = 0x00000002          /* Stream contains audio. */
} DFBStreamCapabilities;

#define DFB_STREAM_DESC_ENCODING_LENGTH       30

#define DFB_STREAM_DESC_TITLE_LENGTH         255

#define DFB_STREAM_DESC_AUTHOR_LENGTH        255

#define DFB_STREAM_DESC_ALBUM_LENGTH         255

#define DFB_STREAM_DESC_GENRE_LENGTH          32

#define DFB_STREAM_DESC_COMMENT_LENGTH       255

/*
 * Information about an audio/video stream.
 */
typedef struct {
     DFBStreamCapabilities                   caps;               /* capabilities */
     struct {
          char encoding[DFB_STREAM_DESC_ENCODING_LENGTH];        /* encoding (e.g. "h264") */
          double                             framerate;          /* number of frames per second */
          double                             aspect;             /* frame aspect ratio */
          int                                bitrate;            /* amount of bits per second */
          int                                afd;                /* active format descriptor */
          int                                width;              /* width as reported by sequence header */
          int                                height;             /* height as reported by sequence header */
     } video;                                                    /* struct containing encoding properties for video */
     struct {
          char encoding[DFB_STREAM_DESC_ENCODING_LENGTH];        /* encoding (e.g. "aac") */
          int                                samplerate;         /* number of samples per second */
          int                                channels;           /* number of channels per sample */
          int                                bitrate;            /* amount of bits per second */
     } audio;                                                    /* struct containing encoding properties for audio */
     char title[DFB_STREAM_DESC_TITLE_LENGTH];                   /* title */
     char author[DFB_STREAM_DESC_AUTHOR_LENGTH];                 /* author */
     char album[DFB_STREAM_DESC_ALBUM_LENGTH];                   /* album */
     short                                   year;               /* year */
     char genre[DFB_STREAM_DESC_GENRE_LENGTH];                   /* genre */
     char comment[DFB_STREAM_DESC_COMMENT_LENGTH];               /* comment */
} DFBStreamDescription;

/*
 * Status of a video provider.
 */
typedef enum {
     DVSTATE_UNKNOWN                       = 0x00000000,         /* unknown status */

     DVSTATE_PLAY                          = 0x00000001,         /* playing */
     DVSTATE_STOP                          = 0x00000002,         /* playback was stopped */
     DVSTATE_FINISHED                      = 0x00000003,         /* playback is finished */
     DVSTATE_BUFFERING                     = 0x00000004          /* buffering, playback is running */
} DFBVideoProviderStatus;

/*
 * Flags controlling playback mode of a video provider.
 */
typedef enum {
     DVPLAY_NOFX                           = 0x00000000,         /* normal playback */
     DVPLAY_REWIND                         = 0x00000001,         /* reverse playback */
     DVPLAY_LOOPING                        = 0x00000002          /* automatically restart playback when end-of-stream is
                                                                    reached. */
} DFBVideoProviderPlaybackFlags;

/*
 * Buffer levels and occupancy for audio/video input buffers.
 */
typedef struct {
     DFBStreamCapabilities                   valid;              /* Validation of audio/video. */

     struct {
         unsigned int                        buffer_size;        /* Size in bytes of the input buffer to video
                                                                    decoder. */
         unsigned int                        minimum_level;      /* The level at which a DVPET_DATALOW event will be
                                                                    generated. */
         unsigned int                        maximum_level;      /* The level at which a DVPET_DATAHIGH event will be
                                                                    generated. */
         unsigned int                        current_level;      /* Current fill level of video input buffer. */
     } video;                                                    /* Video buffer occupancy. */
     struct {
         unsigned int                        buffer_size;        /* Size in bytes of the input buffer to audio
                                                                    decoder. */
         unsigned int                        minimum_level;      /* The level at which a DVPET_DATALOW event will be
                                                                    generated. */
         unsigned int                        maximum_level;      /* The level at which a DVPET_DATAHIGH event will be
                                                                    generated. */
         unsigned int                        current_level;      /* Current fill level of audio input buffer.*/
     } audio;                                                    /* Audio buffer occupancy. */
} DFBBufferOccupancy;

/*
 * Buffer thresholds for audio/video.
 */
typedef struct {
     DFBStreamCapabilities                   selection;          /* Validation of audio/video. */

     struct {
          unsigned int                       minimum_level;      /* The level at which a DVPET_DATALOW event will be
                                                                    generated. */
          unsigned int                       maximum_level;      /* The level at which a DVPET_DATAHIGH event will be
                                                                    generated. */
          unsigned int                       minimum_time;       /* The buffering time at which a DVPET_BUFFERTIMELOW
                                                                    event will be generated. */
          unsigned int                       maximum_time;       /* The buffering time at which a DVPET_BUFFERTIMEHIGH
                                                                    event will be generated. */
     } video;                                                    /* Video buffer thresholds. */
     struct {
          unsigned int                       minimum_level;      /* The level at which a DVPET_DATALOW event will be
                                                                    generated. */
          unsigned int                       maximum_level;      /* The level at which a DVPET_DATAHIGH event will be
                                                                    generated. */
          unsigned int                       minimum_time;       /* The buffering time at which a DVPET_BUFFERTIMELOW
                                                                    event will be generated. */
          unsigned int                       maximum_time;       /* The buffering time at which a DVPET_BUFFERTIMEHIGH
                                                                    event will be generated. */
     } audio;                                                    /* Audio buffer thresholds. */
} DFBBufferThresholds;

/*
 * Called for each written frame.
 */
typedef void (*DVFrameCallback) (
     void                                   *ctx
);

/*
 * IDirectFBVideoProvider is the video provider interface.
 */
D_DEFINE_INTERFACE( IDirectFBVideoProvider,

   /** Retrieving information **/

     /*
      * Retrieve information about the video provider's
      * capabilities.
      */
     DFBResult (*GetCapabilities) (
          IDirectFBVideoProvider            *thiz,
          DFBVideoProviderCapabilities      *ret_caps
     );

     /*
      * Get a surface description that best matches the video
      * contained in the file.
      */
     DFBResult (*GetSurfaceDescription) (
          IDirectFBVideoProvider            *thiz,
          DFBSurfaceDescription             *ret_desc
     );

     /*
      * Get a description of the video stream.
      */
     DFBResult (*GetStreamDescription) (
          IDirectFBVideoProvider            *thiz,
          DFBStreamDescription              *ret_desc
     );

   /** Playback **/

     /*
      * Play the video rendering it into the specified rectangle
      * of the destination surface.
      *
      * Optionally a callback can be registered that is called
      * for each rendered frame. This is especially important if
      * you are playing to a flipping surface. In this case, you
      * should flip the destination surface in your callback.
      */
     DFBResult (*PlayTo) (
          IDirectFBVideoProvider            *thiz,
          IDirectFBSurface                  *destination,
          const DFBRectangle                *dest_rect,
          DVFrameCallback                    callback,
          void                              *ctx
     );

     /*
      * Stop rendering into the destination surface.
      */
     DFBResult (*Stop) (
          IDirectFBVideoProvider            *thiz
     );

     /*
      * Get the status of the playback.
      */
     DFBResult (*GetStatus) (
          IDirectFBVideoProvider            *thiz,
          DFBVideoProviderStatus            *ret_status
     );

   /** Media Control **/

     /*
      * Seek to a position within the stream.
      */
     DFBResult (*SeekTo) (
          IDirectFBVideoProvider            *thiz,
          double                             seconds
     );

     /*
      * Get current position within the stream.
      */
     DFBResult (*GetPos) (
          IDirectFBVideoProvider            *thiz,
          double                            *ret_seconds
     );

     /*
      * Get the length of the stream.
      */
     DFBResult (*GetLength) (
          IDirectFBVideoProvider            *thiz,
          double                            *ret_seconds
     );

   /** Color Adjustment **/

     /*
      * Get the current video color settings.
      */
     DFBResult (*GetColorAdjustment) (
          IDirectFBVideoProvider            *thiz,
          DFBColorAdjustment                *ret_adj
     );

     /*
      * Adjust the video colors.
      */
     DFBResult (*SetColorAdjustment) (
          IDirectFBVideoProvider            *thiz,
          const DFBColorAdjustment          *adj
     );

   /** Interactivity **/

     /*
      * Send an input or window event.
      *
      * This method allows to redirect events to an interactive
      * video provider. Events must be relative to the specified
      * rectangle of the destination surface.
      */
     DFBResult (*SendEvent) (
          IDirectFBVideoProvider            *thiz,
          const DFBEvent                    *event
     );

   /** Advanced control **/

     /*
      * Set the flags controlling playback mode.
      */
     DFBResult (*SetPlaybackFlags) (
          IDirectFBVideoProvider            *thiz,
          DFBVideoProviderPlaybackFlags      flags
     );

     /*
      * Set the speed multiplier.
      *
      * Values below 1.0 reduce playback speed while values over
      * 1.0 increase it.
      * Specifying a value of 0.0 has the effect of putting the
      * playback in pause mode.
      */
     DFBResult (*SetSpeed) (
          IDirectFBVideoProvider            *thiz,
          double                             multiplier
     );

     /*
      * Get current speed multiplier.
      */
     DFBResult (*GetSpeed) (
          IDirectFBVideoProvider            *thiz,
          double                            *ret_multiplier
     );

     /*
      * Set volume level.
      *
      * Values between 0.0 and 1.0 adjust the volume level.
      * Values over 1.0 increase the amplification level.
      */
     DFBResult (*SetVolume) (
          IDirectFBVideoProvider            *thiz,
          float                              level
     );

     /*
      * Get volume level.
      */
     DFBResult (*GetVolume) (
          IDirectFBVideoProvider            *thiz,
          float                             *ret_level
     );

  /** Event buffers **/

     /*
      * Create an event buffer for this video provider and
      * attach it.
      */
     DFBResult (*CreateEventBuffer) (
          IDirectFBVideoProvider             *thiz,
          IDirectFBEventBuffer             **ret_interface
     );

     /*
      * Attach an existing event buffer to this video provider.
      */
     DFBResult (*AttachEventBuffer) (
          IDirectFBVideoProvider            *thiz,
          IDirectFBEventBuffer              *buffer
     );

     /*
      * Enable specific events to be sent from the video
      * provider.
      *
      * The argument is a mask of events that will be set in the
      * videoproviders's event mask.
      * The default event mask is DVPET_ALL.
      */
     DFBResult (*EnableEvents) (
          IDirectFBVideoProvider            *thiz,
          DFBVideoProviderEventType          mask
     );

     /*
      * Disable specific events from being sent from the video
      * provider.
      *
      * The argument is a mask of events that will be cleared in
      * the video providers's event mask.
      * The default event mask is DVPET_ALL.
      */
     DFBResult (*DisableEvents) (
          IDirectFBVideoProvider            *thiz,
          DFBVideoProviderEventType          mask
     );

     /*
      * Detach an event buffer from this video provider.
      */
     DFBResult (*DetachEventBuffer) (
          IDirectFBVideoProvider            *thiz,
          IDirectFBEventBuffer              *buffer
     );

  /** Buffer control **/

     /*
      * Get buffer occupancy (audio/video) when playing stream.
      */
     DFBResult (*GetBufferOccupancy) (
          IDirectFBVideoProvider            *thiz,
          DFBBufferOccupancy                *ret_occ
     );

     /*
      * Set buffer thresholds for the audio/video playback.
      */
     DFBResult (*SetBufferThresholds) (
          IDirectFBVideoProvider            *thiz,
          DFBBufferThresholds                thresh
     );

     /*
      * Get buffer thresholds for the audio/video playback.
      */
     DFBResult (*GetBufferThresholds) (
          IDirectFBVideoProvider            *thiz,
          DFBBufferThresholds               *ret_thresh
     );

   /** Playback **/

     /*
      * Update the video rendering into the specified rectangle
      * of the destination surface.
      */
     DFBResult (*SetDestination) (
          IDirectFBVideoProvider            *thiz,
          IDirectFBSurface                  *destination,
          const DFBRectangle                *dest_rect
     );
)

/***********************
 * IDirectFBDataBuffer *
 ***********************/

/*
 * IDirectFBDataBuffer is the data buffer interface.
 */
D_DEFINE_INTERFACE( IDirectFBDataBuffer,

   /** Buffer handling **/

     /*
      * Flush all data in this buffer.
      *
      * This method only applies to streaming buffers.
      */
     DFBResult (*Flush) (
          IDirectFBDataBuffer               *thiz
     );

     /*
      * Finish writing into a streaming buffer.
      *
      * Subsequent calls to PutData() will fail, while attempts
      * to fetch data from the buffer will return EOF unless
      * there is still data available.
      */
     DFBResult (*Finish) (
          IDirectFBDataBuffer               *thiz
     );

     /*
      * Seek to a given byte position.
      *
      * This method only applies to static buffers.
      */
     DFBResult (*SeekTo) (
          IDirectFBDataBuffer               *thiz,
          unsigned int                       offset
     );

     /*
      * Get the current byte position within a static buffer.
      *
      * This method only applies to static buffers.
      */
     DFBResult (*GetPosition) (
          IDirectFBDataBuffer               *thiz,
          unsigned int                      *ret_offset
     );

     /*
      * Get the length of a static or streaming buffer in bytes.
      *
      * The length of a static buffer is its static size.
      * A streaming buffer has a variable length reflecting the
      * amount of buffered data.
      */
     DFBResult (*GetLength) (
          IDirectFBDataBuffer               *thiz,
          unsigned int                      *ret_length
     );

   /** Waiting for data **/

     /*
      * Wait for data to be available.
      *
      * This method blocks until at least the specified number
      * of bytes is available.
      */
     DFBResult (*WaitForData) (
          IDirectFBDataBuffer               *thiz,
          unsigned int                       length
     );

     /*
      * Wait for data to be available within an amount of time.
      *
      * This method blocks until at least the specified number
      * of bytes is available or the timeout is reached.
      */
     DFBResult (*WaitForDataWithTimeout) (
          IDirectFBDataBuffer               *thiz,
          unsigned int                       length,
          unsigned int                       seconds,
          unsigned int                       milli_seconds
     );

   /** Retrieving data **/

     /*
      * Fetch data from a streaming or static buffer.
      *
      * Static buffers will increase the data pointer.
      * Streaming buffers will flush the data portion.
      * The maximum number of bytes to fetch is specified by
      * 'length', the actual number of bytes fetched is returned
      * via 'ret_read'.
      */
     DFBResult (*GetData) (
          IDirectFBDataBuffer               *thiz,
          unsigned int                       length,
          void                              *ret_data_ptr,
          unsigned int                      *ret_read
     );

     /*
      * Peek data from a streaming or static buffer.
      *
      * Unlike GetData() this method won't increase the data
      * pointer or flush any portions of the data held.
      *
      * Additionally an offset relative to the current data
      * pointer or beginning of the streaming buffer can be
      * specified.
      * The maximum number of bytes to peek is specified by
      * 'length', the actual number of bytes peeked is returned
      * via 'ret_read'.
      */
     DFBResult (*PeekData) (
          IDirectFBDataBuffer               *thiz,
          unsigned int                       length,
          int                                offset,
          void                              *ret_data_ptr,
          unsigned int                      *ret_read
     );

     /*
      * Check if there is data available.
      */
     DFBResult (*HasData) (
          IDirectFBDataBuffer               *thiz
     );

   /** Providing data **/

     /*
      * Append a block of data to a streaming buffer.
      *
      * This method does not wait until the data got fetched.
      * Static buffers don't support this method.
      */
     DFBResult (*PutData) (
          IDirectFBDataBuffer               *thiz,
          const void                        *data_ptr,
          unsigned int                       length
     );

   /** Media from data **/

     /*
      * Create an image provider using the buffers data.
      */
     DFBResult (*CreateImageProvider) (
          IDirectFBDataBuffer               *thiz,
          IDirectFBImageProvider           **ret_interface
     );

     /*
      * Create a video provider using the buffers data.
      */
     DFBResult (*CreateVideoProvider) (
          IDirectFBDataBuffer               *thiz,
          IDirectFBVideoProvider           **ret_interface
     );

     /*
      * Load a font using the buffer's data, given a description
      * of how to load the glyphs.
      */
     DFBResult (*CreateFont) (
          IDirectFBDataBuffer               *thiz,
          const DFBFontDescription          *desc,
          IDirectFBFont                    **ret_interface
     );
)

#ifdef __cplusplus
}
#endif

#endif
