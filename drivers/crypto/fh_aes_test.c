/*
 * fh_aes_test.c
 *
 *  Created on: May 7, 2015
 *      Author: yu.zhang
 */
#ifdef CONFIG_FH_AES_SELF_TEST
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/errno.h>
#include <linux/in.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/highmem.h>
#include <linux/proc_fs.h>
#include <linux/ctype.h>
#include <linux/version.h>
#include <linux/spinlock.h>
#include <linux/dma-mapping.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/bitops.h>
#include <linux/io.h>
#include <linux/irqreturn.h>
#include <asm/irq.h>
#include <asm/page.h>
#include <linux/fh_aes.h>
#include <crypto/hash.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <crypto/rng.h>

//cbc aes 128
#define AES_IV0			0x00010203
#define AES_IV1			0x04050607
#define AES_IV2			0x08090a0b
#define AES_IV3			0x0c0d0e0f

#define AES_KEY0		0x2b7e1516
#define AES_KEY1		0x28aed2a6
#define AES_KEY2		0xabf71588
#define AES_KEY3		0x09cf4f3c

//ecb aes 256
#define AES_ECB_KEY0	0x603deb10
#define AES_ECB_KEY1	0x15ca71be
#define AES_ECB_KEY2	0x2b73aef0
#define AES_ECB_KEY3	0x857d7781
#define AES_ECB_KEY4	0x1f352c07
#define AES_ECB_KEY5	0x3b6108d7
#define AES_ECB_KEY6	0x2d9810a3
#define AES_ECB_KEY7	0x0914dff4

//ctr aes 192
#define AES_CTR_KEY0 	0x8e73b0f7
#define AES_CTR_KEY1 	0xda0e6452
#define AES_CTR_KEY2 	0xc810f32b
#define AES_CTR_KEY3 	0x809079e5
#define AES_CTR_KEY4 	0x62f8ead2
#define AES_CTR_KEY5 	0x522c6b7b

#define AES_CTR_IV0		0xf0f1f2f3
#define AES_CTR_IV1		0xf4f5f6f7
#define AES_CTR_IV2		0xf8f9fafb
#define AES_CTR_IV3		0xfcfdfeff

//cfb aes 192
#define AES_CFB_KEY0 	0x8e73b0f7
#define AES_CFB_KEY1 	0xda0e6452
#define AES_CFB_KEY2	0xc810f32b
#define AES_CFB_KEY3 	0x809079e5
#define AES_CFB_KEY4 	0x62f8ead2
#define AES_CFB_KEY5	0x522c6b7b

#define AES_CFB_IV0		0x00010203
#define AES_CFB_IV1		0x04050607
#define AES_CFB_IV2		0x08090a0b
#define AES_CFB_IV3		0x0c0d0e0f

//ofb aes 256
#define AES_OFB_256_KEY0 0x603deb10
#define AES_OFB_256_KEY1 0x15ca71be
#define AES_OFB_256_KEY2 0x2b73aef0
#define AES_OFB_256_KEY3 0x857d7781
#define AES_OFB_256_KEY4 0x1f352c07
#define AES_OFB_256_KEY5 0x3b6108d7
#define AES_OFB_256_KEY6 0x2d9810a3
#define AES_OFB_256_KEY7 0x0914dff4

#define AES_OFB_IV0		0x00010203
#define AES_OFB_IV1		0x04050607
#define AES_OFB_IV2		0x08090a0b
#define AES_OFB_IV3		0x0c0d0e0f

//des ecb
#define DES_ECB_KEY0 	0x01010101
#define DES_ECB_KEY1 	0x01010101

//des cbc
#define DES_CBC_KEY0 	0x01234567
#define DES_CBC_KEY1 	0x89abcdef

#define DES_CBC_IV0		0x12345678
#define DES_CBC_IV1		0x90abcdef

//ofb cbc
#define DES_OFB_KEY0 	0x01234567
#define DES_OFB_KEY1 	0x89abcdef

#define DES_OFB_IV0		0x12345678
#define DES_OFB_IV1		0x90abcdef

//ecb tri-des
#define DES_TRI_ECB_KEY0	 0x01234567
#define DES_TRI_ECB_KEY1 	 0x89abcdef

#define DES_TRI_ECB_KEY2	 0x23456789
#define DES_TRI_ECB_KEY3	 0xabcdef01

#define DES_TRI_ECB_KEY4	 0x456789ab
#define DES_TRI_ECB_KEY5	 0xcdef0123

//cbc tri-des
#define DES_TRI_CBC_KEY0	 0x01234567
#define DES_TRI_CBC_KEY1 	 0x89abcdef

#define DES_TRI_CBC_KEY2	 0x23456789
#define DES_TRI_CBC_KEY3	 0xabcdef01

#define DES_TRI_CBC_KEY4	 0x456789ab
#define DES_TRI_CBC_KEY5	 0xcdef0123

#define DES_TRI_CBC_IV0		0x12345678
#define DES_TRI_CBC_IV1		0x90abcdef

