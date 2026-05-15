#ifndef HEADER_QURL_VAUTH_H
#define HEADER_QURL_VAUTH_H

/* This is used to generate a base64 encoded PLAIN cleartext message */
qurl_ecode_t qurl_auth_create_plain_message(const char *authzid, const char *authcid, const char *passwd, char **outptr, int *outlen);

/* This is used to generate a base64 encoded LOGIN cleartext message */
qurl_ecode_t qurl_auth_create_login_message(const char *valuep, char **outptr, int *outlen);

/* This is used to generate a base64 encoded EXTERNAL cleartext message */
qurl_ecode_t qurl_auth_create_external_message(const char *user, char **outptr, int *outlen);

/* This is used to generate a base64 encoded OAuth 2.0 message */
qurl_ecode_t qurl_auth_create_oauth_bearer_message(const char *user, const char *host, const long port, const char *bearer, char **outptr, int *outlen);

/* This is used to generate a base64 encoded XOAuth 2.0 message */
qurl_ecode_t qurl_auth_create_xoauth_bearer_message(const char *user, const char *bearer, char **outptr, int *outlen);

#endif /* HEADER_qurl_VAUTH_H */
