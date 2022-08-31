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

#include "deepstream_common.h"
#include "deepstream_ivcore.h"


// Create bin, add queue and the element, link all elements and ghost pads,
// Set the element properties from the parsed config
gboolean
create_ivcore_bin (NvDsIVCoreConfig *config, NvDsIVCoreBin *bin)
{
  GstCaps *caps = NULL;
  gboolean ret = FALSE;

  bin->bin = gst_bin_new ("ivcore_bin");
  if (!bin->bin) {
    NVGSTDS_ERR_MSG_V ("Failed to create 'ivcore_bin'");
    goto done;
  }

  bin->queue =
      gst_element_factory_make (NVDS_ELEM_QUEUE, "ivcore_queue");
  if (!bin->queue) {
    NVGSTDS_ERR_MSG_V ("Failed to create 'ivcore_queue'");
    goto done;
  }

  bin->elem_ivcore =
      gst_element_factory_make (NVDS_ELEM_IVCORE_ELEMENT, "ivcore0");
  if (!bin->elem_ivcore) {
    NVGSTDS_ERR_MSG_V ("Failed to create 'ivcore0'");
    goto done;
  }

  bin->pre_conv = gst_element_factory_make (NVDS_ELEM_VIDEO_CONV, "ivcore_conv0");
  if (!bin->pre_conv) {
    NVGSTDS_ERR_MSG_V ("Failed to create 'ivcore_conv0'");
    goto done;
  }

  bin->cap_filter =
      gst_element_factory_make (NVDS_ELEM_CAPS_FILTER, "ivcore_caps");
  if (!bin->cap_filter) {
    NVGSTDS_ERR_MSG_V ("Failed to create 'ivcore_caps'");
    goto done;
  }

  if (config->blur_objects) {
    caps =
      gst_caps_new_simple ("video/x-raw", "format", G_TYPE_STRING,
          "RGBA", NULL);

    GstCapsFeatures *feature = NULL;
    feature = gst_caps_features_new (MEMORY_FEATURES, NULL);
    gst_caps_set_features (caps, 0, feature);

    g_object_set (G_OBJECT (bin->cap_filter), "caps", caps, NULL);

    gst_caps_unref(caps);
  }

  gst_bin_add_many (GST_BIN (bin->bin), bin->queue,
      bin->pre_conv, bin->cap_filter, bin->elem_ivcore, NULL);

  NVGSTDS_LINK_ELEMENT (bin->queue, bin->pre_conv);
  NVGSTDS_LINK_ELEMENT (bin->pre_conv, bin->cap_filter);
  NVGSTDS_LINK_ELEMENT (bin->cap_filter, bin->elem_ivcore);

  NVGSTDS_BIN_ADD_GHOST_PAD (bin->bin, bin->queue, "sink");

  NVGSTDS_BIN_ADD_GHOST_PAD (bin->bin, bin->elem_ivcore, "src");

  g_object_set (G_OBJECT (bin->elem_ivcore),
      "full-frame", config->full_frame,
      "processing-width", config->processing_width,
      "processing-height", config->processing_height,
      "blur-objects", config->blur_objects,
      "unique-id", config->unique_id,
      "gpu-id", config->gpu_id, 
      "amqp-url",config->amqp_url,
      "amqp-cfg",GET_FILE_PATH(config->amqp_cfg), 
      "fr-embed-model",config->fr_embed_model,
      "thresholds-for-antispoof",config->thresholds_for_antispoof,
      "fr-antispoofing-model",config->fr_antispoofing_model,
      "fr-antispoofing_permute-model",config->fr_antispoofing_permute_model,
      "x-padding",config->x_padding,
      "y-padding",config->y_padding,
      "min-box-size",config->min_box_size,
      "use-face-quality", config->use_face_quality,
      "frame-skip-numbers", config->frame_skip_numbers,
      "sources-to-facespoof-check",config->sources_to_facespoof_check,
      "face_spoof_resolution",config->face_spoof_resolution,
      "xfrontal-threshold", config->xfrontal_threshold,
      "yfrontal-threshold", config->yfrontal_threshold,
      "max-q-size",config->max_q_size,
      "min-trk-push",config->min_trk_push,
      "max-trk-push",config->max_trk_push,
      "data-loc",config->data_loc,
      "min-emb-quality",config->min_emb_quality, 
      "min-face-quality",config->min_face_quality,NULL);

 g_object_set (G_OBJECT (bin->pre_conv), "gpu-id", config->gpu_id, NULL);

 g_object_set (G_OBJECT (bin->pre_conv), "nvbuf-memory-type",
     config->nvbuf_memory_type, NULL);

 ret = TRUE;

done:
  if (!ret) {
    NVGSTDS_ERR_MSG_V ("%s failed", __func__);
  }

  return ret;
}