#define XBUFSIZE	128

struct tcrypt_result {
	struct completion completion;
	int err;
};

static inline void hexdump(unsigned char *buf, unsigned int len);
static void tcrypt_complete(struct crypto_async_request *req, int err);
static int testmgr_alloc_buf(char *buf[XBUFSIZE]);
static int fh_aes_cbc128_self_test(void);
static int fh_aes_ecb256_self_test(void);

static struct tcrypt_result result;
static const u32 plain_text[16] = {
	0x6bc1bee2, 0x2e409f96, 0xe93d7e11, 0x7393172a,
	0xae2d8a57, 0x1e03ac9c, 0x9eb76fac, 0x45af8e51,
	0x30c81c46, 0xa35ce411, 0xe5fbc119, 0x1a0a52ef,
	0xf69f2445, 0xdf4f9b17, 0xad2b417b, 0xe66c3710,
};

static const u32 cipher_text[16] = {
	0x7649abac, 0x8119b246, 0xcee98e9b, 0x12e9197d,
	0x5086cb9b, 0x507219ee, 0x95db113a, 0x917678b2,
	0x73bed6b8, 0xe3c1743b, 0x7116e69e, 0x22229516,
	0x3ff1caa1, 0x681fac09, 0x120eca30, 0x7586e1a7,
};

static const u32 plain_ecb_256_text[16] = {
	0x6bc1bee2, 0x2e409f96, 0xe93d7e11, 0x7393172a,
	0xae2d8a57, 0x1e03ac9c, 0x9eb76fac, 0x45af8e51,
	0x30c81c46, 0xa35ce411, 0xe5fbc119, 0x1a0a52ef,
	0xf69f2445, 0xdf4f9b17, 0xad2b417b, 0xe66c3710,
};

static const u32 cipher_ecb_256_text[16] = {
	0xf3eed1bd, 0xb5d2a03c, 0x064b5a7e, 0x3db181f8,
	0x591ccb10, 0xd410ed26, 0xdc5ba74a, 0x31362870,
	0xb6ed21b9, 0x9ca6f4f9, 0xf153e7b1, 0xbeafed1d,
	0x23304b7a, 0x39f9f3ff, 0x067d8d8f, 0x9e24ecc7,
};

static const u32 plain_ctr_192_text[16] = {
	0x6bc1bee2, 0x2e409f96, 0xe93d7e11, 0x7393172a,
	0xae2d8a57, 0x1e03ac9c, 0x9eb76fac, 0x45af8e51,
	0x30c81c46, 0xa35ce411, 0xe5fbc119, 0x1a0a52ef,
	0xf69f2445, 0xdf4f9b17, 0xad2b417b, 0xe66c3710,
};

static const u32 cipher_ctr_192_text[16] = {
	0x1abc9324, 0x17521ca2, 0x4f2b0459, 0xfe7e6e0b,
	0x090339ec, 0x0aa6faef, 0xd5ccc2c6, 0xf4ce8e94,
	0x1e36b26b, 0xd1ebc670, 0xd1bd1d66, 0x5620abf7,
	0x4f78a7f6, 0xd2980958, 0x5a97daec, 0x58c6b050,
};

static const u32 plain_ofb_256_text[16] = {
	0x6bc1bee2, 0x2e409f96, 0xe93d7e11, 0x7393172a,
	0xae2d8a57, 0x1e03ac9c, 0x9eb76fac, 0x45af8e51,
	0x30c81c46, 0xa35ce411, 0xe5fbc119, 0x1a0a52ef,
	0xf69f2445, 0xdf4f9b17, 0xad2b417b, 0xe66c3710,
};

static const u32 cipher_ofb_256_text[16] = {
	0xdc7e84bf, 0xda79164b, 0x7ecd8486, 0x985d3860,
	0x4febdc67, 0x40d20b3a, 0xc88f6ad8, 0x2a4fb08d,
	0x71ab47a0, 0x86e86eed, 0xf39d1c5b, 0xba97c408,
	0x0126141d, 0x67f37be8, 0x538f5a8b, 0xe740e484,
};

static const u32 plain_des_ecb_text[16] = {
	0x80000000, 0x00000000,
	0x40000000, 0x00000000,
	0x20000000, 0x00000000,
	0x10000000, 0x00000000,
	0x08000000, 0x00000000,
	0x04000000, 0x00000000,
	0x02000000, 0x00000000,
	0x01000000, 0x00000000,
};

static const u32 cipher_des_ecb_text[16] = {
	0x95F8A5E5, 0xDD31D900,
	0xDD7F121C, 0xA5015619,
	0x2E865310, 0x4F3834EA,
	0x4BD388FF, 0x6CD81D4F,
	0x20B9E767, 0xB2FB1456,
	0x55579380, 0xD77138EF,
	0x6CC5DEFA, 0xAF04512F,
	0x0D9F279B, 0xA5D87260,
};

static const u32 plain_des_cbc_text[6] = {
	0x4e6f7720, 0x69732074,
	0x68652074, 0x696d6520,
	0x666f7220, 0x616c6c20,
};

