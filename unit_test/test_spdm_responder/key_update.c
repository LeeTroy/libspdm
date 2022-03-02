/**
 *  Copyright Notice:
 *  Copyright 2021 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
 **/

#include "spdm_unit_test.h"
#include "internal/libspdm_responder_lib.h"
#include "internal/libspdm_secured_message_lib.h"

spdm_key_update_request_t m_libspdm_key_update_request1 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_KEY_UPDATE,
      SPDM_KEY_UPDATE_OPERATIONS_TABLE_UPDATE_KEY, 0x3 }
};
uintn m_libspdm_key_update_request1_size = sizeof(m_libspdm_key_update_request1);

spdm_key_update_request_t m_libspdm_key_update_request2 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_KEY_UPDATE,
      SPDM_KEY_UPDATE_OPERATIONS_TABLE_UPDATE_KEY, 0x3 }
};
uintn m_libspdm_key_update_request2_size = LIBSPDM_MAX_MESSAGE_BUFFER_SIZE;

spdm_key_update_request_t m_libspdm_key_update_request3 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_KEY_UPDATE,
      SPDM_KEY_UPDATE_OPERATIONS_TABLE_UPDATE_ALL_KEYS, 0x71 }
};
uintn m_libspdm_key_update_request3_size = sizeof(m_libspdm_key_update_request3);

spdm_key_update_request_t m_libspdm_key_update_request4 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_KEY_UPDATE,
      SPDM_KEY_UPDATE_OPERATIONS_TABLE_UPDATE_ALL_KEYS, 0x71 }
};
uintn m_libspdm_key_update_request4_size = LIBSPDM_MAX_MESSAGE_BUFFER_SIZE;

spdm_key_update_request_t m_libspdm_key_update_request5 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_KEY_UPDATE,
      SPDM_KEY_UPDATE_OPERATIONS_TABLE_VERIFY_NEW_KEY, 0x4A }
};
uintn m_libspdm_key_update_request5_size = sizeof(m_libspdm_key_update_request5);

spdm_key_update_request_t m_libspdm_key_update_request6 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_KEY_UPDATE,
      SPDM_KEY_UPDATE_OPERATIONS_TABLE_VERIFY_NEW_KEY, 0x4A }
};
uintn m_libspdm_key_update_request6_size = LIBSPDM_MAX_MESSAGE_BUFFER_SIZE;

spdm_key_update_request_t m_libspdm_key_update_request7 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_KEY_UPDATE,
      SPDM_KEY_UPDATE_OPERATIONS_TABLE_UPDATE_KEY, 0x92 }
};
uintn m_libspdm_key_update_request7_size = sizeof(m_libspdm_key_update_request7);

spdm_key_update_request_t m_libspdm_key_update_request8 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_KEY_UPDATE,
      SPDM_KEY_UPDATE_OPERATIONS_TABLE_UPDATE_ALL_KEYS, 0x13 }
};
uintn m_libspdm_key_update_request8_size = sizeof(m_libspdm_key_update_request8);

spdm_key_update_request_t m_libspdm_key_update_request9 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_KEY_UPDATE,
      SPDM_KEY_UPDATE_OPERATIONS_TABLE_VERIFY_NEW_KEY, 0x22 }
};
uintn m_libspdm_key_update_request9_size = sizeof(m_libspdm_key_update_request9);

spdm_key_update_request_t m_libspdm_key_update_request10 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_KEY_UPDATE, 0xFF, 0x12 }
};
uintn m_libspdm_key_update_request10_size = sizeof(m_libspdm_key_update_request10);

static void libspdm_set_standard_key_update_test_state(
    libspdm_context_t *spdm_context, uint32_t *session_id)
{
    libspdm_session_info_t    *session_info;

    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NORMAL;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_UPD_CAP;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCRYPT_CAP;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MAC_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCRYPT_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MAC_CAP;

    spdm_context->transcript.message_a.buffer_size = 0;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo =
        m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.dhe_named_group =
        m_libspdm_use_dhe_algo;
    spdm_context->connection_info.algorithm.aead_cipher_suite =
        m_libspdm_use_aead_algo;

    *session_id = 0xFFFFFFFF;
    spdm_context->latest_session_id = *session_id;
    spdm_context->last_spdm_request_session_id_valid = true;
    spdm_context->last_spdm_request_session_id = *session_id;
    session_info = &spdm_context->session_info[0];
    libspdm_session_info_init(spdm_context, session_info, *session_id, true);
    libspdm_secured_message_set_session_state(
        session_info->secured_message_context,
        LIBSPDM_SESSION_STATE_ESTABLISHED);

    libspdm_set_mem(spdm_context->last_update_request, 4, 0x00);
}

