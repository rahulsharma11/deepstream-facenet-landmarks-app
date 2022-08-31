/*
 * Copyright (c) 2018-2019, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _NVGSTDS_IVCORE_H_
#define _NVGSTDS_IVCORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <gst/gst.h>

typedef struct
{
  // Create a bin for the element only if enabled
  gboolean enable;
  // Struct members to store config / properties for the element
  gboolean full_frame;
  gint processing_width;
  gint processing_height;
  gint min_box_size;
  gint max_q_size;
  gint min_emb_quality; 
  gint min_face_quality;
  gboolean blur_objects;
  guint unique_id;
  guint gpu_id;
  gchar *amqp_url;
  gchar *amqp_cfg;
  gchar *fr_embed_model;
  gchar *thresholds_for_antispoof;
  gchar *fr_antispoofing_model;
  gchar* fr_antispoofing2_model;
  gchar* fr_antispoofing_permute_model;
  guint x_padding;
  guint y_padding;
  gint min_trk_push;
  gint max_trk_push;
  gchar *data_loc;
  gint use_face_quality;
  gint frame_skip_numbers;
  gchar* sources_to_facespoof_check;
  gchar* face_spoof_resolution;
  gint xfrontal_threshold;
  gint yfrontal_threshold;
  // For nvvidconv
  guint nvbuf_memory_type;
} NvDsIVCoreConfig;

// Struct to store references to the bin and elements
typedef struct
{
  GstElement *bin;
  GstElement *queue;
  GstElement *pre_conv;
  GstElement *cap_filter;
  GstElement *elem_ivcore;
} NvDsIVCoreBin;

// Function to create the bin and set properties
gboolean
create_ivcore_bin (NvDsIVCoreConfig *config, NvDsIVCoreBin *bin);

#ifdef __cplusplus
}
#endif

#endif /* _NVGSTDS_IVCORE_H_ */