static const u32 cipher_des_cbc_text[6] = {
	0xe5c7cdde, 0x872bf27c,
	0x43e93400, 0x8c389c0f,
	0x68378849, 0x9a7c05f6,
};

static const u32 plain_des_ofb_text[6] = {
	0x4e6f7720, 0x69732074,
	0x43e93400, 0x8c389c0f,
	0x68378849, 0x9a7c05f6,
};

static const u32 cipher_des_ofb_text[6] = {
	0xf3096249, 0xc7f46e51,
	0x1e7e5e50, 0xcbbec410,
	0x3335a18a, 0xde4a9115,
};

static const u32 plain_des_tri_ecb_text[6] = {
	0x4e6f7720, 0x69732074,
	0x43e93400, 0x8c389c0f,
	0x68378849, 0x9a7c05f6,
};

static const u32 cipher_des_tri_ecb_text[6] = {
	0x314f8327, 0xfa7a09a8,
	0xd5895fad, 0xe98faedf,
	0x98f470eb, 0x3553a5da,
};

static const u32 plain_des_tri_cbc_text[6] = {
	0x4e6f7720, 0x69732074,
	0x43e93400, 0x8c389c0f,
	0x68378849, 0x9a7c05f6,
};

static const u32 cipher_des_tri_cbc_text[6] = {
	0xf3c0ff02, 0x6c023089,
	0xc43add8f, 0xd8cd5e43,
	0x2bfd41d3, 0x130bcf40,
};

static inline void hexdump(unsigned char *buf, unsigned int len)
{
	while (len--)
		AES_DBG("%02x", *buf++);
	AES_DBG("\n");
}

static void tcrypt_complete(struct crypto_async_request *req, int err)
{
	//struct tcrypt_result *res = req->data;
	if (err == -EINPROGRESS)
		return;
//      res->err = err;
//      complete(&res->completion);
	AES_DBG("crypt all over....\n");
	complete(&result.completion);

}

static int testmgr_alloc_buf(char *buf[XBUFSIZE])
{
	int i;
	for (i = 0; i < XBUFSIZE; i++) {
		buf[i] = (void *)__get_free_page(GFP_KERNEL);
		if (!buf[i])
			goto err_free_buf;
	}

	return 0;
err_free_buf:
	while (i-- > 0)
		free_page((unsigned long)buf[i]);

	return -ENOMEM;
}

