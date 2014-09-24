// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2013 The NovaCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef NOVACOIN_MINER_H
#define NOVACOIN_MINER_H

#include "main.h"
#include "wallet.h"



#if !HAVE_DECL_BE32DEC
static inline uint32_t be32dec(const void *pp)
{
    const uint8_t *p = (uint8_t const *)pp;
    return ((uint32_t)(p[3]) + ((uint32_t)(p[2]) << 8) +
        ((uint32_t)(p[1]) << 16) + ((uint32_t)(p[0]) << 24));
}
#endif

#if !HAVE_DECL_LE32DEC
static inline uint32_t le32dec(const void *pp)
{
    const uint8_t *p = (uint8_t const *)pp;
    return ((uint32_t)(p[0]) + ((uint32_t)(p[1]) << 8) +
        ((uint32_t)(p[2]) << 16) + ((uint32_t)(p[3]) << 24));
}
#endif

#if !HAVE_DECL_BE32ENC
static inline void be32enc(void *pp, uint32_t x)
{
    uint8_t *p = (uint8_t *)pp;
    p[3] = x & 0xff;
    p[2] = (x >> 8) & 0xff;
    p[1] = (x >> 16) & 0xff;
    p[0] = (x >> 24) & 0xff;
}
#endif

#if !HAVE_DECL_LE32ENC
static inline void le32enc(void *pp, uint32_t x)
{
    uint8_t *p = (uint8_t *)pp;
    p[0] = x & 0xff;
    p[1] = (x >> 8) & 0xff;
    p[2] = (x >> 16) & 0xff;
    p[3] = (x >> 24) & 0xff;
}
#endif

/* Generate a new block, without valid proof-of-work */
CBlock* CreateNewBlock(CWallet* pwallet, bool fProofOfStake=false, int64_t* pFees = 0);

/** Modify the extranonce in a block */
void IncrementExtraNonce(CBlock* pblock, CBlockIndex* pindexPrev, unsigned int& nExtraNonce);

/** Do mining precalculation */
void FormatHashBuffers(CBlock* pblock, char* pmidstate, char* pdata, char* phash1);

/** Check mined proof-of-work block */
bool CheckWork(CBlock* pblock, CWallet& wallet, CReserveKey& reservekey);

/** Check mined proof-of-stake block */
bool CheckStake(CBlock* pblock, CWallet& wallet);

/** Base sha256 mining transform */
void SHA256Transform(void* pstate, void* pinput, const void* pinit);


void sha256_init(uint32_t *state);
void sha256_transform(uint32_t *state, const uint32_t *block, int swap);
void sha256d(unsigned char *hash, const unsigned char *data, int len);

#if defined(__ARM_NEON__) || defined(__i386__) || defined(__x86_64__)
#define HAVE_SHA256_4WAY 1
int sha256_use_4way();
void sha256_init_4way(uint32_t *state);
void sha256_transform_4way(uint32_t *state, const uint32_t *block, int swap);
#endif

#if defined(__x86_64__) && defined(USE_AVX2)
#define HAVE_SHA256_8WAY 1
int sha256_use_8way();
void sha256_init_8way(uint32_t *state);
void sha256_transform_8way(uint32_t *state, const uint32_t *block, int swap);
#endif

extern int scanhash_sha256d(int thr_id, uint32_t *pdata,
    const uint32_t *ptarget, uint32_t max_nonce, unsigned long *hashes_done);

extern unsigned char *scrypt_buffer_alloc();
extern int scanhash_scrypt(int thr_id, uint32_t *pdata,
    unsigned char *scratchbuf, const uint32_t *ptarget,
    uint32_t max_nonce, unsigned long *hashes_done);

extern int scanhash_bitblock(int thr_id, uint32_t *pdata, const uint32_t *ptarget,
    uint32_t max_nonce, unsigned long *hashes_done);

struct thr_info {
    int		id;
    pthread_t	pth;
    struct thread_q	*q;
};

struct work_restart {
    volatile unsigned long	restart;
    char			padding[128 - sizeof(unsigned long)];
};

extern bool opt_debug;
extern bool opt_protocol;
extern int opt_timeout;
extern bool want_longpoll;
extern bool have_longpoll;
extern bool want_stratum;
extern bool have_stratum;
extern char *opt_cert;
extern char *opt_proxy;
extern long opt_proxy_type;
extern bool use_syslog;
extern pthread_mutex_t applog_lock;
extern struct thr_info *thr_info;
extern int longpoll_thr_id;
extern int stratum_thr_id;
extern struct work_restart *work_restart;

extern void applog(int prio, const char *fmt, ...);
//extern json_t *json_rpc_call(CURL *curl, const char *url, const char *userpass,
  //  const char *rpc_req, bool, bool, int *);
extern char *bin2hex(const unsigned char *p, size_t len);
extern bool hex2bin(unsigned char *p, const char *hexstr, size_t len);
extern int timeval_subtract(struct timeval *result, struct timeval *x,
    struct timeval *y);
extern bool fulltest(const uint32_t *hash, const uint32_t *target);
extern void diff_to_target(uint32_t *target, double diff);

struct stratum_job {
    char *job_id;
    unsigned char prevhash[32];
    size_t coinbase_size;
    unsigned char *coinbase;
    unsigned char *xnonce2;
    int merkle_count;
    unsigned char **merkle;
    unsigned char version[4];
    unsigned char nbits[4];
    unsigned char ntime[4];
    bool clean;
    double diff;
};



struct thread_q;

extern struct thread_q *tq_new(void);
extern void tq_free(struct thread_q *tq);
extern bool tq_push(struct thread_q *tq, void *data);
extern void *tq_pop(struct thread_q *tq, const struct timespec *abstime);
extern void tq_freeze(struct thread_q *tq);
extern void tq_thaw(struct thread_q *tq);


extern int scanhash_scrypt(int thr_id, uint32_t *pdata,
    unsigned char *scratchbuf, const uint32_t *ptarget,
    uint32_t max_nonce, unsigned long *hashes_done);

void GenerateBitcoins(bool fGenerate, CWallet* pwallet);
void BitcoinMiner(CWallet *pwallet, bool fProofOfStake);
#endif // NOVACOIN_MINER_H
