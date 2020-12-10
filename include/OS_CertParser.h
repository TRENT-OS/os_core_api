/* Copyright (C) 2020, Hensoldt Cyber GmbH */

/**
 * @file
 * @addtogroup OS_CertParser OS CertParser API
 * @{
 * @brief OS CertParser API library
 */

#pragma once

#include "OS_Crypto.h"

/**
 * Supported certificate encodings
 */
typedef enum
{
    OS_CertParserCert_Encoding_NONE = 0x00,

    /**
     * DER encoded certificate (binary)
     */
    OS_CertParserCert_Encoding_DER,

    /**
     * PEM encoded certificate (base-64 encoded DER)
     */
    OS_CertParserCert_Encoding_PEM
} OS_CertParserCert_Encoding_t;

/**
 * Flags indicating reasons for verification failures
 */
typedef enum
{
    OS_CertParser_VerifyFlags_NONE           = 0x00,

    /**
     * The key involved in verifying a signature is too small (e.g., for RSA
     * less than 2048 bits)
     */
    OS_CertParser_VerifyFlags_INVALID_KEY    = (1u << 0),

    /**
     * The signature is invalid
     */
    OS_CertParser_VerifyFlags_INVALID_SIG    = (1u << 1),

    /**
     * There is a mismatch in the common names of the certificates in a chain
     */
    OS_CertParser_VerifyFlags_CN_MISMATCH    = (1u << 2),

    /**
     * The way a certificate's extension fields are used is incorrect
     */
    OS_CertParser_VerifyFlags_EXT_MISMATCH   = (1u << 3),

    /**
     * Any other error
     */
    OS_CertParser_VerifyFlags_OTHER_ERROR    = (1u << 4),
} OS_CertParser_VerifyFlags_t;

/**
 * Attribute types which can be read from a x509 certificate
 */
typedef enum
{
    OS_CertParserCert_AttribType_NONE = 0x00,

    /**
     * Get the public key of the cert
     */
    OS_CertParserCert_AttribType_PUBLICKEY,

    /**
     * Get the subject field of the cert
     */
    OS_CertParserCert_AttribType_SUBJECT,

    /**
     * Get the issuer field of the cert
     */
    OS_CertParserCert_AttribType_ISSUER
} OS_CertParserCert_AttribType_t;

/**
 *  Maximum length of subject in X509
 */
#define OS_CertParserCert_Subject_MAX_LEN 256

/**
 *  Maximum length of issuer in X509
 */
#define OS_CertParserCert_Issuer_MAX_LEN 256

/**
 * x509 cert attribute data
 */
typedef struct
{
    /**
     * Type of certificate attribute
     */
    OS_CertParserCert_AttribType_t type;

    /**
     * Use respective field based on type
     */
    union
    {
        /**
         * Certificate's public key
         */
        OS_CryptoKey_Data_t publicKey;

        /**
         * Subject field of x509 cert
         */
        char subject[OS_CertParserCert_Subject_MAX_LEN];

        /**
         * Issuer field of x509 cert
         */
        char issuer[OS_CertParserCert_Issuer_MAX_LEN];
    } data;
} OS_CertParserCert_Attrib_t;

/**
 * Configuration of certificate parser
 */
typedef struct
{
    /**
     * Handle to an initialized Crypto API instance
     */
    OS_Crypto_Handle_t hCrypto;
} OS_CertParser_Config_t;

///@cond INTERNAL --------------------------------------------------------------
typedef struct OS_CertParserCert OS_CertParserCert_t;
typedef struct OS_CertParserChain OS_CertParserChain_t;
typedef struct OS_CertParser OS_CertParser_t;
typedef OS_CertParser_t* OS_CertParser_Handle_t;
typedef OS_CertParserCert_t* OS_CertParserCert_Handle_t;
typedef OS_CertParserChain_t* OS_CertParserChain_Handle_t;
///@endcond --------------------------------------------------------------------

/**
 * @brief Initialize parser handle
 *
 * @param hParser pointer to handle of OS CertParser API
 * @param config configuration of parser
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if the internal state could not be initialized
 * @retval OS_ERROR_NOT_SUPPORTED if \p config is not supported
 * @retval OS_ERROR_INSUFFICIENT_SPACE if allocation of \p hParser failed
 */
OS_Error_t
OS_CertParser_init(
    OS_CertParser_Handle_t*       hParser,
    const OS_CertParser_Config_t* config);

/**
 * @brief Free parser handle
 *
 * NOTE: This function can also free all associated chains and certs, just
 *       be careful that you are not having other references which might
 *       still be in use!
 *
 * @param hParser handle of OS CertParser API
 * @param freeChains set to true if all CA chains and the certificates associated
 *  with them shall be freed as well
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_CertParser_free(
    OS_CertParser_Handle_t hParser,
    const bool             freeChains);

/**
 * @brief Add trusted CA chain to parser
 *
 * Add reference to a chain to the parser; the parser can hold references
 * to many chains, which can then be addressed with their respective index
 * during certificate verification.
 *
 * NOTE: Just the reference to a chain is added; the chain (and its respective
 *       certs) SHOULD NOT be free'd while it is associated to the parser.
 *
 * @param hParser handle of OS CertParser API
 * @param hChain chain containing at least one CA certificate
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_INSUFFICIENT_SPACE if enlarging internal buffer of
 *  \p hParser failed
 */
