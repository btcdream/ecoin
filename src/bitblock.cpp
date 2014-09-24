//#include "cpuminer-config.h"
#include "miner.h"

#include <string.h>
#include <stdint.h>

#include "sph_blake.h"
#include "sph_bmw.h"
#include "sph_groestl.h"
#include "sph_jh.h"
#include "sph_keccak.h"
#include "sph_skein.h"
#include "sph_luffa.h"
#include "sph_cubehash.h"
#include "sph_shavite.h"
#include "sph_simd.h"
#include "sph_echo.h"
#include "sph_hamsi.h"
#include "sph_fugue.h"
#include "sph_shabal.h"
#include "sph_whirlpool.h"

inline void bitblockhash(void *output, const void *input)
{
    unsigned char hash[128];

    memset(hash, 0, 128);
  
    sph_blake512_context     ctx_blake;
    sph_bmw512_context       ctx_bmw;
    sph_groestl512_context   ctx_groestl;
    sph_jh512_context        ctx_jh;
    sph_keccak512_context    ctx_keccak;
    sph_skein512_context     ctx_skein;
    sph_luffa512_context     ctx_luffa;
    sph_cubehash512_context  ctx_cubehash;
    sph_shavite512_context   ctx_shavite;
    sph_simd512_context      ctx_simd;
    sph_echo512_context      ctx_echo;
    sph_hamsi512_context     ctx_hamsi;
    sph_fugue512_context     ctx_fugue;
    sph_shabal512_context    ctx_shabal;
    sph_whirlpool_context    ctx_whirlpool;

    sph_blake512_init(&ctx_blake);
    sph_blake512(&ctx_blake, input, 80);
    sph_blake512_close(&ctx_blake, hash);

    sph_bmw512_init(&ctx_bmw);
    sph_bmw512(&ctx_bmw, hash, 64);
    sph_bmw512_close(&ctx_bmw, hash + 64);

    sph_groestl512_init(&ctx_groestl);
    sph_groestl512 (&ctx_groestl, hash + 64, 64);
    sph_groestl512_close(&ctx_groestl, hash);

    sph_skein512_init(&ctx_skein);
    sph_skein512(&ctx_skein, hash, 64);
    sph_skein512_close(&ctx_skein, hash + 64);
    
    sph_jh512_init(&ctx_jh);
    sph_jh512(&ctx_jh, hash + 64, 64);
    sph_jh512_close(&ctx_jh, hash);
    
    sph_keccak512_init(&ctx_keccak);
    sph_keccak512(&ctx_keccak, hash, 64);
    sph_keccak512_close(&ctx_keccak, hash + 64);

    sph_luffa512_init(&ctx_luffa);
    sph_luffa512(&ctx_luffa, hash + 64, 64);
    sph_luffa512_close(&ctx_luffa, hash);
    
    sph_cubehash512_init(&ctx_cubehash);
    sph_cubehash512(&ctx_cubehash, hash, 64);
    sph_cubehash512_close(&ctx_cubehash, hash + 64);
    
    sph_shavite512_init(&ctx_shavite);
    sph_shavite512(&ctx_shavite, hash + 64, 64);
    sph_shavite512_close(&ctx_shavite, hash);
        
    sph_simd512_init(&ctx_simd);
    sph_simd512(&ctx_simd, hash, 64);
    sph_simd512_close(&ctx_simd, hash + 64);

    sph_echo512_init(&ctx_echo);
    sph_echo512(&ctx_echo, hash + 64, 64);
    sph_echo512_close(&ctx_echo, hash);

    sph_hamsi512_init(&ctx_hamsi);
    sph_hamsi512(&ctx_hamsi, hash, 64);
    sph_hamsi512_close(&ctx_hamsi, hash + 64);

    sph_fugue512_init(&ctx_fugue);
    sph_fugue512(&ctx_fugue, hash + 64, 64);
    sph_fugue512_close(&ctx_fugue, hash);

    sph_shabal512_init(&ctx_shabal);
    sph_shabal512(&ctx_shabal, hash, 64);
    sph_shabal512_close(&ctx_shabal, hash + 64);

    sph_whirlpool_init(&ctx_whirlpool);
    sph_whirlpool(&ctx_whirlpool, hash + 64, 64);
    sph_whirlpool_close(&ctx_whirlpool, hash);


    memcpy(output, hash, 32);
}



char *bin2hex(const unsigned char *p, size_t len)
{
    int i;
    char *s = malloc((len * 2) + 1);
    if (!s)
        return NULL;

    for (i = 0; i < len; i++)
        sprintf(s + (i * 2), "%02x", (unsigned int) p[i]);

    return s;
}

bool hex2bin(unsigned char *p, const char *hexstr, size_t len)
{
    char hex_byte[3];
    char *ep;

    hex_byte[2] = '\0';

    while (*hexstr && len) {
        if (!hexstr[1]) {
          //  applog(LOG_ERR, "hex2bin str truncated");
            return false;
        }
        hex_byte[0] = hexstr[0];
        hex_byte[1] = hexstr[1];
        *p = (unsigned char) strtol(hex_byte, &ep, 16);
        if (*ep) {
//            applog(LOG_ERR, "hex2bin failed on '%s'", hex_byte);
            return false;
        }
        p++;
        hexstr += 2;
        len--;
    }

    return (len == 0 && *hexstr == 0) ? true : false;
}

bool opt_debug = false;
bool fulltest(const uint32_t *hash, const uint32_t *target)
{
    int i;
    bool rc = true;

    for (i = 7; i >= 0; i--) {
        if (hash[i] > target[i]) {
            rc = false;
            break;
        }
        if (hash[i] < target[i]) {
            rc = true;
            break;
        }
    }

    if (opt_debug) {
        uint32_t hash_be[8], target_be[8];
        char *hash_str, *target_str;

        for (i = 0; i < 8; i++) {
            be32enc(hash_be + i, hash[7 - i]);
            be32enc(target_be + i, target[7 - i]);
        }
        hash_str = bin2hex((unsigned char *)hash_be, 32);
        target_str = bin2hex((unsigned char *)target_be, 32);

//        applog(LOG_DEBUG, "DEBUG: %s\nHash:   %s\nTarget: %s",
//            rc ? "hash <= target"
//               : "hash > target (false positive)",
//            hash_str,
//            target_str);

        free(hash_str);
        free(target_str);
    }

    return rc;
}


struct work_restart *work_restart = NULL;
int scanhash_bitblock(int thr_id, uint32_t *pdata, const uint32_t *ptarget,
    uint32_t max_nonce, unsigned long *hashes_done)
{
    uint32_t n = pdata[19] - 1;
    const uint32_t first_nonce = pdata[19];
    const uint32_t Htarg = ptarget[7];

    uint32_t hash64[8] __attribute__((aligned(32)));
    uint32_t endiandata[32];

    //we need bigendian data...
    //lessons learned: do NOT endianchange directly in pdata, this will all proof-of-works be considered as stale from minerd.... 
    int kk=0;
    for (; kk < 32; kk++)
    {
	      be32enc(&endiandata[kk], ((uint32_t*)pdata)[kk]);
    };

    do {
	      pdata[19] = ++n;
	      be32enc(&endiandata[19], n); 
	      bitblockhash(hash64, &endiandata);
            if (((hash64[7]&0xFFFFFF00)==0) && 
			      fulltest(hash64, ptarget)) {
                *hashes_done = n - first_nonce + 1;
		      return true;
	      }
    } while (n < max_nonce && !work_restart[thr_id].restart);

    *hashes_done = n - first_nonce + 1;
    pdata[19] = n;
    return 0;
}
