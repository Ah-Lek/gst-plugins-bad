/* GStreamer Wayland video sink
 *
 * Copyright (C) 2011 Intel Corporation
 * Copyright (C) 2011 Sreerenj Balachandran <sreerenj.balachandran@intel.com>
 * Copyright (C) 2012 Wim Taymans <wim.taymans@gmail.com>
 * Copyright (C) 2014 Collabora Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "wlvideoformat.h"
#ifdef HAVE_WAYLAND_KMS
#include "wayland-kms-client-protocol.h"
#endif

GST_DEBUG_CATEGORY_EXTERN (gstwayland_debug);
#define GST_CAT_DEFAULT gstwayland_debug

typedef struct
{
  guint wl_format;
  GstVideoFormat gst_format;
} wl_VideoFormat;

#ifdef HAVE_WAYLAND_KMS
static const wl_VideoFormat formats[] = {
#if G_BYTE_ORDER == G_BIG_ENDIAN
  {WL_KMS_FORMAT_XRGB8888, GST_VIDEO_FORMAT_xRGB},
  {WL_KMS_FORMAT_ARGB8888, GST_VIDEO_FORMAT_ARGB},
  {WL_KMS_FORMAT_XBGR8888, GST_VIDEO_FORMAT_xBGR},
  {WL_KMS_FORMAT_RGBX8888, GST_VIDEO_FORMAT_RGBx},
  {WL_KMS_FORMAT_BGRX8888, GST_VIDEO_FORMAT_BGRx},
  {WL_KMS_FORMAT_ABGR8888, GST_VIDEO_FORMAT_ABGR},
  {WL_KMS_FORMAT_RGBA8888, GST_VIDEO_FORMAT_RGBA},
  {WL_KMS_FORMAT_BGRA8888, GST_VIDEO_FORMAT_BGRA},
#else
  {WL_KMS_FORMAT_XRGB8888, GST_VIDEO_FORMAT_BGRx},
  {WL_KMS_FORMAT_ARGB8888, GST_VIDEO_FORMAT_BGRA},
  {WL_KMS_FORMAT_XBGR8888, GST_VIDEO_FORMAT_RGBx},
  {WL_KMS_FORMAT_RGBX8888, GST_VIDEO_FORMAT_xBGR},
  {WL_KMS_FORMAT_BGRX8888, GST_VIDEO_FORMAT_xRGB},
  {WL_KMS_FORMAT_ABGR8888, GST_VIDEO_FORMAT_RGBA},
  {WL_KMS_FORMAT_RGBA8888, GST_VIDEO_FORMAT_ABGR},
  {WL_KMS_FORMAT_BGRA8888, GST_VIDEO_FORMAT_ARGB},
#endif
  {WL_KMS_FORMAT_RGB888, GST_VIDEO_FORMAT_RGB},
  {WL_KMS_FORMAT_BGR888, GST_VIDEO_FORMAT_BGR},
  {WL_KMS_FORMAT_RGB565, GST_VIDEO_FORMAT_RGB16},
  {WL_KMS_FORMAT_BGR565, GST_VIDEO_FORMAT_BGR16},

  {WL_KMS_FORMAT_YUYV, GST_VIDEO_FORMAT_YUY2},
  {WL_KMS_FORMAT_YVYU, GST_VIDEO_FORMAT_YVYU},
  {WL_KMS_FORMAT_UYVY, GST_VIDEO_FORMAT_UYVY},
  {WL_KMS_FORMAT_AYUV, GST_VIDEO_FORMAT_AYUV},
  {WL_KMS_FORMAT_NV12, GST_VIDEO_FORMAT_NV12},
  {WL_KMS_FORMAT_NV21, GST_VIDEO_FORMAT_NV21},
  {WL_KMS_FORMAT_NV16, GST_VIDEO_FORMAT_NV16},
  {WL_KMS_FORMAT_YUV410, GST_VIDEO_FORMAT_YUV9},
  {WL_KMS_FORMAT_YVU410, GST_VIDEO_FORMAT_YVU9},
  {WL_KMS_FORMAT_YUV411, GST_VIDEO_FORMAT_Y41B},
  {WL_KMS_FORMAT_YUV420, GST_VIDEO_FORMAT_I420},
  {WL_KMS_FORMAT_YVU420, GST_VIDEO_FORMAT_YV12},
  {WL_KMS_FORMAT_YUV422, GST_VIDEO_FORMAT_Y42B},
  {WL_KMS_FORMAT_YUV444, GST_VIDEO_FORMAT_v308},
};
#else /* HAVE_WAYLAND_KMS */
static const wl_VideoFormat formats[] = {
#if G_BYTE_ORDER == G_BIG_ENDIAN
  {WL_SHM_FORMAT_XRGB8888, GST_VIDEO_FORMAT_xRGB},
  {WL_SHM_FORMAT_ARGB8888, GST_VIDEO_FORMAT_ARGB},
  {WL_SHM_FORMAT_XBGR8888, GST_VIDEO_FORMAT_xBGR},
  {WL_SHM_FORMAT_RGBX8888, GST_VIDEO_FORMAT_RGBx},
  {WL_SHM_FORMAT_BGRX8888, GST_VIDEO_FORMAT_BGRx},
  {WL_SHM_FORMAT_ABGR8888, GST_VIDEO_FORMAT_ABGR},
  {WL_SHM_FORMAT_RGBA8888, GST_VIDEO_FORMAT_RGBA},
  {WL_SHM_FORMAT_BGRA8888, GST_VIDEO_FORMAT_BGRA},
#else
  {WL_SHM_FORMAT_XRGB8888, GST_VIDEO_FORMAT_BGRx},
  {WL_SHM_FORMAT_ARGB8888, GST_VIDEO_FORMAT_BGRA},
  {WL_SHM_FORMAT_XBGR8888, GST_VIDEO_FORMAT_RGBx},
  {WL_SHM_FORMAT_RGBX8888, GST_VIDEO_FORMAT_xBGR},
  {WL_SHM_FORMAT_BGRX8888, GST_VIDEO_FORMAT_xRGB},
  {WL_SHM_FORMAT_ABGR8888, GST_VIDEO_FORMAT_RGBA},
  {WL_SHM_FORMAT_RGBA8888, GST_VIDEO_FORMAT_ABGR},
  {WL_SHM_FORMAT_BGRA8888, GST_VIDEO_FORMAT_ARGB},
#endif
  {WL_SHM_FORMAT_RGB888, GST_VIDEO_FORMAT_RGB},
  {WL_SHM_FORMAT_BGR888, GST_VIDEO_FORMAT_BGR},
  {WL_SHM_FORMAT_RGB565, GST_VIDEO_FORMAT_RGB16},
  {WL_SHM_FORMAT_BGR565, GST_VIDEO_FORMAT_BGR16},

  {WL_SHM_FORMAT_YUYV, GST_VIDEO_FORMAT_YUY2},
  {WL_SHM_FORMAT_YVYU, GST_VIDEO_FORMAT_YVYU},
  {WL_SHM_FORMAT_UYVY, GST_VIDEO_FORMAT_UYVY},
  {WL_SHM_FORMAT_AYUV, GST_VIDEO_FORMAT_AYUV},
  {WL_SHM_FORMAT_NV12, GST_VIDEO_FORMAT_NV12},
  {WL_SHM_FORMAT_NV21, GST_VIDEO_FORMAT_NV21},
  {WL_SHM_FORMAT_NV16, GST_VIDEO_FORMAT_NV16},
  {WL_SHM_FORMAT_YUV410, GST_VIDEO_FORMAT_YUV9},
  {WL_SHM_FORMAT_YVU410, GST_VIDEO_FORMAT_YVU9},
  {WL_SHM_FORMAT_YUV411, GST_VIDEO_FORMAT_Y41B},
  {WL_SHM_FORMAT_YUV420, GST_VIDEO_FORMAT_I420},
  {WL_SHM_FORMAT_YVU420, GST_VIDEO_FORMAT_YV12},
  {WL_SHM_FORMAT_YUV422, GST_VIDEO_FORMAT_Y42B},
  {WL_SHM_FORMAT_YUV444, GST_VIDEO_FORMAT_v308},
};
#endif /* HAVE_WAYLAND_KMS */

guint
gst_video_format_to_wayland_format (GstVideoFormat format)
{
  guint i;

  for (i = 0; i < G_N_ELEMENTS (formats); i++)
    if (formats[i].gst_format == format)
      return formats[i].wl_format;

  GST_WARNING ("wayland video format not found");
  return -1;
}

GstVideoFormat
gst_wayland_format_to_video_format (guint wl_format)
{
  guint i;

  for (i = 0; i < G_N_ELEMENTS (formats); i++)
    if (formats[i].wl_format == wl_format)
      return formats[i].gst_format;

  GST_WARNING ("gst video format not found");
  return GST_VIDEO_FORMAT_UNKNOWN;
}

const gchar *
gst_wayland_format_to_string (guint wl_format)
{
  return gst_video_format_to_string
      (gst_wayland_format_to_video_format (wl_format));
}