OS_Error_t
OS_CertParser_addTrustedChain(
    OS_CertParser_Handle_t            hParser,
    const OS_CertParserChain_Handle_t hChain);

/**
 * @brief Verify a certificate chain with a trusted CA chain
 *
 * This function takes a chain and verifies it against one of the trusted
 * CA chains added to the parser. Which chain to use is indicated by the
 * \p index parameter. If verification succeeds, this function returns no
 * error and \p result is set to 0. If there is a specific verification
 * error, this function returns OS_ERROR_GENERIC and \p result will
 * have the respective error flags set.
 *
 * @param hParser handle of OS CertParser API
 * @param index index of CA chain to use
 * @param hChain chain to verify against CA chain
 * @param result flags indicating verification result
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if the underlying x509 parser returned an error
 * @retval OS_ERROR_NOT_FOUND if \p index is out of range
 * @retval OS_ERROR_GENERIC if \p hChain could not be verified
 */OS_Error_t
OS_CertParser_verifyChain(
    const OS_CertParser_Handle_t      hParser,
    const size_t                      index,
    const OS_CertParserChain_Handle_t hChain,
    OS_CertParser_VerifyFlags_t*      result);

/**
 * @brief Initialize a cert handle
 *
 * Create a cert handle by parsing a blob of cert data in different encodings
 * into its internal structure. This function will make sure the certificate
 * algorithms are supported.
 *
 * @param hCert pointer to cert handle to be initialized
 * @param hParser handle of OS CertParser API
 * @param encoding encoding type of cert
 * @param data raw cert data
 * @param len length of cert data in bytes
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if the underlying x509 parser returned an error
 * @retval OS_ERROR_NOT_SUPPORTED if hash or pk algo of certificate are not
 *  supported by the parser
 * @retval OS_ERROR_INSUFFICIENT_SPACE if allocation of \p hCert failed
 */
OS_Error_t
OS_CertParserCert_init(
    OS_CertParserCert_Handle_t*        hCert,
    const OS_CertParser_Handle_t       hParser,
    const OS_CertParserCert_Encoding_t encoding,
    const uint8_t*                     data,
    const size_t                       len);

/**
 * @brief Free a cert handle
 *
 * @param hCert certificate handle to free
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_CertParserCert_free(
    OS_CertParserCert_Handle_t hCert);

/**
 * @brief Extract an attribute from a cert
 *
 * x509 certificates have many different fields such as ISSUER, SUBJECT,
 * etc. This function allows to extract some of those fields into a usable
 * form.
 *
 * @param hCert certificate handle
 * @param type type of attribute to extract
 * @param attrib buffer to attribute data
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if the underlying x509 parser returned an error
 */
OS_Error_t
OS_CertParserCert_getAttrib(
    const OS_CertParserCert_Handle_t     hCert,
    const OS_CertParserCert_AttribType_t type,
    OS_CertParserCert_Attrib_t*          attrib);

/**
 * @brief Initialize a certificate chain handle
 *
 * @param hChain chain handle to initialize
 * @param hParser handle of OS CertParser API
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_INSUFFICIENT_SPACE if allocation of \p hChain failed
 */
OS_Error_t
OS_CertParserChain_init(
    OS_CertParserChain_Handle_t* hChain,
    const OS_CertParser_Handle_t hParser);

/**
 * @brief Free certificate chain handle
 *
 * NOTE: This function can free all associated certs; just make sure there are
 *       no other references to those certs in use when they are freed.
 *
 * @param hChain chain handle to free
 * @param freeCerts set to true if all associated certs should be free'd
 *  as well
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_CertParserChain_free(
    OS_CertParserChain_Handle_t hChain,
    const bool                  freeCerts);

/**
 * @brief Add certificate to certificate chain
 *
 * Add a certificate to a certificate chain. To access the certificate later,
 * its position in the chain can be used as index. This function will also
 * check if \p hCert actually belongs to \p hChain by ensuring that the issuer
 * of \p hCert matches the last certificate in \p hChain.
 *
 * NOTE: Just the reference to \p hCert is added; the certificate SHOULD NOT
 *       be free'd  while it is associated with a chain that is in use (or
 *       associated with a parser handle).
 *
 * @param hChain chain handle to add cert to
 * @param hCert certificate to add to the chain
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if subject of the last element in the chain and the #
 *  subject of the new cert do not match (i.e., cert is not part of the chain)
 * @retval OS_ERROR_INSUFFICIENT_SPACE if enlarging internal buffers of
 *  \p hChain failed
 */
OS_Error_t
OS_CertParserChain_addCert(
    OS_CertParserChain_Handle_t      hChain,
    const OS_CertParserCert_Handle_t hCert);

/**
 * @brief Get reference to certificate in chain
 *
 * Get pointer to a certificate in \p hChain. The certificate is selected by
 * giving its \p index, i.e., its position in \p hChain.
 *
 * @param hChain chain handle
 * @param index index of certificate in chain
 * @param hCert pointer to cert handle
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_NOT_FOUND if \p index is out of range
 */
OS_Error_t
OS_CertParserChain_getCert(
    const OS_CertParserChain_Handle_t hChain,
    const size_t                      index,
    OS_CertParserCert_Handle_t*       hCert);

/**
 * @brief Get number of certs in chain
 *
 * @param hChain chain handle
 * @param len pointer to length
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_CertParserChain_getLength(
    const OS_CertParserChain_Handle_t hChain,
    size_t*                           len);

/** @} */
