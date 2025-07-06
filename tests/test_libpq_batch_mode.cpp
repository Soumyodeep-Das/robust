#include "test_libpq_batch_mode.hpp"

int PQisInBatchMode(PGconn *conn)
{
    return PQisInBatchMode(conn);
}

int PQbatchIsAborted(PGconn *conn)
{
    return PQbatchIsAborted(conn);
}

int PQqueriesInBatch(PGconn *conn)
{
    return PQqueriesInBatch(conn);
}

int PQbeginBatchMode(PGconn *conn)
{
    return PQbeginBatchMode(conn);
}

int PQendBatchMode(PGconn *conn)
{
    return PQendBatchMode(conn);
}

int PQsendEndBatch(PGconn *conn)
{
    return PQsendEndBatch(conn);
}

char *PQgetNextQuery(PGconn *conn)
{
    return PQgetNextQuery(conn);
}