static void libspdm_set_standard_key_update_test_secrets(
    libspdm_secured_message_context_t *secured_message_context,
    uint8_t *m_rsp_secret_buffer, uint8_t rsp_secret_fill,
    uint8_t *m_req_secret_buffer, uint8_t req_secret_fill)
{
    libspdm_set_mem(m_rsp_secret_buffer, secured_message_context
                    ->hash_size, rsp_secret_fill);
    libspdm_set_mem(m_req_secret_buffer, secured_message_context
                    ->hash_size, req_secret_fill);

    libspdm_copy_mem(secured_message_context->application_secret.response_data_secret,
                     sizeof(secured_message_context->application_secret.response_data_secret),
                     m_rsp_secret_buffer, secured_message_context->aead_key_size);
    libspdm_copy_mem(secured_message_context->application_secret.request_data_secret,
                     sizeof(secured_message_context->application_secret.request_data_secret),
                     m_req_secret_buffer, secured_message_context->aead_key_size);

    libspdm_set_mem(secured_message_context->application_secret
                    .response_data_encryption_key,
                    secured_message_context->aead_key_size, (uint8_t)(0xFF));
    libspdm_set_mem(secured_message_context->application_secret
                    .response_data_salt,
                    secured_message_context->aead_iv_size, (uint8_t)(0xFF));


    libspdm_set_mem(secured_message_context->application_secret
                    .request_data_encryption_key,
                    secured_message_context->aead_key_size, (uint8_t)(0xEE));
    libspdm_set_mem(secured_message_context->application_secret
                    .request_data_salt,
                    secured_message_context->aead_iv_size, (uint8_t)(0xEE));

    secured_message_context->application_secret.
    response_data_sequence_number = 0;
    secured_message_context->application_secret.
    request_data_sequence_number = 0;
}

static void libspdm_compute_secret_update(uintn hash_size,
                                          const uint8_t *in_secret, uint8_t *out_secret,
                                          uintn out_secret_size)
{
    uint8_t m_bin_str9[128];
    uintn m_bin_str9_size;
    uint16_t length;

    length = (uint16_t) hash_size;
    libspdm_copy_mem(m_bin_str9, sizeof(m_bin_str9), &length, sizeof(uint16_t));
    libspdm_copy_mem(m_bin_str9 + sizeof(uint16_t),
                     sizeof(m_bin_str9) - sizeof(uint16_t),
                     SPDM_BIN_CONCAT_LABEL, sizeof(SPDM_BIN_CONCAT_LABEL) - 1);
    libspdm_copy_mem(m_bin_str9 + sizeof(uint16_t) + sizeof(SPDM_BIN_CONCAT_LABEL) - 1,
                     sizeof(m_bin_str9) - (sizeof(uint16_t) + sizeof(SPDM_BIN_CONCAT_LABEL) - 1),
                     SPDM_BIN_STR_9_LABEL, sizeof(SPDM_BIN_STR_9_LABEL));
    m_bin_str9_size = sizeof(uint16_t) + sizeof(SPDM_BIN_CONCAT_LABEL) - 1 +
                      sizeof(SPDM_BIN_STR_9_LABEL) - 1;
    /*context is NULL for key update*/

    libspdm_hkdf_expand(m_libspdm_use_hash_algo, in_secret, hash_size, m_bin_str9,
                        m_bin_str9_size, out_secret, out_secret_size);
}

/**
 * Test 1: receiving a correct KEY_UPDATE message from the requester with
 * the UpdateKey operation.
 * Expected behavior: the responder accepts the request, produces a valid
 * KEY_UPDATE_ACK response message, and the request data key is updated.
 **/
void libspdm_test_responder_key_update_case1(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x1;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*request side updated*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side *not* updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request1_size,
                                             &m_libspdm_key_update_request1,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_key_update_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_KEY_UPDATE_ACK);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_KEY_UPDATE_OPERATIONS_TABLE_UPDATE_KEY);
    assert_int_equal(spdm_response->header.param2,
                     m_libspdm_key_update_request1.header.param2);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 2: receiving a KEY_UPDATE message larger than specified, with the
 * UpdateKey operation.
 * Expected behavior: the responder refuses the KEY_UPDATE message and
 * produces an ERROR message indicating the InvalidRequest. No keys
 * are updated.
 **/
