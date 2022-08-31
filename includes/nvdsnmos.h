/*
 * Copyright (c) 2022, NVIDIA CORPORATION. All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 *
 */

/**
 * @file
 * <b>NVIDIA DeepStream Networked Media Open Specifications (NMOS) Library</b>
 *
 * @b Description: This file defines common elements used in the API
 * exposed by the NvDsNmos library.
 */

/**
 * @defgroup  ee_nvdsnmos  NvDsNmos API Common Elements
 *
 * Defines common elements used in the API exposed by the NvDsNmos library.
 *
 * @ingroup NvDsNmosApi
 * @{
 */

#ifndef NVDSNMOS_H
#define NVDSNMOS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _NvDsNmosNodeServer NvDsNmosNodeServer;

/**
 * Type for a callback from NvDsNmos library when an IS-05 Connection API
 * activation occurs.
 *
 * @param[in] server A pointer to the server issuing the callback.
 * @param[in] id     The unique identifier for the sender or receiver
 *                   to be activated or deactivated.
 * @param[in] sdp    The updated Session Description Protocol data
 *                   for the sender or receiver, or a null pointer when
 *                   the sender or receiver is being deactivated.
 *                   The new data only updates the transport parameters
 *                   of the sender or receiver, not the media format.
 *                   The x-nvds-id session-level attribute specifies
 *                   the unique identifier for the sender or receiver,
 *                   @p id.
 *                   For a receiver, the x-nvds-iface-ip media-level
 *                   attribute is used to specify the interface IP
 *                   address on which the stream is received.
 *                   For a sender, the x-nvds-src-port media-level
 *                   attribute is used to specify the source port
 *                   from which the stream is transmitted.
 * @return Whether the activation could be applied.
 */
typedef bool (* nmos_connection_rtp_activation_callback)(
    NvDsNmosNodeServer *server,
    const char *id,
    const char *sdp);

/**
 * Defines some common severity/logging levels for log messages from
 * the NvDsNmos library.
 */
enum {
    /** Low level debugging information. */
    NVDSNMOS_LOG_DEVEL = -40,
    /** Chatty messages such as detailed API request/response tracking. */
    NVDSNMOS_LOG_VERBOSE = -10,
    /** Higher level information about expected API events. */
    NVDSNMOS_LOG_INFO = 0,
    /** Minor problems that could be recovered automatically by the library. */
    NVDSNMOS_LOG_WARNING = 10,
    /** More serious recoverable errors such as rejected requests. */
    NVDSNMOS_LOG_ERROR = 20,
    /** Errors which are unlikely to be recoverable without restarting the server. */
    NVDSNMOS_LOG_SEVERE = 30,
    /** Errors which are likely to cause the server to immediately terminate. */
    NVDSNMOS_LOG_FATAL = 40
};

/**
 * Type for a callback from NvDsNmos library for log messages.
 *
 * @param[in] server     A pointer to the server issuing the callback.
 * @param[in] categories A comma separated list of topics, indicating
 *                       e.g. the submodule originating the log message.
 * @param[in] level      The severity/verbosity level. Values greater
 *                       than zero are warnings and errors. Values less
 *                       than zero are debugging or trace messages.
 * @param[in] message    The message itself.
 */
typedef void (* nmos_logging_callback)(
    NvDsNmosNodeServer *server,
    const char *categories,
    int level,
    const char *message);

typedef struct _NvDsNmosReceiverConfig NvDsNmosReceiverConfig;
typedef struct _NvDsNmosSenderConfig NvDsNmosSenderConfig;

/**
 * Defines configuration settings used to create an @ref NvDsNmosNodeServer.
 * The structure should be zero initialized.
 */
typedef struct _NvDsNmosNodeConfig
{
    /** Holds the fully-qualified host name, e.g. nmos-node.local.
        May be null in which case the system host name is determined
        automatically. */
    const char *host_name;
    /** Holds the port number for the HTTP APIs, e.g. 80.
        May be null in which case default ports are used for each API. */
    unsigned int http_port;

    /** Holds a string used to ensure repeatable UUID generation.
        May be null in which case a random seed is used; not recommended. */
    const char *seed;

    /** Holds configuration settings for the receivers. The array's size
        must be equal to #num_receivers. May be null. */
    NvDsNmosReceiverConfig *receivers;
    /** Holds the number of #receivers. May be zero. */
    unsigned int num_receivers;
    /** Holds configuration settings for the senders. The array's size
        must be equal to #num_senders. May be null. */
    NvDsNmosSenderConfig *senders;
    /** Holds the number of #senders. May be zero. */
    unsigned int num_senders;

    /** Holds the callback for handling an IS-05 Connection API activation.
        May be null. */
    nmos_connection_rtp_activation_callback rtp_connection_activated;

    /** Holds the callback for handling log messages. May be null. */
    nmos_logging_callback log_callback;
    /** Holds the minimum severity/verbosity level for which to make
        logging callbacks. */
    int log_level;
    /** Holds topics for which to make logging callbacks. The array's size
        must be equal to #num_log_categories. May be null. */
    const char **log_categories;
    /** Holds the number #log_categories. May be zero. */
    unsigned int num_log_categories;
} NvDsNmosNodeConfig;

