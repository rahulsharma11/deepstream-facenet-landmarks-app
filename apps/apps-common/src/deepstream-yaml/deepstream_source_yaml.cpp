/*
 * Copyright (c) 2022, NVIDIA CORPORATION. All rights reserved.
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
#include "deepstream_config_yaml.h"
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

#define N_DECODE_SURFACES 16
#define N_EXTRA_SURFACES 1

gboolean
parse_source_yaml (NvDsSourceConfig *config, std::vector<std::string> headers,
    std::vector<std::string> source_values,  gchar *cfg_file_path)
{
  gboolean ret = FALSE;

  config->latency = 100;
  config->num_decode_surfaces = N_DECODE_SURFACES;
  config->num_extra_surfaces = N_EXTRA_SURFACES;

  for(unsigned int i = 0; i < headers.size(); i++)
  {
    std::string paramKey = headers[i];

    if (paramKey == "type") {
      gint temp = std::stoi(source_values[i]);
      config->type = (NvDsSourceType) temp;
    } else if (paramKey == "enable") {
      config->enable = std::stoul(source_values[i]);
    } else if (paramKey ==  "camera-width") {
      config->source_width = std::stoi(source_values[i]);
    } else if (paramKey == "camera-height") {
      config->source_height = std::stoi(source_values[i]);
    } else if (paramKey == "camera-fps-n") {
      config->source_fps_n = std::stoi(source_values[i]);
    } else if (paramKey == "camera-fps-d") {
      config->source_fps_d = std::stoi(source_values[i]);
    } else if (paramKey == "camera-csi-sensor-id") {
      config->camera_csi_sensor_id = std::stoi(source_values[i]);
    } else if (paramKey == "camera-v4l2-dev-node") {
      config->camera_v4l2_dev_node =
          std::stoi(source_values[i]);;
    } else if (paramKey == "udp-buffer-size") {
      config->udp_buffer_size = std::stoi(source_values[i]);
    } else if (paramKey == "alsa-device") {
      std::string temp = source_values[i];
      config->alsa_device = (char*) malloc(sizeof(char) * 1024);
      std::strncpy (config->alsa_device, temp.c_str(), 1024);
    } else if (paramKey == "uri") {
      std::string temp = source_values[i];
      char* uri = (char*) malloc(sizeof(char) * 1024);
      std::strncpy (uri, temp.c_str(), 1024);
      char *str;
      if (g_str_has_prefix (uri, "file://")) {
        str = g_strdup (uri + 7);
        config->uri = (char*) malloc(sizeof(char) * 1024);
        get_absolute_file_path_yaml (cfg_file_path, str, config->uri);
        config->uri = g_strdup_printf ("file://%s", config->uri);
        g_free (uri);
        g_free (str);
      } else {
        config->uri = uri;
      }
    } else if (paramKey == "latency") {
      config->latency = std::stoi(source_values[i]);
    } else if (paramKey == "num-sources") {
      config->num_sources = std::stoul(source_values[i]);
      if (config->num_sources < 1) {
        config->num_sources = 1;
      }
    } else if (paramKey == "gpu-id") {
      config->gpu_id = std::stoul(source_values[i]);
    } else if (paramKey == "num-decode-surfaces") {
      config->num_decode_surfaces = std::stoul(source_values[i]);
    } else if (paramKey == "num-extra-surfaces") {
      config->num_extra_surfaces = std::stoul(source_values[i]);
    } else if (paramKey == "drop-frame-interval") {
      config->drop_frame_interval = std::stoul(source_values[i]);
    } else if (paramKey == "camera-id") {
      config->camera_id = std::stoul(source_values[i]);
    } else if (paramKey == "rtsp-reconnect-interval-sec") {
      config->rtsp_reconnect_interval_sec =
          std::stoi(source_values[i]);
    } else if (paramKey == "rtsp-reconnect-attempts") {
      config->rtsp_reconnect_attempts =
          std::stoul(source_values[i]);
    } else if (paramKey == "intra-decode-enable") {
      config->Intra_decode = (gboolean) std::stoul(source_values[i]);
    } else if (paramKey ==  "cudadec-memtype") {
      config->cuda_memory_type =
          std::stoul(source_values[i]);
    } else if (paramKey == "nvbuf-memory-type") {
      config->nvbuf_memory_type =
          std::stoul(source_values[i]);
    } else if (paramKey == "select-rtp-protocol") {
      config->select_rtp_protocol =
          std::stoul(source_values[i]);
    } else if (paramKey == "source-id") {
      config->source_id = std::stoul(source_values[i]);
    } else if (paramKey == "smart-record") {
      config->smart_record = std::stoul(source_values[i]);
    } else if (paramKey == "smart-rec-dir-path") {
      std::string temp = source_values[i];
      config->dir_path = (char*) malloc(sizeof(char) * 1024);
      std::strncpy (config->dir_path, temp.c_str(), 1024);

      if (access (config->dir_path, 2)) {
        if (errno == ENOENT || errno == ENOTDIR) {
          g_print ("ERROR: Directory (%s) doesn't exist.\n", config->dir_path);
        } else if (errno == EACCES) {
          g_print ("ERROR: No write permission in %s\n", config->dir_path);
        }
        goto done;
      }
    } else if (paramKey == "smart-rec-file-prefix") {
      std::string temp = source_values[i];
      config->file_prefix = (char*) malloc(sizeof(char) * 1024);
      std::strncpy (config->file_prefix, temp.c_str(), 1024);
    } else if (paramKey == "smart-rec-video-cache") {
      cout << "Deprecated config smart-rec-video-cache used in source. Use smart-rec-cache instead"  << endl;

      config->smart_rec_cache_size = std::stoul(source_values[i]);
    } else if (paramKey == "smart-rec-cache") {
      config->smart_rec_cache_size =
        std::stoul(source_values[i]);
    } else if (paramKey == "smart-rec-container") {
      config->smart_rec_container = std::stoul(source_values[i]);
    } else if (paramKey == "smart-rec-start-time") {
      config->smart_rec_start_time = std::stoul(source_values[i]);
    } else if (paramKey == "smart-rec-default-duration") {
      config->smart_rec_def_duration = std::stoul(source_values[i]);
    } else if (paramKey == "smart-rec-duration") {
      config->smart_rec_duration = std::stoul(source_values[i]);
    } else if (paramKey == "smart-rec-interval") {
      config->smart_rec_interval = std::stoul(source_values[i]);
    } else {
      cout << "[WARNING] Unknown param found in source : " << paramKey << endl;
    }
  }

  ret = TRUE;
done:
  if (!ret) {
    cout <<  __func__ << " failed" << endl;
  }
  return ret;
}