void libspdm_test_responder_key_update_case2(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x2;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*no keys are updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request2_size,
                                             &m_libspdm_key_update_request2,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 3: receiving a correct KEY_UPDATE from the requester, but the
 * responder is in a Busy state.
 * Expected behavior: the responder accepts the request, but produces an
 * ERROR message indicating the Busy state. No keys are updated.
 **/
void libspdm_test_responder_key_update_case3(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x3;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    /*busy state*/
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_BUSY;

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*no keys are updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request1_size,
                                             &m_libspdm_key_update_request1,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1, SPDM_ERROR_CODE_BUSY);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_int_equal(spdm_context->response_state,
                     LIBSPDM_RESPONSE_STATE_BUSY);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 4: receiving a correct KEY_UPDATE message from the requester, but
 * the responder requires resynchronization with the requester.
 * Expected behavior: the responder accepts the request, but produces an
 * ERROR message indicating the NeedResynch state. No keys are updated.
 **/
void libspdm_test_responder_key_update_case4(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x4;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    /*need resync state*/
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NEED_RESYNC;

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*no keys are updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request1_size,
                                             &m_libspdm_key_update_request1,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_REQUEST_RESYNCH);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_int_equal(spdm_context->response_state,
                     LIBSPDM_RESPONSE_STATE_NEED_RESYNC);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 5: receiving a correct KEY_UPDATE from the requester, but the
 * responder could not produce the response in time.
 * Expected behavior: the responder accepts the request, but produces an
 * ERROR message indicating the ResponseNotReady state. No keys are
 * updated.
 **/
void libspdm_test_responder_key_update_case5(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;
    spdm_error_data_response_not_ready_t *error_data;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x5;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    /*not ready state*/
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NOT_READY;

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*no keys are updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request1_size,
                                             &m_libspdm_key_update_request1,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size,
                     sizeof(spdm_error_response_t) +
                     sizeof(spdm_error_data_response_not_ready_t));
    spdm_response = (void *)response;
    error_data =
        (spdm_error_data_response_not_ready_t *)(spdm_response + 1);
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_RESPONSE_NOT_READY);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_int_equal(spdm_context->response_state,
                     LIBSPDM_RESPONSE_STATE_NOT_READY);
    assert_int_equal(error_data->request_code, SPDM_KEY_UPDATE);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 6: receiving a correct KEY_UPDATE from the requester, but the
 * responder is not set no receive a FINISH message because previous messages
 * (namely, GET_CAPABILITIES, NEGOTIATE_ALGORITHMS or GET_DIGESTS) have not
 * been received.
 * Expected behavior: the responder rejects the request, and produces an
 * ERROR message indicating the UnexpectedRequest. No keys are updated.
 **/
void libspdm_test_responder_key_update_case6(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x6;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    /*not negotiated state*/
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NOT_STARTED;

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*no keys are updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request1_size,
                                             &m_libspdm_key_update_request1,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_UNEXPECTED_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

void libspdm_test_responder_key_update_case7(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x7;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    /*"filling" buffers*/
    spdm_context->transcript.message_m.buffer_size =
        spdm_context->transcript.message_m.max_buffer_size;
    spdm_context->transcript.message_b.buffer_size =
        spdm_context->transcript.message_b.max_buffer_size;
    spdm_context->transcript.message_c.buffer_size =
        spdm_context->transcript.message_c.max_buffer_size;
    spdm_context->transcript.message_mut_b.buffer_size =
        spdm_context->transcript.message_mut_b.max_buffer_size;
    spdm_context->transcript.message_mut_c.buffer_size =
        spdm_context->transcript.message_mut_c.max_buffer_size;
#endif

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*request side updated*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side *not* updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request1_size,
                                             &m_libspdm_key_update_request1,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_key_update_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_KEY_UPDATE_ACK);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_KEY_UPDATE_OPERATIONS_TABLE_UPDATE_KEY);
    assert_int_equal(spdm_response->header.param2,
                     m_libspdm_key_update_request1.header.param2);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(session_info->session_transcript.message_m.buffer_size, 0);
    assert_int_equal(spdm_context->transcript.message_b.buffer_size, 0);
    assert_int_equal(spdm_context->transcript.message_c.buffer_size, 0);
    assert_int_equal(spdm_context->transcript.message_mut_b.buffer_size, 0);
    assert_int_equal(spdm_context->transcript.message_mut_c.buffer_size, 0);
#endif
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 8: receiving a correct KEY_UPDATE message from the requester, but the
 * responder has no capabilities for key update.
 * Expected behavior: the responder refuses the KEY_UPDATE message and
 * produces an ERROR message indicating the UnsupportedRequest. No keys are
 * updated.
 **/
void libspdm_test_responder_key_update_case8(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x8;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    /*no capabilities*/
    spdm_context->connection_info.capability.flags &=
        !SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_UPD_CAP;
    spdm_context->local_context.capability.flags &=
        !SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP;

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*no keys are updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request1_size,
                                             &m_libspdm_key_update_request1,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_UNSUPPORTED_REQUEST);
    assert_int_equal(spdm_response->header.param2, SPDM_KEY_UPDATE);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 9: receiving a correct KEY_UPDATE message from the requester, but the
 * responder is not correctly setup by not initializing a session during
 * KEY_EXCHANGE.
 * Expected behavior: the responder refuses the KEY_UPDATE message and produces
 * an ERROR message indicating the UnsupportedRequest. No keys are updated.
 **/
