#ifndef TEST_LIBPQ_BATCH_MODE_HPP
#define TEST_LIBPQ_BATCH_MODE_HPP

#include <libpq-fe.h>

#ifdef __cplusplus
extern "C" {
#endif

int PQisInBatchMode(PGconn *conn);
int PQbatchIsAborted(PGconn *conn);
int PQqueriesInBatch(PGconn *conn);
int PQbeginBatchMode(PGconn *conn);
int PQendBatchMode(PGconn *conn);
int PQsendEndBatch(PGconn *conn);
char *PQgetNextQuery(PGconn *conn);

#ifdef __cplusplus
}
#endif

#endif // TEST_LIBPQ_BATCH_MODE_HPP