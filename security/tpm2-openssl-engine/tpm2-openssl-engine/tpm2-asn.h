/*
 * Copyright (c) 2017 Wind River Systems, Inc.
*
* SPDX-License-Identifier: Apache-2.0
*
 */
/* ====================================================================
 * Copyright (c) 1999-2001 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    licensing@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 * This product is inspired by the original TPM 1.2 openssl engine written
 * by Kent Yoder <kyoder@users.sf.net> for the Trousers Project. This product
 * includes TPM key blob ASN-1 encoding scheme from James Bottomley
 * <james.bottomley@HansenPartnership.com>
 *
 */

#ifndef _TPM2_ASN_H
#define _TPM2_ASN_H

#include <openssl/asn1t.h>
#include <openssl/pem.h>

/*
 * Define the format of a TPM key file.  The current format covers
 * both TPM1.2 keys as well as symmetrically encrypted private keys
 * produced by TSS2_Import and the TPM2 format public key which
 * contains things like the policy but which is cryptographically tied
 * to the private key.
 *
 * TPMKey ::= SEQUENCE {
 * type        OBJECT IDENTIFIER
 * emptyAuth   [0] EXPLICIT BOOLEAN OPTIONAL
 * parent      [1] EXPLICIT INTEGER OPTIONAL
 * pubkey      [2] EXPLICIT OCTET STRING OPTIONAL
 * privkey     OCTET STRING
 * }
 */

typedef struct {
   ASN1_OBJECT *type;
   ASN1_BOOLEAN emptyAuth;
   ASN1_INTEGER *parent;
   ASN1_OCTET_STRING *pubkey;
   ASN1_OCTET_STRING *privkey;
} TSSLOADABLE;

/* the two type oids are in the TCG namespace 2.23.133; we choose an
 *  unoccupied child (10) for keytype file and two values:
 *    1 : Key that is directly loadable
 *    2 : Key that must first be imported then loaded
 */
#define OID_12Key      "2.23.133.10.1"
#define OID_loadableKey        "2.23.133.10.2"
#define OID_importableKey  "2.23.133.10.3"

ASN1_SEQUENCE(TSSLOADABLE) = {
   ASN1_SIMPLE(TSSLOADABLE, type, ASN1_OBJECT),
   ASN1_EXP_OPT(TSSLOADABLE, emptyAuth, ASN1_BOOLEAN, 0),
   ASN1_EXP_OPT(TSSLOADABLE, parent, ASN1_INTEGER, 1),
   ASN1_EXP_OPT(TSSLOADABLE, pubkey, ASN1_OCTET_STRING, 2),
   ASN1_SIMPLE(TSSLOADABLE, privkey, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END(TSSLOADABLE)

IMPLEMENT_ASN1_FUNCTIONS(TSSLOADABLE)
//DECLARE_ASN1_FUNCTIONS(TSSLOADABLE)

/* This is the PEM guard tag */
#define TSSLOADABLE_PEM_STRING "TSS2 KEY BLOB"

static IMPLEMENT_PEM_write_bio(TSSLOADABLE, TSSLOADABLE, TSSLOADABLE_PEM_STRING, TSSLOADABLE)
static IMPLEMENT_PEM_read_bio(TSSLOADABLE, TSSLOADABLE, TSSLOADABLE_PEM_STRING, TSSLOADABLE)

#endif