void libspdm_test_responder_key_update_case9(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x9;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    /*uninitialized session*/
    libspdm_secured_message_set_session_state(
        session_info->secured_message_context,
        LIBSPDM_SESSION_STATE_NOT_STARTED);

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*no keys are updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request1_size,
                                             &m_libspdm_key_update_request1,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 10: receiving a correct KEY_UPDATE message from the requester with
 * the UpdateAllKeys operation.
 * Expected behavior: the responder accepts the request, produces a valid
 * KEY_UPDATE_ACK response message, and both the request data key and the
 * response data key are updated.
 **/
void libspdm_test_responder_key_update_case10(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0xA;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*request side updated*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side updated*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_rsp_secret_buffer, m_rsp_secret_buffer,
                                  secured_message_context->hash_size);

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request3_size,
                                             &m_libspdm_key_update_request3,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_key_update_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_KEY_UPDATE_ACK);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_KEY_UPDATE_OPERATIONS_TABLE_UPDATE_ALL_KEYS);
    assert_int_equal(spdm_response->header.param2,
                     m_libspdm_key_update_request3.header.param2);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 11: receiving a KEY_UPDATE message larger than specified, with the
 * UpdateAllKeys operation.
 * Expected behavior: the responder refuses the KEY_UPDATE message and
 * produces an ERROR message indicating the InvalidRequest. No keys
 * are updated.
 **/