static int fh_aes_cbc128_self_test(void)
{
	struct crypto_ablkcipher *tfm;
	struct ablkcipher_request *req;
	const char *algo;

	struct scatterlist sg[8];
	struct scatterlist dst_sg[8];

	const u32 key[4] = { AES_KEY0, AES_KEY1, AES_KEY2, AES_KEY3 };
	const u32 iv[4] = { AES_IV0, AES_IV1, AES_IV2, AES_IV3 };
	char *xbuf[XBUFSIZE];
	char *dst_xbuf[XBUFSIZE];
	//int ret = -ENOMEM;
	void *data;
	void *dst_data;

	if (testmgr_alloc_buf(xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	if (testmgr_alloc_buf(dst_xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	AES_DBG("aes self test get in...\n");
	AES_DBG(" *_* step 1\n");
	tfm =
		crypto_alloc_ablkcipher("cbc-aes-fh",
					CRYPTO_ALG_TYPE_ABLKCIPHER |
					CRYPTO_ALG_ASYNC, 0);
	if (IS_ERR(tfm)) {
		AES_DBG("aes_test: failed to alloc cipher!\n");
		return -1;
	}

	AES_DBG(" *_* step 2\n");
	algo = crypto_tfm_alg_driver_name(crypto_ablkcipher_tfm(tfm));
	init_completion(&result.completion);

	AES_DBG(" *_* step 3\n");
	crypto_ablkcipher_setkey(tfm, (u8 *) key, 16);

	AES_DBG(" *_* step 4\n");
	req = ablkcipher_request_alloc(tfm, GFP_KERNEL);
	if (!req) {
		AES_DBG(KERN_ERR "alg: skcipher: Failed to allocate request "
			"for %s\n", algo);
		return -1;
	}

	AES_DBG(" *_* step 5\n");
	ablkcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG,
					tcrypt_complete, &result);

	AES_DBG(" *_* step 6\n");
	data = xbuf[0];
	dst_data = dst_xbuf[0];

	//encrypt
	memcpy(data, plain_text, 64);
	memset(dst_data, 0, 64);
	sg_init_one(&sg[0], data, 64);
	sg_init_one(&dst_sg[0], dst_data, 64);

	AES_DBG(" *_* step 7\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 64, (void *)iv);

	AES_DBG(" *_* step 8\n");
	crypto_ablkcipher_encrypt(req);

	wait_for_completion(&result.completion);

	if (memcmp(dst_data, cipher_text, 64))
		AES_PRINT_RESULT(" encrypt error....\n");
	else
		AES_PRINT_RESULT(" encrypt ok....\n");

	//decrypt
	memcpy(data, cipher_text, 64);
	memset(dst_data, 0, 64);
	sg_init_one(&sg[0], data, 64);
	sg_init_one(&dst_sg[0], dst_data, 64);
	AES_DBG(" *_* step 8\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 64, (void *)iv);
	AES_DBG(" *_* step 9\n");
	crypto_ablkcipher_decrypt(req);
	wait_for_completion(&result.completion);

	if (memcmp(dst_data, plain_text, 64))
		AES_PRINT_RESULT(" decrypt error....\n");
	else
		AES_PRINT_RESULT(" decrypt ok....\n");

	return 0;

}

static int fh_aes_ecb256_self_test(void)
{
	struct crypto_ablkcipher *tfm;
	struct ablkcipher_request *req;
	const char *algo;
	struct scatterlist sg[8];
	struct scatterlist dst_sg[8];
	const u32 key[8] = {
		AES_ECB_KEY0, AES_ECB_KEY1, AES_ECB_KEY2, AES_ECB_KEY3,
		AES_ECB_KEY4, AES_ECB_KEY5, AES_ECB_KEY6, AES_ECB_KEY7
	};
	//const u32 iv[4] = {AES_IV0,AES_IV1,AES_IV2,AES_IV3};

	char *xbuf[XBUFSIZE];
	char *dst_xbuf[XBUFSIZE];

	//int ret = -ENOMEM;
	void *data;
	void *dst_data;

	if (testmgr_alloc_buf(xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	if (testmgr_alloc_buf(dst_xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	AES_DBG("aes self test get in...\n");
	AES_DBG(" *_* step 1\n");
	tfm =
		crypto_alloc_ablkcipher("ecb-aes-fh",
					CRYPTO_ALG_TYPE_ABLKCIPHER |
					CRYPTO_ALG_ASYNC, 0);
	if (IS_ERR(tfm)) {
		AES_DBG("aes_test: failed to alloc cipher!\n");
		return -1;
	}

	AES_DBG(" *_* step 2\n");
	algo = crypto_tfm_alg_driver_name(crypto_ablkcipher_tfm(tfm));
	init_completion(&result.completion);

	AES_DBG(" *_* step 3\n");
	crypto_ablkcipher_setkey(tfm, (u8 *) key, 32);

	AES_DBG(" *_* step 4\n");
	req = ablkcipher_request_alloc(tfm, GFP_KERNEL);
	if (!req) {
		AES_DBG(KERN_ERR "alg: skcipher: Failed to allocate request "
			"for %s\n", algo);
		return -1;
	}

	AES_DBG(" *_* step 5\n");
	ablkcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG,
					tcrypt_complete, &result);

	AES_DBG(" *_* step 6\n");
	data = xbuf[0];
	dst_data = dst_xbuf[0];

	//encrypt
	memcpy(data, plain_ecb_256_text, 64);
	memset(dst_data, 0, 64);
	sg_init_one(&sg[0], data, 64);
	sg_init_one(&dst_sg[0], dst_data, 64);

	AES_DBG(" *_* step 7\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 64, NULL);

	AES_DBG(" *_* step 8\n");
	crypto_ablkcipher_encrypt(req);

	wait_for_completion(&result.completion);

	if (memcmp(dst_data, cipher_ecb_256_text, 64))
		AES_PRINT_RESULT(" encrypt error....\n");
	else
		AES_PRINT_RESULT(" encrypt ok....\n");

	//decrypt
	memcpy(data, cipher_ecb_256_text, 64);
	memset(dst_data, 0, 64);
	sg_init_one(&sg[0], data, 64);
	sg_init_one(&dst_sg[0], dst_data, 64);

	AES_DBG(" *_* step 8\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 64, NULL);

	AES_DBG(" *_* step 9\n");
	crypto_ablkcipher_decrypt(req);

	wait_for_completion(&result.completion);

	if (memcmp(dst_data, plain_ecb_256_text, 64))
		AES_PRINT_RESULT(" decrypt error....\n");
	else
		AES_PRINT_RESULT(" decrypt ok....\n");

	return 0;

}

static int fh_aes_ofb256_self_test(void)
{
	struct crypto_ablkcipher *tfm;
	struct ablkcipher_request *req;
	const char *algo;
	struct scatterlist sg[8];
	struct scatterlist dst_sg[8];
	const u32 key[8] = {
		AES_OFB_256_KEY0, AES_OFB_256_KEY1, AES_OFB_256_KEY2,
		AES_OFB_256_KEY3,
		AES_OFB_256_KEY4, AES_OFB_256_KEY5, AES_OFB_256_KEY6,
		AES_OFB_256_KEY7
	};
	const u32 iv[4] =
	{ AES_OFB_IV0, AES_OFB_IV1, AES_OFB_IV2, AES_OFB_IV3 };
	char *xbuf[XBUFSIZE];
	char *dst_xbuf[XBUFSIZE];
	void *data;
	void *dst_data;

	if (testmgr_alloc_buf(xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	if (testmgr_alloc_buf(dst_xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	AES_DBG("aes self test get in...\n");
	AES_DBG(" *_* step 1\n");
	tfm =
		crypto_alloc_ablkcipher("ofb-aes-fh",
					CRYPTO_ALG_TYPE_ABLKCIPHER |
					CRYPTO_ALG_ASYNC, 0);
	if (IS_ERR(tfm)) {
		AES_DBG("aes_test: failed to alloc cipher!\n");
		return -1;
	}

	AES_DBG(" *_* step 2\n");
	algo = crypto_tfm_alg_driver_name(crypto_ablkcipher_tfm(tfm));
	init_completion(&result.completion);

	AES_DBG(" *_* step 3\n");
	crypto_ablkcipher_setkey(tfm, (u8 *) key, 32);

	AES_DBG(" *_* step 4\n");
	req = ablkcipher_request_alloc(tfm, GFP_KERNEL);
	if (!req) {
		AES_DBG(KERN_ERR "alg: skcipher: Failed to allocate request "
			"for %s\n", algo);
		return -1;
	}

	AES_DBG(" *_* step 5\n");
	ablkcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG,
					tcrypt_complete, &result);

	AES_DBG(" *_* step 6\n");
	data = xbuf[0];
	dst_data = dst_xbuf[0];
	//encrypt
	memcpy(data, plain_ofb_256_text, 64);
	memset(dst_data, 0, 64);
	sg_init_one(&sg[0], data, 64);
	sg_init_one(&dst_sg[0], dst_data, 64);

	AES_DBG(" *_* step 7\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 64, (void *)iv);

	AES_DBG(" *_* step 8\n");
	crypto_ablkcipher_encrypt(req);
	wait_for_completion(&result.completion);
	if (memcmp(dst_data, cipher_ofb_256_text, 64))
		AES_PRINT_RESULT(" encrypt error....\n");
	else
		AES_PRINT_RESULT(" encrypt ok....\n");
	//decrypt
	memcpy(data, cipher_ofb_256_text, 64);
	memset(dst_data, 0, 64);
	sg_init_one(&sg[0], data, 64);
	sg_init_one(&dst_sg[0], dst_data, 64);
	AES_DBG(" *_* step 8\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 64, (void *)iv);
	AES_DBG(" *_* step 9\n");
	crypto_ablkcipher_decrypt(req);

	wait_for_completion(&result.completion);

	if (memcmp(dst_data, plain_ofb_256_text, 64))
		AES_PRINT_RESULT(" decrypt error....\n");
	else
		AES_PRINT_RESULT(" decrypt ok....\n");

	return 0;
}

static int fh_des_ecb_self_test(void)
{
	struct crypto_ablkcipher *tfm;
	struct ablkcipher_request *req;
	const char *algo;
	struct scatterlist sg[8];
	struct scatterlist dst_sg[8];
	const u32 key[2] = { DES_ECB_KEY0, DES_ECB_KEY1 };
	char *xbuf[XBUFSIZE];
	char *dst_xbuf[XBUFSIZE];
	void *data;
	void *dst_data;

	if (testmgr_alloc_buf(xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	if (testmgr_alloc_buf(dst_xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	AES_DBG("aes self test get in...\n");

	AES_DBG(" *_* step 1\n");
	tfm =
		crypto_alloc_ablkcipher("ecb-des-fh",
					CRYPTO_ALG_TYPE_ABLKCIPHER |
					CRYPTO_ALG_ASYNC, 0);
	if (IS_ERR(tfm)) {
		AES_DBG("aes_test: failed to alloc cipher!\n");
		return -1;
	}

	AES_DBG(" *_* step 2\n");
	algo = crypto_tfm_alg_driver_name(crypto_ablkcipher_tfm(tfm));
	init_completion(&result.completion);

	AES_DBG(" *_* step 3\n");
	crypto_ablkcipher_setkey(tfm, (u8 *) key, 8);

	AES_DBG(" *_* step 4\n");
	req = ablkcipher_request_alloc(tfm, GFP_KERNEL);
	if (!req) {
		AES_DBG(KERN_ERR "alg: skcipher: Failed to allocate request "
			"for %s\n", algo);
		return -1;
	}

	AES_DBG(" *_* step 5\n");
	ablkcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG,
					tcrypt_complete, &result);

	AES_DBG(" *_* step 6\n");
	data = xbuf[0];
	dst_data = dst_xbuf[0];

	//encrypt
	memcpy(data, plain_des_ecb_text, 64);
	memset(dst_data, 0, 64);
	sg_init_one(&sg[0], data, 64);
	sg_init_one(&dst_sg[0], dst_data, 64);

	AES_DBG(" *_* step 7\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 64, NULL);

	AES_DBG(" *_* step 8\n");
	crypto_ablkcipher_encrypt(req);

	wait_for_completion(&result.completion);

	if (memcmp(dst_data, cipher_des_ecb_text, 64))
		AES_PRINT_RESULT(" encrypt error....\n");
	else
		AES_PRINT_RESULT(" encrypt ok....\n");

	//decrypt
	memcpy(data, cipher_des_ecb_text, 64);
	memset(dst_data, 0, 64);
	sg_init_one(&sg[0], data, 64);
	sg_init_one(&dst_sg[0], dst_data, 64);

	AES_DBG(" *_* step 8\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 64, NULL);

	AES_DBG(" *_* step 9\n");
	crypto_ablkcipher_decrypt(req);

	wait_for_completion(&result.completion);

	if (memcmp(dst_data, plain_des_ecb_text, 64))
		AES_PRINT_RESULT(" decrypt error....\n");
	else
		AES_PRINT_RESULT(" decrypt ok....\n");

	return 0;

}

static int fh_des_cbc_self_test(void)
{
	struct crypto_ablkcipher *tfm;
	struct ablkcipher_request *req;
	const char *algo;
	struct scatterlist sg[8];
	struct scatterlist dst_sg[8];
	const u32 key[2] = { DES_CBC_KEY0, DES_CBC_KEY1 };
	const u32 iv[2] = { DES_CBC_IV0, DES_CBC_IV1 };
	char *xbuf[XBUFSIZE];
	char *dst_xbuf[XBUFSIZE];
	void *data;
	void *dst_data;

	if (testmgr_alloc_buf(xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	if (testmgr_alloc_buf(dst_xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	AES_DBG("aes self test get in...\n");

	AES_DBG(" *_* step 1\n");
	tfm =
		crypto_alloc_ablkcipher("cbc-des-fh",
					CRYPTO_ALG_TYPE_ABLKCIPHER |
					CRYPTO_ALG_ASYNC, 0);
	if (IS_ERR(tfm)) {
		AES_DBG("aes_test: failed to alloc cipher!\n");
		return -1;
	}

	AES_DBG(" *_* step 2\n");
	algo = crypto_tfm_alg_driver_name(crypto_ablkcipher_tfm(tfm));
	init_completion(&result.completion);

	AES_DBG(" *_* step 3\n");
	crypto_ablkcipher_setkey(tfm, (u8 *) key, 8);

	AES_DBG(" *_* step 4\n");
	req = ablkcipher_request_alloc(tfm, GFP_KERNEL);
	if (!req) {
		AES_DBG(KERN_ERR "alg: skcipher: Failed to allocate request "
			"for %s\n", algo);
		return -1;
	}

	AES_DBG(" *_* step 5\n");
	ablkcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG,
					tcrypt_complete, &result);

	AES_DBG(" *_* step 6\n");
	data = xbuf[0];
	dst_data = dst_xbuf[0];

	//encrypt
	memcpy(data, plain_des_cbc_text, 24);
	memset(dst_data, 0, 24);
	sg_init_one(&sg[0], data, 24);
	sg_init_one(&dst_sg[0], dst_data, 24);

	AES_DBG(" *_* step 7\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 24, (void *)iv);

	AES_DBG(" *_* step 8\n");
	crypto_ablkcipher_encrypt(req);

	wait_for_completion(&result.completion);

	if (memcmp(dst_data, cipher_des_cbc_text, 24))
		AES_PRINT_RESULT(" encrypt error....\n");
	else
		AES_PRINT_RESULT(" encrypt ok....\n");

	//decrypt
	memcpy(data, cipher_des_cbc_text, 24);
	memset(dst_data, 0, 24);
	sg_init_one(&sg[0], data, 24);
	sg_init_one(&dst_sg[0], dst_data, 24);

	AES_DBG(" *_* step 8\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 24, (void *)iv);

	AES_DBG(" *_* step 9\n");
	crypto_ablkcipher_decrypt(req);

	wait_for_completion(&result.completion);

	if (memcmp(dst_data, plain_des_cbc_text, 24))
		AES_PRINT_RESULT(" decrypt error....\n");
	else
		AES_PRINT_RESULT(" decrypt ok....\n");

	return 0;
}

static int fh_des_ofb_self_test(void)
{
	struct crypto_ablkcipher *tfm;
	struct ablkcipher_request *req;
	const char *algo;
	struct scatterlist sg[8];
	struct scatterlist dst_sg[8];
	const u32 key[2] = { DES_OFB_KEY0, DES_OFB_KEY1 };
	const u32 iv[2] = { DES_OFB_IV0, DES_OFB_IV1 };
	char *xbuf[XBUFSIZE];
	char *dst_xbuf[XBUFSIZE];
	void *data;
	void *dst_data;

	if (testmgr_alloc_buf(xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	if (testmgr_alloc_buf(dst_xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}
	AES_DBG("aes self test get in...\n");
	AES_DBG(" *_* step 1\n");
	tfm =
		crypto_alloc_ablkcipher("ofb-des-fh",
					CRYPTO_ALG_TYPE_ABLKCIPHER |
					CRYPTO_ALG_ASYNC, 0);
	if (IS_ERR(tfm)) {
		AES_DBG("aes_test: failed to alloc cipher!\n");
		return -1;
	}

	AES_DBG(" *_* step 2\n");
	algo = crypto_tfm_alg_driver_name(crypto_ablkcipher_tfm(tfm));
	init_completion(&result.completion);

	AES_DBG(" *_* step 3\n");
	crypto_ablkcipher_setkey(tfm, (u8 *) key, 8);

	AES_DBG(" *_* step 4\n");
	req = ablkcipher_request_alloc(tfm, GFP_KERNEL);
	if (!req) {
		AES_DBG(KERN_ERR "alg: skcipher: Failed to allocate request "
			"for %s\n", algo);
		return -1;
	}

	AES_DBG(" *_* step 5\n");
	ablkcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG,
					tcrypt_complete, &result);

	AES_DBG(" *_* step 6\n");
	data = xbuf[0];
	dst_data = dst_xbuf[0];

	//encrypt
	memcpy(data, plain_des_ofb_text, 24);
	memset(dst_data, 0, 24);
	sg_init_one(&sg[0], data, 24);
	sg_init_one(&dst_sg[0], dst_data, 24);

	AES_DBG(" *_* step 7\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 24, (void *)iv);
	AES_DBG(" *_* step 8\n");
	crypto_ablkcipher_encrypt(req);
	wait_for_completion(&result.completion);
	if (memcmp(dst_data, cipher_des_ofb_text, 24))
		AES_PRINT_RESULT(" encrypt error....\n");
	else
		AES_PRINT_RESULT(" encrypt ok....\n");

	//decrypt
	memcpy(data, cipher_des_ofb_text, 24);
	memset(dst_data, 0, 24);
	sg_init_one(&sg[0], data, 24);
	sg_init_one(&dst_sg[0], dst_data, 24);
	AES_DBG(" *_* step 8\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 24, (void *)iv);

	AES_DBG(" *_* step 9\n");
	crypto_ablkcipher_decrypt(req);
	wait_for_completion(&result.completion);
	if (memcmp(dst_data, plain_des_ofb_text, 24))
		AES_PRINT_RESULT(" decrypt error....\n");
	else
		AES_PRINT_RESULT(" decrypt ok....\n");

	return 0;

}

static int fh_des_tri_ecb_self_test(void)
{
	struct crypto_ablkcipher *tfm;
	struct ablkcipher_request *req;
	const char *algo;
	struct scatterlist sg[8];
	struct scatterlist dst_sg[8];
	const u32 key[6] = {
		DES_TRI_ECB_KEY0, DES_TRI_ECB_KEY1, DES_TRI_ECB_KEY2,
		DES_TRI_ECB_KEY3, DES_TRI_ECB_KEY4, DES_TRI_ECB_KEY5
	};
	char *xbuf[XBUFSIZE];
	char *dst_xbuf[XBUFSIZE];
	void *data;
	void *dst_data;

	if (testmgr_alloc_buf(xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	if (testmgr_alloc_buf(dst_xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	AES_DBG("aes self test get in...\n");

	AES_DBG(" *_* step 1\n");
	tfm =
		crypto_alloc_ablkcipher("ecb-des3-fh",
					CRYPTO_ALG_TYPE_ABLKCIPHER |
					CRYPTO_ALG_ASYNC, 0);
	if (IS_ERR(tfm)) {
		AES_DBG("aes_test: failed to alloc cipher!\n");
		return -1;
	}

	AES_DBG(" *_* step 2\n");
	algo = crypto_tfm_alg_driver_name(crypto_ablkcipher_tfm(tfm));
	init_completion(&result.completion);

	AES_DBG(" *_* step 3\n");
	crypto_ablkcipher_setkey(tfm, (u8 *) key, 24);

	AES_DBG(" *_* step 4\n");
	req = ablkcipher_request_alloc(tfm, GFP_KERNEL);
	if (!req) {
		AES_DBG(KERN_ERR "alg: skcipher: Failed to allocate request "
			"for %s\n", algo);
		return -1;
	}

	AES_DBG(" *_* step 5\n");
	ablkcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG,
					tcrypt_complete, &result);

	AES_DBG(" *_* step 6\n");
	data = xbuf[0];
	dst_data = dst_xbuf[0];

	//encrypt
	memcpy(data, plain_des_tri_ecb_text, 24);
	memset(dst_data, 0, 24);
	sg_init_one(&sg[0], data, 24);
	sg_init_one(&dst_sg[0], dst_data, 24);

	AES_DBG(" *_* step 7\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 24, (void *)NULL);

	AES_DBG(" *_* step 8\n");
	crypto_ablkcipher_encrypt(req);

	wait_for_completion(&result.completion);

	if (memcmp(dst_data, cipher_des_tri_ecb_text, 24))
		AES_PRINT_RESULT(" encrypt error....\n");
	else
		AES_PRINT_RESULT(" encrypt ok....\n");

	//decrypt
	memcpy(data, cipher_des_tri_ecb_text, 24);
	memset(dst_data, 0, 24);
	sg_init_one(&sg[0], data, 24);
	sg_init_one(&dst_sg[0], dst_data, 24);

	AES_DBG(" *_* step 8\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 24, (void *)NULL);

	AES_DBG(" *_* step 9\n");
	crypto_ablkcipher_decrypt(req);

	wait_for_completion(&result.completion);

	if (memcmp(dst_data, plain_des_tri_ecb_text, 24))
		AES_PRINT_RESULT(" decrypt error....\n");
	else
		AES_PRINT_RESULT(" decrypt ok....\n");

	return 0;

}

static int fh_des_tri_cbc_self_test(void)
{
	struct crypto_ablkcipher *tfm;
	struct ablkcipher_request *req;
	const char *algo;
	struct scatterlist sg[8];
	struct scatterlist dst_sg[8];
	const u32 key[6] = {
		DES_TRI_CBC_KEY0, DES_TRI_CBC_KEY1, DES_TRI_CBC_KEY2,
		DES_TRI_CBC_KEY3, DES_TRI_CBC_KEY4, DES_TRI_CBC_KEY5
	};
	const u32 iv[2] = { DES_TRI_CBC_IV0, DES_TRI_CBC_IV1 };
	char *xbuf[XBUFSIZE];
	char *dst_xbuf[XBUFSIZE];
	void *data;
	void *dst_data;

	if (testmgr_alloc_buf(xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	if (testmgr_alloc_buf(dst_xbuf)) {
		AES_DBG("no pages.\n");
		return -1;
	}

	AES_DBG("aes self test get in...\n");

	AES_DBG(" *_* step 1\n");
	tfm =
		crypto_alloc_ablkcipher("cbc-des3-fh",
					CRYPTO_ALG_TYPE_ABLKCIPHER |
					CRYPTO_ALG_ASYNC, 0);
	if (IS_ERR(tfm)) {
		AES_DBG("aes_test: failed to alloc cipher!\n");
		return -1;
	}

	AES_DBG(" *_* step 2\n");
	algo = crypto_tfm_alg_driver_name(crypto_ablkcipher_tfm(tfm));
	init_completion(&result.completion);

	AES_DBG(" *_* step 3\n");
	crypto_ablkcipher_setkey(tfm, (u8 *) key, 24);

	AES_DBG(" *_* step 4\n");
	req = ablkcipher_request_alloc(tfm, GFP_KERNEL);
	if (!req) {
		AES_DBG(KERN_ERR "alg: skcipher: Failed to allocate request "
			"for %s\n", algo);
		return -1;
	}

	AES_DBG(" *_* step 5\n");
	ablkcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG,
					tcrypt_complete, &result);

	AES_DBG(" *_* step 6\n");
	data = xbuf[0];
	dst_data = dst_xbuf[0];

	//encrypt
	memcpy(data, plain_des_tri_cbc_text, 24);
	memset(dst_data, 0, 24);
	sg_init_one(&sg[0], data, 24);
	sg_init_one(&dst_sg[0], dst_data, 24);

	AES_DBG(" *_* step 7\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 24, (void *)iv);

	AES_DBG(" *_* step 8\n");
	crypto_ablkcipher_encrypt(req);

	wait_for_completion(&result.completion);

	if (memcmp(dst_data, cipher_des_tri_cbc_text, 24))
		AES_PRINT_RESULT(" encrypt error....\n");
	else
		AES_PRINT_RESULT(" encrypt ok....\n");

	//decrypt
	memcpy(data, cipher_des_tri_cbc_text, 24);
	memset(dst_data, 0, 24);
	sg_init_one(&sg[0], data, 24);
	sg_init_one(&dst_sg[0], dst_data, 24);

	AES_DBG(" *_* step 8\n");
	ablkcipher_request_set_crypt(req, sg, dst_sg, 24, (void *)iv);

	AES_DBG(" *_* step 9\n");
	crypto_ablkcipher_decrypt(req);

	wait_for_completion(&result.completion);

	if (memcmp(dst_data, plain_des_tri_cbc_text, 24))
		AES_PRINT_RESULT(" decrypt error....\n");
	else
		AES_PRINT_RESULT(" decrypt ok....\n");

	return 0;

}

void fh_aes_self_test_all(void)
{
	pr_info("aes cbc128 self test go...\n");
	fh_aes_cbc128_self_test();
	pr_info("aes ecb256 self test go...\n");
	fh_aes_ecb256_self_test();
	pr_info("aes ctr192 self test go...\n");
	fh_aes_ofb256_self_test();
	pr_info("aes ofb 256 self test go...\n");
	fh_aes_ofb256_self_test();
	pr_info("des ecb self test go...\n");
	fh_des_ecb_self_test();
	pr_info("des cbc self test go...\n");
	fh_des_cbc_self_test();
	pr_info("des ofb self test go...\n");
	fh_des_ofb_self_test();
	pr_info("des tri ecb self test go...\n");
	fh_des_tri_ecb_self_test();
	pr_info("des tri cbc self test go...\n");
	fh_des_tri_cbc_self_test();
}

#endif