/**
 * Defines configuration settings used to create receivers in an
 * @ref NvDsNmosNodeServer.
 */
typedef struct _NvDsNmosReceiverConfig
{
    /** Holds the Session Description Protocol data used to configure
        the receiver. Must not be null. The SDP data must be valid
        as per the relevant IETF RFC and SMPTE standards for the
        media format and transport.
        The x-nvds-id session-level attribute specifies the unique
        identifier for the receiver.
        The x-nvds-iface-ip media-level attribute is used to specify
        the interface IP address on which the stream is received. */
    const char *sdp;
} NvDsNmosReceiverConfig;

/**
 * Defines configuration settings used to create senders in an
 * @ref NvDsNmosNodeServer.
 */
typedef struct _NvDsNmosSenderConfig
{
    /** Holds the Session Description Protocol data used to configure
        the sender. Must not be null. The SDP data must be valid
        as per the relevant IETF RFC and SMPTE standards for the
        media format and transport.
        The x-nvds-id session-level attribute specifies the unique
        identifier for the sender.
        The x-nvds-src-port media-level attribute is used to specify
        the source port from which the stream is transmitted. */
    const char *sdp;
} NvDsNmosSenderConfig;

/**
 * Holds the implementation details of a running NvDsNmos server.
 * The structure should be zero initialized, with the possible
 * exception of the #user_data member.
 */
typedef struct _NvDsNmosNodeServer
{
    /**
     * Holds a pointer to user data, not used by the NvDsNmos library.
     * Can be used for example to access application-specific data in
     * callbacks from the NvDsNmos library.
     */
    void *user_data;
    /**
     * Holds an opaque pointer used by the NvDsNmos library.
     */
    void *impl;
} NvDsNmosNodeServer;

/**
 * Initialize and start an NMOS Node server according to the specified
 * configuration settings.
 *
 * The server should be deinitialized using @ref destroy_nmos_node_server.
 *
 * @param[in] config Pointer to the configuration settings.
 * @param[in] server Pointer to the server to be initialized.
 * @return Whether the server has been created and successfully started.
 */
bool create_nmos_node_server(
    const NvDsNmosNodeConfig *config,
    NvDsNmosNodeServer *server);

/**
 * Stop and deinitialize an NMOS Node server.
 *
 * The server should have been successfully initialized using
 * @ref create_nmos_node_server.
 *
 * @param[in] server  Pointer to the server to be deinitialized.
 * @return Whether the server has been successfully stopped and deinitialized.
 */
bool destroy_nmos_node_server(
    NvDsNmosNodeServer *server);

/**
 * Update the configuration settings of a sender or receiver.
 *
 * @param[in] server A pointer to the server to be updated.
 * @param[in] id     The unique identifier for the sender or receiver
 *                   to be activated or deactivated.
 * @param[in] sdp    The updated Session Description Protocol data
 *                   for the sender or receiver, or a null pointer when
 *                   the sender or receiver is being deactivated.
 *                   The new data only updates the transport parameters
 *                   of the sender or receiver, not the media format.
 *                   The x-nvds-id session-level attribute specifies
 *                   the unique identifier for the sender or receiver,
 *                   @p id.
 *                   For a receiver, the x-nvds-iface-ip media-level
 *                   attribute is used to specify the interface IP
 *                   address on which the stream is received.
 *                   For a sender, the x-nvds-src-port media-level
 *                   attribute is used to specify the source port
 *                   from which the stream is transmitted.
 * @return Whether the update has been successfully applied.
 */
bool nmos_connection_rtp_activate(
    NvDsNmosNodeServer *server,
    const char *id,
    const char *sdp);

#ifdef __cplusplus
}
#endif

#endif

/** @} */