void libspdm_test_responder_key_update_case11(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0xB;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*no keys are updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request4_size,
                                             &m_libspdm_key_update_request4,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 12: receiving a correct KEY_UPDATE message from the requester with
 * the VerifyNewKey operation. The responder is setup as if a valid
 * KEY_UPDATE request with the UpdateKey has been previously received.
 * Expected behavior: the responder accepts the request, produces a valid
 * KEY_UPDATE_ACK response message, and the request data key is updated.
 **/
void libspdm_test_responder_key_update_case12(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0xC;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*last request: UpdateKey*/
    libspdm_copy_mem(spdm_context->last_update_request,
                     sizeof(spdm_context->last_update_request),
                     &m_libspdm_key_update_request1,
                     m_libspdm_key_update_request1_size);

    /*mocked major secret update*/
    libspdm_copy_mem(&secured_message_context->application_secret_backup.request_data_secret,
                     sizeof(secured_message_context->application_secret_backup.request_data_secret),
                     &secured_message_context->application_secret.request_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->hash_size);

    /*request side updated*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side *not* updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request5_size,
                                             &m_libspdm_key_update_request5,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_key_update_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_KEY_UPDATE_ACK);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_KEY_UPDATE_OPERATIONS_TABLE_VERIFY_NEW_KEY);
    assert_int_equal(spdm_response->header.param2,
                     m_libspdm_key_update_request5.header.param2);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 13: receiving a KEY_UPDATE message larger than specified, with the
 * VerifyNewKey operation. The responder is setup as if a valid KEY_UPDATE
 * request with the UpdateKey has been previously received.
 * Expected behavior: the responder refuses the KEY_UPDATE message and
 * produces an ERROR message indicating the InvalidRequest. The request
 * data key is not rolled back to before the UpdateKey.
 **/
void libspdm_test_responder_key_update_case13(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0xD;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*last request: UpdateKey*/
    libspdm_copy_mem(spdm_context->last_update_request,
                     sizeof(spdm_context->last_update_request),
                     &m_libspdm_key_update_request1,
                     m_libspdm_key_update_request1_size);

    /*mocked major secret update*/
    libspdm_copy_mem(&secured_message_context->application_secret_backup.request_data_secret,
                     sizeof(secured_message_context->application_secret_backup.request_data_secret),
                     &secured_message_context->application_secret.request_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->hash_size);

    /*request side updated*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side *not* updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request6_size,
                                             &m_libspdm_key_update_request6,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 14: receiving a correct KEY_UPDATE message from the requester with
 * the VerifyNewKey operation. The responder is setup as if a valid
 * KEY_UPDATE request with the UpdateAllKeys has been previously received.
 * Expected behavior: the responder accepts the request, produces a valid
 * KEY_UPDATE_ACK response message, and both the request data key and the
 * response data key are updated.
 **/
void libspdm_test_responder_key_update_case14(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0xE;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*last request: UpdateallKeys*/
    libspdm_copy_mem(spdm_context->last_update_request,
                     sizeof(spdm_context->last_update_request),
                     &m_libspdm_key_update_request3,
                     m_libspdm_key_update_request3_size);

    /*mocked major secret update*/
    libspdm_copy_mem(&secured_message_context->application_secret_backup.request_data_secret,
                     sizeof(secured_message_context->application_secret_backup.request_data_secret),
                     &secured_message_context->application_secret.request_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_copy_mem(&secured_message_context->application_secret_backup.response_data_secret,
                     sizeof(secured_message_context->application_secret_backup.response_data_secret),
                     &secured_message_context->application_secret.response_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->hash_size);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .response_data_secret,
                                  secured_message_context->application_secret
                                  .response_data_secret,
                                  secured_message_context->hash_size);

    /*request side updated*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side updated*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_rsp_secret_buffer, m_rsp_secret_buffer,
                                  secured_message_context->hash_size);

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request5_size,
                                             &m_libspdm_key_update_request5,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_key_update_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_KEY_UPDATE_ACK);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_KEY_UPDATE_OPERATIONS_TABLE_VERIFY_NEW_KEY);
    assert_int_equal(spdm_response->header.param2,
                     m_libspdm_key_update_request5.header.param2);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 15: receiving a KEY_UPDATE message larger than specified, with the
 * VerifyNewKey operation. The responder is setup as if a valid KEY_UPDATE
 * request with the UpdateAllKeys has been previously received.
 * Expected behavior: the responder refuses the KEY_UPDATE message and
 * produces an ERROR message indicating the InvalidRequest. Neither the
 * request data key nor the response data key are rolled back to before
 * the UpdateAllKeys.
 **/
void libspdm_test_responder_key_update_case15(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0xF;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*last request: UpdateAllKeys*/
    libspdm_copy_mem(spdm_context->last_update_request,
                     sizeof(spdm_context->last_update_request),
                     &m_libspdm_key_update_request3,
                     m_libspdm_key_update_request3_size);

    /*mocked major secret update*/
    libspdm_copy_mem(&secured_message_context->application_secret_backup.request_data_secret,
                     sizeof(secured_message_context->application_secret_backup.request_data_secret),
                     &secured_message_context->application_secret.request_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_copy_mem(&secured_message_context->application_secret_backup.response_data_secret,
                     sizeof(secured_message_context->application_secret_backup.response_data_secret),
                     &secured_message_context->application_secret.response_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->hash_size);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .response_data_secret,
                                  secured_message_context->application_secret
                                  .response_data_secret,
                                  secured_message_context->hash_size);

    /*request side updated*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side updated*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_rsp_secret_buffer, m_rsp_secret_buffer,
                                  secured_message_context->hash_size);

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request6_size,
                                             &m_libspdm_key_update_request6,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 16: receiving a invalid KEY_UPDATE message from the requester with
 * the VerifyNewKey operation. The responder is setup as if no valid
 * KEY_UPDATE request with either the UpdateKey or UpdateAllKeys has been
 * previously received.
 * Expected behavior: the responder refuses the KEY_UPDATE message and
 * produces an ERROR message indicating the InvalidRequest. No keys are
 * updated.
 **/
void libspdm_test_responder_key_update_case16(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x10;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*no mocked major secret update*/

    /*no keys are updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request5_size,
                                             &m_libspdm_key_update_request5,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 17: receiving a correct retry of the KEY_UPDATE message from the
 * requester with the UpdateKey operation.
 * Expected behavior: the responder accepts the request and produces a valid
 * KEY_UPDATE_ACK response message. The request data key is kept updated, as
 * from the original request.
 **/
void libspdm_test_responder_key_update_case17(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x11;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*last request: UpdateKey*/
    libspdm_copy_mem(spdm_context->last_update_request,
                     sizeof(spdm_context->last_update_request),
                     &m_libspdm_key_update_request1,
                     m_libspdm_key_update_request1_size);

    /*mocked major secret update*/
    libspdm_copy_mem(&secured_message_context->application_secret_backup.request_data_secret,
                     sizeof(secured_message_context->application_secret_backup.request_data_secret),
                     &secured_message_context->application_secret.request_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->hash_size);

    /*request side updated (once)*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side *not* updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request1_size,
                                             &m_libspdm_key_update_request1,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_key_update_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_KEY_UPDATE_ACK);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_KEY_UPDATE_OPERATIONS_TABLE_UPDATE_KEY);
    assert_int_equal(spdm_response->header.param2,
                     m_libspdm_key_update_request1.header.param2);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 18: receiving a correct retry of the KEY_UPDATE message from the
 * requester with the UpdateAllKeys operation.
 * Expected behavior: the responder accepts the request and produces a valid
 * KEY_UPDATE_ACK response message. Both the request data key and the response
 * data key are kept updated, as from the original request.
 **/
void libspdm_test_responder_key_update_case18(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x12;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*last request: UpdateAllKeys*/
    libspdm_copy_mem(spdm_context->last_update_request,
                     sizeof(spdm_context->last_update_request),
                     &m_libspdm_key_update_request3,
                     m_libspdm_key_update_request3_size);

    /*mocked major secret update*/
    libspdm_copy_mem(&secured_message_context->application_secret_backup.request_data_secret,
                     sizeof(secured_message_context->application_secret_backup.request_data_secret),
                     &secured_message_context->application_secret.request_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_copy_mem(&secured_message_context->application_secret_backup.response_data_secret,
                     sizeof(secured_message_context->application_secret_backup.response_data_secret),
                     &secured_message_context->application_secret.response_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->hash_size);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .response_data_secret,
                                  secured_message_context->application_secret
                                  .response_data_secret,
                                  secured_message_context->hash_size);

    /*request side updated (once)*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side updated (once)*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_rsp_secret_buffer, m_rsp_secret_buffer,
                                  secured_message_context->hash_size);

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request3_size,
                                             &m_libspdm_key_update_request3,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_key_update_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_KEY_UPDATE_ACK);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_KEY_UPDATE_OPERATIONS_TABLE_UPDATE_ALL_KEYS);
    assert_int_equal(spdm_response->header.param2,
                     m_libspdm_key_update_request3.header.param2);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 19: receiving a correct retry of the KEY_UPDATE message from the
 * requester with the VerifyNewKey operation. The responder is setup as if
 * a valid KEY_UPDATE request with the UpdateKey has been previously
 * received.
 * Expected behavior: the responder accepts the request and produces a valid
 * KEY_UPDATE_ACK response message. The request data key is kept updated, as
 * from the original request.
 **/
void libspdm_test_responder_key_update_case19(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x13;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*last request: VerifyNewKey*/
    libspdm_copy_mem(spdm_context->last_update_request,
                     sizeof(spdm_context->last_update_request),
                     &m_libspdm_key_update_request5,
                     m_libspdm_key_update_request5_size);

    /*no keys updated (already activated)*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request5_size,
                                             &m_libspdm_key_update_request5,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_key_update_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_KEY_UPDATE_ACK);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_KEY_UPDATE_OPERATIONS_TABLE_VERIFY_NEW_KEY);
    assert_int_equal(spdm_response->header.param2,
                     m_libspdm_key_update_request5.header.param2);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 20: receiving a correct retry of the KEY_UPDATE message from the
 * requester with the VerifyNewKey operation. The responder is setup as if
 * a valid KEY_UPDATE request with the UpdateAllKeys has been previously
 * received.
 * Expected behavior: the responder accepts the request and produces a valid
 * KEY_UPDATE_ACK response message. Both the request data key and the
 * response data key are kept updated, as from the original request.
 **/
void libspdm_test_responder_key_update_case20(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x14;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*last request: UpdateAllKeys*/
    libspdm_copy_mem(spdm_context->last_update_request,
                     sizeof(spdm_context->last_update_request),
                     &m_libspdm_key_update_request5,
                     m_libspdm_key_update_request5_size);

    /*mocked major secret update*/
    libspdm_copy_mem(&secured_message_context->application_secret_backup.request_data_secret,
                     sizeof(secured_message_context->application_secret_backup.request_data_secret),
                     &secured_message_context->application_secret.request_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_copy_mem(&secured_message_context->application_secret_backup.response_data_secret,
                     sizeof(secured_message_context->application_secret_backup.request_data_secret),
                     &secured_message_context->application_secret.response_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->hash_size);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .response_data_secret,
                                  secured_message_context->application_secret
                                  .response_data_secret,
                                  secured_message_context->hash_size);

    /*request side updated (once)*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side updated (once)*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_rsp_secret_buffer, m_rsp_secret_buffer,
                                  secured_message_context->hash_size);

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request5_size,
                                             &m_libspdm_key_update_request5,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_key_update_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_KEY_UPDATE_ACK);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_KEY_UPDATE_OPERATIONS_TABLE_VERIFY_NEW_KEY);
    assert_int_equal(spdm_response->header.param2,
                     m_libspdm_key_update_request5.header.param2);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 21: receiving an incorrect retry of the KEY_UPDATE message from the
 * requester with the UpdateAllKeys operation, when the retry of an UpdateKey
 * was expected. The responder is setup as if a valid KEY_UPDATE request
 * with the UpdateKey has been previously received.
 * Expected behavior: the responder refuses the KEY_UPDATE message and
 * produces an ERROR message indicating the InvalidRequest. Only the request
 * data key is kept updated, as from the original request.
 **/
void libspdm_test_responder_key_update_case21(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x15;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*last request: UpdateKey*/
    libspdm_copy_mem(spdm_context->last_update_request,
                     sizeof(spdm_context->last_update_request),
                     &m_libspdm_key_update_request1,
                     m_libspdm_key_update_request1_size);

    /*mocked major secret update*/
    libspdm_copy_mem(&secured_message_context->application_secret_backup.request_data_secret,
                     sizeof(secured_message_context->application_secret_backup.request_data_secret),
                     &secured_message_context->application_secret.request_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->hash_size);

    /*request side updated (once)*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side *not* updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request3_size,
                                             &m_libspdm_key_update_request3,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 22: receiving an incorrect retry of the KEY_UPDATE message from the
 * requester with the UpdateKey operation, when the retry of an UpdateAllKeys
 * was expected. The responder is setup as if a valid KEY_UPDATE request
 * with the UpdateAllKeys has been previously received.
 * Expected behavior: the responder refuses the KEY_UPDATE message and
 * produces an ERROR message indicating the InvalidRequest. Both the request
 * data key and the response data key are kept updated, as from the original
 * request.
 **/
void libspdm_test_responder_key_update_case22(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x16;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*last request: UpdateAllKeys*/
    libspdm_copy_mem(spdm_context->last_update_request,
                     sizeof(spdm_context->last_update_request),
                     &m_libspdm_key_update_request3,
                     m_libspdm_key_update_request3_size);

    /*mocked major secret update*/
    libspdm_copy_mem(&secured_message_context->application_secret_backup.request_data_secret,
                     sizeof(secured_message_context->application_secret_backup.request_data_secret),
                     &secured_message_context->application_secret.request_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_copy_mem(&secured_message_context->application_secret_backup.response_data_secret,
                     sizeof(secured_message_context->application_secret_backup.response_data_secret),
                     &secured_message_context->application_secret.response_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->hash_size);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .response_data_secret,
                                  secured_message_context->application_secret
                                  .response_data_secret,
                                  secured_message_context->hash_size);

    /*request side updated (once)*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side updated*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_rsp_secret_buffer, m_rsp_secret_buffer,
                                  secured_message_context->hash_size);

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request1_size,
                                             &m_libspdm_key_update_request1,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 23: receiving an incorrect retry of the KEY_UPDATE message from the
 * requester with the UpdateKey operation, but with a different token.
 * Expected behavior: the responder refuses the KEY_UPDATE message and
 * produces an ERROR message indicating the InvalidRequest. The request
 * data key is kept updated, as from the original request.
 **/
void libspdm_test_responder_key_update_case23(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x17;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*last request: UpdateKey*/
    libspdm_copy_mem(spdm_context->last_update_request,
                     sizeof(spdm_context->last_update_request),
                     &m_libspdm_key_update_request1,
                     m_libspdm_key_update_request1_size);

    /*mocked major secret update*/
    libspdm_copy_mem(&secured_message_context->application_secret_backup.request_data_secret,
                     sizeof(secured_message_context->application_secret_backup.request_data_secret),
                     &secured_message_context->application_secret.request_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->hash_size);

    /*request side updated (once)*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side *not* updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request7_size,
                                             &m_libspdm_key_update_request7,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 24: receiving an incorrect retry of the KEY_UPDATE message from the
 * requester with the UpdateAllKeys operation, but with a different token.
 * Expected behavior: the responder refuses the KEY_UPDATE message and
 * produces an ERROR message indicating the InvalidRequest. Both the request
 * data key and the response data key are kept updated, as from the original
 * request.
 **/
void libspdm_test_responder_key_update_case24(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x18;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*last request: UpdateAllKeys*/
    libspdm_copy_mem(spdm_context->last_update_request,
                     sizeof(spdm_context->last_update_request),
                     &m_libspdm_key_update_request3,
                     m_libspdm_key_update_request3_size);

    /*mocked major secret update*/
    libspdm_copy_mem(&secured_message_context->application_secret_backup.request_data_secret,
                     sizeof(secured_message_context->application_secret_backup.request_data_secret),
                     &secured_message_context->application_secret.request_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_copy_mem(&secured_message_context->application_secret_backup.response_data_secret,
                     sizeof(secured_message_context->application_secret_backup.response_data_secret),
                     &secured_message_context->application_secret.response_data_secret,
                     LIBSPDM_MAX_HASH_SIZE);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->application_secret
                                  .request_data_secret,
                                  secured_message_context->hash_size);
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  secured_message_context->application_secret
                                  .response_data_secret,
                                  secured_message_context->application_secret
                                  .response_data_secret,
                                  secured_message_context->hash_size);

    /*request side updated (once)*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_req_secret_buffer, m_req_secret_buffer,
                                  secured_message_context->hash_size);
    /*response side updated (once)*/
    libspdm_compute_secret_update(secured_message_context->hash_size,
                                  m_rsp_secret_buffer, m_rsp_secret_buffer,
                                  secured_message_context->hash_size);

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request8_size,
                                             &m_libspdm_key_update_request8,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 25: receiving an incorrect retry of the KEY_UPDATE message from the
 * requester with the VerifyNewKey operation, but with a different token.
 * Expected behavior: the responder refuses the KEY_UPDATE message and
 * produces an ERROR message indicating the InvalidRequest. No keys are
 * updated, as they were previously activated by the last VerifyNewKey.
 **/
void libspdm_test_responder_key_update_case25(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x19;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*last request: VerifyNewKey*/
    libspdm_copy_mem(spdm_context->last_update_request,
                     sizeof(spdm_context->last_update_request),
                     &m_libspdm_key_update_request5,
                     m_libspdm_key_update_request5_size);

    /*no keys updated (already activated)*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request9_size,
                                             &m_libspdm_key_update_request9,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

/**
 * Test 26: receiving a KEY_UPDATE message with a reserved operation code.
 * Expected behavior: the responder refuses the KEY_UPDATE message and
 * produces an ERROR message indicating the InvalidRequest. No keys
 * are updated.
 **/
void libspdm_test_responder_key_update_case26(void **state)
{
    return_status status;
    libspdm_test_context_t            *spdm_test_context;
    libspdm_context_t                 *spdm_context;
    uint32_t session_id;
    libspdm_session_info_t            *session_info;
    libspdm_secured_message_context_t *secured_message_context;

    uintn response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_key_update_response_t *spdm_response;

    uint8_t m_req_secret_buffer[LIBSPDM_MAX_HASH_SIZE];
    uint8_t m_rsp_secret_buffer[LIBSPDM_MAX_HASH_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x1A;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_set_standard_key_update_test_state(
        spdm_context, &session_id);

    session_info = &spdm_context->session_info[0];
    secured_message_context = session_info->secured_message_context;

    libspdm_set_standard_key_update_test_secrets(
        session_info->secured_message_context,
        m_rsp_secret_buffer, (uint8_t)(0xFF),
        m_req_secret_buffer, (uint8_t)(0xEE));

    /*no keys are updated*/

    response_size = sizeof(response);
    status = libspdm_get_response_key_update(spdm_context,
                                             m_libspdm_key_update_request10_size,
                                             &m_libspdm_key_update_request10,
                                             &response_size, response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_memory_equal(secured_message_context
                        ->application_secret.request_data_secret,
                        m_req_secret_buffer, secured_message_context->hash_size);
    assert_memory_equal(secured_message_context
                        ->application_secret.response_data_secret,
                        m_rsp_secret_buffer, secured_message_context->hash_size);
}

libspdm_test_context_t m_libspdm_responder_key_update_test_context = {
    LIBSPDM_TEST_CONTEXT_SIGNATURE,
    false,
};

int libspdm_responder_key_update_test_main(void)
{
    const struct CMUnitTest spdm_responder_key_update_tests[] = {
        /* Success Case -- UpdateKey*/
        cmocka_unit_test(libspdm_test_responder_key_update_case1),
        /* Bad request size*/
        cmocka_unit_test(libspdm_test_responder_key_update_case2),
        /* response_state: SPDM_RESPONSE_STATE_BUSY*/
        cmocka_unit_test(libspdm_test_responder_key_update_case3),
        /* response_state: SPDM_RESPONSE_STATE_NEED_RESYNC*/
        cmocka_unit_test(libspdm_test_responder_key_update_case4),
        /* response_state: SPDM_RESPONSE_STATE_NOT_READY*/
        cmocka_unit_test(libspdm_test_responder_key_update_case5),
        /* connection_state Check*/
        cmocka_unit_test(libspdm_test_responder_key_update_case6),
        /* Buffer reset*/
        cmocka_unit_test(libspdm_test_responder_key_update_case7),
        /* Unsupported KEY_UPD capabilities*/
        cmocka_unit_test(libspdm_test_responder_key_update_case8),
        /* Uninitialized session*/
        cmocka_unit_test(libspdm_test_responder_key_update_case9),
        /* Success Case -- UpdateAllKeys*/
        cmocka_unit_test(libspdm_test_responder_key_update_case10),
        /* Bad request size*/
        cmocka_unit_test(libspdm_test_responder_key_update_case11),
        /* Success Case -- VerifyNewKey (from UpdateKey)*/
        cmocka_unit_test(libspdm_test_responder_key_update_case12),
        /* Bad request size*/
        cmocka_unit_test(libspdm_test_responder_key_update_case13),
        /* Success Case -- VerifyNewKey (from UpdateAllKeys)*/
        cmocka_unit_test(libspdm_test_responder_key_update_case14),
        /* Bad request size*/
        cmocka_unit_test(libspdm_test_responder_key_update_case15),
        /* Uninitialized key update*/
        cmocka_unit_test(libspdm_test_responder_key_update_case16),
        /* Retry -- UpdateKey*/
        cmocka_unit_test(libspdm_test_responder_key_update_case17),
        /* Retry -- UpdateAllKeys*/
        cmocka_unit_test(libspdm_test_responder_key_update_case18),
        /* Retry -- VerifyNewKey (from UpdateKey)*/
        cmocka_unit_test(libspdm_test_responder_key_update_case19),
        /* Retry -- VerifyNewKey (from UpdateAllKeys)*/
        cmocka_unit_test(libspdm_test_responder_key_update_case20),
        /* VerifyNewKey expected*/
        cmocka_unit_test(libspdm_test_responder_key_update_case21),
        cmocka_unit_test(libspdm_test_responder_key_update_case22),
        /* Retry -- wrong token*/
        cmocka_unit_test(libspdm_test_responder_key_update_case23),
        cmocka_unit_test(libspdm_test_responder_key_update_case24),
        cmocka_unit_test(libspdm_test_responder_key_update_case25),
        /* Invalid operation*/
        cmocka_unit_test(libspdm_test_responder_key_update_case26),
    };

    libspdm_setup_test_context(&m_libspdm_responder_key_update_test_context);

    return cmocka_run_group_tests(spdm_responder_key_update_tests,
                                  libspdm_unit_test_group_setup,
                                  libspdm_unit_test_group_teardown);
}
